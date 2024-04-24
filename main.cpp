#include "classes/tuple.hpp"
#include "classes/bucket.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem> 

using namespace std;

int hashFunction(int input, int bits = 3) {
    return input & ((1 << bits) - 1);
    //input and 0b_111
}

string to_binary(int number, int bit_count = 32) {
    string binary_representation = "";
    for (int i = bit_count - 1; i >= 0; --i) {
        int bit = (number >> i) & 1; // Obter o valor do bit na posição i
        binary_representation += to_string(bit); // Adicionar à string
    }

    return binary_representation;
}

struct tripla {
public:
    int ind, pl, ref; 
};


class Directory {
public:
    int pg;
    Bucket* bucketAtual;
    string refAtual;
    vector<tripla> diretorio;

    Directory(int initialPg) : pg(initialPg), bucketAtual(nullptr), refAtual(""), diretorio({}) {}

    void createBuckets() {

        if (diretorio.size() == 0)
            diretorio.clear();

        for (int i = 0; i < (1 << pg); ++i) {
            int ref = hashFunction(i, pg);
            string filename = to_binary(ref, pg) + ".txt";
            // cout << filename << endl;

            this->bucketAtual = Bucket::create(filename, pg);
            if (this->bucketAtual == nullptr) {
                cout << "Erro em createBuckets: bucketAtual nulo" << endl;
                return;
            }
            // TODO: Destruir o bucket logo depois
            
            // Assim funciona
            diretorio.push_back({ref, pg, ref});

        }
    }

};

int main () {
    string input = "1,22.03,2004";
    Pedido pedido;
    pedido.fromString(input);
    cout << pedido.toString() << endl;
    cout << "hello world" << endl;

    Bucket* bucket = Bucket::create("teste.txt", 3, {
        {1, 2013},
        {2, 2014},
        {3, 2015},
    });

    bucket->insert({4, 2016});

    // Bucket* bucket = Bucket::read("teste.txt");

    cout << bucket->toString() << endl;

    Directory* diretorio = new Directory(3);

    diretorio->createBuckets();
}