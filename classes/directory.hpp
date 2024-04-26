#pragma once
#include "bucket.hpp"
#include <vector>
#include <set>

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
                refStr = to_binary(ref, ind.second) + ".txt";
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
            delete this->bucketAtual;
            this->duplicarBucket(indice, ano);
        } else {
            for (auto value : values) {
                bool inseriu = this->bucketAtual->insert({value.first, value.third});
                if (!inseriu) {
                    // Caso em que duplica o bucket
                    cout << "Caso em que duplica o bucket" << endl;
                    delete this->bucketAtual;
                    this->duplicarBucket(indice, ano);
                } else {
                    // Inseriu no bucket, e agora? 
                }
            }
        }
    }

    int buscarBuckets(int ano) {
        int anoHashed = hashFunction(ano, pg);
        int ref = diretorio[anoHashed].third;
        int profLocal = diretorio[anoHashed].second;
        string refBinary = to_binary(ref, profLocal);
        string filename = refBinary + ".txt";

        bucketAtual = Bucket::read(filename);
        int count = 0;
        for (pair<int, int> p : bucketAtual->values) {
            if (p.second == ano) count++;
        }
        delete bucketAtual;
        return count;
    }

    void duplicarBucket(int dirIndex, int ano) {
        // Indice a ser duplicado
        tripla<int,int,int> ind = this->diretorio[dirIndex];

        // Antiga referencia ao bucket
        int oldRefBucket = ind.third;

        this->diretorio[dirIndex].second++; // Extendendo a profundidade local
        int newPL = this->diretorio[dirIndex].second;

        // Caso passe a profundidade global, remodelar o hash
        if (newPL > this->pg) {
            int newInd = (1 << this->pg) | ind.first;
            int oldInd = ind.first;

            string filename = to_binary(oldRefBucket, this->pg) + ".txt";
            this->bucketAtual = Bucket::read(filename);
            vector<pair<int,int>> oldBucketValues = this->bucketAtual->values;
            this->bucketAtual->deleteBucket();
            delete this->bucketAtual;

            // Passando valores para copia do bucket antigo
            this->bucketAtual = Bucket::create("0" + filename, newPL, {});
            cout << "Arquivo" << this->bucketAtual->ref << " criado" << endl;
            delete this->bucketAtual;

            // E cria um bucket novo tambem
            this->bucketAtual = Bucket::create("1" + filename, newPL, {});
            cout << "Arquivo" << this->bucketAtual->ref << " criado" << endl;
            delete this->bucketAtual;

            // Finalmente, aumenta a profundidade global
            this->pg++;

            int dirSize = (int)diretorio.size();

            for (int i = 0; i < dirSize; ++i) {
                // Se for o indice duplicado
                if (i+dirSize == newInd) {
                    diretorio.push_back({dirSize+i, newPL, dirSize+i});
                }
                // Caso nao seja, pega a copia
                else
                    diretorio.push_back({i+dirSize, diretorio[i].second, diretorio[i].third});
            }

            set<int> insertedYears;
            // Reinserir o oldBucketValues
            for (int i = 0; i < oldBucketValues.size(); ++i) {
                // Inserindo ano para nao precisar inserir de novo
                int anoAtual = oldBucketValues[i].second;
                if (insertedYears.count(anoAtual) <= 0) {
                    insertedYears.insert(anoAtual);
                    this->inserirValor(anoAtual);
                }
            }

        } else {
            // Renomear o arquivo apenas

            // Pegando dados do txt
            string filename = to_binary(oldRefBucket, newPL-1) + ".txt";
            this->bucketAtual = Bucket::read(filename);
            vector<pair<int,int>> oldBucketValues = this->bucketAtual->values;
            this->bucketAtual->deleteBucket();
            delete this->bucketAtual;

            filename = to_binary(ind.first & ((1 << newPL) -1), newPL) + ".txt";
            this->bucketAtual = Bucket::create(filename, newPL, {});
            delete this->bucketAtual;

            set<int> insertedYears;
            // Reinserir o oldBucketValues
            for (int i = 0; i < oldBucketValues.size(); ++i) {
                // Inserindo ano para nao precisar inserir de novo
                int anoAtual = oldBucketValues[i].second;
                if (insertedYears.count(anoAtual) <= 0) {
                    insertedYears.insert(anoAtual);
                    this->inserirValor(anoAtual);
                }
            }
        }

        // for (int i = 0; i < this->diretorio.size(); ++i) {
        //     if (this->diretorio[i].third == oldRefBucket) {
        //         this->diretorio[i].second++;
        //     }
        // }

        // cout << "Tentando inserir novamente o " 
        //      << ano << ", informacoes: "
        //      << this->toString();
        this->inserirValor(ano);
    }

    string toString() const {
        std::ostringstream oss;

        // Mostrar a profundidade global do diretório
        oss << "Profundidade global: " << pg << "\n";

        // Mostrar as entradas do diretório
        oss << "Entradas do diretório:\n";
        for (const auto& entry : diretorio) {
            oss << "Índice: " << entry.first
                << ", Profundidade local: " << entry.second
                << ", Referência: " << entry.third << "\n";
        }

        return oss.str(); // Retorna a representação como uma string
    }

};