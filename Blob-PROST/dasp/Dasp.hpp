/*
 * Dasp.hpp
 *
 *  Created on: Dec 22, 2016
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_DASP_HPP_
#define BLOB_PROST_DASP_DASP_HPP_

#include "../../src/ale_interface.hpp"

#include "SearchAgent.hpp"
#include "Prior.hpp"


class Dasp : public Prior {
public:
	Dasp(ALEInterface& ale, Parameters* param);
//	Dasp(ALEInterface& ale, Parameters* param);
	virtual ~Dasp();

	// Parameters:
	std::vector<std::vector<bool>> getMinimalActionSequenceSet(int sequence_length,
			int planning_episodes, int frames);
	std::vector<std::vector<bool>> getMinimalActionSequenceSet(int sequence_length,
			int planning_episodes, int frames, ALEState initState);
	std::vector<double> getPrior(ALEState initState, int steps_per_planning);

private:
	StellaEnvironment* m_env;
	SearchAgent* search_agent;
	ALEState init_state;
	void run(int planning_episodes, int frames, ALEState initState);
	bool isDone(int planning_episodes);
	void episodeEnd();
	void episodeStart(Action& action_a, Action& action_b);
	void episodeStep(Action& action_a, Action& action_b);

//	bool iterativeDeepening
};

#endif /* BLOB_PROST_DASP_DASP_HPP_ */
