#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>



//_____________________________________Tools_______________________________________


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
	return fabs(cos(2*3.14159265358979*u2))*sqrt(-2*log(u1));
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


double sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double dSigmoid(double x)
{
	return x * (1 - x);
}

double init_weight()
{
	return ( (double)rand()) / ( (double)RAND_MAX);
}

void shuffle(int *array, size_t n)
{
	if(n > 1)
	{
		for(size_t i = 0; i < n -1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			int a = array[j];
			array[j] = array[i];
			array[i] = a;
		}
	}
}


//_____________________________________Propagation_________________________________

void fProp(double* layer, int layerLen, double layerWeights[][layerLen], double* layerBias, double* prevLayer, int prevLayerLen)
{
	for (int j  = 0; j < layerLen; j++)
	{
		double activation = layerBias[j];
		for (int k = 0; k < prevLayerLen; k++)
		{
			activation += prevLayer[k] * layerWeights[k][j];
		}
		layer[j] = sigmoid(activation);
	}

}



//_____________________________________Main________________________________________
int main(int argc, const char* argv[])
{

	if (argc != 1)
		errx(1, "Need (1) argumment : number of training epochs (>=10)");


	//______________________def Const_________________________________
	

	srand(time(NULL)-clock());

	static const int numInputs = 2;
	static const int numHiddenNodes = 8;
	static const int numOutputs = 1;
	
	int epochs = strtol("10000000", NULL, 10);

	const double lr = 0.5f;
	const double momentum = 0.0f;

	double hiddenLayer[numHiddenNodes];
	double outputLayer[numOutputs];

	double hiddenLayerBias[numHiddenNodes];
	double outputLayerBias[numOutputs];

	double hiddenWeights[numInputs][numHiddenNodes];
	double outputWeights[numHiddenNodes][numOutputs];

	static const int numTrainingSets = 4;

	double training_inputs[][2] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f }
	};

	const double training_outputs[][1] = {
		{ 0.0f },
		{ 1.0f },
		{ 1.0f },
		{ 0.0f },
	};
	
	
	//______________________Init Params_______________________________
	
	for (int i = 0; i < numInputs; i++)
	{
		for (int j = 0; j < numHiddenNodes; j++)
		{
			hiddenWeights[i][j] = normalRand()/sqrt(2);
		}
	}


	for (int i = 0; i < numInputs; i++)
	{
		hiddenLayerBias[i] = normalRand();
		for (int j = 0; j < numHiddenNodes; j++)
		{
			outputWeights[i][j] = normalRand()/sqrt(numHiddenNodes);
		}
	}

	for (int i = 0; i < numOutputs; i++)
	{
		outputLayerBias[i] = normalRand();
	}





	//______________________Training__________________________________

	int trainingSetOrder[] = {0,1,2,3};
	
	for (int n = 0; n < epochs; n++)
	{
		shuffle(trainingSetOrder, numTrainingSets);

		for (int x = 0; x < numTrainingSets; x++)
		{
			int i = trainingSetOrder[x];

			// Propagate forward from input to hidden layer then from hidden layer to ouput

			fProp(hiddenLayer, numHiddenNodes, hiddenWeights, hiddenLayerBias, training_inputs[i], numInputs);
			fProp(outputLayer, numOutputs, outputWeights, outputLayerBias, hiddenLayer, numHiddenNodes);


			// Backwards propagation
			double deltaOutput[numOutputs];
			for (int j = 0; j < numOutputs; j++)
			{
				double errorOutput = (training_outputs[i][j] - outputLayer[j]);
				deltaOutput[j] = errorOutput * dSigmoid(outputLayer[j]) * 2;
			}

			double deltaHidden[numHiddenNodes];
			for (int j = 0; j < numHiddenNodes; j++)
			{
				double errorHidden = 0.0f;
				for (int k = 0; k < numOutputs; k++)
				{
					errorHidden += deltaOutput[j] * outputWeights[j][k];
				}
				deltaHidden[j] = errorHidden * dSigmoid(hiddenLayer[j]);
			}
			
			for (int j = 0; j < numOutputs; j++)
			{
				outputLayerBias[j] += deltaOutput[j] * lr + outputLayerBias[j] * momentum;
				for (int k = 0; k < numHiddenNodes; k++)
				{
					outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr + outputWeights[k][j] * momentum;
				}
			}

			for (int j = 0; j < numHiddenNodes; j++)
			{
				hiddenLayerBias[j] += deltaHidden[j] * lr + hiddenLayerBias[j] * momentum;
				for (int k = 0; k < numInputs; k++)
				{
					hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr + hiddenWeights[k][j] * momentum;
				}
			}
		}
		if (n%(epochs/10) == 0)
		{
			printf("Epoch: %d\n", n);
			for (int i = 0; i < numTrainingSets; i++)
			{
				fProp(hiddenLayer, numHiddenNodes, hiddenWeights, hiddenLayerBias, training_inputs[i], numInputs);
				fProp(outputLayer, numOutputs, outputWeights, outputLayerBias, hiddenLayer, numHiddenNodes);
				printf("\tinput: %.0lf & %.0lf  ->  %lf\n", training_inputs[i][0], training_inputs[i][1], outputLayer[0]);
			}
			printf("\n\n");
		}
	}
	
	for (int i = 0; i < numTrainingSets; i++)
	{
		for (int j  = 0; j < numHiddenNodes; j++)
		{
			double activation = hiddenLayerBias[j];
			for (int k = 0; k < numInputs; k++)
			{
				activation += training_inputs[i][k] * hiddenWeights[k][j];
			}
			hiddenLayer[j] = sigmoid(activation);
		}

		for (int j  = 0; j < numOutputs; j++)
		{
			double activation = outputLayerBias[j];
			for (int k = 0; k < numHiddenNodes; k++)
			{
				activation += hiddenLayer[k] * outputWeights[k][j];
			}
			outputLayer[j] = sigmoid(activation);
		}
		printf("input: %.0lf & %.0lf  ->  %lf\n", training_inputs[i][0], training_inputs[i][1], outputLayer[0]);
	}

	return EXIT_SUCCESS;
}

