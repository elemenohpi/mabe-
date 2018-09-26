//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/Hintzelab/MABE/wiki/License

// Evaluates agents on how many '1's they can output. This is a purely fixed task
// that requires to reactivity to stimuli.
// Each correct '1' confers 1.0 point to score, or the decimal output determined by 'mode'.

#include "MkWorld.h"

shared_ptr<ParameterLink<int>> MkWorld::modePL = Parameters::register_parameter("WORLD_MK-mode", 0, "0 = bit outputs before adding, 1 = add outputs");
shared_ptr<ParameterLink<int>> MkWorld::numberOfOutputsPL = Parameters::register_parameter("WORLD_MK-numberOfOutputs", 1, "number of outputs in this world");
shared_ptr<ParameterLink<int>> MkWorld::evaluationsPerGenerationPL = Parameters::register_parameter("WORLD_MK-evaluationsPerGeneration", 1, "Number of times to test each Genome per generation (useful with non-deterministic brains)");
shared_ptr<ParameterLink<string>> MkWorld::groupNamePL = Parameters::register_parameter("WORLD_MK_NAMES-groupNameSpace", (string)"root::", "namespace of group to be evaluated");
shared_ptr<ParameterLink<string>> MkWorld::brainNamePL = Parameters::register_parameter("WORLD_MK_NAMES-brainNameSpace", (string)"root::", "namespace for parameters used to define brain");

MkWorld::MkWorld(shared_ptr<ParametersTable> _PT) :
		AbstractWorld(_PT) {

	// columns to be added to ave file
	popFileColumns.clear();
	popFileColumns.push_back("score");
	popFileColumns.push_back("score_VAR"); // specifies to also record the variance (performed automatically because _VAR)
}

void MkWorld::evaluateSolo(shared_ptr<Organism> org, int analyze, int visualize, int debug) {
	auto brain = org->brains[brainNamePL->get(PT)];

	double correctCount = 0;

	// Get the matrix of inputs and the right output from the xlsx file
	ifstream filereader("World/MkWorld/input.csv");

	if (!filereader.is_open())
		cout<<"MkWorld: \"input.csv does not exist. Exiting!\"";

	// Evaluate the brain
	int fakecounter = 0;
	while(filereader.good())
	{
		double inputs[23];
		int output;
		string temp;


		// getline(filereader, temp,',');
		// cout<<temp<<endl;

		if(fakecounter == 0)
		{
			for(int i = 0; i < 24; i++)
			{
				if(i < 23)
					getline(filereader, temp, ',');
				else
					getline(filereader, temp, '\r');
				// cout<<"Column "<<i<<" "<<temp<<endl;
			}
		}
		fakecounter++;
		for (int i = 0; i < 24; i++)
		{
			// cout<<"Row "<<fakecounter<<" -> ";
			if(!(i == 23))
				getline(filereader, temp, ',');
			else
				getline(filereader, temp, '\r');
			// cout<<"read: "<<temp<<" ";
			if (i == 0)
			{
				output = strtod(temp.c_str(), NULL);
				// cout<<"saved as output as "<<output<<endl;
			} else {
				inputs[i-1] = strtod(temp.c_str(), NULL);
				// cout<<"saved as inputs["<<i-1<<"] as "<<inputs[i-1]<<endl;
			}		
		}
		
		brain->resetBrain();

		for (int i = 0; i < 23; i++)
		{
			brain->setInput(i, inputs[i]);
		}
		brain->update();
		double answer = brain->readOutput(0);
		int processedAnswer = 0;
		if (answer >= 0)
			processedAnswer = 1;
		if (processedAnswer == output)
			correctCount++;
		// cout<<"answer: "<<answer<<" processedAnswer: "<<processedAnswer<<" output: "<<output<<" correctCount: "<<correctCount<<endl;
		// exit(1);
	}

	filereader.close();

	double score = (correctCount / 858.0) * 100.0;
	org->dataMap.append("score", score);
	if (visualize) cout << "organism with ID " << org->ID << " scored " << score << endl;
}

