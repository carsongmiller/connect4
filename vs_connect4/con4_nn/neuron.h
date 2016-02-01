#ifndef NEURON_H
#define NEURON_H
#include <list>

using namespace std;

class Neuron
{
	struct input
	{
		int r, c;				// row and column location of input (relative to heart)
		int condition;			// state of the cell that would cause activation
		bool isActive;			// whether the input is active or not
	};

	list<input> in;				// list of inputs to neuron
	int heartR, heartC;			// heart input row and column
	int action;					// col to be played in if neuron activated
	void neuronMutate();		// slightly mutate the neuron
	int effectiveness;			// how often this neuron causes a positive move to be made
	int numInputs;				// how many inputs the neuron has
};

class Net
{
	list<Neuron> neuron;		// lsit of neurons in network
	int fitnes;					// fitness of particular network
	int generation;				// generation number
	int species;				// species number in given generation
	int numNeurons;				// number of neurons in the network
	void addNeuron();			// add a new neuron to the network
								// new neurons always consist only of a heart input
	bool whiteFlag;				// true if network has no active neurons
};

#endif