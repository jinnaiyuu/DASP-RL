/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare
 * Released under GNU General Public License www.gnu.org/licenses/gpl-3.0.txt
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 *
 * *****************************************************************************
 *  SearchAgent.hpp 
 *
 * The implementation of the SearchAgent class, which uses Search Algorithms
 * to act in the game
 **************************************************************************** */

#ifndef __SEARCH_AGENT_HPP__
#define __SEARCH_AGENT_HPP__

#include "../../src/common/Constants.h"
#include "PlayerAgent.hpp"
#include "../../src/emucore/OSystem.hxx"
#include "../../src/environment/ale_state.hpp"
#include "SearchTree.hpp"
#include <fstream>
//#include "../common/Parameters.hpp"
class Parameters;

class SearchAgent : public PlayerAgent {
    public:
	SearchAgent(OSystem * _osystem, RomSettings * _settings, StellaEnvironment* _env, bool player_B = false, Parameters* param = nullptr);
        virtual ~SearchAgent();
		
        /* *********************************************************************
            This method is called when the game ends. 
         ******************************************************************** */
        virtual void episode_end(void);
        
        virtual Action episode_start(void);

        virtual Action agent_step();

        void saveUsedAction(int frame_number, Action action);

        std::vector<std::vector<bool>> getUsefulActionSequenceSet();

        void set_sim_steps_per_node(int sim_steps);

	protected:
        /* *********************************************************************
            Returns the best action from the set of possible actions
         ******************************************************************** */
        virtual Action act();
        
        int num_available_actions();
        ActionVect &get_available_actions();

	void set_search_tree_player_B( bool b ){ if(search_tree) search_tree->set_player_B(b); }
protected:
	Action m_curr_action;
	ALEState state;
	RomSettings * m_rom_settings;
	SearchTree * search_tree;
	StellaEnvironment* m_env;
	int sim_steps_per_node;
	
	std::string search_method;
	unsigned m_current_episode;
	std::ofstream m_trace;

};

#endif // __SEARCH_AGENT_HPP__

