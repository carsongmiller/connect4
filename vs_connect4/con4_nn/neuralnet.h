#ifndef NEURALNET_H
#define NEURALNET_H
#include <list>
#include "host.h"

using namespace std;



struct input
{
	int r, c;				// row and column location of input (relative to current heart)
	int condition;			// state of the cell that would cause activation
	//int status;
};



class Neuron
{
public:
	list<input> inputList;		// list of inputs to neuron
	list<input>::iterator iter;	//iterator to 
	int action;					// col to be played in if neuron activated
	void mutate();				// slightly mutate the neuron
	//int effectiveness;			// how often this neuron causes a positive move to be made
	int numInputs;				// how many inputs the neuron has
	int ID;

	Neuron();
	bool checkInputRedund(input i);
	bool checkActive(int board[][w_],  int heartR, int heartC);
};



class Net
{
public:
	list<Neuron> neuronList;	// list of neurons in network
	list<Neuron>::iterator iter;
	int fitness;					// fitness of particular network
	int generation;				// generation number
	int species;				// species number in given generation
	int numNeurons;				// number of neurons in the network
	bool whiteFlag;				// true if network has no active neurons
	int heartR, heartC;			//reference point for testing neurons (will cycle through all cells)
	int score[w_];

	Net();
	Net(int gen, int spec);
	void addNeuron();			// add a new neuron to the network.  new neurons always consist only of a heart input
	int calcFitness();
	void killNeuron(int p);
	void calcScores();
	int decideMove();
};

#endif