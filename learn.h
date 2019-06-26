#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "tags.h"


void learn(int argc, char const *argv[]);

void addTagToCloud(const map<string, size_t> &uniqTmp,
                   map<string, TTag> &cloudOfTags,
                   const vector<string> &vecOfTags,
                   const size_t qtyOfWordsInArt);

void addWord(const string &line,
             map<string, size_t> &uniqTmp,
             size_t &qtyOfWordsInArt,
             set<string> &uniq);

void collectStatistics(ifstream &fin,
                       map<string, TTag> &cloudOfTags,
                       size_t &qtyOfAllArt,
                       size_t &qtyOfAllUniqWords);

void writeStatistics(ofstream &fout,
                     const map<string, TTag> &cloudOfTags,
                     const size_t qtyOfAllArt,
                     const size_t qtyOfAllUniqWords);

#endif