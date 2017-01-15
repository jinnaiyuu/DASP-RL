/****************************************************************************************
 ** Mathematical tools that may be useful across several applications, even outside ALE
 ** environment. This class is meant to be static since ideally this should only implement
 ** functions. Adding non-static objects should be further discussed before implemented.
 **
 ** Author: Marlos C. Machado
 ***************************************************************************************/

#ifndef MATHEMATICS_H
#define MATHEMATICS_H
#include "Mathematics.hpp"
#endif
#include <assert.h>
#include <cstdlib>
#include <random>

int Mathematics::argmax(std::vector<float> array) {
	assert(array.size() > 0);
	//Discover max value of the array:
	double max = array[0];
	for (unsigned int i = 0; i < array.size(); i++) {
		if (max < array[i]) {
			max = array[i];
		}
	}
	//We need to break ties, thus we save all
	//indices that hold the same max value:
	std::vector<int> indices;
	for (unsigned int i = 0; i < array.size(); i++) {
		if (fabs(array[i] - max) < 1e-10) {
			indices.push_back(i);
		}
	}
	assert(indices.size() > 0);
	//Now we randomly pick one of the best
	return indices[rand() % indices.size()];
}

int Mathematics::argmax(std::vector<float> array, std::mt19937* randAgent) {
	assert(array.size() > 0);
	//Discover max value of the array:
	double max = array[0];
	for (unsigned int i = 0; i < array.size(); i++) {
		if (max < array[i]) {
			max = array[i];
		}
	}
	//We need to break ties, thus we save all
	//indices that hold the same max value:
	std::vector<int> indices;
	for (unsigned int i = 0; i < array.size(); i++) {
		if (fabs(array[i] - max) < 1e-6) {
			indices.push_back(i);
		}
	}
	assert(indices.size() > 0);
	//Now we randomly pick one of the best
	return indices[(*randAgent)() % indices.size()];
}

int Mathematics::softmax(std::vector<float> array, float temperature,
		std::mt19937* randAgent) {
	assert(array.size() > 0);
	// https://lingpipe-blog.com/2009/06/25/log-sum-of-exponentials/

	// Normalize the maximum value to 0 to avoid exponential overflow
	double max = array[0];
	for (unsigned int i = 0; i < array.size(); i++) {
		if (max < array[i]) {
			max = array[i];
		}
	}

	std::vector<float> norm_array(array.size());
	double sum = 0;
	for (unsigned int i = 0; i < array.size(); i++) {
		norm_array[i] = exp((array[i] - max) / temperature);
		sum += norm_array[i];
	}

	for (unsigned int i = 0; i < array.size(); i++) {
		norm_array[i] /= sum;
	}

	return chooseFromProbability(norm_array, randAgent);
}

int Mathematics::chooseFromProbability(std::vector<float> array,
		std::mt19937* randAgent) {
	double sumOfWeights = std::accumulate(array.begin(), array.end(), 0.0);
	std::uniform_real_distribution<double> unif(0, sumOfWeights);
	double rand = unif(*randAgent);

	printf("sum=%.1f, rand=%.1f\n", sumOfWeights, rand);
	double accum = 0.0;
	for (int i = 0; i < array.size(); ++i) {
		accum += array[i];
		printf("accum=%.1f, array[%d]=%.1f\n", accum, i, array[i]);
		if (rand <= accum) {
			return i;
		}
	}
	return (*randAgent)() % array.size();
}
