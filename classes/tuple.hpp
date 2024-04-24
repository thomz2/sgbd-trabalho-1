#pragma once
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Pedido {
public:
    int chave;
    double valor;
    int ano;

    //Pedido() {};
    //~Pedido() {};

    void fromString(string i_line) {
        char virgula;
        istringstream iss(i_line);
        iss >> chave >> virgula >> valor >> virgula >> ano;
        return;
    };

    string toString() {
        char virgula = ',';
        ostringstream oss;
        oss << fixed << setprecision(2) 
            << chave << virgula << valor << virgula << ano << endl;
        return oss.str();
    }
};