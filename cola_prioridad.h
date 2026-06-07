#include <iostream>

template <class T>
class nodo_cola{
private:
    int key;
    T data;
    nodo_cola<T>* sig;
public:
    nodo_cola(int, T);
    nodo_cola<T>* add(T, int);
    void conect(nodo_cola<T>*);
    void print();
    nodo_cola<T>* get_sig(){ 
        return sig;
    }
    int get_key(){
        return key;
    }
    T get_data(){
        return data;
    }

};

template <class T>
nodo_cola<T>::nodo_cola(int k, T dat){
    key = k;
    data = dat;
    sig = NULL;
}


template <class T>
void nodo_cola<T>::conect(nodo_cola<T>* s){
    sig = s;
}

template <class T>
nodo_cola<T>* nodo_cola<T>::add(T dat, int k){
    if(k >= key){
        if(sig == NULL){
            sig = new nodo_cola<T>(k,dat);
            return this;
        }else{
            sig = sig->add(dat,k);
            return this;
        }
    }else{
        nodo_cola* temp = new nodo_cola<T>(k,dat);
        temp->conect(this);
        return temp;
    }
}


template <class T>
void nodo_cola<T>::print(){
    std::cout << data << " ---> " << key << std::endl;
    if(sig != NULL){
        sig->print();
    }
}

//-----------------------------------------------------------------

template <class T>
class cola_priori{
private:
    nodo_cola<T>* head;
public:
    cola_priori(){
        head = NULL;
    }
    void push(T, int);
    T pop();
    int get_next_key();
    bool empty();
    void print();
};

template <class T>
bool cola_priori<T>::empty(){
    return head == NULL;
}

template <class T>
void cola_priori<T>::push(T dat, int k){

    if(head == NULL){
        head = new nodo_cola(k,dat);
    }else{
        head = head->add(dat,k);
    }
}


template <class T>
T cola_priori<T>::pop(){
    if(head == NULL){
        return T{};
    }

    nodo_cola<T>* temp = head;
    head = head->get_sig();
    T dat = temp->get_data();
    delete temp;
    return dat;
}

template <class T>
int cola_priori<T>::get_next_key(){
    return head->get_key();
}

template <class T>
void cola_priori<T>::print(){
    if(head != NULL){
        head->print();
    }
}