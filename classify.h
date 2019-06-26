#ifndef CLASS_H
#define CLASS_H

#include "tags.h"


const double MAYBE = 3 + exp(-4);
const size_t MAX_QTY_TAGS = 7;
const double MIN_PROBABILITY = 0.01;


void classify(int argc, char const *argv[]) ;

void readStatistics(ifstream &stat,
                    map<string, TTag> &cloudOfTags,
                    size_t &qtyOfAllArt,
                    size_t &qtyOfAllUniqWords);

void classification(ifstream &fin, ofstream &fout,
                    map<string, TTag> &cloudOfTags,
                    size_t &qtyOfAllArt,
                    size_t &qtyOfAllUniqWords);

void addWordsInMap(map<string, size_t> &mapOfWords,
                   const string &line);

#endif