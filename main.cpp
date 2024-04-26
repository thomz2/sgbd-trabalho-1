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

// int hashFunction(int input, int bits = 3) {
//     return input & ((1 << bits) - 1);
//     //input and 0b_111
// }

void commandINSERIR(Directory* diretorio, int ano) {
    diretorio->inserirValor(ano);
}

void commandREMOVER(Directory* diretorio, int ano) {
    cout << "REM" << endl;
}

int commandBUSCAR(Directory* diretorio, int ano) {
    diretorio->buscarBuckets(ano);
}


int main () {
    // Directory* diretorio = new Directory(1);

    // diretorio->createBuckets();
    // diretorio->inserirValor(2013);
    // diretorio->inserirValor(2005); // Caso especifico
    // // diretorio->inserirValor(2222); // Caso que nao existe o 2022
    // diretorio->inserirValor(2012);
    // diretorio->inserirValor(2020);
    // diretorio->inserirValor(2024);
    // // diretorio->inserirValor(1998);
    // diretorio->inserirValor(1995);
    // // readInputFile();

    // cout << diretorio->toString() << endl;

    Directory* diretorio;

    ifstream fileIn("in.txt");
    if (!fileIn.is_open()) {
        std::cerr << "Erro ao abrir o arquivo IN." << std::endl;
        return 1;
    }
    ofstream fileOut("out.txt");
    if (!fileOut.is_open()) {
        std::cerr << "Erro ao abrir o arquivo OUT." << std::endl;
        return 1;
    }

    int index = 0;
    string line;
    while (getline(fileIn, line)) {
        if (index == 0) { // PG/<prof global>
            string strProf = line.substr(line.find('/') + 1);
            int intProf; // Converta a string para um inteiro
            istringstream iss(strProf);
            if (!(iss >> intProf)) {
                cout << "Erro ao converter para inteiro." << endl; return 1;
            }
            // setProfundidadeGlobal(intProf);
            diretorio = new Directory(intProf);
            diretorio->createBuckets();
            fileOut << line << endl;
        }

        else if (line.compare(0, 4, "INC:") == 0) {
            int colon = line.find(':');
            string xStr = line.substr(colon+1);
            int x;
            istringstream(xStr) >> x;
            commandINSERIR(diretorio, x);

            int global = diretorio->pg;
            cout << diretorio->toString() << endl;
            int local = diretorio->diretorio[hashFunction(x, global)].second;
            
            fileOut << "INC:" << x << '/' << global << ',' << local << endl;
        }

        else if (line.compare(0, 4, "REM:") == 0) {
            cout << "Linha: " << line << endl;
            int colon = line.find(':');
            string xStr = line.substr(colon+1);
            int x;
            istringstream(xStr) >> x;
            commandREMOVER(diretorio, x);
            int global = 1; int local = 1;
            int numRemovidos = 2;
            fileOut << "REM:" << x << '/' << numRemovidos << ',' << global << ',' << local << endl;
        }

        else if (line.compare(0, 4, "BUS:") == 0) {
            int colon = line.find(':');
            string xStr = line.substr(colon+1);
            int x;
            istringstream(xStr) >> x;
            // commandBUSCAR(diretorio, x);
            int numSelecionados = commandBUSCAR(diretorio, x);
            fileOut << "BUS:" << x << '/' << numSelecionados << endl;
        }

        index += 1;
        continue;
    }

    fileIn.close();
    fileOut.close();

    return 0;
}