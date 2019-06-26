#include "classify.h"


void classify(int argc, char const *argv[])
{
    size_t qtyOfAllArt = 0;
    size_t qtyOfAllUniqWords = 0;
    ifstream fileIn;
    ofstream fileOut;
    ifstream fileStat;
    map<string, TTag> cloudOfTags;
    
    for (int i = 2; i < argc; ++i) {
        if (!strcmp(argv[i], "--input")) {
            fileIn.open(argv[++i]);
        } else if (!strcmp(argv[i], "--output")) {
            fileOut.open(argv[++i]);
        } else if (!strcmp(argv[i], "--stats")) {
            fileStat.open(argv[++i]);
        } else {
            cerr << "Error\nUsage: prog classify\
             --stats <file with statistics>\
             --input <input file> --output <output file>\n";
            exit(4);
        }
    }
    
    readStatistics(fileStat, cloudOfTags, qtyOfAllArt, qtyOfAllUniqWords);
    
    classification(fileIn, fileOut, cloudOfTags, qtyOfAllArt, qtyOfAllUniqWords);
}


void readStatistics(ifstream &fileStat,
                    map<string, TTag> &cloudOfTags,
                    size_t &qtyOfAllArt,
                    size_t &qtyOfAllUniqWords)
{
    fileStat >> qtyOfAllArt >> qtyOfAllUniqWords;
    
    while (fileStat.peek() != EOF) {
        string inputLine, uniqWord;
        size_t qtyOfMeetings = 0;
        
        fileStat >> inputLine;
        TTag tagTmp;
        size_t qtyOfUniqWords = 0;
        fileStat >> tagTmp.qtyOfArt >> tagTmp.qtyOfWords >> qtyOfUniqWords;
        
        while (qtyOfUniqWords > 0) {
            fileStat >> uniqWord >> qtyOfMeetings;
            tagTmp.wordAndQty.emplace(uniqWord, qtyOfMeetings);
            --qtyOfUniqWords;
        }
        
        cloudOfTags.emplace(inputLine, tagTmp);
        fileStat.get();
    }
}


void addWordsInMap(map<string, size_t> &mapOfWords,
                   const string &inputLine)
{
    string tmp = "";
    for (size_t i = 0; i < inputLine.size(); ++i) {
        if (isalnum(inputLine[i])) {
            tmp += tolower(inputLine[i]);
        } else {
            if (tmp != "") {
                auto i = mapOfWords.find(tmp);
                if (i == mapOfWords.end()) {
                    mapOfWords.emplace(tmp, 1);
                } else {
                    ++i->second;
                }
            }
            tmp = "";
        }
    }
}


void classification(ifstream &fileIn,
                    ofstream &fileOut,
                    map<string, TTag> &cloudOfTags,
                    size_t &qtyOfAllArt,
                    size_t &qtyOfAllUniqWords)
{
    size_t numberOfLines;
    string inputLine;
    
    while (fileIn.peek() != EOF) {
        map<string, size_t> mapOfWords;
        vector<pair<long double, string> > finishHim;
        
        fileIn >> numberOfLines;
        fileIn.get();
        
        while (numberOfLines > 0) {
            getline(fileIn, inputLine);
            addWordsInMap(mapOfWords, inputLine);
            --numberOfLines;
        }
        
        for (auto &tag : cloudOfTags) {
            long double result = log((long double)(tag.second.qtyOfArt) / qtyOfAllArt);//P(S)
            long double curMinLogProbability = log(1.0 / (qtyOfAllUniqWords + (tag.second.qtyOfWords / 1.5))); // P(0|S)
            
            for (auto &word : mapOfWords) {
                auto check = tag.second.wordAndQty.find(word.first);
                if (check == tag.second.wordAndQty.end()) {
                    result += curMinLogProbability * word.second;
                } else {
                    result += log((long double)(check->second + 1) / (qtyOfAllUniqWords + (tag.second.qtyOfWords / 1.5))) * word.second;
                }
            }
            finishHim.emplace_back(result, tag.first);
        }
        
        long double sum = 0;
        for (size_t i = 0; i < finishHim.size(); ++i)  {
            finishHim[i].first = exp(finishHim[i].first);
            sum += finishHim[i].first;
        }
        for (size_t i = 0; i < finishHim.size(); ++i) {
            finishHim[i].first = finishHim[i].first / sum;
        }
        
        sort(finishHim.rbegin(), finishHim.rend());
        
        fileOut << "#" << finishHim[0].second;
        for (size_t i = 1; i < finishHim.size() && i <= MAX_QTY_TAGS && finishHim[i].first > MIN_PROBABILITY; ++i) {
            fileOut << ", #" << finishHim[i].second;
        }
        fileOut << "\n";
    }
}
