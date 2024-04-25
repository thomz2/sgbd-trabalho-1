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

void commandINSERIR(Directory* diretorio, int ano) {
    cout << "INS" << endl;
}

void commandREMOVER(Directory* diretorio, int ano) {
    cout << "REM" << endl;
}

void commandBUSCAR(Directory* diretorio, int ano) {
    cout << "BUS" << endl;
}


int main () {
    // Directory* diretorio = new Directory(2);
    // diretorio->createBuckets();
    // diretorio->inserirValor(2013);
    // diretorio->inserirValor(2005);
    // diretorio->inserirValor(2222);
    // // readInputFile();
    // readInputFile(diretorio);

    Directory* diretorio;

    ifstream file("in.txt");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    int index = 0;
    string line;
    while (getline(file, line)) {
        if (index == 0) { // PG/<prof global>
            string strProf = line.substr(line.find('/') + 1);
            int intProf; // Converta a string para um inteiro
            std::istringstream iss(strProf);
            if (!(iss >> intProf)) {
                std::cerr << "Erro ao converter para inteiro." << std::endl; return 1;
            }
            // setProfundidadeGlobal(intProf);
            diretorio = new Directory(intProf);
        }

        if (line.compare(0, 4, "INS:")) {
            int colon = line.find(':');
            string xStr = line.substr(colon+1);
            int x;
            istringstream(xStr) >> x;
            commandINSERIR(diretorio, x);
        }

        else if (line.compare(0, 4, "REM:")) {
            int colon = line.find(':');
            string xStr = line.substr(colon+1);
            int x;
            istringstream(xStr) >> x;
            commandREMOVER(diretorio, x);
        }

        else if (line.compare(0, 4, "BUS:")) {
            int colon = line.find(':');
            string xStr = line.substr(colon+1);
            int x;
            istringstream(xStr) >> x;
            commandBUSCAR(diretorio, x);
        }

        index += 1;
        continue;
    }

    return 0;
}