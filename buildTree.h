#ifndef buildTree_h__
#define buildTree_h__

#include "Internal.h"
#include <boost/shared_ptr.hpp>

typedef pair<double,vector<unsigned int> > gain;

int majorityClassificationOfParent(shared_ptr<Internal> node);

int majorityClassification(shared_ptr<Internal> node, unsigned int v);

bool hasExamples(shared_ptr<Internal> node, unsigned int v);

int isLeaf(shared_ptr<Internal> node, unsigned int v);

int buildTree(shared_ptr<vector<shared_ptr<Internal> > > open, unsigned int depth, double costOfAncestors, unsigned long long int maxAttempts, unsigned int numberOfFeatures);

#endif
