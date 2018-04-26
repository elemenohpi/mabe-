//  MABE is a product of The Hintza Lab @ MSU
//     for general research information:
//         http://hintzelab.msu.edu/
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//          github.com/Hintzelab/MABE/wiki

//  This file was auto-generated with MBuilder.py

#ifndef __AutoBuild__Modules__
#define __AutoBuild__Modules__
#include "World/XorWorld/XorWorld.h"
#include "World/BerryWorld/BerryWorld.h"
#include "World/TestWorld/TestWorld.h"
#include "World/CoopWorld/CoopWorld.h"
#include "Genome/CircularGenome/CircularGenome.h"
#include "Genome/MultiGenome/MultiGenome.h"
#include "Brain/CGPBrain/CGPBrain.h"
#include "Brain/MarkovBrain/MarkovBrain.h"
#include "Brain/ConstantValuesBrain/ConstantValuesBrain.h"
#include "Brain/HumanBrain/HumanBrain.h"
#include "Brain/ARNBrain/ARNBrain.h"
#include "Optimizer/SimpleOptimizer/SimpleOptimizer.h"

#include "Archivist/DefaultArchivist.h"
#include "Archivist/LODwAPArchivist/LODwAPArchivist.h"
#include "Archivist/SSwDArchivist/SSwDArchivist.h"


//create a world
shared_ptr<AbstractWorld> makeWorld(shared_ptr<ParametersTable> PT){
  shared_ptr<AbstractWorld> newWorld;
  bool found = false;
  string worldType = AbstractWorld::worldTypePL->get(PT);
  if (worldType == "Xor") {
    newWorld = make_shared<XorWorld>(PT);
    found = true;
    }
  if (worldType == "Berry") {
    newWorld = make_shared<BerryWorld>(PT);
    found = true;
    }
  if (worldType == "Test") {
    newWorld = make_shared<TestWorld>(PT);
    found = true;
    }
  if (worldType == "Coop") {
    newWorld = make_shared<CoopWorld>(PT);
    found = true;
    }
  if (!found){
    cout << "  ERROR! could not find WORLD-worldType \"" << worldType << "\".\n  Exiting." << endl;
    exit(1);
    }
  return newWorld;
}


//create an optimizer
shared_ptr<AbstractOptimizer> makeOptimizer(shared_ptr<ParametersTable> PT){
  shared_ptr<AbstractOptimizer> newOptimizer;
  bool found = false;
  string optimizerType = AbstractOptimizer::Optimizer_MethodStrPL->get(PT);
  if (optimizerType == "Simple") {
    newOptimizer = make_shared<SimpleOptimizer>(PT);
    found = true;
    }
  if (!found){
    cout << "  ERROR! could not find OPTIMIZER-optimizer \"" << optimizerType << "\".\n  Exiting." << endl;
    exit(1);
    }
  return newOptimizer;
}


//create an archivist
shared_ptr<DefaultArchivist> makeArchivist(vector<string> popFileColumns, shared_ptr<Abstract_MTree> _maxFormula, shared_ptr<ParametersTable> PT, string groupPrefix = ""){
  shared_ptr<DefaultArchivist> newArchivist;
  bool found = false;
  string archivistType = DefaultArchivist::Arch_outputMethodStrPL->get(PT);
  if (archivistType == "LODwAP") {
    newArchivist = make_shared<LODwAPArchivist>(popFileColumns, _maxFormula, PT, groupPrefix);
    found = true;
    }
  if (archivistType == "SSwD") {
    newArchivist = make_shared<SSwDArchivist>(popFileColumns, _maxFormula, PT, groupPrefix);
    found = true;
    }
  if (archivistType == "Default") {
    newArchivist = make_shared<DefaultArchivist>(popFileColumns, _maxFormula, PT, groupPrefix);
    found = true;
    }
  if (!found){
    cout << "  ERROR! could not find ARCHIVIST-outputMethod \"" << archivistType << "\".\n  Exiting." << endl;
    exit(1);
    }
  return newArchivist;
}


//create a template genome
shared_ptr<AbstractGenome> makeTemplateGenome(shared_ptr<ParametersTable> PT){
  shared_ptr<AbstractGenome> newGenome;
  bool found = false;
  string genomeType = AbstractGenome::genomeTypeStrPL->get(PT);
  if (genomeType == "Circular") {
    newGenome = CircularGenome_genomeFactory(PT);
    found = true;
    }
  if (genomeType == "Multi") {
    newGenome = MultiGenome_genomeFactory(PT);
    found = true;
    }
  if (found == false){
    cout << "  ERROR! could not find GENOME-genomeType \"" << genomeType << "\".\n  Exiting." << endl;
    exit(1);
    }
  return newGenome;
}


//create a template brain
shared_ptr<AbstractBrain> makeTemplateBrain(int inputs, int outputs, shared_ptr<ParametersTable> PT){
  shared_ptr<AbstractBrain> newBrain;
  bool found = false;
  string brainType = AbstractBrain::brainTypeStrPL->get(PT);
  if (brainType == "CGP") {
    newBrain = CGPBrain_brainFactory(inputs, outputs, PT);
    found = true;
    }
  if (brainType == "Markov") {
    newBrain = MarkovBrain_brainFactory(inputs, outputs, PT);
    found = true;
    }
  if (brainType == "ConstantValues") {
    newBrain = ConstantValuesBrain_brainFactory(inputs, outputs, PT);
    found = true;
    }
  if (brainType == "Human") {
    newBrain = HumanBrain_brainFactory(inputs, outputs, PT);
    found = true;
    }
  if (brainType == "ARN") {
    newBrain = ARNBrain_brainFactory(inputs, outputs, PT);
    found = true;
    }
  if (found == false){
    cout << "  ERROR! could not find BRAIN-brainType \"" << brainType << "\".\n  Exiting." << endl;
    exit(1);
    }
  return newBrain;
}


//configure Defaults and Documentation
void configureDefaultsAndDocumentation(){
  Parameters::root->setParameter("BRAIN-brainType", (string)"CGP");
  Parameters::root->setDocumentation("BRAIN-brainType", "brain to be used, [CGP, Markov, ConstantValues, Human, ARN]");

  Parameters::root->setParameter("GENOME-genomeType", (string)"Circular");
  Parameters::root->setDocumentation("GENOME-genomeType", "genome to be used, [Circular, Multi]");

  Parameters::root->setParameter("ARCHIVIST-outputMethod", (string)"LODwAP");
  Parameters::root->setDocumentation("ARCHIVIST-outputMethod", "output method, [LODwAP, SSwD, Default]");

  Parameters::root->setParameter("OPTIMIZER-optimizer", (string)"Simple");
  Parameters::root->setDocumentation("OPTIMIZER-optimizer", "optimizer to be used, [Simple]");

  Parameters::root->setParameter("WORLD-worldType", (string)"Xor");
  Parameters::root->setDocumentation("WORLD-worldType","world to be used, [Xor, Berry, Test, Coop]");
}


#endif /* __AutoBuild__Modules__ */
