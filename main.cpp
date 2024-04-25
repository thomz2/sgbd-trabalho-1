// -std=c++17

#include "classes/tuple.hpp"
#include "classes/bucket.hpp"
#include "classes/directory.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem> 

using namespace std;

void setProfundidadeGlobal(int prof) {
    cout << "PF: " << prof << endl;
}

void commandINSERIR(int ano, int global, int local) {
    cout << "INS" << endl;
}

void commandREMOVER(int ano, int quant, int global, int local) {
    cout << "REM" << endl;
}

void commandBUSCAR(int ano, int quant) {
    cout << "BUS" << endl;
}



int readInputFile() {
    ifstream file("in.txt");
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    int index = 0;
    string line;
    while (getline(file, line)) {
        if (index == 0) { // PG/<prof global>
            string strProf = line.substr(line.find('/') + 1);
            int intProf; // Converta a string para um inteiro
            istringstream iss(strProf);
            if (!(iss >> intProf)) {
                cout << "Erro ao converter para inteiro." << endl; return 1;
            }
            setProfundidadeGlobal(intProf);
        }

        if (line.compare(0, 4, "INS:")) {
            int slash = line.find('/');
            int comma = line.find(',');
            string xStr = line.substr(4, 4); //anos tem tamanho 4 digitos
            string globalStr = line.substr(slash + 1, comma - slash - 1);
            string localStr = line.substr(comma + 1); 

            int x, global, local;
            istringstream(xStr) >> x;
            istringstream(globalStr) >> global;
            istringstream(localStr) >> local;
            commandINSERIR(x, global, local);
        }

        else if (line.compare(0, 4, "REM:")) {
            int slash = line.find('/');
            int comma = line.find(',');
            int comma2 = line.find(',', comma+1);
            string xStr = line.substr(4, 4); //anos tem tamanho 4 digitos
            string quantStr = line.substr(slash + 1, slash - comma - 1);
            string globalStr = line.substr(comma + 1, comma2 - comma - 1);
            string localStr = line.substr(comma2 + 1); 

            int x, quant, global, local;
            istringstream(xStr) >> x;
            istringstream(quantStr) >> quant;
            istringstream(globalStr) >> global;
            istringstream(localStr) >> local;
            commandREMOVER(x, quant, global, local);
        }

        else if (line.compare(0, 4, "BUS:")) {
            int slash = line.find('/');
            string xStr = line.substr(4, 4); //anos tem tamanho 4 digitos
            string quantStr = line.substr(slash + 1);

            int x, quant;
            istringstream(xStr) >> x;
            istringstream(quantStr) >> quant;
            commandBUSCAR(x, quant);
        }

        index += 1;
        continue;
    }

    return 0;
}

int main () {
    // string input = "1,22.03,2004";
    // Pedido pedido;
    // pedido.fromString(input);
    // cout << pedido.toString() << endl;
    // cout << "hello world" << endl;

    // Bucket* bucket = Bucket::create("teste.txt", 3, {
    //     {1, 2013},
    //     {2, 2014},
    //     {3, 2015},
    // });

    // bucket->insert({4, 2016});

    // Bucket* bucket = Bucket::read("teste.txt");

    // cout << bucket->toString() << endl;

    Directory* diretorio = new Directory(1);

    diretorio->createBuckets();
    diretorio->inserirValor(2013);
    diretorio->inserirValor(2005); // Caso especifico
    // diretorio->inserirValor(2222); // Caso que nao existe o 2022
    diretorio->inserirValor(2012);
    diretorio->inserirValor(2020);
    diretorio->inserirValor(2024);
    // diretorio->inserirValor(1998);
    diretorio->inserirValor(1995);
    // readInputFile();

    cout << diretorio->toString() << endl;
}