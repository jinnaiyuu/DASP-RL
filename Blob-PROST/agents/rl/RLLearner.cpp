#ifndef MATHEMATICS_H
#define MATHEMATICS_H
#include "../../common/Mathematics.hpp"
#endif

#ifndef RL_LEARNER_H
#define RL_LEARNER_H
#include "RLLearner.hpp"
#include <random>
#endif

#include <vector>
#include <numeric>
#include "../../dasp/Dasp.hpp"

RLLearner::RLLearner(ALEInterface& ale, Parameters *param, int seed) {
	randomActionTaken = 0;

	actionSelectionStrategy = param->getActionSelectionStrategy();

	gamma = param->getGamma();
	finalEpsilon = param->getEpsilon();
	toUseOnlyRewardSign = param->getUseRewardSign();
	toBeOptimistic = param->getOptimisticInitialization();

	if (toBeOptimistic == 1) {
		printf("optimistic initial value\n");
	}

	episodeLength = param->getEpisodeLength();
	numEpisodesEval = param->getNumEpisodesEval();
	totalNumberOfFramesToLearn = param->getLearningLength();

	epsilonDecay = param->getEpsilonDecay();
	finalExplorationFrame = param->getFinalExplorationFrame();

	//Get the number of effective actions:
//    printf("minaction=%d\n",param->isMinimalAction());
	if (param->isMinimalAction() == 0) {
		printf("defaultaction\n");
		actions = ale.getLegalActionSet();
	} else if (param->isMinimalAction() == 1) {
		printf("minimalaction\n");
		actions = ale.getMinimalActionSet();
	} else if (param->isMinimalAction() == 2) {
		printf("dasp\n");
		// DASP
		Dasp* dasp = new Dasp(ale, param);
		std::vector<std::vector<bool>> actionSeqSet =
				dasp->getMinimalActionSequenceSet(
						param->getDaspSequenceLength(), 1,
						param->getStepsPerPlanning());
		delete dasp;
//    	actions = actionSeqSet[0];
		printf("actionSeqSet.size()=%lu\n", actionSeqSet.size());

		assert(actionSeqSet.size() == 1);
		for (int i = 0; i < actionSeqSet.size(); ++i) {
			printf("actionSeqSet[%d].size()=%lu\n", i, actionSeqSet[i].size());
			for (int j = 0; j < actionSeqSet[i].size(); ++j) {
				if (actionSeqSet[i][j]) {
					printf("o");
					actions.push_back((Action) j);
				} else {
					printf("x");
				}
			}
			printf("\n");

		}
	} else if (param->isMinimalAction() == -1) {
		printf("extended action set\n");
		ActionVect a = ale.getLegalActionSet();
		actions = a;
		actions.insert(actions.end(), a.begin(), a.end());
	} else if (param->isMinimalAction() == -2) {
		printf("extended action set (10 times bigger)\n");
		ActionVect a = ale.getLegalActionSet();
		actions = a;
		for (int i = 1; i < 10; ++i) {
			actions.insert(actions.end(), a.begin(), a.end());
		}
	}

	numActions = actions.size();

	printf("%d actions\n", numActions);

	useActionPrior = param->getUseActionPrior();
	// YJ Action prior

	temperature = param->getTemperature();

	if (useActionPrior) {
		printf("Use Action Prior\n");
		actionPrior = new ActionPrior(ale, param);
		prior = actionPrior->initialPruning();
		printf("Initial prior: ");
		for (int i = 0; i < prior.size(); ++i) {
			printf("%.1f ", prior[i]);
		}
		printf("\n");
	}

	agentRand = param->getRNG();
	agentRand->seed(seed);
}

int RLLearner::chooseAction(vector<float> &QValues, int episode) {
	if (actionSelectionStrategy == "EPSILON_GREEDY") {
		return epsilonGreedy(QValues, episode);
	} else if (actionSelectionStrategy == "SOFTMAX") {
		return softmax(QValues, episode);
	} else {
		printf("chooseAction error: %s\n", actionSelectionStrategy.c_str());
		return epsilonGreedy(QValues, episode);
	}
}

//int RLLearner::epsilonGreedy(vector<float> &QValues) {
//	randomActionTaken = 0;
//
//	if (useActionPrior) {
//		vector<float> factoredQValues;
//		factoredQValues.resize(QValues.size());
//		for (int i = 0; i < factoredQValues.size(); ++i) {
//			factoredQValues[i] = prior[i] * QValues[i];
//		}
//		int action = Mathematics::argmax(factoredQValues, agentRand);
//		int random = (*agentRand)();
//		float epsilon = finalEpsilon;
//		if ((random % int(nearbyint(1.0 / epsilon))) == 0) {
//			//if((rand()%int(1.0/epsilon)) == 0){
//			randomActionTaken = 1;
//			action = Mathematics::chooseFromProbability(factoredQValues,
//					agentRand);
//		}
//		return action;
//	} else {
//		int action = Mathematics::argmax(QValues, agentRand);
//		//With probability epsilon: a <- random action in A(s)
//		int random = (*agentRand)();
//		float epsilon = finalEpsilon;
//		if ((random % int(nearbyint(1.0 / epsilon))) == 0) {
//			//if((rand()%int(1.0/epsilon)) == 0){
//			randomActionTaken = 1;
//			action = (*agentRand)() % numActions;
//		}
//		return action;
//	}
//}

int RLLearner::epsilonGreedy(vector<float> &QValues, int episode) {
	randomActionTaken = 0;

	vector<float> factoredQValues = QValues;

	if (useActionPrior) {
		for (int i = 0; i < factoredQValues.size(); ++i) {
			factoredQValues[i] *= prior[i];
		}
	}

	int action = Mathematics::argmax(factoredQValues, agentRand);

	int random = (*agentRand)();
	float epsilon = finalEpsilon;
	if (episode != -1) {
		if (epsilonDecay && episode <= finalExplorationFrame) {
			epsilon = 1 - (1 - finalEpsilon) * episode / finalExplorationFrame;
		}
	}

	if ((random % int(nearbyint(1.0 / epsilon))) == 0) {
		//if((rand()%int(1.0/epsilon)) == 0){
		randomActionTaken = 1;
		if (useActionPrior) {
			action = Mathematics::chooseFromProbability(factoredQValues,
					agentRand);
		} else {
			action = (*agentRand)() % numActions;
		}
	}
	assert(0 <= action && action < numActions);
	return action;

}

int RLLearner::softmax(vector<float> &QValues, int episode) {
	vector<float> factoredQValues = QValues;

	if (useActionPrior) {
		for (int i = 0; i < factoredQValues.size(); ++i) {
			factoredQValues[i] *= prior[i];
		}
	}

	return Mathematics::softmax(factoredQValues, temperature, agentRand);

}

/**
 * The first parameter is the one that is used by Sarsa. The second is used to
 * pass aditional information to the running algorithm (like 'real score' if one
 * is using a surrogate reward function).
 */
void RLLearner::act(ALEInterface& ale, int action, vector<float> &reward) {
	double r_alg = 0.0, r_real = 0.0;

	r_real = ale.act(actions[action]);
	if (toUseOnlyRewardSign) {
		if (r_real > 0) {
			r_alg = 1.0;
		} else if (r_real < 0) {
			r_alg = -1.0;
		}
		//Normalizing reward according to the first
		//reward, Marc did this in his JAIR paper:
	} else {
		if (r_real != 0.0) {
			if (!sawFirstReward) {
				firstReward = std::abs(r_real);
				sawFirstReward = 1;
			}
		}
		if (sawFirstReward) {
			if (toBeOptimistic) {
				r_alg = (r_real - firstReward) / firstReward + gamma;
			} else {
				r_alg = r_real / firstReward;
			}
		} else {
			if (toBeOptimistic) {
				r_alg = gamma - 1.0;
			}
		}
	}
	reward[0] = r_alg;
	reward[1] = r_real;
	//If doing optimistic initialization, to avoid the agent
	//to "die" soon to avoid -1 as reward at each step, when
	//the agent dies we give him -1 for each time step remaining,
	//this would be the worst case ever...
	if (ale.game_over() && toBeOptimistic) {
		int missedSteps = episodeLength - ale.getEpisodeFrameNumber() + 1;
		double penalty = pow(gamma, missedSteps) - 1;
		reward[0] -= penalty;
	}
}
