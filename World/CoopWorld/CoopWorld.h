//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/Hintzelab/MABE/wiki/License

#pragma once

#include "../AbstractWorld.h"

#include <stdlib.h>
#include <thread>
#include <vector>

using namespace std;

class CoopWorld : public AbstractWorld {

public:

	static shared_ptr<ParameterLink<int>> modePL;
	static shared_ptr<ParameterLink<int>> numberOfOutputsPL;
	static shared_ptr<ParameterLink<int>> evaluationsPerGenerationPL;
	static shared_ptr<ParameterLink<int>> nBackPL;
	static shared_ptr<ParameterLink<string>> groupNamePL;
	static shared_ptr<ParameterLink<string>> brainNamePL;
	static shared_ptr<ParameterLink<double>> hitScorePL; 
	static shared_ptr<ParameterLink<double>> shootResPL;
	static shared_ptr<ParameterLink<double>> hitPenaltyPL;
	static shared_ptr<ParameterLink<double>> ffPenaltyPL;
	static shared_ptr<ParameterLink<double>> structHitScorePL;
	static shared_ptr<ParameterLink<double>> buildResPL;
	static shared_ptr<ParameterLink<double>> structHitPenaltyPL;
	static shared_ptr<ParameterLink<double>> structDefendPL;
	static shared_ptr<ParameterLink<double>> resScorePL;
	static shared_ptr<ParameterLink<double>> foodRewardPL;
	static shared_ptr<ParameterLink<double>> moveRewardPL;
	static shared_ptr<ParameterLink<double>> winRewardPL;
	static shared_ptr<ParameterLink<double>> switchPenaltyPL;
	static shared_ptr<ParameterLink<double>> buildScorePL;
	static shared_ptr<ParameterLink<double>> shootScorePL;
	static shared_ptr<ParameterLink<double>> naPenaltyPL;
	static shared_ptr<ParameterLink<double>> experienceMaxPL;

	CoopWorld(shared_ptr<ParametersTable> _PT = nullptr);
	virtual ~CoopWorld() = default;

	// virtual void evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug);
	virtual void evaluate(map<string, shared_ptr<Group>>& groups, int analyze, int visualize, int debug) override;
	// {
	// 	int popSize = groups[groupNamePL->get(PT)]->population.size();
	// 	for (int i = 0; i < popSize; i++) {
	// 		evaluateSolo(groups[groupNamePL->get(PT)]->population[i], analyze, visualize, debug);
	// 	}
	// }

	virtual unordered_map<string, unordered_set<string>> requiredGroups() override {
		return { { groupNamePL->get(PT),{ "B:" + brainNamePL->get(PT) + ",10," + to_string(numberOfOutputsPL->get(PT)) } } };
		// requires a root group and a brain (in root namespace) and no addtional genome,
		// the brain must have 1 input, and the variable numberOfOutputs outputs
	}

};

