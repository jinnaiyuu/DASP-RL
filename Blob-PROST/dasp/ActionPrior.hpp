/*
 * ActionPrior.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_ACTIONPRIOR_HPP_
#define BLOB_PROST_DASP_ACTIONPRIOR_HPP_
#include "../../src/ale_interface.hpp"
#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "../common/Parameters.hpp"
#endif
#include "Dasp.hpp"
#include "TriggerStrategy.hpp"

class ActionPrior {
public:
	ActionPrior(ALEInterface& ale, Parameters* param);
	virtual ~ActionPrior();

	std::vector<double> initialPruning();
	std::vector<double> adaptivePruning(std::vector<ALEState> trajectory,
			double reward); // TODO: what to put in as arguments?

private:
	std::vector<double> runPruning(ALEState initState, std::string strategy);
	std::vector<double> runDasp(ALEState initState);
	bool needAdaptivePruning(double reward);
	ALEState selectInitState(std::vector<ALEState> trajectory);

	void printStrategies();

	std::vector<double> currentPrior;

	ALEInterface& ale;
	Parameters* param;

	std::string initialStrategy;
	std::string adaptiveStrategy;
	std::string triggerStrategy;
	std::string initStateStrategy;

	// DASP/DASA parameters
	Dasp* dasp;
	int planning_episodes;
	int steps_per_planning;
	std::string search_method;

	// Trigger parameter
	TriggerStrategy* trigger;
};

#endif /* BLOB_PROST_DASP_ACTIONPRIOR_HPP_ */
