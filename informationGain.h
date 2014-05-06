#ifndef informationGain_h__
#define informationGain_h__

#include <math.h>
#include <boost/shared_ptr.hpp>
#include "Node.h"

typedef pair<unsigned int, double> uncertainty;

double log2(double n);
double entropy(shared_ptr<Node> node, unsigned int feature);

uncertainty ent(shared_ptr<Node> node, unsigned int feature, unsigned int value);
uncertainty gini(shared_ptr<Node> node, unsigned int feature, unsigned int value);
uncertainty me(shared_ptr<Node> node, unsigned int feature, unsigned int value);
uncertainty R(shared_ptr<Node> node, unsigned int feature, unsigned int value);

double sum(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value));
double max(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value));
double w_sum(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value));
double w_max(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value));

#endif
