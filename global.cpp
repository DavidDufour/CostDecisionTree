#include "global.h"
#include "Internal.h"
#include <boost/shared_ptr.hpp>

using namespace std;

unsigned int m, k;

unsigned int numberOfNodes = 0;
//upperbound on number of nodes in decision tree
unsigned int upperBound = numeric_limits<unsigned int>::max();

double costOfTree = 0;
//upperbound on expected cost of decision tree
double upperBoundCost = numeric_limits<double>::max();

time_t start = time(0);
unsigned int runTime = 60;//seconds

//used for randomly selecting attributes
mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
discrete_distribution<> dist;

//initial data to be processesd
string relationName;
vector<pair<string, vector<string> > > attributes;
vector<vector<unsigned int> > e;
vector<double> costs;

vector<unsigned int> valuesPerFeature;

bool compare (fgain i, fgain j) { return (i.first > j.first); }

void clearScreen()
{
  cout << "\033[2J";
  cout << "\033[0;0H";
}
