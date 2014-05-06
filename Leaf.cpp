#include "Leaf.h"
#include "global.h"

//effects of removing a leaf node
void Leaf::cut()
{
	--numberOfNodes;
}

//prints classification of examples in leaf
void Leaf::printTree(unsigned int d)
{
	if(classification == -1)
		cout << ": no class";
	else
	  cout << ": " << attributes[k].second[classification];
}

//output C++ code representing decision tree
void Leaf::recursiveSave(ofstream &treeWriter, unsigned int depth)
{
	for(unsigned int i = 0; i < depth; ++i)
		treeWriter << "  ";
	if(classification == -1)
		treeWriter << "  if(((*i)[" << k << "]).compare(\"no class\") == 0)" << endl;
	else
		treeWriter << "  if(((*i)[" << k << "]).compare(\"" << attributes[k].second[classification] << "\") == 0)" << endl;
	for(unsigned int i = 0; i < depth; ++i)
		treeWriter << "  ";
	treeWriter << "    ++correct;" << endl;
	for(unsigned int i = 0; i < depth; ++i)
		treeWriter << "  ";
	treeWriter << "  else" << endl;
	for(unsigned int i = 0; i < depth; ++i)
		treeWriter << "  ";
	treeWriter << "    ++incorrect;" << endl;
}

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}
