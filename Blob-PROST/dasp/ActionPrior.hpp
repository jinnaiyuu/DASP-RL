/*
 * ActionPrior.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_ACTIONPRIOR_HPP_
#define BLOB_PROST_DASP_ACTIONPRIOR_HPP_
#include <ale_interface.hpp>
#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "../common/Parameters.hpp"
#endif

class ActionPrior {
public:
	ActionPrior(ALEInterface& ale, Parameters* param);
	virtual ~ActionPrior();

	std::vector<double> initialPruning();
	std::vector<double> adaptivePruning(); // TODO: what to put in as arguments?


private:
	std::vector<double> runPruning(ALEState initState);
	std::vector<double> currentPrior;
	bool needAdaptivePruning();
	ALEState selectInitState();

	ALEInterface& ale;
	Parameters* param;

	std::string initialStrategy;
	std::string adaptiveStrategy;
	std::string triggerStrategy;
	std::string initStateStrategy;

	// DASP/DASA parameters
    int planning_episodes;
    int steps_per_planning;
    std::string search_method;
};

#endif /* BLOB_PROST_DASP_ACTIONPRIOR_HPP_ */
