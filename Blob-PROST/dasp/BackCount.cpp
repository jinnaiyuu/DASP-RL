/*
 * BackCount.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#include "BackCount.hpp"
#include <random>
BackCount::BackCount(Parameters* param) {
	backSteps = param->getBackCount();
	randomizeBackCount = param->getRandomizeBackCount();
}

BackCount::~BackCount() {
}

ALEState BackCount::getInitState(std::vector<ALEState> trajectory) {
	printf("BackCount::getInitState\n");
	if (randomizeBackCount) {
		int r = rand() % trajectory.size();
		return trajectory[r];
	} else {
		if (trajectory.size() < backSteps) {
			printf("Initial State is used\n");
			return trajectory[0];
		} else {
			return trajectory[trajectory.size() - 10];
		}
	}
}
