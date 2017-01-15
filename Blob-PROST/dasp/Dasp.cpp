/*
 * Dasp.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: yuu
 */

#include "Dasp.hpp"

Dasp::Dasp(ALEInterface& ale, Parameters* param) : Prior() {
	search_agent = new SearchAgent(ale.theOSystem.get(), ale.romSettings.get(),
			ale.environment.get(), false, param);
	m_env = ale.environment.get();
	init_state = ale.cloneState();
//	ALEState* p_new_state = new ALEState(new_state, new_state.serialized() );
}

//Dasp::Dasp(ALEInterface& ale, Parameters* param) {
//	search_agent = new SearchAgent(ale.theOSystem.get(), ale.romSettings.get(),
//			ale.environment.get(), false, param);
//	m_env = ale.environment.get();
//	init_state = ale.cloneState();
////	ALEState* p_new_state = new ALEState(new_state, new_state.serialized() );
//}

Dasp::~Dasp() {
	// TODO Auto-generated destructor stub
}

std::vector<std::vector<bool>> Dasp::getMinimalActionSequenceSet(
		int sequence_length, int planning_episodes, int frames) {
	ALEState initState = m_env->cloneState();
	run(planning_episodes, frames, initState);
	return search_agent->getUsefulActionSequenceSet();
}
std::vector<std::vector<bool>> Dasp::getMinimalActionSequenceSet(
		int sequence_length, int planning_episodes, int frames, ALEState initState) {
	run(planning_episodes, frames, initState);
	return search_agent->getUsefulActionSequenceSet();
}

void Dasp::run(int planning_episodes, int frames, ALEState initState) {
	ALEState restore_state = m_env->cloneSystemState();
	printf("frames=%d\n", frames);
	search_agent->rebuildTree();
	search_agent->set_max_sim_steps_per_frame(frames);
	m_env->setState(initState);
	Action act = search_agent->agent_step();
	m_env->restoreSystemState(restore_state);
}

std::vector<double> Dasp::getPrior(ALEState initState, int steps_per_planning) {
	std::vector<double> prior;
	std::vector<std::vector<bool>> actionSeqSet =
			getMinimalActionSequenceSet(1, 1,
					steps_per_planning, initState);

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



bool Dasp::isDone(int planning_episodes) {
//	return (frames < max_frames);
}

void Dasp::episodeStep(Action& action_a, Action& action_b) {
}

void Dasp::episodeStart(Action& action_a, Action& action_b) {
}

void Dasp::episodeEnd() {
}

