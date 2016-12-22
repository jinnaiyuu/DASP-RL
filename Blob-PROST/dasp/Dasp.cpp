/*
 * Dasp.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: yuu
 */

#include "Dasp.hpp"

Dasp::Dasp(ALEInterface& ale) {
	// TODO Auto-generated constructor stub
//	ActionVect available_actions = ale.getLegalActionSet();
//	search_tree = new BreadthFirstSearch(ale.theSettings(), ale.theOSystem()->settings(),
//			available_actions, ale.environment());
	search_agent = new SearchAgent(ale.theOSystem.get(), ale.romSettings.get(),
			ale.environment.get(), false);

	init_state = ale.cloneState();
//	ALEState* p_new_state = new ALEState(new_state, new_state.serialized() );
}

Dasp::~Dasp() {
	// TODO Auto-generated destructor stub
}

std::vector<std::vector<bool>> Dasp::getMinimalActionSequenceSet(int sequence_length,
		int planning_episodes, int frames) {
	run(planning_episodes, frames);
	return search_agent->getUsefulActionSequenceSet();
}

void Dasp::run(int planning_episodes, int frames) {

	// TODO: interface to change number of frames per planning dynamically
//	for (int i = 0; i < planning_episodes; i++) {
	Action act = search_agent->agent_step();
//	}

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

