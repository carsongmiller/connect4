#include "neuralnet.h"
#include "host.h"
#include <list>
#include <cstdlib>

using namespace std;



/*
==================================================

Network Functions

==================================================
*/

Net::Net()
{
	fitness = NULL;
	generation = 0;
	species = 0;
	numNeurons = 1;
	addNeuron();
	whiteFlag = false;
	int heartX = 0;
	int heartY = 0;
}



Net::Net(int gen, int spec)
{
	fitness = NULL;
	generation = gen;
	species = spec;
	numNeurons = 1;
	addNeuron();
	whiteFlag = false;
	int heartX = 0;
	int heartY = 0;
}



void Net::addNeuron()
{
	neuronList.push_back(Neuron());
	iter = neuronList.end();
	iter--;
	int tempID = iter->ID;
	iter++;
	iter->ID = tempID+1;

	numNeurons++;
}



void Net::killNeuron(int p)
{
	iter = neuronList.begin();
	while (iter != neuronList.end())
	{
		if (iter->ID == p)
		{
			neuronList.erase(iter);
			break;
		}
		iter++;
	}
}



void Net::calcScores()
{
	for (int i = 0; i < w_; i++) //reset all scores to zero
		score[i] = 0;

	for (iter = neuronList.begin(); iter != neuronList.end(); iter++)
	{
		if (iter->checkActive)
		{
			score[iter->action]++;
		}
	}
}



int Net::decideMove() // returns column of move choice or -1 if no move chosen
{
	int best = 0;
	for (int i = 1; i < w_; i++)
	{
		if (score[i] > score[best])
			best = i;
	}

	if (score[best] > 0)
		return best;
	else 
		return -1;
}



int Net::calcFitness()
{

}



/*
==================================================

Neuron Functions

==================================================
*/

Neuron::Neuron() //creates a new neuron with 2 inputs
{
	input new1, new2;
	int temp;

	do
	{
		new1.c = (rand() % 2) * pow(-1, rand() % 2);
		new1.r = (rand() % 2) * pow(-1, rand() % 2);
		new1.condition = rand() % 3;
		//new1.status = false;

		new2.c = (rand() % 2) * pow(-1, rand() % 2);
		new2.r = (rand() % 2) * pow(-1, rand() % 2);
		new2.condition = rand() % 3;
		//new2.status = false;
	} while (new1.c != new2.c || new1.r != new2.r || new1.condition != new2.condition);

	inputList.push_back(new1);
	inputList.push_back(new2);
}



void Neuron::mutate()
{
	input newIn;
	do
	{
		newIn.c = (rand() % 4) * pow(-1, rand() % 2);
		newIn.r = (rand() % 4) * pow(-1, rand() % 2);
		newIn.condition = rand() % 3;
		//newIn.status = false;
	} while (checkInputRedund(newIn));

	inputList.push_back(newIn);
	numInputs++;
}



bool Neuron::checkInputRedund(input i) //checks whether an input is identical to any existing inputs to a neuron
{
	for (iter = inputList.begin(); iter != inputList.end(); iter++)
	{
		if (i.c == iter->c && i.r != iter->r && i.condition != iter->condition)
			return true;
	}

	return false;
}



bool Neuron::checkActive(int board[][w_], int heartR, int heartC)
{
	for (iter = inputList.begin(); iter != inputList.end(); iter++)
	{
		if (iter->condition != board[heartR + iter->r][heartC + iter->c])
			return false;
	}
	return true;
}