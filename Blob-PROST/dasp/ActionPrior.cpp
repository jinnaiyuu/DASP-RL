/*
 * ActionPrior.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#include "ActionPrior.hpp"
#include "Dasp.hpp"

ActionPrior::ActionPrior(ALEInterface& ale, Parameters* param) :
		ale(ale), param(param) {
	initialStrategy = param->getInitialStrategy();
	adaptiveStrategy = param->getAdaptiveStrategy();
	triggerStrategy = param->getTriggerStrategy();
	initStateStrategy = param->getInitStateStrategy();

	planning_episodes = param->getPlanningEpisodes();
	steps_per_planning = param->getStepsPerPlanning();
	search_method = param->getSearchMethod();

//	this->ale = ale;
//	this->param = param;
}

ActionPrior::~ActionPrior() {
}

std::vector<double> ActionPrior::initialPruning() {
	ALEState initState = ale.cloneState();
	return runPruning(initState);
}

std::vector<double> ActionPrior::runPruning(ALEState initState) {
	std::vector<double> prior;
	if (initialStrategy == "DASP") {
		Dasp* dasp = new Dasp(ale, this->param);
		std::vector<std::vector<bool>> actionSeqSet =
				dasp->getMinimalActionSequenceSet(1, 1,
						param->getStepsPerPlanning(), initState);
		delete dasp;
//    	actions = actionSeqSet[0];
		printf("actionSeqSet.size()=%lu\n", actionSeqSet.size());

		assert(actionSeqSet.size() == 1);
		for (int i = 0; i < actionSeqSet.size(); ++i) {
			printf("actionSeqSet[%d].size()=%lu\n", i, actionSeqSet[i].size());
			for (int j = 0; j < actionSeqSet[i].size(); ++j) {
				if (actionSeqSet[i][j]) {
					printf("o");
					prior.push_back(1.0);
				} else {
					printf("x");
					prior.push_back(0.0);
				}
			}
			printf("\n");
		}
		currentPrior = prior;
		return prior;
	} else if (initialStrategy == "DASA") {
		printf("DASA TODO implement\n");
	} else {
		printf("unknown initialStrategy %s\n", initialStrategy.c_str());
	}
}

std::vector<double> ActionPrior::adaptivePruning() {
	if (!needAdaptivePruning()) {
		return currentPrior;
	}

	ALEState initNode = selectInitState();

	return currentPrior;
}

bool ActionPrior::needAdaptivePruning() {
	if (triggerStrategy == "10episodes") {
		return true;
	} else {
		return true;
	}
}
ALEState ActionPrior::selectInitState() {
	return ale.cloneState();
}
