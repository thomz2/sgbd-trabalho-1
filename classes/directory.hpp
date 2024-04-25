#pragma once
#include "bucket.hpp"
#include <vector>

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