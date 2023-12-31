#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <err.h>


//________________________________Tools________________________________________

//_______Activation Functions_________

void sigmoid(double* z, double* activations, int layer, int* topography)
{
	// Apply the sigmoid function to the neuron's ouputs of the layer specified
	// And store it in activations

	int neurons = 0;
	int currentLayer = 0;
	while (currentLayer < layer)
	{
		neurons += topography[currentLayer];
		currentLayer++;
	}

	int i = 0;
	while (i < topography[layer])
	{
		activations[neurons + i] = 1/(1+exp(-z[neurons + i]));
		i++;
	}
}

void sigmoid_prime(double* z, double* activations, int layer, int* topography)
{
	// Apply the derivative of the sigmoid function
	// to the neuron's ouputs of the layer specified
	// And store it in activations

	int neurons = 0;
	int currentLayer = 0;
	while (currentLayer < layer)
	{
		neurons += topography[currentLayer];
		currentLayer++;
	}

	int i = 0;
	while (i < topography[layer])
	{
		activations[neurons + i] = (1/(1+exp(-z[neurons + i]))) * 
				      (1 - (1/(1+exp(-z[neurons + i]))));
		i++;
	}
}

//_______Utils________________________

void printArr(double* arr, int len)
{
	// Pretty prints array of double
	for (int i = 0; i < len; i++)
	{
		printf("%lf - ", arr[i]);
	}
	printf("\n");
}

double randomGen()
{
	// Returns uniformly distributed random number
	return ((double)rand() + 1)/((double)RAND_MAX + 1);
}

double normalRand()
{
	// Return normally distributed numbers
	double u1 = randomGen();
	double u2 = randomGen();
	return cos(2*3.1415*u2)*sqrt(-2*log(u1));
}

void initWeights(double* weights, int len, int* topography)
{
	// Initializes the weights depending on the number of neurons in the
	// layer they come from with normally distributed random numbers with
	// mean 0 and standard deviation of 1/sqrt(n) with n number of neurons
	// in the previous layer

	int layer = 0;
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (topography[layer] == j)
		{
			layer++;
			j = 0;
		} else {
			j++;
		}
		weights[i] = normalRand()/sqrt(topography[layer]);
	}
}

void initBiases(double* biases, int len)
{
	// Initializes the biases with normally distributed numbers

	for (int i = 0; i < len; i++)
	{
		biases[i] = normalRand();
	}
}

int getNumNeurons(int* topography, int len)
{
	// Get the total number of neurons in a Neural Network
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum += topography[i];
	}
	return sum;
}

void initArr(double* arr, int len, double number)
{
	// Initializes the array with the number number
	
	for (int i = 0; i < len; i++)
	{
		arr[i] = number;
	}
}

//________________________________Propagations_________________________________

void dotProduct(double* weights, double* z, double* activations, int layer, int* topography)
{
	
	int neurons = 0;
	int currentLayer = 0;
	while (currentLayer < layer)
	{
		neurons += topography[currentLayer];
		currentLayer++;
	}
	
	int i = 0;
	while (i < topography[layer])
	{

		i++;
	}
}

void dotsum()
{
	return;
}

void forwardProp(double* weights)
{
	return;
}	


//________________________________Main Program_________________________________

int main(int argc, char** argv)
{
	if (argc != 1)
	{
		errx(0, "Need zero argument : %d given !\n", argc-1);
	}
	//____________________________Params Init______________________________
	
	// Initialize random number generator
	
	 
	srand(time(NULL));	 


	//____________________________Main Execution___________________________


	//____________________________Tests____________________________________

	int topography[] = {2,2,1};
	int numNeurons = getNumNeurons(topography, sizeof(topography));
	printf("%d\n", numNeurons);
	double z[numNeurons];
	double activations[numNeurons];
	initArr(z, numNeurons, 1); 
	initArr(activations, numNeurons, 0);
	printArr(z, numNeurons);
	printArr(activations, numNeurons);
	sigmoid(z, activations, 2, topography);
	printArr(activations, numNeurons);

	int topography2[] = {100,1,1,1,1,1,1,1,1,1};
	double w[10] = {0};
	initWeights(w, 10, topography2);
	printArr(w, 10);


	return EXIT_SUCCESS;
}
