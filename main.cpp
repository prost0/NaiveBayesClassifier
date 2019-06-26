#include "tags.h"
#include "learn.h"
#include "classify.h"


#define USAGE "Usage 1: prog learn --input\
 <input file> --output <output file>\n\
Usage 2: prog classify\
--stats <file with statistics>\
--input <input file> --output <output file>\n"

const int MIN_QTY_OF_ARGS = 5;


int main(int argc, char const *argv[])
{
    if (argc < MIN_QTY_OF_ARGS) {
        cerr << "Error\nInvalid quantity of arguments\n" << USAGE;
        exit(1);
    }
    
    if (!strcmp(argv[1], "learn")) {
        learn(argc, argv);
    } else if (!strcmp(argv[1], "classify")) {
        classify(argc, argv);
    } else {
        cerr << "Error\n" << USAGE;
        exit(2);
    }
    
    return 0;
}