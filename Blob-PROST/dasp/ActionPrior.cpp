/*
 * ActionPrior.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#include "ActionPrior.hpp"
#include <algorithm>
#include "EpisodeCount.hpp"
#include "BackCount.hpp"

ActionPrior::ActionPrior(ALEInterface& ale, Parameters* param) :
		ale(ale), param(param) {
	initialStrategy = param->getInitialStrategy();
	adaptiveStrategy = param->getAdaptiveStrategy();
	triggerStrategy = param->getTriggerStrategy();
	initStateStrategy = param->getInitStateStrategy();

	planning_episodes = param->getPlanningEpisodes();
	init_steps_per_planning = param->getStepsPerPlanning();
	search_method = param->getSearchMethod();

	adaptive_steps_per_planning = param->getAdaptiveStepsPerPlanning();

	printStrategies();

	// Initializing Pruning Strategy
	if (initialStrategy == "DASP") {
		initialPrior = new Dasp(ale, this->param);
	} else {
		printf("no initialStrategy: %s\n", initialStrategy.c_str());
	}

	if (adaptiveStrategy == "DASP" || adaptiveStrategy == "DASP_CUP") {
		adaptivePrior = new Dasp(ale, this->param);
	} else {
		printf("no adaptiveStrategy: %s\n", adaptiveStrategy.c_str());
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

	if (initStateStrategy == "BACK_COUNT") {
		initSelector = new BackCount(param);
	} else {
		printf("unknown initStateStrategy: %s\n", initStateStrategy.c_str());
		initSelector = new BackCount(param);
	}
}

ActionPrior::~ActionPrior() {
}

std::vector<double> ActionPrior::initialPruning() {
	ALEState initState = ale.cloneState();
//	return runPruning(initState, initialStrategy);
	currentPrior = initialPrior->getPrior(initState, init_steps_per_planning);
	return currentPrior;
}

//std::vector<double> ActionPrior::runDasp(ALEState initState) {
//	return dasp->getPrior(initState, param->getStepsPerPlanning());
//}

//std::vector<double> ActionPrior::runPruning(ALEState initState,
//		std::string strategy) {
//	std::vector<double> prior;
//	if (strategy == "DASP") {
//		prior = runDasp(initState);
//	} else if (strategy == "DASP_CUP") {
//		prior = runDasp(initState);
//		for (int i = 0; i < currentPrior.size(); ++i) {
//			prior[i] = std::max(prior[i], currentPrior[i]);
//			if (prior[i] > currentPrior[i]) {
//				printf("REVIVED action %d from preemptive pruning!\n", i);
//			}
//		}
//	} else {
//		printf("unknown initial/AdaptiveStrategy %s\n", strategy.c_str());
//	}
//
//	for (int i = 0; i < prior.size(); ++i) {
//		if (prior[i]) {
//			printf("o");
//		} else {
//			printf("x");
//		}
//	}
//	printf("\n");
//	currentPrior = prior;
//	return prior;
//}

std::vector<double> ActionPrior::adaptivePruning(
		std::vector<ALEState> trajectory, double reward) {
	if (!needAdaptivePruning(reward)) {
		printf("not triggered\n");
		return currentPrior;
	}
	printf("triggered\n");

	ALEState initState = selectInitState(trajectory);
	printf("selected initState: %d frame\n", initState.getEpisodeFrameNumber());

	std::vector<double> prior = adaptivePrior->getPrior(initState,
			adaptive_steps_per_planning);
	if (adaptiveStrategy == "DASP_CUP") {
		for (int i = 0; i < currentPrior.size(); ++i) {
			prior[i] = std::max(prior[i], currentPrior[i]);
			if (prior[i] > currentPrior[i]) {
				printf("REVIVED action %d from preemptive pruning!\n", i);
			}
		}
	}
	currentPrior = prior;
	return currentPrior;
}

bool ActionPrior::needAdaptivePruning(double reward) {
	assert(trigger != nullptr);
	return trigger->trigger(reward);
}

ALEState ActionPrior::selectInitState(std::vector<ALEState> trajectory) {
	assert(initSelector != nullptr);
	return initSelector->getInitState(trajectory);
}

void ActionPrior::printStrategies() {
	printf("Action Prior Strategies: \n");
	printf("\tinitialStrategy: %s\n", initialStrategy.c_str());
	printf("\tadaptiveStrategy: %s\n", adaptiveStrategy.c_str());
	printf("\tinitStateStrategy: %s\n", initStateStrategy.c_str());
	printf("\ttriggerStrategy: %s\n", triggerStrategy.c_str());
}
