/*
 * TriggerStrategy.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_TRIGGERSTRATEGY_HPP_
#define BLOB_PROST_DASP_TRIGGERSTRATEGY_HPP_

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "../common/Parameters.hpp"
#endif

class TriggerStrategy {
public:
	TriggerStrategy(Parameters* param);
	virtual ~TriggerStrategy();
	virtual bool trigger(double reward) = 0;
};

#endif /* BLOB_PROST_DASP_TRIGGERSTRATEGY_HPP_ */
