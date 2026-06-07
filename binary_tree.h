#include <iostream>

template <class T>
class nodo_bt{
private:
    T data;
    nodo_bt<T>* izq;
    nodo_bt<T>* der;
public:
    nodo_bt(){
        data = T{};
        izq = NULL;
        der = NULL;
    }
    void set_data(T dat){
        data = dat;
    }
    void set_izq(nodo_bt<T>* a){
        izq = a;
    }
    void set_der(nodo_bt<T>* b){
        der = b;
    }
    T get_data(){
        return data;
    }
    nodo_bt<T>* get_izq(){
        return izq;
    }
    nodo_bt<T>* get_der(){
        return der;
    }
    void print();
    void destroy();
};


template <class T>
void nodo_bt<T>::print(){
    std::cout << data;
    if(izq != NULL){
        std::cout << "<[ ";
        izq->print();
    }
    if(der != NULL){
        std::cout << " | ";
        der->print();
        std::cout << " ]";
    }
}

template <class T>
void nodo_bt<T>::destroy(){
    if(izq != NULL){
        izq->destroy();
    }
    if(der != NULL){
        der->destroy();
    }
    delete this;
}


//-------------------------------------------------------
/*
template <class T>
class binary_tree{
private:
    nodo_bt<T>* head;
public:
    binary_tree(){
        head = NULL;
    }
    ~binary_tree();

};

*/