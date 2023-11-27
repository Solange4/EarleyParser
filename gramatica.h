#ifndef GRAMATICA_H
#define GRAMATICA_H
#include <fstream>
#include <set>
#include <vector>
#include "Produccion.h"

using namespace std;

class Gramatica {
private:
    //   vector de produccion ejemplo S>A
    vector<Produccion> producciones;
    //set almacena distintas variables en una sola, no acepta repeticiones
    set<string> terminales; //conjunto de terminales
    set<string> variables; //conjunto de variables
    bool is_variable(string txt){
        for (auto it=variables.begin(); it!=variables.end(); ++it)
            if(*it == txt)
                return true;
        return false;
    }
    // dividimos terminales y no terminales
    void obtain_ter_var();
public:
    Gramatica(){}
    //con vectores
    Gramatica(const vector<Produccion>& producciones_p);
    //archivo
    Gramatica(const string& file);
    //almacena varios valores dentro de una sola variable pero, no se aceptan repeticiones
    const set<string>* getTerminales() const {
        return &terminales;
    }

    const set<string>* getVariables() const {
        return &variables;
    }

    vector<Produccion> getProducciones() const {
        return producciones;
    }

    bool isVar(string carac) {
        return variables.count(carac);
    }

    bool isTer(string carac) {
        return terminales.count(carac);
    }

    void add(string prod);
    
    void remove(string prod);

    void imprimir_derecha(){
        cout<<endl;
        for (auto it=variables.begin(); it!=variables.end(); ++it)
            cout<<*it<<"  ";
        cout<<endl<<endl;
        for (auto it=terminales.begin(); it!=terminales.end(); ++it)
            cout<<*it<<"  ";
        cout<<endl;
                
    }

};
//Constructor si ingresamos un vector de producciones
Gramatica::Gramatica(const vector<Produccion>& producciones_p) {
    producciones = producciones_p;

    obtain_ter_var();
}
//si insertamos una txt
Gramatica::Gramatica(const string& name_file) {
    // almacenaron de cada produccion
    string prod;
    // fstream objeto
    ifstream file;
    file.open(name_file);

    if (file.is_open()) {
        while (getline(file, prod)) {
            Produccion prod_aux(prod);
            //insertamos una a una a nuestro vector de producciones
            producciones.push_back(prod_aux);
        }
        file.close();
        cout << "Gramatica leida correctamente..\n";
        obtain_ter_var();
    } else
        cout << "Error al leer el archivo..\n";
}

void Gramatica::obtain_ter_var() {
    string parte;
    string prod;
    for (int i = 0; i < producciones.size(); i++) {
        //parte izquierda
        variables.insert(producciones[i].parteIzq());
    }

    for(int i=0; i<producciones.size(); i++){
        //parte derecha
        parte = producciones[i].parteDer();
        for(int j=0; j<parte.size(); j++){
            //separamos string por string
            int pos_str = parte.find_first_of("[", j); //posicion del string
            prod = parte.substr(j, pos_str-j); //se crea un substring del nodo encontrado sin incluir su contexto
            j = parte.find_first_of(" ", j+1); //j cambia  a la posicion donde encuentre 1 espacio
            // si no es una variable, lo agregamos a terminales
            if (!is_variable(prod))
                terminales.insert(prod);
            //si es -1 -> llego al final del string y no encontro coincidencias, innecesario seguir buscando
            if(j == -1){break;} 
        }
    }
}

void Gramatica::add(string prod) {
    // crear una produccion
    //Produccion prod_aux(prod);
    producciones.push_back(prod);
}

void Gramatica::remove(string prod) {
    // en caso queramos quitar alguna produccion
    auto it = producciones.begin();
    Produccion prod_aux(prod);
    for (unsigned i = 0; i < producciones.size(); i++)
        if (producciones[i] == prod_aux)
            it += i;
    producciones.erase(it);
}



#endif /* GRAMATICA_H */

