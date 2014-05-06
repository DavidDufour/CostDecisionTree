#include <unordered_map>
#include "buildTree.h"
#include "informationGain.h"
#include <boost/shared_ptr.hpp>
#include "global.h"

int majorityClassificationOfParent(shared_ptr<Internal> node)
{
	vector<int> classifications(valuesPerFeature[k], 0);

	for(vector<vector<unsigned int> *>::iterator it = node->examples.begin(); it != node->examples.end(); ++it)
		++classifications[(**it)[k]];

	int c = 0;
	for(unsigned int i = 1; i < valuesPerFeature[k]; ++i)
		if(classifications[i] > classifications[c])
			c = i;

	return c;			
}

int majorityClassification(shared_ptr<Internal> node, unsigned int v)
{
	vector<int> classifications(valuesPerFeature[k], 0);

	for(vector<vector<unsigned int> *>::iterator it = node->examples.begin(); it != node->examples.end(); ++it)
		if((**it)[node->feature] == v)
			++classifications[(**it)[k]];

	int c = 0;
	for(unsigned int i = 1; i < valuesPerFeature[k]; ++i)
		if(classifications[i] > classifications[c])
			c = i;

	return c;	
}

bool hasExamples(shared_ptr<Internal> node, unsigned int v)
{
	//look through the the examples to see if there is at least one with value v
	for(vector<vector<unsigned int> *>::iterator it = node->examples.begin(); it != node->examples.end(); ++it)
		if((**it)[node->feature] == v)
			return true;

	//no examples found, return false
	return false;
}

//check children of node, feature and value, returns the classification of the leaf or -1 if it isn't a leaf
int isLeaf(shared_ptr<Internal> node, unsigned int v)
{
	unsigned int classification;

	//looks through the examples to find the classification of the first example with value v for the feature
	vector<vector<unsigned int> *>::iterator it = node->examples.begin();
	for(;(**it)[node->feature] != v; ++it);

	//save the examples classification
	classification = (**it++)[k];

	//check the rest of the examples with that value to see if they all have the same classification
	for(; it != node->examples.end(); ++it)
		if((**it)[node->feature] == v)
			if( (**it)[k] != classification )
				return -1;
	
	//if it's a leaf then return what it's classified with
	return classification;
}

int buildTree(shared_ptr<vector<shared_ptr<Internal> > > open, unsigned int depth, double costOfAncestors, unsigned long long int maxAttempts, unsigned int numberOfFeatures)
{
	vector<unsigned int> combination;

	unsigned int numberOfChildren;

	unsigned long long int attempts = 0;

	while(difftime( time(0), start) < runTime && attempts < maxAttempts && depth < k)
	{
		//empty children of open
		for(vector<shared_ptr<Internal> >::iterator it = open->begin(); it != open->end(); ++it)
			(*it)->children.clear();

		//while(combination not tried)
		combination.clear();
		//randomly choose a feature for each node in open
		for(vector<shared_ptr<Internal> >::iterator it = open->begin(); it != open->end(); ++it)
			combination.push_back((*it)->randomFeature(numberOfFeatures));

		numberOfChildren = 0;
		for(vector<unsigned int>::iterator f = combination.begin(); f != combination.end(); ++f)
			numberOfChildren += valuesPerFeature[*f];		
		
		numberOfNodes += numberOfChildren;			

		shared_ptr<vector<shared_ptr<Internal> > > newOpen = make_shared<vector<shared_ptr<Internal> > >();

		//!!if number of examples in any child is less than 2, skip everything that follows and try a new combination
		
		unsigned int i = 0;
		for(vector<shared_ptr<Internal> >::iterator it = open->begin(); it != open->end(); ++it, ++i)
		{
			(*it)->feature = combination[i];

			costOfTree += costs[(*it)->feature] * ((*it)->examples.size()) / m;

			for(unsigned int j = 0; j < valuesPerFeature[(*it)->feature]; ++j)
			{
				int classification;
				if(!hasExamples(*it, j))
				{
					//create and classify a leaf
					shared_ptr<Leaf> node = make_shared<Leaf>();
					node->classification = majorityClassificationOfParent((*it));

					//put examples in node
					for(vector<vector<unsigned int> *>::const_iterator ex = (*it)->examples.begin(); ex != (*it)->examples.end(); ++ex)
						if((**ex)[(*it)->feature] == j)
							node->examples.push_back(*ex);

					node->value = j;

					(*it)->children.push_back(node);
				}
				else if((*it)->features.size() == 1)
				{
					//create and classify a leaf
					shared_ptr<Leaf> node = make_shared<Leaf>();
					node->classification = majorityClassification((*it), j);

					node->value = j;

					//put examples in node
					for(vector<vector<unsigned int> *>::const_iterator ex = (*it)->examples.begin(); ex != (*it)->examples.end(); ++ex)
						if((**ex)[(*it)->feature] == j)
							node->examples.push_back(*ex);

					(*it)->children.push_back(node);
				}
				else if((classification = isLeaf(*it, j)) != -1)
				{
					//create and classify a leaf
					shared_ptr<Leaf> node = make_shared<Leaf>();
					node->classification = classification;
					//dynamic_pointer_cast<Leaf>(node)->classification = classification;

					node->value = j;

					//put examples in node
					for(vector<vector<unsigned int> *>::const_iterator ex = (*it)->examples.begin(); ex != (*it)->examples.end(); ++ex)
						node->examples.push_back(*ex);

					(*it)->children.push_back(node);
				}
				else
				{
					//create an Internal node
					shared_ptr<Internal> node = make_shared<Internal>();

					node->value = j;

					//put examples in node
					for(vector<vector<unsigned int> *>::const_iterator ex = (*it)->examples.begin(); ex != (*it)->examples.end(); ++ex)
						if((**ex)[(*it)->feature] == j)
							node->examples.push_back(*ex);

					//add features sub the feature the parent had
					for(vector<unsigned int>::iterator fit = (*it)->features.begin(); fit != (*it)->features.end(); ++fit)
						if((*it)->feature != (*fit))
							dynamic_pointer_cast<Internal>(node)->features.push_back(*fit);

					
					//add gains associated with features
					double h;
					for(vector<unsigned int>::iterator fit = node->features.begin(); fit != node->features.end(); ++fit)
					{
						h = entropy(node,(*fit));
						//cout << "h: " << h << ", ";
						h = h - w_sum(node,(*fit),ent);
						//cout << "h - w_sum: " << h << ", ";
						h = h * h;
						//cout << "I^2: " << h << ", C: " << costs[*fit] << ", ";
						h = h / costs[*fit];
						//cout << "I^2/C: " << h / costs[*fit] << endl; 

						node->orderedGains.push_back(make_pair(h, (*fit)));
					}//cout << endl;
					sort(node->orderedGains.begin(), node->orderedGains.end(), compare);

					newOpen->push_back(node);
					(*it)->children.push_back(node);
				}
			}
		}

		if(
			(costOfTree > upperBoundCost) ||
			(costOfTree == upperBoundCost && numberOfNodes >= upperBound)
			)
		{
			++attempts;
			numberOfNodes -= numberOfChildren;
			costOfTree = costOfAncestors;
		}
		else if(newOpen->empty()) 
			return 0;
		else if(buildTree(newOpen, depth+1, costOfTree, maxAttempts, numberOfFeatures) == 0)
			return 0;
		else 
		{
			++attempts;
			numberOfNodes -= numberOfChildren;
			costOfTree = costOfAncestors;
		}
	}
	return -1;
}
