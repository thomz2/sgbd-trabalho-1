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