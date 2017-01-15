/*
 * SelectInitState.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_SELECTINITSTATE_HPP_
#define BLOB_PROST_DASP_SELECTINITSTATE_HPP_
#include "../../src/ale_interface.hpp"
#include <vector>

class SelectInitState {
public:
	SelectInitState();
	virtual ~SelectInitState();
	virtual ALEState getInitState(std::vector<ALEState> trajectory) = 0;
};

#endif /* BLOB_PROST_DASP_SELECTINITSTATE_HPP_ */
