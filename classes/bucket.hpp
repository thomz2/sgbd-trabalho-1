#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdio.h>


using namespace std;

const int CAPACIDADE_BUCKET = 3;

class Bucket {
public:
    int pl;
    string ref;
    vector<pair<int,int>> values;

    Bucket(int initialPl, string ref,  const vector<pair<int,int>>& values = {}) : pl(initialPl), ref(ref) {
        if (values.size() > CAPACIDADE_BUCKET) {
            cout << "Erro ao criar bucket: overflow, " << values.size() << endl;
            return;
        }
        this->values = values;
    }

    ~Bucket() {
        values.clear();
    }

    bool isFull() const {
        return values.size() >= CAPACIDADE_BUCKET;
    }

    bool insert(pair<int,int> value) {
        if (isFull()) {
            // TODO: Codar logica para criar outro bucket
            cout << "Necessario criar outro bucket!" << endl;
            return false;
        }
        values.push_back(value);
        bool escreveu = this->write();
        if (!escreveu) {
            // Retira o valor inserido no vetor e retorna false
            values.pop_back();
            return false;
        }
        return true;
    }

    int deleteYear(int ano) {
        int count = 0;
        for (int i = 0; i < values.size(); ++i) {
            if (values[i].second == ano) {
                values.erase(values.begin() + i);
                count++;
            }
        }
        bool escreveu = this->write();
        if (!escreveu) {
            return -1000000;
        }
        return count;
    }

    void deleteBucket() {
        // Deletar o arquivo associado a este bucket
        string path = "buckets/" + this->ref; // Caminho para o arquivo
        int removeu = remove(path.c_str());
        if (removeu == 0) { // Verificar se o arquivo existe
            cout << "Arquivo " << this->ref << " foi removido." << endl;
        } else {
            cout << "Arquivo " << this->ref << " não encontrado." << endl;
        }
    }

    bool containsPk(int key) const {
        for (pair<int,int> value : values) {
            if (value.first == key) {
                return true;
            }
        }
        return false;
    }

    bool containsYear(int year) const {
        for (pair<int,int> value : values) {
            if (value.second == year) {
                return true;
            }
        }
        return false;
    }

    bool write() {
        ofstream outfile("buckets/" + this->ref);
        if (!outfile.is_open()) {
            cout << "Erro ao criar/abrir arquivo: " << this->ref << endl;
            return false;
        }

        for (const auto &key : this->values) {
            outfile << key.first << "," << key.second << "\n";
        }

        outfile.close();

        return true;
    } 

    static Bucket* create(const string &filename, int pl = 3, const vector<pair<int,int>>& values = {}) {
        Bucket* bucket = new Bucket(pl, filename, values);
        return bucket->write() ? bucket : nullptr;
    }

    static Bucket* read(const string &filename) {

        ifstream infile("buckets/" + filename);
        if (!infile.is_open()) {
            cout << "Erro ao ler arquivo: " << filename << endl;
            return nullptr;
        }

        int pl = filename.size();
        vector<pair<int,int>> values;
        string line;

        while (getline(infile, line)) {
            stringstream ss(line);
            int id, ano;

            char comma; 
            if (ss >> id >> comma >> ano && comma == ',') {
                values.push_back({id, ano});
            } else {
                cout << "Formato inválido na linha: " << line << endl;
                return nullptr;
            }
        }

        infile.close();

        return new Bucket(pl, filename, values);
    }

    string toString() {
        ostringstream oss;

        oss << "Bucket " << this->ref << ":\n";
        for (const auto &key : this->values) {
            oss << key.first << "," << key.second << "\n";
        }

        return oss.str();
    }
};