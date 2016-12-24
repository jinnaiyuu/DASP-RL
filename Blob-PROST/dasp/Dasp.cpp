/*
 * Dasp.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: yuu
 */

#include "Dasp.hpp"

Dasp::Dasp(ALEInterface& ale, Parameters* param) {
	// TODO Auto-generated constructor stub
//	ActionVect available_actions = ale.getLegalActionSet();
//	search_tree = new BreadthFirstSearch(ale.theSettings(), ale.theOSystem()->settings(),
//			available_actions, ale.environment());
	search_agent = new SearchAgent(ale.theOSystem.get(), ale.romSettings.get(),
			ale.environment.get(), false, param);
	m_env = ale.environment.get();
	init_state = ale.cloneState();
//	ALEState* p_new_state = new ALEState(new_state, new_state.serialized() );
}

Dasp::~Dasp() {
	// TODO Auto-generated destructor stub
}

std::vector<std::vector<bool>> Dasp::getMinimalActionSequenceSet(
		int sequence_length, int planning_episodes, int frames) {
	run(planning_episodes, frames);
	return search_agent->getUsefulActionSequenceSet();
}

void Dasp::run(int planning_episodes, int frames) {
	ALEState restore_state = m_env->cloneState();
//	ALEState s = m_env->cloneState();
//	state = m_env->cloneState();
	// TODO: interface to change number of frames per planning dynamically
	printf("eps=%d, frames=%d\n", planning_episodes, frames);
	search_agent->set_sim_steps_per_node(frames);
	for (int i = 0; i < planning_episodes; i++) {
		printf("planning episode %d\n", i);
		Action act = search_agent->agent_step();
		for (int f = 0; f < 5; ++f) {
			m_env->act(act, PLAYER_B_NOOP);
		}
		ALEState s = m_env->cloneState();
		ALEState* p_s = new ALEState(s, s.serialize() );
		search_agent->update_state(p_s);
	}
	m_env->restoreState(restore_state);
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

