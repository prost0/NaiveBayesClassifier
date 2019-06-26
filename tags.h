#ifndef CLASSES_H
#define CLASSES_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include <algorithm>
#include <iomanip>

using namespace std;

struct TTag {
    size_t qtyOfArt;
    size_t qtyOfWords;
    map<string, size_t> wordAndQty;
};

#endif