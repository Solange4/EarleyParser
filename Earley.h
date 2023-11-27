#ifndef EARLEY_H
#define EARLEY_H

#include <string>
#include <vector>
#include <map>
#include "gramatica.h"
#include "Registro.h"
#include "Cadena.h"

using namespace std;

class Earley {
public:
    Earley(const Gramatica& g, const string& cad);
    
    bool parse();
private:
    Gramatica grammar;
    Cadena cadena;
    vector<vector<Registro>> registros;
    map<string,bool> check_var;
    
    unsigned j=0;
    
    void initialization();
    void clausure();
    bool advance();
    void termination();
    
    void comprobar() const;
};
Earley::Earley(const Gramatica& g, const string& cad) {
    grammar = g;
    cadena = cad;

    for (auto ite = grammar.getVariables()->begin(); ite != grammar.getVariables()->end(); ++ite) {
        check_var.insert(std::make_pair(*ite, false));
    }

}

bool Earley::parse() {
    cout<<"Longitud de la cadena: "<<cadena.size()<<endl;
    unsigned i = 0;
    initialization();

    do{
        cout<<"Estado Actual: "<<"/"<<cadena[j]<<"/"<<endl;
        clausure();

        // luego de predecir seteamos en false nuevamente a todas las variables en nuestro objeto MAP check_var esto con el fin de usarlo en el proximo chart. 
        for (auto it = check_var.begin(); it != check_var.end(); it++) {
            it->second = false;
        }
        // advance retorna un booleano, significa que envio un terminal al siguiente chart.
        if (advance()) {
            j++;
            termination();
            //            cout << "terminacion: \n";

        } else {
            cout << "\nPASO " << (i++) + 1 << ": " << endl;
            comprobar();
            cout << "\n No se agregan registros en avance.." << endl;
            return 0;
        }

        cout << "\nPASO " << (i++) + 1 << ": " << endl;
        comprobar();
    } while (j < cadena.size());

    bool included = false;
    // se termino de rellenar todos los charts, revisamos el ultimo chart que provenga de 0, no tenga nada den BETA es decir leido todo y que provenga de S, variable inicial, retornamso true o false
    for (unsigned m = 0; m < registros[j].size() && !registros[j].empty() && !included; m++)
        included = (registros[j][m].getI() == 0 && registros[j][m].getJ() == cadena.size() && registros[j][m].getA() == "S" && registros[j][m].getBeta() == "");

    return included;
}

void Earley::initialization() {
    registros.resize(cadena.size() + 1);

    for (unsigned i = 0; i < grammar.getProducciones().size(); i++) {
        if (grammar.getProducciones()[i].parteIzq() == "S")
            registros[0].push_back(Registro(0, 0, "S", "", grammar.getProducciones()[i].parteDer(), &grammar));
    }

    check_var.find("S")->second = true;
}
//predictor
void Earley::clausure() {
    // auxiliar 
    bool added = true;
    // mientras sea veradero
    while (added) {
        added = false; //seteamos added 
        string split_var; //almacen de split o mejor dicho almacenara el primer simbolo de la prodoccion parte derecha
        unsigned tamanio = registros[j].size(); //tamaño de registros en un chart
        string aux;    //auxiliar 
        for (unsigned k = 0; k < tamanio; k++) { // el predictor recorrera todos los registros insertados eb dicho chart
            split_var = registros[j][k].splitVar(); // verificamos si en algun registro la primera letra de la produccion dada es una variable ejemplo: S>AB entonces A es una variable? si es si, retorna A  sino retorna 'e'
            if (split_var != "e" && check_var.find(split_var)->second == false) { // verificamos si retorno algun valor y revisamos en nuestra variable map si ya ha sido predecida si es false entonces, falta predecir
                aux = split_var; // asignamos nuestro spli a aux
                // para cada produccion de la gramatica
                for (Produccion& prod : grammar.getProducciones()) // recorre todas las produccion de nuestra gramatica
                    if (prod.parteIzq() == split_var) { //revisamos si alguna produccion en su parte izquierda coincide con nuestro split_var
                        added = true; // si ingresa seteamos nuevamente added dandole a entender que quizas exista alguna variable mas por predecir
                        Registro reg(j, j, aux, "", prod.parteDer(), &grammar); // creamos un nuevo registro con los datos asignados se coloca "" porque no hemos leido nada, solo predecimos
                        bool included = false; // auxiliar
                        //comprobamos si existe ya dicho registro en registros[j]
                        for (unsigned m = 0; m < registros[j].size() && !registros[j].empty() && !included; m++) 
                            if (registros[j][m] == reg)
                                included = true; // si included es false concluimos que este registro ya ha sido agregado anteriormente

                        if (!included) {
                            //cout << "clausurado: "; reg.imprimir(); cout << endl;
                            // si included sigue siendo false entonces agregar este al chart correspondiente.
                            registros[j].push_back(reg);
                            check_var.find(split_var)->second = true;
                        }
                    }
            }
        }
        // si added es false quiere decir que o no existen variables que coincidan con la parte derecha, o que encontro un terminal o que todas las variables posibles ya han sido predecidas
        if (!added)
            return;
        // en caso added sea verdadera quiere decir que predecimos alguna variable, por ende revisamos nuestro MAP de variables para ver si tenemos alguna sin predecir
        auto it = check_var.begin();
        for (; it != check_var.end() && it->second == true; ++it);
        // si logro pasar todo el for, quiere decir que en el anterior proceso hemos terminado de predecir la ultima variable por ende no se puede predecir mas
        if (it == check_var.end())
            return;
    }
}
//scaner
bool Earley::advance() {
    string split_ter;
    string aux;
    bool added = false;
    //para revisar todas los registros del chart determinado
    for (unsigned k = 0; k < registros[j].size(); k++) {
        split_ter = registros[j][k].splitTer(); //devuelve si existe el terminal de una produccion dada
        if (split_ter != "e" && split_ter == cadena[j]) { //sino existe ningun terminal en cualquier registro retorna 'e' luego verifica si este terminal coincide con la letra en la posicion J  de la oracion dada
            aux = split_ter; 
            string beta_aux = registros[j][k].getBeta(); //obtenemos el terminal y lo almacenamos en un string
            int i = beta_aux.find_first_of(" ",0)+1;
            if(i-1 == -1){beta_aux = "";}
            else{beta_aux.erase(0, i);} //eliminamos el terminal del string auxiliar
            //creamos un nuevo registro que se va a almacenar en el siguiente chart,  registros[j][k].getAlfa() + aux se refiere a que si ha sido leido algo anteriormente se le agregue tambien aux a leido. 
            Registro reg(registros[j][k].getI(), j + 1, registros[j][k].getA(), registros[j][k].getAlfa() + aux, beta_aux, &grammar); 
            registros[j + 1].push_back(reg);
            added = true;
        }
    }
    return added;
}
//antecesor
void Earley::termination() {
    unsigned cont, h, i;
    cont = h = i = 0;
    while (cont <= j + 1) {
        for (unsigned k = 0; k < registros[i].size(); k++) { //registros i, revisamoss todos los registros
            // buscamos el chart predecesor
            if (registros[i][k].getI() == h && registros[i][k].getJ() == i) {
                // registros[i][k].imprimir();
                unsigned tope = registros[j].size(); //obtenemos el tamaño del chart actual
                for (unsigned l = 0; l < tope; l++) { //registros j, se comparan todos los registros con el actual
                    if (registros[j][l].getI() == i) { // que el predecesor del registro actual que se encuentra en el chart actual concuerde con el chart del cual compararemos sus registros, que el registro en su apartado J  concuerde con nuestro registro actual en I 
                        //registros[j][l].imprimir(); cout << endl;
                        string str_split_var;
                        string aux_split_var;
                        str_split_var = registros[i][k].splitVar(); // hacemos un split para encontrar la variable que almacena dicho registro en la parte derecha 
                        aux_split_var = str_split_var; // auxiliar
                        // si split arroja un resultado se compara con  la variable que almacenaba el terminal leido por ADVANCE, corroboramos tambien que ese registro en el chart actual su parte derecha este vacio, para tambien asegurarnos de que ha sido leido.
                        if (aux_split_var == registros[j][l].getA() && registros[j][l].getBeta().empty()) {
                            // si ingresamos quiere decir que el terminal leido en advance pertenece a la parte derecha de la variable en el chart anterior o en un orden superior por lo tanto....
                            string beta_aux = registros[i][k].getBeta(); // almacenamos la parte derecha restante del registro del chart predecesor
                            int aux = beta_aux.find_first_of(" ",0)+1;
                            if(aux-1 == -1){beta_aux = "";}
                            else{beta_aux.erase(0, aux);}
                            //eliminamos el terminal del string auxiliar
                            // lo eliminamos o en otras palabras hacemos la lectura de la variable
                            Registro reg(h, j, registros[i][k].getA(), registros[i][k].getAlfa() + " " + registros[j][l].getA(), beta_aux, &grammar); //creamos un nuevo registros con las nuevas variables
                            bool included = false;
                            // corroboramos si este proceso ya ha sido realizado con anterioridad
                            for (unsigned m = 0; m < registros[j].size() && !registros[j].empty() && !included; m++)
                                if (registros[j][m] == reg)
                                    included = true;
                            // en caso de que no sea asi, se le agrega al chart actual
                            if (!included) {
                                registros[j].push_back(reg);
                            }
                        }
                    }
                }
            }
        }
        // este if sirve para subir el i en nuestro registro
        if (h < i)
            h++;
        //al h ser mayor o igual a i , i hace referencia al chart "actual" que estamos corroborando. 
        //verificamos si este "auxiliar" es el anterior al chart ACTUAL.
        // si hemos llegado aqui entonces, hemos creado un nuevo antecesor, EJEMPLO un registro i = 1 y j = 2 , que en su parte beta aun tenga un terminal o no terminal
        // entonces, reseteamos todo nuevamente, para buscar su antecesesor alguno de los antecesores creados
        else if (i + 1 == j) { //estanmos ya en el penultimo chart tanto predecesor como actual osea i y j, por lo cual realizamos si ambos 
            i = h = 0;// seteamos para ver su podemos buscar un nuevo antecesor a partir de todos los ya creados
            cont++;
        //para subir el j de cada registro pero, mantener el predecesor en 0 es decir que por mas que se suba un chart se siga revisando si viene de la base el chart inicial
        } else if (h == i) {
            h = 0;
            i++;
        }
    }
}

void Earley::comprobar() const {
    for (unsigned i = 0; i < registros.size(); i++) {
        cout << "j: " << i << "\n";
        for (unsigned j = 0; j < registros[i].size(); j++)
            registros[i][j].imprimir();
    }
}


#endif /* EARLEY_H */

