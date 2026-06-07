#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include "cola_prioridad.h"
#include "binary_tree.h"
#include "bw_fstream.h"

using namespace std;

class huffman{
private:
    ifstream input_text;
    ofstream output_text;

    bw_ifstream input_huff;

    string filename;

    cola_priori<pair<string,string>> freq_table;
    string tree_struct_string;
    string tree_data_string;
    map<char,string> tree_table;

    nodo_bt<char>* tree;

    bool make_freq_table();
    int make_tree_table();
    void update_tree_table(map<char,string>&,string,bool);

    bool make_tree();
    nodo_bt<char>* fill_tree();

public:
    void encode(string);
    void decode(string);
};



bool huffman::make_freq_table(){

    input_text.open(filename, ifstream::in);
    if( !input_text.is_open()){
        cout << "No se puede abrir el archivo" << endl;
        return false;
    }
    
    map<char, int> freq;
    map<char, int>::iterator it;
    char ch;

    while(ch != EOF){
        ch = input_text.get();
        freq[ch]++; 
    }
    freq.erase(EOF); // quitamos el EOF que fue agregado al final
    input_text.close();

    for (it = freq.begin(); it != freq.end(); it++){
        cout << it->first << " === " << it->second << endl;
        freq_table.push( make_pair<string,string>({it->first},"1"), it->second ); 
    }

    return true;
}

void huffman::update_tree_table(map<char,string> &tabla,string chs,bool cond){
    char bit = '0';
    if(cond) {bit = '1';}

    for(int i=0; i< chs.length(); i++){
        tabla[chs[i]].push_back(bit);
    }
}

int huffman::make_tree_table(){

    if( freq_table.empty()){
        return 0;
    }

    pair<string,string> temp1;
    pair<string,string> temp2;
    int freq1;
    int freq2;

    while(true){
        freq1 = freq_table.get_next_key();
        temp1 = freq_table.pop();

        if (temp1.first.length() > 1){
            temp1.second = "0" + temp1.second;
        }

        if( freq_table.empty()) {
            tree_data_string = temp1.first;
            tree_struct_string = temp1.second;
            return 1; 
        }

        freq2 = freq_table.get_next_key();
        temp2 = freq_table.pop();

        if (temp2.first.length() > 1){
            temp2.second = "0" + temp2.second;
        }

        update_tree_table(tree_table,temp1.first,(freq1 >= freq2));
        update_tree_table(tree_table,temp2.first,(freq2 > freq1));

        if(freq1 >= freq2){
            swap(temp1,temp2);
        }

       freq_table.push( make_pair(temp1.first+temp2.first, temp1.second+temp2.second), (freq1 + freq2));
    }
}

void huffman::encode(string fn){
    filename = fn;

    make_freq_table();
    make_tree_table();



    cout << tree_data_string << endl;
    cout << tree_struct_string << endl;
    for (auto it = tree_table.begin(); it != tree_table.end(); it++)
    {
        reverse(it->second.begin(),it->second.end());
        cout << it->first << " ^ " << it->second << endl;
    }
    

    bw_ofstream bw_out;
    
    size_t found = fn.find_first_of(".");
    if(found != string::npos){
        fn.erase(fn.begin()+found,fn.end());
    }
    bw_out.open(fn + ".huff");

    bw_out.write_str_bit(tree_struct_string);
    bw_out.write_str_byte(tree_data_string);

    input_text.open(filename, ifstream::in);
    char ch;
    while(ch != EOF){
        ch = input_text.get();
        bw_out.write_str_bit(tree_table[ch]);
    }

    input_text.close();
    bw_out.close();
    
}

bool huffman::make_tree(){
    int fin = 1;

    while(fin > 0){
        if(!input_huff.read_bit()){
            tree_struct_string.push_back('0');
            fin++;
        }else{
            tree_struct_string.push_back('1');
            fin--;
        }
    }

    tree = fill_tree();

    return true;
}

nodo_bt<char>* huffman::fill_tree(){
    static int i = 0;
    nodo_bt<char>* temp = new nodo_bt<char>;

    if(tree_struct_string[i] == '0'){
        temp->set_data('\0');
        i++;
        temp->set_izq(fill_tree());
        temp->set_der(fill_tree());
    }else{
        i++;
        temp->set_data(input_huff.read_char());
    }
    return temp;
}



void huffman::decode(string fn){
    filename = fn;
    input_huff.open(filename);

    make_tree();

    cout << tree_struct_string << endl;
    tree->print();

    size_t found = fn.find_first_of(".");
    if(found != string::npos){
        fn.erase(fn.begin()+found,fn.end());
    }
    output_text.open(fn + "DEC.txt");

    nodo_bt<char>* ptr_tree = tree;
    while(!input_huff.is_EOF){
        if(ptr_tree->get_data() != '\0'){
            output_text.put(ptr_tree->get_data());
            ptr_tree = tree;
        }

        if(input_huff.read_bit()){
            ptr_tree = ptr_tree->get_der();
        }else{
            ptr_tree = ptr_tree->get_izq();
        }
    }

    input_huff.close();
}



int main(int argc, char const *argv[])
{
    if (argc != 3){
        cout << "AYUDA: use el modificador -e para generar un archivo comprimido, y -d para descomprimir. seguido del nombre del archivo";
        cerr << "numero incorrecto de argumentos";
        return 1;
    }

    huffman codec;

    if(string(argv[1]) =="-e"){
        codec.encode(argv[2]);
    }

    if(string(argv[1]) =="-d"){
        codec.decode(argv[2]);
    }


  

    return 0;
}
