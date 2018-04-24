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

#include <math.h>
#include <memory>
#include <iostream>
#include <set>
#include <vector>

#include "../../Genome/AbstractGenome.h"

#include "../../Utilities/Random.h"

#include "../AbstractBrain.h"

using namespace std;

class ARNBrain: public AbstractBrain {
public:
	static shared_ptr<ParameterLink<string>> genomeNamePL;
	static shared_ptr<ParameterLink<double>> deltaPL;
	static shared_ptr<ParameterLink<double>> betaPL;
	static shared_ptr<ParameterLink<int>> iterationsPL;
	static shared_ptr<ParameterLink<int>> outputModePL;
	static shared_ptr<ParameterLink<double>> outputMinPL;
	static shared_ptr<ParameterLink<double>> outputMaxPL;
	static shared_ptr<ParameterLink<int>> inputModePL;
	static shared_ptr<ParameterLink<double>> extraLimitPL;


	vector<vector<double>> enh_matrix;
	vector<vector<double>> inh_matrix;
	vector<vector<double>> weight_matrix;

	shared_ptr<ParametersTable> myPT;

	vector<shared_ptr<struct Gene>> genes;
	vector<shared_ptr<struct Gene>> tfGenes;
	vector<shared_ptr<struct Gene>> extraGenes;
	vector<shared_ptr<struct Gene>> pGenes;

	double delta;
	double beta;
	double extraLimit;
	double inputMode;
	int iterations;
	double outputMin;
	double outputMax;
	int outputMode;


	ARNBrain() = delete;

	ARNBrain(int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT = Parameters::root);
	ARNBrain(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes, int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT = Parameters::root);

	void detectGenes(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes);

	void updateConcentrations(bool initializing = false);

	void addInput(double concentration, int signature);

	void addOutput(double concentration, int signature);

	void setInput(double value);

	void calcWeights();

	int sumXOR(int a[], int b[]);

	vector<double> normalizedInput();

	void regulate();

	virtual ~ARNBrain() = default;

	virtual void update() override;

	virtual shared_ptr<AbstractBrain> makeBrain(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes) override;
	
	virtual shared_ptr<AbstractBrain> makeCopy(shared_ptr<ParametersTable> _PT = nullptr) override;
	
	virtual string description() override;
	
	virtual DataMap getStats(string& prefix) override;
	
	virtual string getType() override {
		return "ARN";
	}

	virtual void resetBrain() override;

	virtual void initializeGenomes(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes);
	
	virtual unordered_set<string> requiredGenomes() override {
		return { genomeNamePL->get(PT) };
	}

};

inline shared_ptr<AbstractBrain> ARNBrain_brainFactory(int ins, int outs, shared_ptr<ParametersTable> PT = Parameters::root) {
	// cout<<"ARNBrain::ARNBrain_brainFactory()"<<endl;
	return make_shared<ARNBrain>(ins, outs, PT);
}
