/*
 * Prior.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_PRIOR_HPP_
#define BLOB_PROST_DASP_PRIOR_HPP_

#include <vector>
#include "../../src/ale_interface.hpp"

class Prior {
public:
	Prior();
	virtual ~Prior();
	virtual std::vector<double> getPrior(ALEState initState,
			int steps_per_planning) = 0;
};

#endif /* BLOB_PROST_DASP_PRIOR_HPP_ */
