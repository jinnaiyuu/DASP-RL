/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare
 * Released under GNU General Public License www.gnu.org/licenses/gpl-3.0.txt
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  SearchAgent.cpp
 *
 * The implementation of the SearchAgent class, which uses Search Algorithms
 * to act in the game
 **************************************************************************** */

#include "SearchAgent.hpp"
//#include "game_controller.h"
//#include "../../src/common/random_tools.h"
#include "../../src/emucore/Serializer.hxx"
#include "../../src/emucore/Deserializer.hxx"
//#include "../../src/emucore/System.hxx"
#include <sstream>

#include "BreadthFirstSearch.hpp"
#include "IW1Search.hpp"
#include "UCTSearchTree.hpp"
//#include "PIW1Search.hpp"
//#include "UniformCostSearch.hpp"
//#include "BestFirstSearch.hpp"

#include "../../src/common/time.hxx"
#include "../common/Parameters.hpp"
//#include "Priorities.hpp"

SearchAgent::SearchAgent(OSystem* _osystem, RomSettings* _settings,
		StellaEnvironment* _env, bool player_B, Parameters* param) :
		PlayerAgent(_osystem, _settings), m_curr_action(UNDEFINED), m_current_episode(
				0) {
//	search_method = p_osystem->settings().getString("search_method", true);
	search_method = param->getSearchMethod();

//	if (player_B) {
//		available_actions = _settings->getAllActions_B();
//		search_method = p_osystem->settings().getString("search_method_B",
//				false);
//	}

// Depending on the configuration, create a SearchTree of the requested type
	if (search_method == "brfs") {
		search_tree = new BreadthFirstSearch(_settings, _osystem->settings(),
				available_actions, _env, param);
		m_trace.open("brfs.search-agent.trace");
//	} else if (search_method == "ucs") {
//		search_tree = new UniformCostSearch(_settings, _osystem->settings(),
//				available_actions, _env);
//
//		m_trace.open("ucs.search-agent.trace");
//
	} else if (search_method == "iw1") {
		search_tree = new IW1Search(_settings, _osystem->settings(),
				available_actions, _env, param);

		search_tree->set_novelty_pruning();
		m_trace.open("iw1.search-agent.trace");

//	} else if (search_method == "piw1") {
//		search_tree = new PIW1Search(_settings, _osystem->settings(),
//				available_actions, _env);
//
//		search_tree->set_novelty_pruning();
//		m_trace.open("piw1.search-agent.trace");

//	} else if (search_method == "bfs") {
//		search_tree = new BestFirstSearch(_settings, _osystem->settings(),
//				available_actions, _env);
//
//		search_tree->set_novelty_pruning();
//		m_trace.open("bfs.search-agent.trace");
//
	} else if (search_method == "uct") {
		search_tree = new UCTSearchTree(_settings, _osystem->settings(),
				available_actions, _env, param);
		m_trace.open("uct.search-agent.trace");
	} else {
		std::cerr << "Unknown search Method: " << search_method << std::endl;
		exit(-1);
	}
	m_rom_settings = _settings;
	m_env = _env;

	search_tree->set_player_B(player_B);

	Settings &settings = _osystem->settings();
	sim_steps_per_node = settings.getInt("sim_steps_per_node", true);
//	sim_steps_per_node

	m_param = param;
}

SearchAgent::~SearchAgent() {

	m_trace.close();
}

int SearchAgent::num_available_actions() {
	return available_actions.size();
}

ActionVect& SearchAgent::get_available_actions() {
	return available_actions;
}

Action SearchAgent::agent_step() {
	Action act = PlayerAgent::agent_step();

	state.incrementFrame();

	return act;
}

/* *********************************************************************
 Returns a random action from the set of possible actions
 ******************************************************************** */
Action SearchAgent::act() {
//	rebuildTree();
	// Generate a new action every sim_steps_per node; otherwise return the
	//  current selected action 

	// should be NO_OP, otherwise it sends best action every frame for sim_steps_frames!!!!
	if (frame_number % sim_steps_per_node != 0) {
		std::cout << "FRAMENUMBER SOMETHING WRONG" << frame_number << ","
				<< sim_steps_per_node << std::endl;
		return m_curr_action;
	}

	std::cout << "Search Agent action selection: frame=" << frame_number
			<< std::endl;
	std::cout << "Is Terminal Before Lookahead? "
			<< m_rom_settings->isTerminal() << std::endl;
	std::cout << "Evaluating actions: " << std::endl;

	search_tree->set_max_sim_steps_per_frame(max_sim_steps_per_frame);

	float t0 = aptk::time_used();

	state = m_env->cloneState();

	search_tree->clear();
	search_tree->build(state);

//	if (search_tree->is_built) {
//		// Re-use the old tree
//		search_tree->move_to_best_sub_branch();
//		//assert(search_tree->get_root()->state.equals(state));
//		if (search_tree->get_root()->state.equals(state)) {
//			//assert(search_tree->get_root()->state.equals(state));
//			//assert (search_tree->get_root_frame_number() == state.getFrameNumber());
//			search_tree->update_tree();
//
//		} else {
//			//std::cout << "\n\n\tDIFFERENT STATE!\n" << std::endl;
//			search_tree->clear();
//			search_tree->build(state);
//		}
//	} else {
//		// Build a new Search-Tree
//		search_tree->clear();
//		search_tree->build(state);
//	}

	m_curr_action = search_tree->get_best_action();

	search_tree->getJunkActionSequence(frame_number); // TODO: messy
	search_tree->saveUsedAction(frame_number, m_curr_action);

	m_env->restoreState(state);

	std::cout << "Is Terminal After Lookahead? " << m_rom_settings->isTerminal()
			<< std::endl;

	float tf = aptk::time_used();

	float elapsed = tf - t0;

	search_tree->print_frame_data(frame_number, elapsed, m_curr_action,
			m_trace);
	search_tree->print_frame_data(frame_number, elapsed, m_curr_action,
			std::cout);

	return m_curr_action;
}

/* *********************************************************************
 This method is called when the game ends.
 ******************************************************************** */
void SearchAgent::episode_end(void) {
	PlayerAgent::episode_end();
	// Our search-tree is useless now. Clear it
	search_tree->clear();

	search_tree->getDetectedUsedActionsSize();
}

Action SearchAgent::episode_start(void) {
	Action a = PlayerAgent::episode_start();

	state.incrementFrame();
	m_current_episode++;
	return a;
}

std::vector<std::vector<bool>> SearchAgent::getUsefulActionSequenceSet() {
	return search_tree->getUsefulActionSequenceSet();
}

void SearchAgent::set_max_sim_steps_per_frame(int sim_steps) {
	max_sim_steps_per_frame = sim_steps;
}

void SearchAgent::rebuildTree() {
	if (search_tree == nullptr) {
		return;
	}
	delete search_tree;
	frame_number = 0;

	if (search_method == "brfs") {
		search_tree = new BreadthFirstSearch(m_rom_settings, p_osystem->settings(),
				available_actions, m_env, m_param);
		m_trace.open("brfs.search-agent.trace");
	} else if (search_method == "iw1") {
		search_tree = new IW1Search(m_rom_settings, p_osystem->settings(),
				available_actions, m_env, m_param);
		search_tree->set_novelty_pruning();
		m_trace.open("iw1.search-agent.trace");

	} else if (search_method == "uct") {
		search_tree = new UCTSearchTree(m_rom_settings, p_osystem->settings(),
				available_actions, m_env, m_param);
		m_trace.open("uct.search-agent.trace");
	} else {
		std::cerr << "Unknown search Method: " << search_method << std::endl;
		exit(-1);
	}

	search_tree->set_player_B(false);
}
