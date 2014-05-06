#include "informationGain.h"
#include <math.h>
#include <boost/shared_ptr.hpp>
#include "global.h"

double log2(double n)  
{    
  return log(n) / log(2.0);  
}

double entropy(shared_ptr<Node> node, unsigned int feature)
{
	double p[valuesPerFeature[k]];
	double P = 0; 
	
	for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
		p[i] = 0;
	
	for(vector<vector<unsigned int> *>::const_iterator it = node->examples.begin(); it != node->examples.end(); ++it)
  {
    	++p[(**it)[k]];
			++P;
	}

	double entropy = 0;
	if(P != 0)
		for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
			if(p[i] != 0)
				entropy -= (p[i]/P)*log2(p[i]/P);

	return entropy;
}

pair<unsigned int, double> ent(shared_ptr<Node> node, unsigned int feature, unsigned int value)
{
	double p[valuesPerFeature[k]];
	double P = 0; 
	
	for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
		p[i] = 0;
	
	for(vector<vector<unsigned int> *>::const_iterator it = node->examples.begin(); it != node->examples.end(); ++it)
  {
		if((**it)[feature] == value)
		{	
    	++p[(**it)[k]];
			++P;
		}  
	}

	double entropy = 0;
	if(P != 0)
		for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
			if(p[i] != 0)
				entropy -= (p[i]/P)*log2(p[i]/P);

	return make_pair(P,entropy);
}

pair<unsigned int, double> gini(shared_ptr<Node> node, unsigned int feature, unsigned int value)
{
	double p[valuesPerFeature[k]];
	double P = 0; 

	for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
		p[i] = 0;
	
	for(vector<vector<unsigned int> *>::const_iterator it = node->examples.begin(); it != node->examples.end(); ++it)
  {
		if((**it)[feature] == value)
		{	
    	++p[(**it)[k]];
			++P;
		}  
	}

	double giniIndex = 1;
	if(P != 0)
		for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
				giniIndex -= (p[i]/P)*(p[i]/P);

	return make_pair(P, giniIndex);
}

pair<unsigned int, double> me(shared_ptr<Node> node, unsigned int feature, unsigned int value)
{
	double p[valuesPerFeature[k]];
	double P = 0; 

	for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
		p[i] = 0;
	
	for(vector<vector<unsigned int> *>::const_iterator it = node->examples.begin(); it != node->examples.end(); ++it)
  {
		if((**it)[feature] == value)
		{	
    	++p[(**it)[k]];
			++P;
		}  
	}

	unsigned int N = 0;
	double mme = 1;
	if(P != 0)
		for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
			if(p[i] > N)
				mme = p[i];

	return make_pair(P, P - mme);
}

pair<unsigned int, double> R(shared_ptr<Node> node, unsigned int feature, unsigned int value)
{
	double p[valuesPerFeature[k]];
	double P = 0; 

	for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
		p[i] = 0;
	
	for(vector<vector<unsigned int> *>::const_iterator it = node->examples.begin(); it != node->examples.end(); ++it)
  {
		if((**it)[feature] == value)
		{	
    	++p[(**it)[k]];
			++P;
		}  
	}

	double giniIndex = 1;
	if(P != 0)
		for(unsigned int i = 0; i < valuesPerFeature[k]; ++i)
			if(p[i] != 0)
				giniIndex -= (p[i]/P)*(p[i]/P);

	return make_pair(P, P*P*giniIndex/2.0);
}

double sum(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value))
{
	double sum = 0;

	for(unsigned int i = 0; i < valuesPerFeature[feature]; ++i)	
		sum += (*uncertaintyFunction)(node, feature, i).second;

	return sum;
}

double max(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value))
{
	double uncertainty = 0;
	double max = 0;

	for(unsigned int i = 0; i < valuesPerFeature[feature]; ++i)	
		if( (uncertainty = (*uncertaintyFunction)(node, feature, i).second) > max )
			max = uncertainty;

	return max;
}

double w_sum(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value))
{
	double sum = 0;
	pair<unsigned int, double> uncertainty;

	for(unsigned int i = 0; i < valuesPerFeature[feature]; ++i)	
	{
		uncertainty = (*uncertaintyFunction)(node, feature, i);
		sum += uncertainty.second*uncertainty.first;
	}		

	return sum/node->examples.size();
}

double w_max(shared_ptr<Node> node, unsigned int feature, uncertainty (*uncertaintyFunction)(shared_ptr<Node> node, unsigned int feature, unsigned int value))
{
	double current = 0, max = 0;
	pair<unsigned int, double> uncertainty;

	for(unsigned int i = 0; i < valuesPerFeature[feature]; ++i)	
	{
		uncertainty = (*uncertaintyFunction)(node, feature, i);
		current = uncertainty.second*uncertainty.first;
		if(current > max)
			max = current;
	}	

	return max/node->examples.size();
}
