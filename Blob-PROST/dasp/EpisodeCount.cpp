/*
 * EpisodeCount.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: yuu
 */

#include "EpisodeCount.hpp"

EpisodeCount::EpisodeCount(Parameters* param) :
		TriggerStrategy(param), count(0) {
	threshold = param->getEpisodeCount();
}

EpisodeCount::~EpisodeCount() {
}

bool EpisodeCount::trigger(double reward) {
	++count;
	if (count == threshold) {
		count = 0;
		return true;
	} else {
		return false;
	}
}
