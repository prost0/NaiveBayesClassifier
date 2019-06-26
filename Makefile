naiveBayesClassifier : main.o learn.o classify.o
	g++ -Wall -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o naiveBayesClassifier main.o learn.o classify.o
main.o : main.cpp tags.h learn.h classify.h
	g++ -Wall -O2 -c -g -std=c++11 main.cpp
learn.o : learn.cpp tags.h
	g++ -Wall -O2 -c -g -std=c++11 learn.cpp
classify.o : classify.cpp tags.h
	g++ -Wall -O2 -c -g -std=c++11 classify.cpp
clear :
	rm *.o naiveBayesClassifier