all: mabe

mabe: objectFiles/main.o objectFiles/Global.o objectFiles/Group_Group.o objectFiles/Organism_Organism.o objectFiles/Utilities_Data.o objectFiles/Utilities_Parameters.o objectFiles/Utilities_Loader.o objectFiles/World_AbstractWorld.o objectFiles/Genome_AbstractGenome.o objectFiles/Brain_AbstractBrain.o objectFiles/Optimizer_AbstractOptimizer.o objectFiles/Archivist_DefaultArchivist.o objectFiles/Utilities_zupply.o objectFiles/World_XorWorld_XorWorld.o objectFiles/World_BerryWorld_BerryWorld.o objectFiles/World_TestWorld_TestWorld.o objectFiles/World_CoopWorld_CoopWorld.o objectFiles/World_MkWorld_MkWorld.o objectFiles/Genome_CircularGenome_CircularGenome.o objectFiles/Genome_MultiGenome_MultiGenome.o objectFiles/Genome_MultiGenome_Chromosome_AbstractChromosome.o objectFiles/Genome_MultiGenome_Chromosome_TemplatedChromosome.o objectFiles/Brain_CGPBrain_CGPBrain.o objectFiles/Brain_MarkovBrain_MarkovBrain.o objectFiles/Brain_MarkovBrain_GateListBuilder_GateListBuilder.o objectFiles/Brain_MarkovBrain_Gate_DecomposableGate.o objectFiles/Brain_MarkovBrain_Gate_TritDeterministicGate.o objectFiles/Brain_MarkovBrain_Gate_VoidGate.o objectFiles/Brain_MarkovBrain_Gate_GPGate.o objectFiles/Brain_MarkovBrain_Gate_AbstractGate.o objectFiles/Brain_MarkovBrain_Gate_FeedbackGate.o objectFiles/Brain_MarkovBrain_Gate_NeuronGate.o objectFiles/Brain_MarkovBrain_Gate_DeterministicGate.o objectFiles/Brain_MarkovBrain_Gate_DecomposableFeedbackGate.o objectFiles/Brain_MarkovBrain_Gate_EpsilonGate.o objectFiles/Brain_MarkovBrain_Gate_ProbabilisticGate.o objectFiles/Brain_MarkovBrain_GateBuilder_GateBuilder.o objectFiles/Brain_ConstantValuesBrain_ConstantValuesBrain.o objectFiles/Brain_HumanBrain_HumanBrain.o objectFiles/Brain_ARNBrain_ARNBrain.o objectFiles/Optimizer_SimpleOptimizer_SimpleOptimizer.o objectFiles/Archivist_LODwAPArchivist_LODwAPArchivist.o objectFiles/Archivist_SSwDArchivist_SSwDArchivist.o
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread objectFiles/main.o objectFiles/Global.o objectFiles/Group_Group.o objectFiles/Organism_Organism.o objectFiles/Utilities_Data.o objectFiles/Utilities_Parameters.o objectFiles/Utilities_Loader.o objectFiles/World_AbstractWorld.o objectFiles/Genome_AbstractGenome.o objectFiles/Brain_AbstractBrain.o objectFiles/Optimizer_AbstractOptimizer.o objectFiles/Archivist_DefaultArchivist.o objectFiles/Utilities_zupply.o objectFiles/World_XorWorld_XorWorld.o objectFiles/World_BerryWorld_BerryWorld.o objectFiles/World_TestWorld_TestWorld.o objectFiles/World_CoopWorld_CoopWorld.o objectFiles/World_MkWorld_MkWorld.o objectFiles/Genome_CircularGenome_CircularGenome.o objectFiles/Genome_MultiGenome_MultiGenome.o objectFiles/Genome_MultiGenome_Chromosome_AbstractChromosome.o objectFiles/Genome_MultiGenome_Chromosome_TemplatedChromosome.o objectFiles/Brain_CGPBrain_CGPBrain.o objectFiles/Brain_MarkovBrain_MarkovBrain.o objectFiles/Brain_MarkovBrain_GateListBuilder_GateListBuilder.o objectFiles/Brain_MarkovBrain_Gate_DecomposableGate.o objectFiles/Brain_MarkovBrain_Gate_TritDeterministicGate.o objectFiles/Brain_MarkovBrain_Gate_VoidGate.o objectFiles/Brain_MarkovBrain_Gate_GPGate.o objectFiles/Brain_MarkovBrain_Gate_AbstractGate.o objectFiles/Brain_MarkovBrain_Gate_FeedbackGate.o objectFiles/Brain_MarkovBrain_Gate_NeuronGate.o objectFiles/Brain_MarkovBrain_Gate_DeterministicGate.o objectFiles/Brain_MarkovBrain_Gate_DecomposableFeedbackGate.o objectFiles/Brain_MarkovBrain_Gate_EpsilonGate.o objectFiles/Brain_MarkovBrain_Gate_ProbabilisticGate.o objectFiles/Brain_MarkovBrain_GateBuilder_GateBuilder.o objectFiles/Brain_ConstantValuesBrain_ConstantValuesBrain.o objectFiles/Brain_HumanBrain_HumanBrain.o objectFiles/Brain_ARNBrain_ARNBrain.o objectFiles/Optimizer_SimpleOptimizer_SimpleOptimizer.o objectFiles/Archivist_LODwAPArchivist_LODwAPArchivist.o objectFiles/Archivist_SSwDArchivist_SSwDArchivist.o -o mabe

objectFiles/main.o: ./main.cpp
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./main.cpp -o objectFiles/main.o

objectFiles/Global.o: ./Global.cpp ./Global.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Global.cpp -o objectFiles/Global.o

objectFiles/Group_Group.o: ./Group/Group.cpp ./Group/Group.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Group/Group.cpp -o objectFiles/Group_Group.o

objectFiles/Organism_Organism.o: ./Organism/Organism.cpp ./Organism/Organism.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Organism/Organism.cpp -o objectFiles/Organism_Organism.o

objectFiles/Utilities_Data.o: ./Utilities/Data.cpp ./Utilities/Data.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Utilities/Data.cpp -o objectFiles/Utilities_Data.o

objectFiles/Utilities_Parameters.o: ./Utilities/Parameters.cpp ./Utilities/Parameters.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Utilities/Parameters.cpp -o objectFiles/Utilities_Parameters.o

objectFiles/Utilities_Loader.o: ./Utilities/Loader.cpp ./Utilities/Loader.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Utilities/Loader.cpp -o objectFiles/Utilities_Loader.o

objectFiles/World_AbstractWorld.o: ./World/AbstractWorld.cpp ./World/AbstractWorld.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./World/AbstractWorld.cpp -o objectFiles/World_AbstractWorld.o

objectFiles/Genome_AbstractGenome.o: ./Genome/AbstractGenome.cpp ./Genome/AbstractGenome.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Genome/AbstractGenome.cpp -o objectFiles/Genome_AbstractGenome.o

objectFiles/Brain_AbstractBrain.o: ./Brain/AbstractBrain.cpp ./Brain/AbstractBrain.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/AbstractBrain.cpp -o objectFiles/Brain_AbstractBrain.o

objectFiles/Optimizer_AbstractOptimizer.o: ./Optimizer/AbstractOptimizer.cpp ./Optimizer/AbstractOptimizer.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Optimizer/AbstractOptimizer.cpp -o objectFiles/Optimizer_AbstractOptimizer.o

objectFiles/Archivist_DefaultArchivist.o: ./Archivist/DefaultArchivist.cpp ./Archivist/DefaultArchivist.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Archivist/DefaultArchivist.cpp -o objectFiles/Archivist_DefaultArchivist.o

objectFiles/Utilities_zupply.o: ./Utilities/zupply.cpp ./Utilities/zupply.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Utilities/zupply.cpp -o objectFiles/Utilities_zupply.o

objectFiles/World_XorWorld_XorWorld.o: ./World/XorWorld/XorWorld.cpp ./World/XorWorld/XorWorld.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./World/XorWorld/XorWorld.cpp -o objectFiles/World_XorWorld_XorWorld.o

objectFiles/World_BerryWorld_BerryWorld.o: ./World/BerryWorld/BerryWorld.cpp ./World/BerryWorld/BerryWorld.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./World/BerryWorld/BerryWorld.cpp -o objectFiles/World_BerryWorld_BerryWorld.o

objectFiles/World_TestWorld_TestWorld.o: ./World/TestWorld/TestWorld.cpp ./World/TestWorld/TestWorld.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./World/TestWorld/TestWorld.cpp -o objectFiles/World_TestWorld_TestWorld.o

objectFiles/World_CoopWorld_CoopWorld.o: ./World/CoopWorld/CoopWorld.cpp ./World/CoopWorld/CoopWorld.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./World/CoopWorld/CoopWorld.cpp -o objectFiles/World_CoopWorld_CoopWorld.o

objectFiles/World_MkWorld_MkWorld.o: ./World/MkWorld/MkWorld.cpp ./World/MkWorld/MkWorld.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./World/MkWorld/MkWorld.cpp -o objectFiles/World_MkWorld_MkWorld.o

objectFiles/Genome_CircularGenome_CircularGenome.o: ./Genome/CircularGenome/CircularGenome.cpp ./Genome/CircularGenome/CircularGenome.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Genome/CircularGenome/CircularGenome.cpp -o objectFiles/Genome_CircularGenome_CircularGenome.o

objectFiles/Genome_MultiGenome_MultiGenome.o: ./Genome/MultiGenome/MultiGenome.cpp ./Genome/MultiGenome/MultiGenome.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Genome/MultiGenome/MultiGenome.cpp -o objectFiles/Genome_MultiGenome_MultiGenome.o

objectFiles/Genome_MultiGenome_Chromosome_AbstractChromosome.o: ./Genome/MultiGenome/Chromosome/AbstractChromosome.cpp ./Genome/MultiGenome/Chromosome/AbstractChromosome.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Genome/MultiGenome/Chromosome/AbstractChromosome.cpp -o objectFiles/Genome_MultiGenome_Chromosome_AbstractChromosome.o

objectFiles/Genome_MultiGenome_Chromosome_TemplatedChromosome.o: ./Genome/MultiGenome/Chromosome/TemplatedChromosome.cpp ./Genome/MultiGenome/Chromosome/TemplatedChromosome.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Genome/MultiGenome/Chromosome/TemplatedChromosome.cpp -o objectFiles/Genome_MultiGenome_Chromosome_TemplatedChromosome.o

objectFiles/Brain_CGPBrain_CGPBrain.o: ./Brain/CGPBrain/CGPBrain.cpp ./Brain/CGPBrain/CGPBrain.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/CGPBrain/CGPBrain.cpp -o objectFiles/Brain_CGPBrain_CGPBrain.o

objectFiles/Brain_MarkovBrain_MarkovBrain.o: ./Brain/MarkovBrain/MarkovBrain.cpp ./Brain/MarkovBrain/MarkovBrain.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/MarkovBrain.cpp -o objectFiles/Brain_MarkovBrain_MarkovBrain.o

objectFiles/Brain_MarkovBrain_GateListBuilder_GateListBuilder.o: ./Brain/MarkovBrain/GateListBuilder/GateListBuilder.cpp ./Brain/MarkovBrain/GateListBuilder/GateListBuilder.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/GateListBuilder/GateListBuilder.cpp -o objectFiles/Brain_MarkovBrain_GateListBuilder_GateListBuilder.o

objectFiles/Brain_MarkovBrain_Gate_DecomposableGate.o: ./Brain/MarkovBrain/Gate/DecomposableGate.cpp ./Brain/MarkovBrain/Gate/DecomposableGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/DecomposableGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_DecomposableGate.o

objectFiles/Brain_MarkovBrain_Gate_TritDeterministicGate.o: ./Brain/MarkovBrain/Gate/TritDeterministicGate.cpp ./Brain/MarkovBrain/Gate/TritDeterministicGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/TritDeterministicGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_TritDeterministicGate.o

objectFiles/Brain_MarkovBrain_Gate_VoidGate.o: ./Brain/MarkovBrain/Gate/VoidGate.cpp ./Brain/MarkovBrain/Gate/VoidGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/VoidGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_VoidGate.o

objectFiles/Brain_MarkovBrain_Gate_GPGate.o: ./Brain/MarkovBrain/Gate/GPGate.cpp ./Brain/MarkovBrain/Gate/GPGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/GPGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_GPGate.o

objectFiles/Brain_MarkovBrain_Gate_AbstractGate.o: ./Brain/MarkovBrain/Gate/AbstractGate.cpp ./Brain/MarkovBrain/Gate/AbstractGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/AbstractGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_AbstractGate.o

objectFiles/Brain_MarkovBrain_Gate_FeedbackGate.o: ./Brain/MarkovBrain/Gate/FeedbackGate.cpp ./Brain/MarkovBrain/Gate/FeedbackGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/FeedbackGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_FeedbackGate.o

objectFiles/Brain_MarkovBrain_Gate_NeuronGate.o: ./Brain/MarkovBrain/Gate/NeuronGate.cpp ./Brain/MarkovBrain/Gate/NeuronGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/NeuronGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_NeuronGate.o

objectFiles/Brain_MarkovBrain_Gate_DeterministicGate.o: ./Brain/MarkovBrain/Gate/DeterministicGate.cpp ./Brain/MarkovBrain/Gate/DeterministicGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/DeterministicGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_DeterministicGate.o

objectFiles/Brain_MarkovBrain_Gate_DecomposableFeedbackGate.o: ./Brain/MarkovBrain/Gate/DecomposableFeedbackGate.cpp ./Brain/MarkovBrain/Gate/DecomposableFeedbackGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/DecomposableFeedbackGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_DecomposableFeedbackGate.o

objectFiles/Brain_MarkovBrain_Gate_EpsilonGate.o: ./Brain/MarkovBrain/Gate/EpsilonGate.cpp ./Brain/MarkovBrain/Gate/EpsilonGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/EpsilonGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_EpsilonGate.o

objectFiles/Brain_MarkovBrain_Gate_ProbabilisticGate.o: ./Brain/MarkovBrain/Gate/ProbabilisticGate.cpp ./Brain/MarkovBrain/Gate/ProbabilisticGate.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/Gate/ProbabilisticGate.cpp -o objectFiles/Brain_MarkovBrain_Gate_ProbabilisticGate.o

objectFiles/Brain_MarkovBrain_GateBuilder_GateBuilder.o: ./Brain/MarkovBrain/GateBuilder/GateBuilder.cpp ./Brain/MarkovBrain/GateBuilder/GateBuilder.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/MarkovBrain/GateBuilder/GateBuilder.cpp -o objectFiles/Brain_MarkovBrain_GateBuilder_GateBuilder.o

objectFiles/Brain_ConstantValuesBrain_ConstantValuesBrain.o: ./Brain/ConstantValuesBrain/ConstantValuesBrain.cpp ./Brain/ConstantValuesBrain/ConstantValuesBrain.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/ConstantValuesBrain/ConstantValuesBrain.cpp -o objectFiles/Brain_ConstantValuesBrain_ConstantValuesBrain.o

objectFiles/Brain_HumanBrain_HumanBrain.o: ./Brain/HumanBrain/HumanBrain.cpp ./Brain/HumanBrain/HumanBrain.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/HumanBrain/HumanBrain.cpp -o objectFiles/Brain_HumanBrain_HumanBrain.o

objectFiles/Brain_ARNBrain_ARNBrain.o: ./Brain/ARNBrain/ARNBrain.cpp ./Brain/ARNBrain/ARNBrain.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Brain/ARNBrain/ARNBrain.cpp -o objectFiles/Brain_ARNBrain_ARNBrain.o

objectFiles/Optimizer_SimpleOptimizer_SimpleOptimizer.o: ./Optimizer/SimpleOptimizer/SimpleOptimizer.cpp ./Optimizer/SimpleOptimizer/SimpleOptimizer.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Optimizer/SimpleOptimizer/SimpleOptimizer.cpp -o objectFiles/Optimizer_SimpleOptimizer_SimpleOptimizer.o

objectFiles/Archivist_LODwAPArchivist_LODwAPArchivist.o: ./Archivist/LODwAPArchivist/LODwAPArchivist.cpp ./Archivist/LODwAPArchivist/LODwAPArchivist.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Archivist/LODwAPArchivist/LODwAPArchivist.cpp -o objectFiles/Archivist_LODwAPArchivist_LODwAPArchivist.o

objectFiles/Archivist_SSwDArchivist_SSwDArchivist.o: ./Archivist/SSwDArchivist/SSwDArchivist.cpp ./Archivist/SSwDArchivist/SSwDArchivist.h
	c++ -Wno-c++98-compat -w -Wall -std=c++11 -O3 -lpthread -pthread -c ./Archivist/SSwDArchivist/SSwDArchivist.cpp -o objectFiles/Archivist_SSwDArchivist_SSwDArchivist.o

clean:
	rm -r objectFiles/* mabe

cleanup:
	rm -r objectFiles/*
