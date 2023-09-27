#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



//_____________________________________Tools_______________________________________


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





//_____________________________________Main________________________________________
int main(int argc, const char* argv[])
{

	//______________________def Const_________________________________
	

	srand(time(NULL));

	static const int numInputs = 2;
	static const int numHiddenNodes = 2;
	static const int numOutputs = 1;
	
	int epochs = strtol(argv[1], NULL, 10);

	const double lr = 0.1f;

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
			hiddenWeights[i][j] = init_weight();
		}
	}


	for (int i = 0; i < numInputs; i++)
	{
		hiddenLayerBias[i] = init_weight();
		for (int j = 0; j < numHiddenNodes; j++)
		{
			outputWeights[i][j] = init_weight();
		}
	}

	for (int i = 0; i < numOutputs; i++)
	{
		outputLayerBias[i] = init_weight();
	}






	//______________________Training__________________________________

	int trainingSetOrder[] = {0,1,2,3};
	
	for (int n = 0; n < epochs; n++)
	{
		shuffle(trainingSetOrder, numTrainingSets);

		for (int x = 0; x < numTrainingSets; x++)
		{
			int i = trainingSetOrder[x];

			// Forward propagation
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

			// Backwards propagation
			double deltaOutput[numOutputs];
			for (int j = 0; j < numOutputs; j++)
			{
				double errorOutput = (training_outputs[i][j] - outputLayer[j]);
				deltaOutput[j] = errorOutput * dSigmoid(outputLayer[j]);
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
				outputLayerBias[j] += deltaOutput[j] * lr;
				for (int k = 0; k < numHiddenNodes; k++)
				{
					outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
				}
			}

			for (int j = 0; j < numHiddenNodes; j++)
			{
				hiddenLayerBias[j] += deltaHidden[j] * lr;
				for (int k = 0; k < numInputs; k++)
				{
					hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
				}
			}
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
		printf("input: %lf & %lf  ->  %lf", training_inputs[i][0], training_inputs[i][1], outputLayer[0]);
	}

	return EXIT_SUCCESS;
}

