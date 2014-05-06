#include <boost/shared_ptr.hpp>
#include "buildTree.h"
#include "informationGain.h"
#include "ArffReader.h"
#include "saveTree.h"
#include "Internal.h"
#include "Leaf.h"
#include "global.h"

using namespace std;

int main(int argc, char* argv[])
{
	arffReader(argv[1]);

	m = e.size();
	k = e[0].size() - 1;

	ifstream costFile;
  costFile.open(argv[2]);
  if (costFile.is_open())
  {
		double cost;
    while ( costFile >> cost )
      costs.push_back(cost);
    costFile.close();
  }
	//for(int z = 0; z < 4; ++z)
	//	cout << costs[z] << " ";
	//cout << endl;

	if(m < 1 || k < 2)
	{
		cout << "Not enough examples or attributes." << endl;
		return -1;
	}

	//check if all the examples are already classified the same
	unsigned int c;
	for(c = 1; c < m && e[0][k] == e[c][k]; ++c);
	if(c == m)
	{
		shared_ptr<Leaf> root = make_shared<Leaf>();

		numberOfNodes = 1;

		root->classification = e[0][k];

		for(unsigned int i = 0; i < m; ++i)
			root->examples.push_back(&e[i]);

		root->printTree(0);
		root.reset();
		return 0;
	}

	shared_ptr<vector<shared_ptr<Internal> > > open = make_shared<vector<shared_ptr<Internal> > >();

	shared_ptr<Internal> root = 0;
	shared_ptr<Internal> bestTree = 0;

	unsigned int numberOfFeatures = 1;
	unsigned long long int attempts = 1;
	while(difftime( time(0), start) < runTime && upperBound > 3)
	{
		numberOfNodes = 1;
		costOfTree = 0;

		root.reset();
		root = make_shared<Internal>();

		for(unsigned int i = 0; i < m; ++i)
			root->examples.push_back(&e[i]);

		for(unsigned int i = 0; i < k; ++i)
			root->features.push_back(i);

		double h;
		for(vector<unsigned int>::iterator it = root->features.begin(); it != root->features.end(); ++it)
		{
			h = entropy(root,(*it));
			//cout << "h: " << h << ", ";
			h = h - w_sum(root,(*it),ent);
			//cout << "h - w_sum: " << h << ", ";
			h = h * h;
			//cout << "I^2: " << h << ", C: " << costs[*it] << ", ";
			h = h / costs[*it];
			//cout << "I^2/C: " << h / costs[*it] << endl; 

			root->orderedGains.push_back(make_pair(h, (*it)));
		}//cout << endl;
		sort(root->orderedGains.begin(), root->orderedGains.end(), compare);

		open->clear();
		open->push_back(root);

		if(buildTree(open, 0, costOfTree, attempts, numberOfFeatures) == -1)
		{
			//cout << "Random search complete" << endl;
			attempts = attempts + 1;
		}
		else
		{
			upperBound = numberOfNodes;
			upperBoundCost = costOfTree;
			cout << difftime( time(0), start) << " - Tree of cost: " << upperBoundCost << " Size: " << upperBound << endl;
			if(bestTree)
				bestTree->cut();
			bestTree = root;
		}

		if(numberOfFeatures != 3)
			numberOfFeatures = 3;
	}
	
	cout << "Tree with cost " << upperBoundCost << " found." << endl;
	if(bestTree != 0)
	{
		bestTree->printTree(0);
		cout << endl;

		saveTree(bestTree, argv[3]);

		bestTree->cut();
		bestTree.reset();
	}	
	else
		cout << "No tree found." << endl;

	return 0;
}

