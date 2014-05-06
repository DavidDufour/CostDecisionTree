#ifndef Node_h__
#define Node_h__

#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;

typedef pair<double, unsigned int> fgain; //information gain, feature

class Node
{
  public:
    //subset of examples from it's parents
    vector<vector<unsigned int> *> examples;

		unsigned int value;

    virtual void cut();
    virtual void printTree(unsigned int d);
		virtual void recursiveSave(ofstream &treeWriter, unsigned int depth);

	  Node();
	  virtual ~Node();
};

#endif
