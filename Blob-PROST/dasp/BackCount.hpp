/*
 * BackCount.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_BACKCOUNT_HPP_
#define BLOB_PROST_DASP_BACKCOUNT_HPP_
#include "SelectInitState.hpp"
#include "../../src/ale_interface.hpp"
#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "../common/Parameters.hpp"
#endif
#include <vector>

class BackCount : public SelectInitState {
public:
	BackCount(Parameters* param);
	virtual ~BackCount();
	ALEState getInitState(std::vector<ALEState> trajectory);

private:
	int backSteps;
	int randomizeBackCount;
};

#endif /* BLOB_PROST_DASP_BACKCOUNT_HPP_ */
