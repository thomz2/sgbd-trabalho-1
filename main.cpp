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

}