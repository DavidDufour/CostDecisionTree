#include "Internal.h"
#include "global.h"

using namespace std;

//Randomly select a feature from top three features as determined by information gain
unsigned int Internal::randomFeature(unsigned int numberOfFeatures)
{
	vector<double> probabilities;

	if(features.size() < numberOfFeatures)
	{
		for(unsigned int i = 0; i < features.size(); ++i)
			probabilities.push_back(1.0/features.size());
	}
	else	
	{
		for(unsigned int i = 0; i < numberOfFeatures; ++i)
			probabilities.push_back(1.0/numberOfFeatures);
		for(unsigned int i = numberOfFeatures; i < features.size(); ++i)
			probabilities.push_back(0.0);
	}

	dist = discrete_distribution<>(probabilities.begin(), probabilities.end());

	unsigned int r = dist(gen);

	return orderedGains[r].second;
}

//effects of removing an internal node
void Internal::cut()
{
  --numberOfNodes;

	for(vector<shared_ptr<Node> >::iterator child = children.begin(); child != children.end(); ++child)
	{
		(*child)->cut();
  }

	children.clear();
}

//recursively print internal nodes and thier branches
void Internal::printTree(unsigned int d)
{
	for(vector<shared_ptr<Node> >::iterator child = children.begin(); child != children.end(); ++child)
	{
		cout << endl;
		for(unsigned int i = 0; i < d; ++i)
			cout << "|  ";
  	cout << (attributes[feature]).first << " = " << (attributes[feature]).second[(*child)->value];
		
		(*child)->printTree(d+1);
	}
}

//recursively save internal nodes of decision tree in C++ file
void Internal::recursiveSave(ofstream &treeWriter, unsigned int depth)
{
	for(vector<shared_ptr<Node> >::iterator it = children.begin(); it != children.end(); ++it)
	{
		for(unsigned int i = 0; i < depth; ++i)
			treeWriter << "  ";
		treeWriter << "  if(((*i)[" << feature << "]).compare(\"" << (attributes[feature]).second[(*it)->value] << "\") == 0)" << endl;
		for(unsigned int i = 0; i < depth; ++i)
			treeWriter << "  ";	
		treeWriter << "  {" << endl;

		//treeWriter.flush();
		
		(*it)->recursiveSave(treeWriter, depth + 1);
		
		//treeWriter.flush();

		for(unsigned int i = 0; i < depth; ++i)
			treeWriter << "  ";
		treeWriter << "  }" << endl;
	}
}

Internal::Internal(void)
{
}

Internal::~Internal(void)
{
}
