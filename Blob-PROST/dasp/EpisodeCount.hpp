/*
 * EpisodeCount.hpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#ifndef BLOB_PROST_DASP_EPISODECOUNT_HPP_
#define BLOB_PROST_DASP_EPISODECOUNT_HPP_
#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "../common/Parameters.hpp"
#endif
#include "TriggerStrategy.hpp"

class EpisodeCount : public TriggerStrategy {
public:
	EpisodeCount(Parameters* param);
	virtual ~EpisodeCount();

	bool trigger(double reward);

	int count;
	int threshold;
};

#endif /* BLOB_PROST_DASP_EPISODECOUNT_HPP_ */
