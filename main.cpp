#include "tuple.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>



using namespace std;

class Bucket {
    int pl;
    string ref;
};

class Directory {
    int pg;
    vector<Bucket*> buckets;

    Directory(int initialPg) : pg(initialPg) {}
};

int main () {
    string input = "1,22.03,2004";
    Pedido pedido;
    pedido.fromString(input);
    cout << pedido.toString() << endl;
    cout << "hello world" << endl;
}