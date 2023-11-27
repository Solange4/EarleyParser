#ifndef PRODUCCION_H
#define PRODUCCION_H
#include <string>
#include <iostream>
#include <map>
using namespace std;

class Produccion{
private:
    string _produccion; //la produccion total
    pair<string, string> _contexto_izq; //contexto de la izquierda

    map<string,map<string,string>> _contexto; //contexto de la derecha

public:
    Produccion(const string& otra);
    
    string parteIzq() const;
    string parteDer() const;
    
    void imprimir() const;
    bool operator==(const Produccion& other) const;
};

Produccion::Produccion(const std::string& otra) {
    _produccion = otra;
}

string Produccion::parteIzq() const {
    // find_first_of, busca el caracter el primer caracter desde una posicion dada en el string
    int pos_angle = _produccion.find_first_of('[', 0);

    //en el caso de que la izquierda no tenga contexto [NUM, TENSE, GND]
    int another_pos_angle = _produccion.find_first_of('-', 0);

    //para reconocer al S-> que es el estado inicial
    if(pos_angle > another_pos_angle)
        return _produccion.substr(0, another_pos_angle-1);
    //substr retorna un "string" desde una o un par de posiciones dadas, en este caso buscamos ">" entonces
    // al encontrarlo revisamos desde el inicio del string hasta ">", cuando encontramos ese sub string al ser independiente
    // tenemos en la parte izquierda un solo no terminal entonces solo lo almacenamos en un char la posicion 0 de ese sub string
    
    return _produccion.substr(0, pos_angle);
}

string Produccion::parteDer() const {
    //encontramos la posicion de > desde la posicion 0 
    int pos_angle = _produccion.find_first_of('>', 0);
    // esta parte es la de no terminales o terminales, entonces asumimos que tenemos mas de un elemento
    // retornarmos un string a partir del caracter sigueinte a > "hasta" (en realidad es cuanto elementos vamos a usar) el tanaño del string -1 caso el tamaño sea 6, etnonces el ultimo elemento esta en la pos 5
    return _produccion.substr(pos_angle + 2, _produccion.size() - 1);
}

void Produccion::imprimir() const {
    // imprime la produccion
    cout << _produccion << endl;
}

bool Produccion::operator==(const Produccion& other) const {
    //sobrecargamos el operador == para comparar dos producciones retorna un valor booleano
    return _produccion== other._produccion;
}


#endif /* PRODUCCION_H */

