#ifndef global_h__
#define global_h__

#include <iostream>
#include <fstream>
#include <utility>
#include <limits>
#include <map>
#include <ctime>
#include <chrono>
#include <random>
#include <boost/shared_ptr.hpp>
#include "Internal.h"
#include "Leaf.h"

extern unsigned int m, k;

extern unsigned int numberOfNodes;
extern unsigned int upperBound;

extern double costOfTree;
extern double upperBoundCost;

extern time_t start;
extern unsigned int runTime;

extern mt19937 gen;
extern discrete_distribution<> dist;

extern string relationName;
extern vector<pair<string, vector<string> > > attributes;
extern vector<vector<unsigned int> > e;
extern vector<double> costs;

extern vector<unsigned int> valuesPerFeature;

bool compare (fgain i, fgain j);

void clearScreen();

#endif
