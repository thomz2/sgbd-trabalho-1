#pragma once
#include "bucket.hpp"
#include <vector>

template <typename t1, typename t2, typename t3>
struct tripla {
public:
    t1 first;
    t2 second;
    t3 third; 
};

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

tripla<int, double, int> split(string line, char delimiter = ',') {
    stringstream ss(line);
    string item;

    getline(ss, item, delimiter);
    int id = stoi(item);
    getline(ss, item, delimiter);
    double valor = stof(item);
    getline(ss, item, delimiter);
    int ano = stoi(item);

    return {id, valor, ano};

}


class Directory {
public:
    int pg;
    Bucket* bucketAtual;
    string refAtual;
    vector<tripla<int, int, int>> diretorio;

    Directory(int initialPg) : pg(initialPg), bucketAtual(nullptr), refAtual(""), diretorio({}) {}

    void createBuckets() {

        if (diretorio.size() == 0)
            diretorio.clear();

        for (int i = 0; i < (1 << pg); ++i) {
            int ref = hashFunction(i, pg);
            string filename = to_binary(ref, pg) + ".txt";
            cout << filename << endl;

            this->bucketAtual = Bucket::create(filename, pg);
            if (this->bucketAtual == nullptr) {
                cout << "Erro em createBuckets: bucketAtual nulo" << endl;
                return;
            }
            delete this->bucketAtual;
            
            // Assim funciona
            diretorio.push_back({ref, pg, ref});
            // Bucket: indice, profundidade local e referencia 
        }

        cout << endl;
    }

    void inserirValor(int ano) {
        this->refAtual = "data.csv";

        ifstream infile(this->refAtual); // Abrir o arquivo para leitura

        if (!infile.is_open()) {
            cout << "Erro ao abrir o arquivo: " << this->refAtual << endl;
            return;
        }

        // Vetor para guardar valores a serem inseridos
        vector<tripla<int,double,int>> values;

        string line;

        bool achouAlgo = false;

        // Ler o arquivo linha por linha
        while (getline(infile, line)) {
            tripla<int,double,int> linha = split(line, ',');
            if (linha.third == ano) {
                values.push_back(linha);
                achouAlgo = true;
                cout << "Linha: " << linha.first << ' ' << linha.second << ' ' << linha.third << endl;
            }
        }

        if (!achouAlgo) {
            cout << "Nenhuma ocorrencia do ano " << ano << " encontrada" << endl;
            return;
        }

        infile.close();

        // Agora lidar com inserção nos buckets
        int indice = hashFunction(ano, this->pg);
        cout << "Indice: " << indice << endl;
        int ref = -1;
        string refStr = "";

        // Busca a referencia do bucket no diretorio
        for (auto ind : this->diretorio) {
            if (indice == ind.first) {

                // assegura a referencia com a profundidade local
                int profundidade_local = ind.second;
                ref = ind.first & ((1 << profundidade_local) - 1);

                // Soh por botar mesmo
                ind.third = ref;

                // Transformando para string com o numero de bits especificado
                refStr = to_binary(ref, profundidade_local) + ".txt";
                cout << "indice encontrado: " << ind.first 
                     << " | profundidade local: " << ind.second 
                     << " | ref: " << refStr 
                     << endl << endl;
                break;
            }
        }

        if (ref == -1) {
            cout << "Erro ao buscar a referencia do bucket no diretorio" << endl;
            return;
        }

        this->bucketAtual = Bucket::read(refStr);

        if (this->bucketAtual->isFull()) {
            // Caso em que duplica o bucket
            cout << "Caso em que duplica o bucket" << endl;
        } else {
            for (auto value : values) {
                bool inseriu = this->bucketAtual->insert({value.first, value.third});
                if (!inseriu) {
                    // Caso em que duplica o bucket
                    cout << "Caso em que duplica o bucket" << endl;
                }
            }
        }
    }

};