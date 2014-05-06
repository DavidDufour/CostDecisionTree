#ifndef Internal_h__
#define Internal_h__

#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Node.h"

class Internal :
	public Node
{
public:
  unsigned int feature;
  //ordered list of features and their information gain
  vector<fgain> orderedGains;

  //list of features that it's children can have
  vector<unsigned int> features;

  shared_ptr<Internal> parent;    
  vector<shared_ptr<Node> > children;

	unsigned int randomFeature(unsigned int numberOfFeatures);

  void cut();

  void printTree(unsigned int d);

	void recursiveSave(ofstream &treeWriter, unsigned int depth);

	Internal();
	~Internal();
};

#endif
