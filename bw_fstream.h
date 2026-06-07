#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

class byte_cursor{
private:
    unsigned char position;
public:
    void set_position(unsigned short int);
    int get_position();
    bool move_right();
    void write(unsigned char&);
    bool read(unsigned char);
};

void byte_cursor::set_position(unsigned short int pos){
    pos = pos % 8;
    pos = 7 - pos;
    position = pow(2,pos);
    // pos = 0  - pos = 7
    //  2^7     - 2^1
    // 10000000 - 00000001
}

int byte_cursor::get_position(){
    return position;
}

bool byte_cursor::move_right(){
    if(position > 1){
        position = position / 2;
        return true;
    }else{
        position = 128;
        //     1    -     2    -     4    -     8    -    16    -    32    -    64    -   128
        // 00000001 - 00000010 - 00000100 - 00001000 - 00010000 - 00100000 - 01000000 - 10000000
        return false;
    }
}

void byte_cursor::write(unsigned char &output){
    output = output | position;
}

bool byte_cursor::read(unsigned char input){
    input = input & position;
    return input == position;
}


//----------------------------------------------------------------------------------

class bw_ofstream{
private:
    ofstream file;
    string filename;
    unsigned char block;
    byte_cursor cursor;

public:
    bw_ofstream();

    void open(string);
    void write_str_bit(string&); //covierte cada char individual '0' o '1' en bits
    void write_str_byte(string&);
    void aling();
    void close();

};

bw_ofstream::bw_ofstream(){
    filename = "";
    block = 0;
    cursor.set_position(0);
}

void bw_ofstream::open(string fn){
    filename = fn;
    file.open(filename, ifstream::out | ifstream::trunc);
    file.close();
}


void bw_ofstream::write_str_bit(string& cadena){
    file.open(filename, ifstream::out | ifstream::binary | ifstream::app);

    for (int i=0; i < cadena.length(); i++)
    {

        if(cadena[i] != '0'){
            cursor.write(block);
        }
        if(!cursor.move_right()){
            file.write( reinterpret_cast<char*>(&block) , 1);
            block = 0;
        }
    }
    file.close();
}


void bw_ofstream::write_str_byte(string& cadena){
    aling();

    file.open(filename, ifstream::out | ifstream::app);
    for (int i=0; i < cadena.length(); i++)
    {
        file << cadena[i];
    }
    file.close();

}

void bw_ofstream::aling(){
    if (block != 0){
        file.open(filename, ifstream::out | ifstream::binary | ifstream::app);
        file.write( reinterpret_cast<char*>(&block) , 1);
        block = 0;
        file.close();
    }
    cursor.set_position(0);
}

void bw_ofstream::close(){
    aling();
}

//----------------------------------------------------------------------------------

class bw_ifstream{
private:
    ifstream file;
    string filename;
    unsigned char block;
    byte_cursor cursor;

public:
    bool is_EOF;
    bw_ifstream();

    bool open(string);
    bool read_bit(); 
    char read_char();
    void aling();
    void close();
};

bw_ifstream::bw_ifstream(){
    filename = "";
    block = 0;
    cursor.set_position(0);
}

bool bw_ifstream::open(string fn){
    filename = fn;
    file.open(filename, ifstream::in);
    if(!file.is_open()){
        return false;
    }
    is_EOF = false;
    file.read( reinterpret_cast<char*>(&block) , 1);
    if(file.eof()){
        is_EOF = true;
    }

    return true;
}

bool bw_ifstream::read_bit(){

    bool temp = cursor.read(block);
    
    if(!cursor.move_right()){
        file.read( reinterpret_cast<char*>(&block) , 1);
        if(file.eof()){
            is_EOF = true;
        }
    }

    return temp;
}

char bw_ifstream::read_char(){
    aling();

    unsigned char temp = block;
    file.read( reinterpret_cast<char*>(&block) , 1);
    if(file.eof()){
        is_EOF = true;
    }
    return temp;
}

void bw_ifstream::aling(){
    if (cursor.get_position() != 128){
        file.read( reinterpret_cast<char*>(&block) , 1);
        if(file.eof()){
            is_EOF = true;
        }
        cursor.set_position(0);
    } 
}

void bw_ifstream::close(){
    block = 0;
    cursor.set_position(0);
    file.close();
}