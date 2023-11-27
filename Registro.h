#ifndef REGISTRO_H
#define REGISTRO_H
#include <string>
#include <iostream>
#include "gramatica.h"
using namespace std;

class Registro {
public:
    Registro();
    Registro(unsigned i_p, unsigned j_p, const string& A_p, const string& alfa_p, const string& beta_p, Gramatica* grammar_p);

    unsigned getI() const {
        return i;
    }

    unsigned getJ() const {
        return j;
    }

    string getA() const {
        return A;
    }

    string getAlfa() const {
        return alfa;
    }

    string getBeta() const {
        return beta;
    }

    string splitVar() const;

    string splitTer() const;

    void imprimir() const;

    bool operator==(const Registro& other) const;
private:
    Gramatica* grammar;
    unsigned i, j; //posicion 1 y 2
    string A; //posicion 3 // parte izquierda
    string alfa; //posicion 4  // punto
    string beta; //posicion 5  // parte derecha
};
Registro::Registro() {
}

Registro::Registro(unsigned i_p, unsigned j_p, const string& A_p, const string& alfa_p, const string& beta_p, Gramatica* grammar_p){
    i = i_p;
    j = j_p;
    A = A_p;
    alfa = alfa_p;
    beta = beta_p;
    grammar = grammar_p;
}

string Registro::splitVar() const{
    unsigned pos;
    for(auto it = grammar->getVariables()->begin(); it != grammar->getVariables()->end(); ++it) {
        if(beta.empty()){return "e";}
        string aux = beta.substr(0, beta.find_first_of("[",0));
        if(aux == *it)
            return *it;
    }    
    return "e";
}

string Registro::splitTer() const{
    string car;
    unsigned pos;
    for (auto it = grammar->getTerminales()->begin(); it != grammar->getTerminales()->end(); ++it){
        if(beta.empty()){return "e";}
        string aux = beta.substr(0, beta.find_first_of("[",0));
        if (aux == *it)
            return *it;
    }
    
    return "e";
}

bool Registro::operator==(const Registro& other) const{
    return A==other.A && i == other.i && j == other.j && alfa == other.alfa && beta==other.beta;
}


void Registro::imprimir() const{
    cout << "(" << i << ", " << j << ", " << A << ", " << alfa << ", " << beta << ")" << endl;
}
#endif /* REGISTRO_H */

