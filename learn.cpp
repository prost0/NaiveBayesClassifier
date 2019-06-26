#include "learn.h"

void learn(int argc, char const *argv[])
{
    size_t qtyOfAllArt = 0;
    size_t qtyOfAllUniqWords = 0;
    map<string, TTag> cloudOfTags;
    ifstream fileIn;
    ofstream fileOut;
    
    for (int32_t i = 2; i < argc; ++i) {
        if (!strcmp(argv[i], "--input")) {
            fileIn.open(argv[++i]);
        } else if (!strcmp(argv[i], "--output")) {
            fileOut.open(argv[++i]);
        } else {
            cerr << "Error\nUsage: prog learn --input\
             <input file> --output <output file>\n";
            exit(3);
        }
    }
    
    collectStatistics(fileIn, cloudOfTags, qtyOfAllArt, qtyOfAllUniqWords);
    writeStatistics(fileOut, cloudOfTags, qtyOfAllArt, qtyOfAllUniqWords);
}


void addWord(const string &line,
             map<string, size_t> &uniqTmp,
             size_t &qtyOfWordsInArt,
             set<string> &uniqAll)
{
    string tmp = "";
    for (size_t i = 0; i < line.size(); ++i) {
        if (isalnum(line[i])) {
            tmp += tolower(line[i]);
        } else {
            if (tmp != "") {
                uniqAll.insert(tmp);
                auto iter = uniqTmp.find(tmp);
                if (iter == uniqTmp.end()) {
                    uniqTmp.emplace(tmp, 1);
                } else {
                    ++iter->second;
                }
                
                ++qtyOfWordsInArt;
                tmp = "";
            }
        }
    }
}


void addTagToCloud(const map<string, size_t> &uniqTmp,
                   map<string, TTag> &cloudOfTags,
                   const vector<string> &vecOfTagsNames,
                   const size_t qtyOfWordsInArt)
{
    for (size_t tagNumber = 0; tagNumber < vecOfTagsNames.size(); ++tagNumber) {
        auto iter = cloudOfTags.find(vecOfTagsNames[tagNumber]);
        
        if (iter == cloudOfTags.end()) {
            TTag newTag;
            newTag.qtyOfArt = 1;
            newTag.qtyOfWords = qtyOfWordsInArt;
            for (auto &i : uniqTmp) {
                newTag.wordAndQty.emplace(i.first, i.second);
            }
            cloudOfTags.emplace(vecOfTagsNames[tagNumber], newTag);
            
        } else {
            ++(iter->second).qtyOfArt;
            (iter->second).qtyOfWords += qtyOfWordsInArt;
            for (auto &i : uniqTmp) {
                auto flag = (iter->second).wordAndQty.find(i.first);
                if (flag != (iter->second).wordAndQty.end()) {
                    flag->second += i.second;
                } else {
                    (iter->second).wordAndQty.emplace(i.first, i.second);
                }
            }
            
        }
    }
    
}


void collectStatistics(ifstream &fileIn,
                       map<string, TTag> &cloudOfTags,
                       size_t &qtyOfAllArt,
                       size_t &qtyOfAllUniqWords)
{
    set<string> uniqAll;
    string curTagName;
    string inputLine;
    size_t qtyOfWordsInArt = 0;
    size_t nOfLines;
    
    while (fileIn.peek() != EOF) {
        map<string, size_t> uniqTmp;
        qtyOfWordsInArt = 0;
        
        fileIn >> nOfLines;
        fileIn.get();
        ++qtyOfAllArt;
        
        while (nOfLines > 0) {
            getline(fileIn, inputLine);
            addWord(inputLine, uniqTmp, qtyOfWordsInArt, uniqAll);
            --nOfLines;
        }
        
        getline(fileIn, inputLine);
        vector<string> vecOfTagsNames;
        string curTagName = "";
        inputLine += ",";
        
        for (size_t i = 0; i < inputLine.size(); ++i) {
            if (inputLine[i] == ',') {
                vecOfTagsNames.push_back(curTagName);
                curTagName = "";
            } else {
                curTagName += tolower(inputLine[i]);
            }
        }
        
        addTagToCloud(uniqTmp, cloudOfTags, vecOfTagsNames, qtyOfWordsInArt);
    }
    qtyOfAllUniqWords = uniqAll.size();
}


void writeStatistics(ofstream &fileOut, const map<string, TTag> &cloudOfTags,
                     const size_t qtyOfAllArt, const size_t qtyOfAllUniqWords)
{
    fileOut << qtyOfAllArt << "\n" << qtyOfAllUniqWords << "\n";
    
    for (auto &i : cloudOfTags) {
        fileOut << i.first << "\n" << i.second.qtyOfArt << " " << i.second.qtyOfWords << "\n";
        fileOut << i.second.wordAndQty.size() << "\n";
        
        for (auto &iter : i.second.wordAndQty) {
            fileOut << iter.first << " " << iter.second << "\n";
        }
    }
}