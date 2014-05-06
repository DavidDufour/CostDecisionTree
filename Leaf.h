#ifndef Leaf_h__
#define Leaf_h__

#include <iostream>
#include <fstream>
#include "Node.h"
#include "Internal.h"

using namespace std;

class Leaf :
	public Node
{
  public:
		shared_ptr<Internal> parent;

		int classification;
		
		void cut();

		void printTree(unsigned int d);

		void recursiveSave(ofstream &treeWriter, unsigned int depth);

		Leaf();
		~Leaf();
};

#endif

