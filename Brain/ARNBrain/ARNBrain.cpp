//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/Hintzelab/MABE/wiki/License
//  Artificial Regulatory Network brain (ARNBrain) is made by Iliya Miralavy (iliyaalavy@gmail.com)

#include "ARNBrain.h"

shared_ptr<ParameterLink<string>> ARNBrain::genomeNamePL = Parameters::register_parameter("BRAIN_ARN_NAMES-genomeNameSpace", (string)"root::", "namespace used to set parameters for genome used to encode this brain");
shared_ptr<ParameterLink<double>> ARNBrain::deltaPL = Parameters::register_parameter("BRAIN_ARN-delta", 1.0, "value of the delta in the ARN formula");
shared_ptr<ParameterLink<double>> ARNBrain::betaPL = Parameters::register_parameter("BRAIN_ARN-beta", 1.0, "value of the beta in the ARN formula");
shared_ptr<ParameterLink<int>> ARNBrain::iterationsPL = Parameters::register_parameter("BRAIN_ARN-iterations", 10, "number of regulatory iterations should the brain do before giving back the outputs?");
shared_ptr<ParameterLink<double>> ARNBrain::outputMinPL = Parameters::register_parameter("BRAIN_ARN-outputMin", 0.0, "min value of outputs. should be positive (for now)");
shared_ptr<ParameterLink<double>> ARNBrain::outputMaxPL = Parameters::register_parameter("BRAIN_ARN-outputMax", 1.0, "max value of outputs. should be positive (for now) and greater than min value");
shared_ptr<ParameterLink<int>> ARNBrain::outputModePL = Parameters::register_parameter("BRAIN_ARN-outputMode", 0, "output mode-> 0: normal (continious) mode 1: binary mode (no chance of all 1) 2: integer mode 3: fair binary mode (No chance of all 0)");
shared_ptr<ParameterLink<int>> ARNBrain::inputModePL = Parameters::register_parameter("BRAIN_ARN-inputMode", 0, "input mode-> 0: concentration mode 1: sequence mode");
shared_ptr<ParameterLink<double>> ARNBrain::extraLimitPL = Parameters::register_parameter("BRAIN_ARN-extraLimit", 0.4, "maximum concentration allowed for extra (input) proteins");


//Different gene types
const int TF_GENE = 1; // transcription factors - normal genes
const int EXTRA_GENE = 2; // extra - input genes
const int P_GENE = 3; // producet - output genes

const bool ARN_DEBUG_SUPER = false;
const bool ARN_DEBUG = false;
const bool ARN_SHOW_OUTPUT = false;

// int brain_iteration_counter = 0;
int inputIndex = 0;

struct Gene {
	int enhancer[32];
	int inhibitor[32];
	int protein[32];
	double concentration = 0;
	int type;
};

ARNBrain::ARNBrain(int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT) :
		AbstractBrain(_nrInNodes, _nrOutNodes, _PT) {
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::ARNBrain() (1st Constructor) "<<endl;
	myPT = _PT;
	delta = deltaPL->get(_PT);
	beta = betaPL->get(_PT);
	extraLimit = extraLimitPL->get(_PT);
	inputMode = inputModePL->get(myPT);
	iterations = iterationsPL->get(myPT);
	outputMode = outputModePL->get(myPT);
	outputMax = outputMaxPL->get(myPT);
	outputMin = outputMinPL->get(myPT);
	// Columns to be added to ave file
	popFileColumns.clear();
}

ARNBrain::ARNBrain(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes, int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT) :
		AbstractBrain(_nrInNodes, _nrOutNodes, _PT) {
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::ARNBrain() (2nd Constructor) "<<endl;
	myPT = _PT;
	delta = deltaPL->get(_PT);
	beta = betaPL->get(_PT);
	extraLimit = extraLimitPL->get(_PT);
	inputMode = inputModePL->get(myPT);
	iterations = iterationsPL->get(myPT);
	outputMode = outputModePL->get(myPT);
	outputMax = outputMaxPL->get(myPT);
	outputMin = outputMinPL->get(myPT);
	popFileColumns.clear();

	extraGenes.clear();
	pGenes.clear();
	//Add as much input genes as needed to the brain with an initial concentration of 0.0'
	for(int i = 0; i<nrInputValues; i++)
	{
		addInput(0.0, i);
	}

	for(int i = 0; i<nrOutputValues; i++)
	{
		addOutput(0, i);
	}

	// Detect all the genes in the genome
	// cout<<"detecting Genes: ";
	detectGenes(_genomes);
	// cout<<tfGenes.size()<<" "<<extraGenes.size()<<" "<<pGenes.size()<<endl;
	// Calculate the weight matrix of exp(B*(enhancer_i - inhibitor_i))
	calcWeights();
}

shared_ptr<AbstractBrain> ARNBrain::makeBrain(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes) {
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::makeBrain()"<<endl;
	// Make a new brain
	shared_ptr<ARNBrain> newBrain = make_shared<ARNBrain>(_genomes, nrInputValues, nrOutputValues, PT);
	return newBrain;
}

void ARNBrain::calcWeights()
{	
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::calcWeights() "<<endl;

	/* g e t t i n g r e g u l a t e d 
	r           P & TF
	e
	g
	u
	l TF & EXTRA
	a
	t
	i
	n
	g */

	// a is a temp for enh_matrix and b is a temp for inh_matrix and c is a temp for weight_matrix
	vector<vector<double>> a(genes.size() - pGenes.size(), vector<double>(genes.size() - extraGenes.size())); // cause extra genes dont get regulated
	vector<vector<double>> b(genes.size() - pGenes.size(), vector<double>(genes.size() - extraGenes.size())); // cause extra genes dont get regulated
	vector<vector<double>> c(genes.size() - pGenes.size(), vector<double>(genes.size() - extraGenes.size()));
	weight_matrix = c;
	// Initialize the weight matrixes with 0.0
	for(int i = 0; i < genes.size() - pGenes.size(); i++)
	{
		for(int j = 0; j < genes.size() - extraGenes.size(); j++)
		{
			a[i][j] = 0;	
			b[i][j] = 0;
			weight_matrix[i][j] = 0;
		}
	}

	// The order of genes in the genes vector is important. It should be like: [TF genes][P genes][EXTRA genes]
	int maxEnhancer = 0;
	int maxInhibitor = 0;

	int counter = 0; //tf and extra
	for(int i = 0; i < genes.size(); i++)
	{
		if(genes[i]->type != P_GENE) // cause P genes don't regulate
		{
			int xor_enhance, xor_inhibit;
			for(int j = 0; j < genes.size() - extraGenes.size(); j++)
			{
				xor_enhance = sumXOR(genes[counter]->protein, genes[j]->enhancer);
				xor_inhibit = sumXOR(genes[counter]->protein, genes[j]->inhibitor);
				a[counter][j] = xor_enhance;
				b[counter][j] = xor_inhibit;
				if(xor_enhance > maxEnhancer)
					maxEnhancer = xor_enhance;
				if(xor_inhibit > maxInhibitor)
					maxInhibitor = xor_inhibit;
			}
			counter++;
		}
	}

	for(int i = 0; i < genes.size()- pGenes.size(); i++)
	{
		for(int j = 0; j < (genes.size() - extraGenes.size()); j++)
		{
			a[i][j] -= maxEnhancer;
			a[i][j] *= beta;
			a[i][j] = exp(a[i][j]);
			b[i][j] -= maxInhibitor;
			b[i][i] *= beta;
			b[i][j] = exp(b[i][j]);
		}
	}

	enh_matrix = a;
	inh_matrix = b;

	for(int i = 0; i < genes.size()- pGenes.size(); i++)
	{
		for(int j = 0; j < (genes.size() - extraGenes.size()); j++)
		{
			weight_matrix[i][j] = enh_matrix[i][j] - inh_matrix[i][j];
		}
	}
	if(weight_matrix.size() < 1)
	{
		cout<<"ARNBrain: I undexpectadly couldn't make the weight_matrix! Exiting!"<<endl;
		exit(1);
	}
}

int ARNBrain::sumXOR(int a[], int b[])
{
	if(ARN_DEBUG_SUPER)
		cout<<"ARNBrain::sumXOR() "<<endl;
	int sum = 0;
	for(int i = 0; i < 32; i++)
		if(a[i] xor b[i])
			sum++;
	return sum;
}

void ARNBrain::detectGenes(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes) {
		if(ARN_DEBUG || ARN_DEBUG_SUPER)
			cout<<"ARNBrain::detectGenes() "<<endl;
		genes.clear();
		tfGenes.clear();

		// Enhancer 32bit - Inhibitor 32bit - Promoter 32bit follows XYZ01010101 rule (decimal = 85)
		auto genome = _genomes[genomeNamePL->get(PT)];
		if(genome == nullptr)
		{
			cout<<"ARNBrain:: The genome parameter fed to my detectGenes function is null"<<endl;
		}
		auto handler = genome->newHandler(genome, true);
		handler->advanceIndex(88);
		while(!handler->atEOG())
		{
			if(handler->readInt(0, 256) == 85) //XXXXXXXX == 01010101 Promoter sequence - IMPORTANT - Index goes ahead by 2 - so we're on 90 1st time
			{
				handler->toggleReadDirection();
				handler->advanceIndex(90); //2 for read rest for sites
				handler->toggleReadDirection();
				int enhancer[32], inhibitor[32], protein[32];
				for(int i = 0; i<32; i++)
				{
					enhancer[i] = handler->readInt(0, 1);
				}
				for(int i = 0; i<32; i++)
				{
					inhibitor[i] = handler->readInt(0, 1);
				}
				//advance the index to the start of the protein sequence
				handler->advanceIndex(3);
				//Start of the sequence
				int oneCounter = 0;
				for(int i = 0; i < 32; i++)
				{
					if(handler->readInt(0,1) == 1)
						oneCounter++;
					handler->advanceIndex(32);
					if(handler->readInt(0,1) == 1)
						oneCounter++;
					handler->advanceIndex(32);
					if(handler->readInt(0,1) == 1)
						oneCounter++;
					handler->advanceIndex(32);
					if(handler->readInt(0,1) == 1)
						oneCounter++;
					handler->advanceIndex(32);
					if(handler->readInt(0,1) == 1)
						oneCounter++;
					handler->advanceIndex(-128);

					if(oneCounter > 3)
						protein[i] = 1;
					else
						protein[i] = 0;
					oneCounter = 0;
				}
				handler->advanceIndex(128);
				//Save the gene
				shared_ptr<struct Gene> gene { new struct Gene };
				for(int i = 0; i < 32; i++)
				{
					gene->enhancer[i] = enhancer[i];
					gene->inhibitor[i] = inhibitor[i];
					gene->protein[i] = protein[i];
				}
				gene->concentration = 0;
				gene->type = TF_GENE;
				genes.push_back(gene);
				tfGenes.push_back(gene);
			}
		}
		for(auto gene : pGenes)
			genes.push_back(gene);
		for(auto gene : extraGenes)
			genes.push_back(gene);
		
		// cout<<"Size of => genes: "<<genes.size()<<" extras: "<<extraGenes.size()<<" tfGenes: "<<tfGenes.size()<<" pGenes: "<<pGenes.size()<<endl;

		updateConcentrations(true);
}

void ARNBrain::addInput(double concentration, int signature)
{
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::addInput() "<<endl;
	//Here my trick is to always give the same input sequences to the input/output genes.
	shared_ptr<struct Gene> gene { new struct Gene };
	for(int i = 0; i < 32; i++)
	{
		// if(i <= signature)
		// 	gene->enhancer[i] = 1;
		// else
		// 	gene->enhancer[i] = 0;
		// if(i > signature)
		// 	gene->inhibitor[i] = 1;
		// else
		// 	gene->inhibitor[i] = 0;
		// gene->protein[i] = (i % 2 == 0)?1:0;
		gene->enhancer[i] = Random::getInt(0,1);
		gene->inhibitor[i] = Random::getInt(0,1); 
		gene->protein[i] = Random::getInt(0,1);
	}
	gene->concentration = concentration;
	gene->type = EXTRA_GENE;
	extraGenes.push_back(gene);
}

void ARNBrain::addOutput(double concentration, int signature) 
{
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::addOutput() "<<endl;
	//Here my trick is to always give the same input sequences to the input/output genes.
	shared_ptr<struct Gene> gene { new struct Gene };
	for(int i = 0; i < 32; i++)
	{
		if(i <= signature)
			gene->enhancer[i] = 0;
		else
			gene->enhancer[i] = 1;
		if(i > signature)
			gene->inhibitor[i] = 0;
		else
			gene->inhibitor[i] = 1;
		gene->protein[i] = (i % 2 == 0)?1:0;
	}
	gene->concentration = concentration;
	gene->type = P_GENE;
	pGenes.push_back(gene);
}

// Normalizes the concentration levels of all the protein types
void ARNBrain::updateConcentrations(bool initializing)
{
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::updateConcetrations() Mode: "<<initializing<<endl;

	if(initializing)
	{
		for(auto gene : tfGenes)
		{
			if(tfGenes.size() == 0)
			{
				cout<<"ARNBrain:: tfGenes.size() returned 0 in my updateConcetrations() function. A bug maybe?"<<endl;
				continue;
			}
			gene->concentration = 1.0/tfGenes.size();
		}
		for(auto gene : pGenes)
		{
			if(pGenes.size() == 0)
			{
				cout<<"ARNBrain:: tfGenes.size() returned 0 in my updateConcetrations() function. A bug maybe?"<<endl;
				continue;
			}
			gene->concentration = 1.0/pGenes.size();
		}
		for(auto gene : extraGenes)
		{
			if(pGenes.size() == 0)
			{
				cout<<"ARNBrain:: genes.size() returned 0 in my updateConcetrations() function. A bug maybe?"<<endl;
				exit(1);
				continue;
			}
			gene->concentration = 0;
		}
	}

	// Concentration values will indeed change. We need to therefore normalize them	
	double totalTF = 0, totalP = 0, totalExtra = 0;

	for(auto gene : extraGenes)
	{
		totalExtra += gene->concentration;
	}

	for(auto gene : tfGenes)
	{
		totalTF += gene->concentration;
	}

	for(auto gene : pGenes)
	{
		totalP += gene->concentration;
	}

	for(auto gene : tfGenes)
	{
		gene->concentration = gene->concentration / totalTF;
	}

	for(auto gene : pGenes)
	{
		gene->concentration = gene->concentration / totalP;
	}
}

void ARNBrain::setInput(double value)
{
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::setInput() "<<endl;
	// ToDo::Code the input modes (sequence mode)
	if(inputMode == 0)
	{
		if(extraGenes.size() < 1)
		{
			cout<<"ARNBrain: You are trying to set my input when I don't have any! Exiting!"<<endl;
			exit(1);
		}
		extraGenes[inputIndex]->concentration = value;
		inputIndex++;
		if(inputIndex > extraGenes.size())
		{
			inputIndex = 0;
			// ToDo:: add if debug to help debugging the world. 
			cout<<"ARNBrain: \"The number of your world inputs is larger than the number of my input genes.\""<<endl;
		}
	}else if(inputMode == 1) 
	{
		// ToDo::Sequence Mode
		cout<<"ARNBrain: \"Sorry! My developer was too lazy to code this section of mine. E-Mail him at iliyaalavy@gmail.com to force him to complete me! Peace.\"";
		exit(1);
	}else
	{
		cout<<"ARNBrain: \"I'm afraid you have entered a wrong value as my input mode in the settings_organism.cfg file\" value: "<<inputMode<<endl;
		exit(1);
	}
}

void ARNBrain::resetBrain() {
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::resetBrain() "<<endl;
	updateConcentrations(true);
	AbstractBrain::resetBrain();
}

vector<double> ARNBrain::normalizedInput()
{
	if(inputValues.size() < 1)
	{
		cout<<"ARNBrain: I see no inputValues"<<endl;
		exit(1);
	}
	//Normalize the inputs
	//find the smallest and the largest inputs
	double smallestInput = inputValues[0];
	double largestInput = inputValues[0];
	for(int i = 0; i < nrInputValues; i++)
	{
		if(inputValues[i] > largestInput)
			largestInput = inputValues[i];
		if(inputValues[i] < smallestInput)
			smallestInput = inputValues[i];
	}

	vector<double> normalizedInputs;

	if(nrInputValues == 1) //only one input
	{
		if(inputValues[0] < 0)
			normalizedInputs.push_back(0.0);
		else if(inputValues[0] > extraLimit)
			normalizedInputs.push_back(extraLimit);
	}else 
	{
		double sumInputs = 0;
		for(int i = 0; i < nrInputValues; i++)
		{
			sumInputs += inputValues[i];	
		}
		for(int i = 0; i < nrInputValues; i++)
		{
			normalizedInputs.push_back(inputValues[i]/sumInputs);
		}
	}

	return normalizedInputs;
}

void ARNBrain::regulate()
{
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::regulate()"<<endl;
	for(int iter = 0; iter < iterations; iter++)
	{
		vector<double> updated_conc;
		// TF + P
		for(int j = 0; j < weight_matrix[0].size(); j++)
		{
			double signal = 0.0;
			int counter = 0;

			// TF + EXTRA
			for(int i = 0; i < genes.size(); i++)
			{
				if(genes[i]->type != P_GENE)
				{
					signal += genes[i]->concentration * weight_matrix[counter][j];
					counter++;
				}
			}

			if(genes[j]->type == EXTRA_GENE)
			{
				cout<<"ARNBrain:: My genes aren't ordered correctly (TF->P->EXTRA)! Exiting!"<<endl;
				exit(1);
			}

			signal = signal / (double) (weight_matrix[0].size());
			double new_conc = signal * delta + genes[j]->concentration;
			// cout<<"new_conc: "<<new_conc<<" signal: "<<signal<<" delta: "<<delta<<" conc: "<<genes[j]->concentration<<endl;
			updated_conc.push_back(new_conc);
		}

		for(int j = 0; j < weight_matrix[0].size(); j++)
		{
			if(updated_conc[j] < 1e-10)
				updated_conc[j] = 1e-10;
			genes[j]->concentration = updated_conc[j];
		}
		updateConcentrations(false);
	}			
}

void ARNBrain::update() {
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::update() "<<endl;
	
	//Normalize the input values
	vector<double> normalizedInputs = normalizedInput();

	// Start setting up the inputs.
	if(ARN_SHOW_OUTPUT)
		cout<<"\n#################ARNBrain Inputs: ";

	// Update the concentration of the extra proteins
	inputIndex = 0;
	for(int i = 0; i < nrInputValues; i++)
	{
		if(ARN_SHOW_OUTPUT)
			cout<<normalizedInputs[i]<<" | ";
		//Set the input concentrations.
		setInput(normalizedInputs[i]);		
	}

	if(ARN_SHOW_OUTPUT)
		cout<<endl;

	//#########################################################This is where the regulation happens

	regulate();

	// cout<<"something"<<endl;
	if(ARN_SHOW_OUTPUT)
		cout<<"#################ARNBrain Output Mode: "<<outputMode<<" Output: ";

	if(pGenes.size() < 1)
	{
		cout<<"ARNBrain: I think I broke the pGenes."<<endl;
		exit(1);
	}
	double max_output = pGenes[0]->concentration;
	for(int m = 0; m < nrOutputValues; m++)
	{
		if(pGenes[m]->concentration > max_output)
			max_output = pGenes[m]->concentration;
	}
	bool max_output_flag = false;
	for(int i = 0; i < nrOutputValues; i++)
	{
		// cout<<outputMax<<" and "<<outputMin<<endl;

		double tempConc = pGenes[i]->concentration; 
		if(outputMode == 0 || outputMode == 2) //continious mode or integer mode
		{
			if(outputMax <= outputMin)
			{
				cout<<"ARNBrain: \"My outputMax is smaller or equal to my outputMin. Exiting!\"";
				exit(1);
			}
			double diff = outputMax - outputMin;
			if(outputMode == 2) // integer mode
			{
				outputValues[i] = round(diff * tempConc + outputMin);
				if(ARN_SHOW_OUTPUT)
					cout<<outputValues[i]<<" | ";
			}
			else // continious mode
			{
				outputValues[i] = diff * tempConc + outputMin;
				if(ARN_SHOW_OUTPUT)
					cout<<outputValues[i]<<" | ";
			}
		}else if(outputMode == 1 || outputMode == 3) //binary mode or the Fair Binary Mode
		{
			if(outputMode == 1)
				outputValues[i] = round(tempConc);
			else if (outputMode == 3)
			{
				double threshold = 1.0/pGenes.size()/2;
				if(tempConc >= threshold)
					outputValues[i] = 1;
				else if(tempConc < threshold)
					outputValues[i] = 0;
			}
			if(ARN_SHOW_OUTPUT)
				cout<<outputValues[i]<<" | ";
		}else if(outputMode == 4)
		{
			if(tempConc == max_output && !max_output_flag)
			{
				outputValues[i] = 1;
				max_output_flag = true;
			}
			else
				outputValues[i] = 0;
			if(ARN_SHOW_OUTPUT)
				cout<<outputValues[i]<<" | ";
		}else 
		{
			cout<<"ARNBrain: \"My outputMode is unknown to me! Exiting!\"";	
		}
	}
	if(ARN_SHOW_OUTPUT)
		cout<<endl<<endl;
}

string ARNBrain::description() {
	if(ARN_DEBUG_SUPER)
		cout<<"ARNBrain::description() "<<endl;
	string S = "Artificial Regulatory Brain\n";
	return S;
}

DataMap ARNBrain::getStats(string& prefix) {
	if(ARN_DEBUG_SUPER)
		cout<<"ARNBrain::getStats() "<<endl;
	DataMap dataMap;
	return (dataMap);
}

void ARNBrain::initializeGenomes(unordered_map<string, shared_ptr<AbstractGenome>>& _genomes) {
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::initializeGenomes() "<<endl;
	auto genomeName = genomeNamePL->get(PT);
	_genomes[genomeName]->fillRandom();
}

shared_ptr<AbstractBrain> ARNBrain::makeCopy(shared_ptr<ParametersTable> _PT)
{
	if(ARN_DEBUG || ARN_DEBUG_SUPER)
		cout<<"ARNBrain::makeCopy() "<<endl;
	if (_PT == nullptr) {
		_PT = PT;
	}
	auto newBrain = make_shared<ARNBrain>(nrInputValues, nrOutputValues, _PT);
	for(auto gene : genes)
	{
		shared_ptr<struct Gene> newGene { new struct Gene };
		for(int i = 0; i < 32; i++)
		{
			newGene->enhancer[i] = gene->enhancer[i];
			newGene->inhibitor[i] = gene->inhibitor[i];
			newGene->protein[i] = gene->protein[i];
		}
		newGene->concentration = gene->concentration;
		newGene->type = gene->type;
		newBrain->genes.push_back(newGene);
	}
	for(auto gene : tfGenes)
	{
		shared_ptr<struct Gene> newGene { new struct Gene };
		for(int i = 0; i < 32; i++)
		{
			newGene->enhancer[i] = gene->enhancer[i];
			newGene->inhibitor[i] = gene->inhibitor[i];
			newGene->protein[i] = gene->protein[i];
		}
		newGene->concentration = gene->concentration;
		newGene->type = gene->type;
		newBrain->tfGenes.push_back(newGene);
	}

	for(auto gene : extraGenes)
	{
		shared_ptr<struct Gene> newGene { new struct Gene };
		for(int i = 0; i < 32; i++)
		{
			newGene->enhancer[i] = gene->enhancer[i];
			newGene->inhibitor[i] = gene->inhibitor[i];
			newGene->protein[i] = gene->protein[i];
		}
		newGene->concentration = gene->concentration;
		newGene->type = gene->type;
		newBrain->extraGenes.push_back(newGene);
	}

	for(auto gene : pGenes)
	{
		shared_ptr<struct Gene> newGene { new struct Gene };
		for(int i = 0; i < 32; i++)
		{
			newGene->enhancer[i] = gene->enhancer[i];
			newGene->inhibitor[i] = gene->inhibitor[i];
			newGene->protein[i] = gene->protein[i];
		}
		newGene->concentration = gene->concentration;
		newGene->type = gene->type;
		newBrain->pGenes.push_back(newGene);
	}

	newBrain->weight_matrix = weight_matrix;

	return newBrain;
}


