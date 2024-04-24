#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem> 
#pragma once

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

    bool is_full() const {
        return values.size() >= CAPACIDADE_BUCKET;
    }

    bool insert(pair<int,int> value) {
        if (is_full()) {
            // Logica para criar outro bucket
            cout << "Necessario criar outro bucket!" << endl;
            return false;
        }
        values.push_back(value);
        return true;
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

    void clear() {
        values.clear();
    }

    static Bucket* create(const string &filename, int pl = 3, const vector<pair<int,int>>& values = {}) {

        Bucket* bucket = new Bucket(pl, filename, values);

        ofstream outfile("buckets/" + filename);
        if (!outfile.is_open()) {
            cout << "Erro ao criar arquivo: " << filename << endl;
            return nullptr;
        }

        for (const auto &key : bucket->values) {
            outfile << key.first << "," << key.second << "\n";
        }

        outfile.close();

        return bucket;
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

        while (std::getline(infile, line)) {
            stringstream ss(line);
            int id, ano;

            char comma; 
            if (ss >> id >> comma >> ano && comma == ',') {
                values.push_back({id, ano});
            } else {
                cout << "Formato inválido na linha: " << line << endl;
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