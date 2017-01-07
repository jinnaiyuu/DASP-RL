/*
 * ActionPrior.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#include "ActionPrior.hpp"
#include <algorithm>
#include "TriggerStrategy.hpp"
#include "EpisodeCount.hpp"

ActionPrior::ActionPrior(ALEInterface& ale, Parameters* param) :
		ale(ale), param(param) {
	initialStrategy = param->getInitialStrategy();
	adaptiveStrategy = param->getAdaptiveStrategy();
	triggerStrategy = param->getTriggerStrategy();
	initStateStrategy = param->getInitStateStrategy();

	planning_episodes = param->getPlanningEpisodes();
	steps_per_planning = param->getStepsPerPlanning();
	search_method = param->getSearchMethod();

	printStrategies();

	// Initializing Pruning Strategy
	if (initialStrategy == "DASP" || adaptiveStrategy == "DASP_CUP") {
		dasp = new Dasp(ale, this->param);
	}

	// Initializing trigger strategy
	if (triggerStrategy == "EPISODE_COUNT") {
		printf("Episodes count strategy with %d steps\n",
				param->getEpisodeCount());
		trigger = new EpisodeCount(param);
	} else {
		printf("unknown triggerStrategy: %s", triggerStrategy.c_str());
		trigger = new EpisodeCount(param);
	}

}

ActionPrior::~ActionPrior() {
}

std::vector<double> ActionPrior::initialPruning() {
	ALEState initState = ale.cloneState();
	return runPruning(initState, initialStrategy);
}

std::vector<double> ActionPrior::runDasp(ALEState initState) {
	std::vector<double> prior;
	std::vector<std::vector<bool>> actionSeqSet =
			dasp->getMinimalActionSequenceSet(1, 1,
					param->getStepsPerPlanning(), initState);

	assert(actionSeqSet.size() == 1);
	for (int i = 0; i < actionSeqSet.size(); ++i) {
		for (int j = 0; j < actionSeqSet[i].size(); ++j) {
			if (actionSeqSet[i][j]) {
				prior.push_back(1.0);
			} else {
				prior.push_back(0.0);
			}
		}
	}
	return prior;
}

std::vector<double> ActionPrior::runPruning(ALEState initState,
		std::string strategy) {
	std::vector<double> prior;
	if (strategy == "DASP") {
		prior = runDasp(initState);
	} else if (strategy == "DASP_CUP") {
		prior = runDasp(initState);
		for (int i = 0; i < currentPrior.size(); ++i) {
			prior[i] = std::max(prior[i], currentPrior[i]);
			if (prior[i] > currentPrior[i]) {
				printf("REVIVED action %d from preemptive pruning!\n", i);
			}
		}
	} else {
		printf("unknown initial/AdaptiveStrategy %s\n", strategy.c_str());
	}

	for (int i = 0; i < prior.size(); ++i) {
		if (prior[i]) {
			printf("o");
		} else {
			printf("x");
		}
	}
	printf("\n");
	currentPrior = prior;
	return prior;
}

std::vector<double> ActionPrior::adaptivePruning(
		std::vector<ALEState> trajectory, double reward) {
	if (!needAdaptivePruning(reward)) {
		printf("not triggered\n");
		return currentPrior;
	}
	printf("triggered\n");

	ALEState initState = selectInitState(trajectory);
	printf("selected initState: %d frame\n", initState.getEpisodeFrameNumber());

	return runPruning(initState, adaptiveStrategy);
}

bool ActionPrior::needAdaptivePruning(double reward) {
	return trigger->trigger(reward);
}

ALEState ActionPrior::selectInitState(std::vector<ALEState> trajectory) {
	if (initStateStrategy == "10_STEPS_BACK") {
		if (trajectory.size() < 10) {
			return trajectory[0];
		} else {
			return trajectory[trajectory.size() - 10];
		}
	} else if (initStateStrategy) {
		printf("unknown initStateStrategy: %s", initStateStrategy.c_str());
		return trajectory.back();
	}
}

void ActionPrior::printStrategies() {
	printf("Action Prior Strategies: \n");
	printf("\tinitialStrategy: %s\n", initialStrategy.c_str());
	printf("\tadaptiveStrategy: %s\n", adaptiveStrategy.c_str());
	printf("\tinitStateStrategy: %s\n", initStateStrategy.c_str());
	printf("\ttriggerStrategy: %s\n", triggerStrategy.c_str());
}
