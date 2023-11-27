#ifndef CADENA_H
#define CADENA_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Cadena{
private:
    string word;
    vector<string> palabra;
    int tam;
    void get_init(string &);
public:
    Cadena(){}
    Cadena(string);
    ~Cadena();
    int size(){ return tam; }
    string &operator[](int i) {
         if( i > palabra.size()) {
            cout << "Index out of bounds" <<endl; 
            // return first element.
            return palabra[0];
         }
         
         return palabra[i];
      }

    string getWord(){return word;}
    Cadena& operator&=(string &cad) {
        word=cad;
        get_init(word);
        return *this;
    }
    friend bool operator== (const Cadena &c1, const string &c2){
        return c1.word==c2;
    }
    friend bool operator!= (const Cadena &c1, const string &c2){
        return c1.word!=c2;
    }
    friend ostream& operator<<(ostream& os, const Cadena& dt){
        os<<dt.word;
        return os;
    }
    
};

Cadena::Cadena(string cad){
    word = cad;
    tam = 0;
    get_init(cad);
}

Cadena::~Cadena(){
}

void Cadena::get_init(string &cad){
    int aux = cad.size();
    for(int i=0; i<aux; i++){
        tam++;
        i = cad.find_first_of(" ", 0);
        palabra.push_back(cad.substr(0, i));
        cad.erase(0, i+1);
        if(i == -1)
            break;
    }
}

#endif