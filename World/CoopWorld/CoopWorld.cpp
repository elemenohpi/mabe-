// ToDo:: Add experience effect to the tasks
// ToDo:: Make the code run faster

//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/Hintzelab/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/Hintzelab/MABE/wiki/License
// 
//  The Cooperation World is Designed by Iliya Miralavy (eLeMeNohPi) and Darren Incorvaia. 
//  Copyright (c) 2018 Michigan State University. All rights reserved.

/*
############ IMPORTANT NOTES
- Team ID starts from 1 and not 0. Team 1 = 1, Team 2 = 2 etc.

############ GRID

0 = Unavailable
0.005 = Available 
0.010 = FOOD
0.015 = Agent
0.020 = Structure
0.025 = Bullet
	
############ BRAIN

0.025 = TRUE

Inputs (12) - Sum of the inputs has the maximum value of 0.3
	i0 = hasBuildingRes
	i1 = hasShootingRes
	i2 = Sense[8]
	i10 = FacingX
	i11 = FacingY
Outputs (7) - Sum of the outputs has the maximum value of 0.1
	i0 = Move
	i1 = TurnL
	i2 = TurnR
	i3 = Shoot
	i4 = Build
	i5 = Harvest
	i6 = Nothing
*/

//##########################################	Things we can try 	 #####################################################
// Give the agents an initial task
// Adding searching bonus for harvesters
// Should agents know if they're going to die? like HP? like score?
// Change the evolution strategy. The way it works now is that every team member has an identical brain. So one brain should learn how to evovle cooperation O.o
// Change the player selection methods. How they team up
// Organisms with 3 brains instead of copying the organism three times.
// Playing with the world parameters
// Adding Hitpoint to the agents to be used instead of score 0. We will need a different score (fitness) function
// Resetting brains vs. not doing that (I'm not totally aware what reset brains does tbh)
// Penalize the agents for every mistake they make. Such as shooting when no resource is available
// Tweak the vision algorithm
// Change the restricted area inbetween agent regions to a poison/sea area
// Did I already say to try the task-switching penalty?
// Should we give a compass to our agents? Or a memory or the direction their facing? Cuz shooters!
// We can tweak the vision algorithms!
// Tweak the spaces that are allowed for the agents to move to
// Make grid of objects instead of the grid of numbers
// Giving score for successfully defending?
// What if organisms play against themselves? (regions are different so they will learn different playing conditions, right?)
// Change the fitness fomula. Give it to the settings.cfg file
// Agents should get rewarded for winning fast
//##########################################	End of Things we can try 	##############################################


//ToDo:: Task Switching!

#include "CoopWorld.h"

shared_ptr<ParameterLink<int>> CoopWorld::modePL = Parameters::register_parameter("WORLD_COOP-mode", 0, "0 = bit outputs before adding, 1 = add outputs");
shared_ptr<ParameterLink<int>> CoopWorld::numberOfOutputsPL = Parameters::register_parameter("WORLD_COOP-numberOfOutputs", 7, "number of outputs in this world");
shared_ptr<ParameterLink<int>> CoopWorld::evaluationsPerGenerationPL = Parameters::register_parameter("WORLD_COOP-evaluationsPerGeneration", 1, "Number of times to test each Genome per generation (useful with non-deterministic brains)");
shared_ptr<ParameterLink<string>> CoopWorld::groupNamePL = Parameters::register_parameter("WORLD_COOP_NAMES-groupNameSpace", (string)"root::", "namespace of group to be evaluated");
shared_ptr<ParameterLink<string>> CoopWorld::brainNamePL = Parameters::register_parameter("WORLD_COOP_NAMES-brainNameSpace", (string)"root::", "namespace for parameters used to define brain");
shared_ptr<ParameterLink<int>> CoopWorld::nBackPL = Parameters::register_parameter("WORLD_COOP-numberOfBackIntegers", 3, "number of integers the brain should remember");

class Game {
	private:
		//Agents
		struct Agent {
			int id;
			int x;
			int y;
			int team_id;
			int facingX;
			int facingY; 
			float task = 0;
			/*
			how facing direction works:
			-1-1  +0-1  +1-1
	
			-1+0  AAAA  +1+0
			
			-1+1  +0+1  +1+1
			*/
		};

		//Teams
		struct Team {
			int id;
			vector<shared_ptr<struct Agent>> agents;
			double resource = 50;
			double score = 50;
			bool lost = false;
			bool hasFood = false;

			//ToDo:: log these for behavoral analysis
			int totalHarvest = 0;
			int totalShots = 0;
			int totalSuccessfulShots = 0;
			int totalGotHit = 0;
			int totalSwitch = 0;
			int totalFF = 0;
			int totalBuilt = 0;
			int totalStructFF = 0;
			int totalStructHit = 0;
			int totalStructGotHit = 0;
			double sGained = 0;
			double sLost = 0;
		};

		//Bullets
		struct Bullet {
			int shooter; //Who shot the bullet
			int x;
			int y;
			int facingX;
			int facingY; 
			int id;
		};

		//Vector of teams
		vector<shared_ptr<struct Team>> teams;

		//Vector of bullets
		vector<shared_ptr<struct Bullet>> bullets;

		//Vectors of the four different organisms
		vector<shared_ptr<Organism>> p1;
		vector<shared_ptr<Organism>> p2;
		vector<shared_ptr<Organism>> p3;
		vector<shared_ptr<Organism>> p4;

		//Pointers to the brain name and the parameter table
		shared_ptr<ParameterLink<string>> brainNamePL;
		shared_ptr<ParametersTable> PT;

		//Grid Constants
		const double NA     = 0;
		const double EMPTY  = 0.005;
		const double FOOD   = 0.010;
		const double AGENT  = 0.015;
		const double STRUCT = 0.020;
		const double BULLET = 0.025;

		//Task Constants
		const int NONE = 0;
		const float HARVESTER = 0.001;
		const float ATTACKER = 0.002;
		const float BUILDER = 0.003;

		//World Constants and Parameters
		const double T = 0.025; //True value
		const double F = 0; //False value

		double grid[16][16]; 
		int gridSize = (sizeof(grid)/sizeof(*grid));

		//SHOOTING Parameters
		const double HIT_SCORE = 3; //Score gained for successful agent hits
		const double SHOOT_RES = 4; //Resource needed for shooting
		const double HIT_PENALTY = 3; //Penalty for getting shot
		const double FF_PENALTY = -1; //Penalty for friendly fire
		
		//BUILDING Parameters
		const double STRUCT_HIT_SCORE = 1; //Score gained for successfully hitting structures
		const double BUILD_RES = 1; //Resource needed for building
		const double STRUCT_HIT_PENALTY = 0; //Penalty for shooting at own structures
		const double STRUCT_DEFEND = 5; //Reward for successfully defending the bullets

		//HARVESTING Parameters
		const double RES_SCORE = 5; //Score gained for harvesting 
		const double FOOD_REWARD = 3; //The amount of resource reward for harvesting
		
		// const double ELIMINATION_REWARD = 50; //Reward for successfully eliminating another team. This hasn't been coded yet. We might not need it at all
		const double WIN_REWARD = 0; //Reward for winning the game
		const double SWITCH_PENALTY = 0; //Penalty of task switching
	
		const int MAX_CYCLES = 1000; //Maximum allowed cycles in the game

		unsigned long long int unique_number = 1; //This number, magically, will always be unique for assigning to indexes!



		const int START_SCORE = 50; //DO NOT USE THIS TO CHANGE THE STARTING SCORE. CHANGE IT ACCORDING TO THE START SCORE DEFINED IN TEAM STRUCT

		double final_scores[4]; //To save team scores

		int exp_number;

	public:
		Game(vector<shared_ptr<Organism>> p1, vector<shared_ptr<Organism>> p2, vector<shared_ptr<Organism>> p3, vector<shared_ptr<Organism>> p4, shared_ptr<ParameterLink<string>> brainNamePL, shared_ptr<ParametersTable> PT, int exp_number)
		{
			this->exp_number = exp_number;
			//Visualization
			//ToDo:: Fix the condition
			if(true)
			{
				ofstream file;
				ostringstream oss; 
				oss<<"Results/"<<Global::update<<"_"<<exp_number<<".txt";
				file.open (oss.str(), ios::out);
				file << gridSize << "\n";
				file.close();
			}

			// if(exp_number > 1)
			// 	exit(1);

			//Game over condition
			bool gameOver = false;

			//Initialize the grid
			initGrid();	

			//Initialize the teams. P1 to P4 are not actually players. We only need their brains. The game has it's own agent/team structure 
			initTeams();

			//Save a global instance of all of the organisms and the parameter table
			this->p1 = p1;
			this->p2 = p2;
			this->p3 = p3;
			this->p4 = p4;
			this->brainNamePL = brainNamePL;
			this->PT = PT;

			//Run the game
			for(int i = 0; i < MAX_CYCLES && !gameOver; i++)
			{
				int cycle = i;
				// for(auto b: bullets)
				// {
				// 	cout<<b->id<<" "<<b->x<<","<<b->y<<" "<<b->shooter<<endl;
				// }
				// cout<<bullets.size()<<endl;

				//Let Agents do their things. Here the brains should be used
				//For each team we have
								

				for(shared_ptr<struct Team> team : teams)
				{
					if(team->lost)
						continue;
					//For each agent we have in all the teams. Extra amount of codes due to avoiding the vector of vectors for our organisms
					for(int i = 0; i < 3; i++)
					{	
						//If a team doesn't have food, then TRY to place one!						
						if(!team->hasFood)
						{
							placeFood(team->id);
						}
						
						shared_ptr<struct Agent> agent = team->agents[i];
						// auto brain = p1[i]->brains[brainNamePL->get(PT)]; //initial input. This will be changed soonish. This might raise errors.
						//ToDo:: remove if this works
						shared_ptr<AbstractBrain> brain;

						//Get the brain depending on the team_id
						if(team->id == 1)
						{
							//We need our brain first
							brain = p1[i]->brains[brainNamePL->get(PT)];
							//Then we need our sensory information
						}else if(team->id == 2)
						{
							brain = p2[i]->brains[brainNamePL->get(PT)];
						}else if(team->id == 3)
						{
							brain = p3[i]->brains[brainNamePL->get(PT)];
						}else if(team->id == 4)
						{
							brain = p4[i]->brains[brainNamePL->get(PT)];
						}else {
							cout<<"\n\n\nInvalid team id! Exiting!\n\n\n";
							exit(1);
						}

						//Set the brain inputs
						//0
						brain->setInput(0, hasShootingRes(team));
						//1
						brain->setInput(1, hasBuildingRes(team));
						//2 to 7
						auto vision = getVision(agent, 0);
						for(int iter = 0; iter<6; iter++)
						{
							brain->setInput(iter+2, vision[iter]); //Set the vision inputs
						}
						//8
						//facingX and facingY are in the range of -1 to 1. They should be between 0 to 0.025
						brain->setInput(8, (agent->facingX + 1.0)/8.0);
						//9
						brain->setInput(9, (agent->facingY + 1.0)/8.0);
						//12 agent id
						// brain->setInput(13, 1/team->score);
						// //ToDo:: delete?^^
						// brain->setInput(12, agent->task);
						//ToDo:: delete?^^

						//Update the brain
						brain->update();

						//Read the outputs
						double action[7];
						for(int iter = 0; iter < 7; iter++)
						{
							action[iter] = brain->readOutput(iter);
						}

						//The largest output is the action
										
						doAction(agent, team, maxIndex(action, 7), cycle);	
					}
				}

				//visualization
				//ToDo:: Fix the condition
				//ax,ay t ax,ay t ax,ay t 100 ax,ay t ax,ay t ax,ay t 100 -bx,by bx,by bx,by -s,x,y f,x,y f,x,y s,x,ySPACE
				//agent score agent score agent score-bullets
				if(true)
				{
					//Open the file
					ofstream file;
					ostringstream oss; 
					oss<<"Results/"<<Global::update<<"_"<<exp_number<<".txt";
					file.open (oss.str(), ios::app);
					file<<cycle<<":";
					for(auto team : teams)
					{
						for(auto agent : team->agents)
						{
							file<<agent->x<<","<<agent->y<<" "<<agent->task<<" "<<agent->facingX<<" "<<agent->facingY<<" ";
						}
						file<<team->score<<" ";
						file<<team->resource<<" ";
					}
					file<<":";
					//Bullet positions
					for(auto bullet : bullets)
					{
						file<<bullet->x<<","<<bullet->y<<" ";
					}
					file<<":";
					//Structure and food positions
					for(int i = 0; i < gridSize; i++)
					{
						for(int j = 0; j < gridSize; j++)
						{	
							if(grid[i][j] == EMPTY && !isAgentThere(i, j))
							{
								continue;
							}else if(grid[i][j] == STRUCT)
							{
								file<<"s,"<<i<<","<<j<<" ";
							}else if(grid[i][j] == FOOD)
							{
								file<<"f,"<<i<<","<<j<<" ";
							}	
						}	
					}
					file<<"\n";
					//Close the file
					file.close();
				}

				updateBullets();

				//Check if any team lost
				for (int c = 0; c < teams.size(); c++)
				{
					if(teams[c]->score < 0)
					{
						final_scores[teams[c]->id-1] = teams[c]->score;
						teams[c]->lost = true;
						for(auto a : teams[c]->agents)
						{
							grid[a->x][a->y] = EMPTY;
							a->x = -10;
							a->y = -10;
						}
					}
				}
					
				//Check the game over scenario
				int lost_team_count = 0;
				for(auto t : teams)
				{
					if(t->lost)
						lost_team_count++;
				}
				if(lost_team_count == 3)
				{
					gameOver = true;
					//Somebody won the game
				}else if(lost_team_count == 4)
				{
					gameOver = true;
					//it was a tie loss
				}


			}

			//End of the game
			double accuracies[4];
			for(auto team : teams)
			{
				final_scores[team->id-1] = team->score - START_SCORE;
				if(team->totalShots == 0)
					accuracies[team->id-1] = 0;
				else
					accuracies[team->id-1] = (team->totalSuccessfulShots + team->totalStructHit * 1.0) / (team->totalShots * 1.0) * 100;
			}

			auto winner = findTeam(maxIndex(final_scores, 4)+1);
			final_scores[maxIndex(final_scores, 4)] += WIN_REWARD;
			winner->sGained += WIN_REWARD;
			
			// cout<<"Winner "<<winner->id<<"\t score: "<<winner->score+WIN_REWARD - START_SCORE<<"\t harvest: "<<winner->totalHarvest<<"\t built: "<<winner->totalBuilt;
			// cout<<"\t shot: "<<winner->totalShots<<"\t HitAgents: "<<winner->totalSuccessfulShots<<"\t HitStruct: "<<winner->totalStructHit;
			// cout<<"\t Accuracy: "<<accuracy<<"%";
			// cout<<"\t TSwitch: "<<winner->totalSwitch<<endl;
			
			//Score the players
			p1[0]->dataMap.append("score", final_scores[0]);
			p1[0]->dataMap.append("accuracy", accuracies[0]);
			p1[0]->dataMap.append("totalShots", findTeam(1)->totalShots);
			p1[0]->dataMap.append("totalHarvest", findTeam(1)->totalHarvest);
			p1[0]->dataMap.append("totalSuccessfulShots", findTeam(1)->totalSuccessfulShots);
			p1[0]->dataMap.append("totalGotHit", findTeam(1)->totalGotHit);
			p1[0]->dataMap.append("totalSwitch", findTeam(1)->totalSwitch);
			p1[0]->dataMap.append("totalFF", findTeam(1)->totalFF);
			p1[0]->dataMap.append("totalBuilt", findTeam(1)->totalBuilt);
			p1[0]->dataMap.append("totalStructFF", findTeam(1)->totalStructFF);
			p1[0]->dataMap.append("totalStructHit", findTeam(1)->totalStructHit);
			p1[0]->dataMap.append("totalStructGotHit", findTeam(1)->totalStructGotHit);
			p1[0]->dataMap.append("sGained", findTeam(1)->sGained);
			p1[0]->dataMap.append("sLost", findTeam(1)->sLost);

			p2[0]->dataMap.append("score", final_scores[1]);
			p2[0]->dataMap.append("accuracy", accuracies[1]);	
			p2[0]->dataMap.append("totalShots", findTeam(2)->totalShots);
			p2[0]->dataMap.append("totalHarvest", findTeam(2)->totalHarvest);
			p2[0]->dataMap.append("totalSuccessfulShots", findTeam(2)->totalSuccessfulShots);
			p2[0]->dataMap.append("totalGotHit", findTeam(2)->totalGotHit);
			p2[0]->dataMap.append("totalSwitch", findTeam(2)->totalSwitch);
			p2[0]->dataMap.append("totalFF", findTeam(2)->totalFF);
			p2[0]->dataMap.append("totalBuilt", findTeam(2)->totalBuilt);
			p2[0]->dataMap.append("totalStructFF", findTeam(2)->totalStructFF);
			p2[0]->dataMap.append("totalStructHit", findTeam(2)->totalStructHit);
			p2[0]->dataMap.append("totalStructGotHit", findTeam(2)->totalStructGotHit);
			p2[0]->dataMap.append("sGained", findTeam(2)->sGained);
			p2[0]->dataMap.append("sLost", findTeam(2)->sLost);

			p3[0]->dataMap.append("score", final_scores[2]);
			p3[0]->dataMap.append("accuracy", accuracies[2]);
			p3[0]->dataMap.append("totalShots", findTeam(3)->totalShots);
			p3[0]->dataMap.append("totalHarvest", findTeam(3)->totalHarvest);
			p3[0]->dataMap.append("totalSuccessfulShots", findTeam(3)->totalSuccessfulShots);
			p3[0]->dataMap.append("totalGotHit", findTeam(3)->totalGotHit);
			p3[0]->dataMap.append("totalSwitch", findTeam(3)->totalSwitch);
			p3[0]->dataMap.append("totalFF", findTeam(3)->totalFF);
			p3[0]->dataMap.append("totalBuilt", findTeam(3)->totalBuilt);
			p3[0]->dataMap.append("totalStructFF", findTeam(3)->totalStructFF);
			p3[0]->dataMap.append("totalStructHit", findTeam(3)->totalStructHit);
			p3[0]->dataMap.append("totalStructGotHit", findTeam(3)->totalStructGotHit);
			p3[0]->dataMap.append("sGained", findTeam(3)->sGained);
			p3[0]->dataMap.append("sLost", findTeam(3)->sLost);

			p4[0]->dataMap.append("score", final_scores[3]);
			p4[0]->dataMap.append("accuracy", accuracies[3]);
			p4[0]->dataMap.append("totalShots", findTeam(4)->totalShots);
			p4[0]->dataMap.append("totalHarvest", findTeam(4)->totalHarvest);
			p4[0]->dataMap.append("totalSuccessfulShots", findTeam(4)->totalSuccessfulShots);
			p4[0]->dataMap.append("totalGotHit", findTeam(4)->totalGotHit);
			p4[0]->dataMap.append("totalSwitch", findTeam(4)->totalSwitch);
			p4[0]->dataMap.append("totalFF", findTeam(4)->totalFF);
			p4[0]->dataMap.append("totalBuilt", findTeam(4)->totalBuilt);
			p4[0]->dataMap.append("totalStructFF", findTeam(4)->totalStructFF);
			p4[0]->dataMap.append("totalStructHit", findTeam(4)->totalStructHit);
			p4[0]->dataMap.append("totalStructGotHit", findTeam(4)->totalStructGotHit);
			p4[0]->dataMap.append("sGained", findTeam(4)->sGained);
			p4[0]->dataMap.append("sLost", findTeam(4)->sLost);
		}


		//Updates all the bullets
		void updateBullets()
		{
			vector<int> remove_ids;
			//Update the bullets
			for(auto bullet : bullets)
			{
				//Check if hit
				if((grid[bullet->x][bullet->y] != EMPTY || isAgentThere(bullet->x, bullet->y)) && grid[bullet->x][bullet->y] != FOOD && grid[bullet->x][bullet->y] != NA)
				{
					//The bullet hit
					int team_id = getRegion(bullet->x, bullet->y);
					auto team = findTeam(team_id); 					
					if(isAgentThere(bullet->x, bullet->y))
					{
						if(team_id == bullet->shooter)
						{
							//Friendly fire
							if(team == NULL)
							{
								cout<<"\nUnexpected Error: Team not found whereas an agent of that team is hit. Exiting\n";
								exit(1);
							}
							team->score -= HIT_PENALTY;
							team->sLost += HIT_PENALTY;
							team->score -= FF_PENALTY;
							team->sLost += FF_PENALTY;
							// Log
							team->totalFF++;
							team->totalGotHit++;
						}else 
						{
							//Target hit
							if(team != NULL)
							{
								team->score -= HIT_PENALTY;
								team->sLost += HIT_PENALTY;
								team->totalGotHit++;
								findTeam(bullet->shooter)->score += HIT_SCORE;
								findTeam(bullet->shooter)->sGained += HIT_SCORE;
								findTeam(bullet->shooter)->totalSuccessfulShots++;
							}
						}
						//Remove the bullet
						remove_ids.push_back(bullet->id);
					} else if(grid[bullet->x][bullet->y] == STRUCT)
					{
						if(team_id == bullet->shooter)
						{
							//Friendly Fire
							team->score -= STRUCT_HIT_PENALTY;
							team->sLost += STRUCT_HIT_PENALTY;
							team->totalStructFF++;
							team->totalStructGotHit++;
						}else 
						{
							//Successful Defend
							findTeam(bullet->shooter)->score += STRUCT_HIT_SCORE;
							findTeam(bullet->shooter)->sGained += STRUCT_HIT_SCORE;
							team->score += STRUCT_DEFEND;
							team->sGained += STRUCT_DEFEND;
							team->totalStructGotHit++;
							findTeam(bullet->shooter)->totalStructHit++;
						}
						//Remove the structure and the bullet

						grid[bullet->x][bullet->y] = EMPTY;
						remove_ids.push_back(bullet->id);
					}
				}else 
				{
					//Move the bullet
					int X, Y;
					frontGridSpaceB(bullet, X, Y);
					// cout<<"File: "<<Global::update<<"_"<<exp_number<<"\t"<<bullet->x<<","<<bullet->y<<" facing: "<<bullet->facingX<<"|"<<bullet->facingY<<" ==> "<<X<<","<<Y<<endl;
					//Check to see if the bullet will be in grid or not
					if(!isInGrid(X, Y))
					{
						//Remove the bullet
						remove_ids.push_back(bullet->id);
						//cout<<"this happened"<<endl;
					}else 
					{
						//Move the bullet forward
						bullet->x = X;
						bullet->y = Y;
					}
				}
			}
			
			//Now remove all the bulelts
			for(int id : remove_ids)
			{
				removeBullet(id);
			}
		}

		bool isAgentThere(int x, int y)
		{
			for(auto team : teams)
				for(auto agent : team->agents)
				{
					if(agent->x == x && agent->y == y)
					{
						return true;
					}
				}
			return false;
		}

		//Finds a team with a given ID
		shared_ptr<struct Team> findTeam(int id)
		{
			for(auto team : teams)
			{
				if (team->id == id)
					return team;
			}
			return NULL;
		}

		//removes a bullet from the game
		void removeBullet(int id)
		{
			for(int i = 0; i<bullets.size(); i++)
			{
				if(bullets[i]->id == id)
				{
					bullets.erase(bullets.begin()+i);
					return;
				}
			}
		}

		//Gives back the region_id (team_id) of a given coordination
		int getRegion(int x, int y)
		{
			if(!isInGrid(x, y))
				return -1;
			if(x <= gridSize/2 && y <= gridSize/2)
				return 1;
			if(x <= gridSize/2 && y >= gridSize/2)
				return 2;
			if(x >= gridSize/2 && y <= gridSize/2)
				return 3;
			if(x >= gridSize/2 && y >= gridSize/2)
				return 4;
			return 0;
		}

		//Sets all the grid values to -1 meaning it's inaccessible
		void resetGrid()
		{
			for (int i = 0; i < gridSize; i++)
			{
				for (int j = 0; j < gridSize; j++)
				{
					grid[i][j] = NA;
				}
			}
		}

		//ToDo:: Fix this if necessary
		void visualizeGrid()
		{
			cout<<"\n\n";
			for(int i = 0; i < gridSize; i++)
			{
				for(int j = 0; j <gridSize; j++)
				{
					cout<<grid[i][j]<<"\t";
				}
				cout<<endl;
			}
			cout<<endl;
		}

		//Does one of the possible actiosn
		void doAction(shared_ptr<struct Agent> agent, shared_ptr<struct Team> team, int index, int cycle)
		{		
			switch(index)
			{
				case 0:
					//Move 
					move(agent);
					break;
				case 1:
					//Turn L
					turnL(agent);
					break;
				case 2:
					//Turn R
					turnR(agent);
					break;
				case 3:
					//Shoot towards front
					if(agent->task == NONE)
					{
						agent->task = ATTACKER;
					}else if(agent->task != ATTACKER)
					{
						team->score -= SWITCH_PENALTY;
						team->sLost += SWITCH_PENALTY;
						agent->task = ATTACKER;
						team->totalSwitch++;
					}
					shoot(agent, team, cycle);
					break;
				case 4:
					//Build in front
					if(agent->task == NONE)
					{
						agent->task = BUILDER;
					}else if(agent->task != BUILDER)
					{
						team->score -= SWITCH_PENALTY;
						team->sLost += SWITCH_PENALTY;
						agent->task = BUILDER;
						team->totalSwitch++;
					}
					build(agent, team);
					break;
				case 5: 
					//Harvest the food
					if(agent->task == NONE)
					{
						agent->task = HARVESTER;
					}else if(agent->task != HARVESTER)
					{
						team->score -= SWITCH_PENALTY;
						team->sLost += SWITCH_PENALTY;
						agent->task = HARVESTER;
						team->totalSwitch++;
					}
					harvest(agent, team);
					break;
				default:
					//Do Nothing
					break;
			}
		}

		//Shoots towards the front
		void shoot(shared_ptr<struct Agent> agent, shared_ptr<struct Team> team, int cycle)
		{
			//Check if the team has enough resources
			if(!hasShootingRes(team))
				return;
			// Find the front space
			int X, Y;
			frontGridSpace(agent, X, Y);
			//Check if the front space is in grid or not
			if(!isInGrid(X, Y))
				return;
			//Substract the cost
			team->resource -= SHOOT_RES;
			// Log
			team->totalShots++;
			//Make the bullet
			shared_ptr<struct Bullet> bullet { new struct Bullet};
			bullet->shooter = agent->team_id;
			bullet->x = X;
			bullet->y = Y;
			bullet->facingX = agent->facingX;
			bullet->facingY = agent->facingY;
			bullet->id = unique_number++;
			bullets.push_back(bullet);	
			// cout<<"here\n";
			// cout<<cycle<<" id: "<<bullet->id<<" coord: "<<X<<","<<Y<<" agent: "<<agent->team_id<<"_"<<agent->id<<" "<<agent->x<<","<<agent->y<<endl;
		}

		//Builds a structure in front
		void build(shared_ptr<struct Agent> agent, shared_ptr<struct Team> team)
		{
			//Check if has enough res
			if(!hasBuildingRes(team))
				return;
			//Find the space in front
			int X, Y;
			frontGridSpace(agent, X, Y);
			//Check if the front space is in grid
			if(!isInGrid(X, Y))
				return;
			if(grid[X][Y] == EMPTY && !isAgentThere(X, Y))
			{
				//Update the grid
				grid[X][Y] = STRUCT; 
				//substract the cost
				team->resource -= BUILD_RES;
				// Log
				team->totalBuilt++;
			}
		}

		//Harvests the food in front
		void harvest(shared_ptr<struct Agent> agent, shared_ptr<struct Team> team)
		{
			int X, Y;
			frontGridSpace(agent, X, Y);
			if(isInGrid(X, Y) && grid[X][Y] == FOOD)
			{
				team->resource += FOOD_REWARD;
				team->score += RES_SCORE;
				team->sGained += RES_SCORE;
				grid[X][Y] = EMPTY;
				// cout<<"Team: "<<team->id<<" Agent: "<<agent->id<<" Pos: "<<agent->x<<", "<<agent->y<<" Food: "<<X<<", "<<Y<<endl;
				team->totalHarvest++;
				team->hasFood = false;
			}
		}

		//Turns the agent 45 degrees to the left
		void turnL(shared_ptr<struct Agent> agent) //Todo:: The most stupid algorithm ever. Completely hard-coded. A dangerous game is being played here! Tripple check!
		{
			//ToDo:: Dumb algorithm
			/*
			how facing direction works:
			-1-1  +0-1  +1-1
	
			-1+0  AAAA  +1+0
			
			-1+1  +0+1  +1+1
			*/

			if(agent->facingX == -1 && agent->facingY == -1)
			{
				agent->facingX = -1;
				agent->facingY = 0;
			}else if(agent->facingX == -1 && agent->facingY == 0)
			{
				agent->facingX = -1;
				agent->facingY = +1;
			}else if(agent->facingX == -1 && agent->facingY == +1)
			{
				agent->facingX = 0;
				agent->facingY = 1;
			}else if(agent->facingX == 0 && agent->facingY == 1)
			{
				agent->facingX = 1;
				agent->facingY = 1;
			}else if(agent->facingX == 1 && agent->facingY == 1)
			{
				agent->facingX = 1;
				agent->facingY = 0;
			}else if(agent->facingX == 1 && agent->facingY == 0)
			{
				agent->facingX = 1;
				agent->facingY = -1;
			}else if(agent->facingX == 1 && agent->facingY == -1)
			{
				agent->facingX = 0;
				agent->facingY = -1;
			}else if(agent->facingX == 0 && agent->facingY == -1)
			{
				agent->facingX = -1;
				agent->facingY = -1;
			}
		}

		//Turns the agent 45 degrees to the Right
		void turnR(shared_ptr<struct Agent> agent)
		{
			//ToDo:: Dumb algorithm
			/*
			how facing direction works:
			-1-1  +0-1  +1-1
	
			-1+0  AAAA  +1+0
			
			-1+1  +0+1  +1+1
			*/

			if(agent->facingX == -1 && agent->facingY == -1)
			{
				agent->facingX = 0;
				agent->facingY = -1;
			}else if(agent->facingX == 0 && agent->facingY == -1)
			{
				agent->facingX = 1;
				agent->facingY = -1;
			}else if(agent->facingX == 1 && agent->facingY == -1)
			{
				agent->facingX = 1;
				agent->facingY = 0;
			}else if(agent->facingX == 1 && agent->facingY == 0)
			{
				agent->facingX = 1;
				agent->facingY = 1;
			}else if(agent->facingX == 1 && agent->facingY == 1)
			{
				agent->facingX = 0;
				agent->facingY = 1;
			}else if(agent->facingX == 0 && agent->facingY == 1)
			{
				agent->facingX = -1;
				agent->facingY = 1;
			}else if(agent->facingX == -1 && agent->facingY == 1)
			{
				agent->facingX = -1;
				agent->facingY = 0;
			}else if(agent->facingX == -1 && agent->facingY == 0)
			{
				agent->facingX = -1;
				agent->facingY = -1;
			}
		}

		//Moves the agent to the space in front if possible
		void move(shared_ptr<struct Agent> agent)
		{
			int X, Y; 
			frontGridSpace(agent, X, Y);
			if(isInGrid(X, Y) && grid[X][Y] == EMPTY && !isAgentThere(X, Y)) 
			{
				// cout<<"File: "<<Global::update<<"_"<<exp_number<<"\t"<<agent->x<<","<<agent->y<<" facing: "<<agent->facingX<<"|"<<agent->facingY<<" ==> "<<X<<","<<Y<<endl;
				grid[agent->x][agent->y] = EMPTY;
				grid[X][Y] = AGENT;
				agent->x = X;
				agent->y = Y;
			}
		}

		//Returns the index of the maximum value of the input array a
		int maxIndex(double a[], int length)
		{
			double max = a[0];
			int biggest = 0;
			for(int i=0; i<length; i++)
			{
				if(a[i]>max)
				{
					max = a[i];
					biggest = i;
				}
			}
			return biggest;
		}

		//Checks if the input x and y is in the grid
		bool isInGrid(int x, int y)
		{
			if(x >= 0 && y >= 0 && x < gridSize && y < gridSize)
				return true;
			return false;
		}

		//returns T if enough resource for shooting is available
		double hasShootingRes(shared_ptr<struct Team> team)
		{
			if(team->resource >= SHOOT_RES)
				return T;
			return F;
		}

		//returns T if enough resource for building is available. T can be used instead of true but it throws a warning. 
		double hasBuildingRes(shared_ptr<struct Team> team)
		{
			if(team->resource >= BUILD_RES)
				return T;
			return F;
		}

		//Returns X and Y of the grid space the agent is facing 
		void frontGridSpace(shared_ptr<struct Agent> agent, int &X, int &Y)
		{
			X = agent->x + agent->facingX;
			Y = agent->y + agent->facingY;
		}

		//Returns X and Y of the grid space the bullet is facing 
		void frontGridSpaceB(shared_ptr<struct Bullet> bullet, int &X, int &Y)
		{
			X = bullet->x + bullet->facingX;
			Y = bullet->y + bullet->facingY;
		}

		//Returns the vision array of an agent
		vector<double> getVision(shared_ptr<struct Agent> agent, int mode = 0)
		{
			int x = agent->x;
			int y = agent->y;
			vector<double> vision;
			if(mode == 0)
			{
				// V V V
				// V V V
				//   A	
				shared_ptr<struct Agent> fakeAgent { new struct Agent };
				fakeAgent->facingX = agent->facingX;
				fakeAgent->facingY = agent->facingY;
				fakeAgent->x = agent->x;
				fakeAgent->y = agent->y;
				int X, Y;
				frontGridSpace(fakeAgent, X, Y); 
				vision.push_back(isInGrid(X, Y)?(!isThereABullet(X, Y)?grid[X][Y]:BULLET):NA);
				fakeAgent->x = X;
				fakeAgent->y = Y;
				frontGridSpace(fakeAgent, X, Y); 
				vision.push_back(isInGrid(X, Y)?(!isThereABullet(X, Y)?grid[X][Y]:BULLET):NA);
				turnL(fakeAgent);
				frontGridSpace(fakeAgent, X, Y); 
				vision.push_back(isInGrid(X, Y)?(!isThereABullet(X, Y)?grid[X][Y]:BULLET):NA);
				turnR(fakeAgent);
				turnR(fakeAgent);
				frontGridSpace(fakeAgent, X, Y); 
				vision.push_back(isInGrid(X, Y)?(!isThereABullet(X, Y)?grid[X][Y]:BULLET):NA);
				fakeAgent->facingX = agent->facingX;
				fakeAgent->facingY = agent->facingY;
				fakeAgent->x = agent->x;
				fakeAgent->y = agent->y;
				turnL(fakeAgent);
				frontGridSpace(fakeAgent, X, Y); 
				vision.push_back(isInGrid(X, Y)?(!isThereABullet(X, Y)?grid[X][Y]:BULLET):NA);
				turnR(fakeAgent);
				turnR(fakeAgent);
				frontGridSpace(fakeAgent, X, Y); 
				vision.push_back(isInGrid(X, Y)?(!isThereABullet(X, Y)?grid[X][Y]:BULLET):NA);
			}else if(mode == 1)
			{
				// V V V
				// V A V
				// V V V
				//SenseMode!
				//It returns the grid value of the 8 spaces around it + bullets
				vision.push_back(isInGrid(x-1, y-1)?(!isThereABullet(x-1, y-1)?grid[x-1][y-1]:BULLET):NA);
				vision.push_back(isInGrid(x-1, y-0)?(!isThereABullet(x-1, y-0)?grid[x-1][y-0]:BULLET):NA);
				vision.push_back(isInGrid(x-1, y+1)?(!isThereABullet(x-1, y+1)?grid[x-1][y+1]:BULLET):NA);
				vision.push_back(isInGrid(x-0, y+1)?(!isThereABullet(x-0, y+1)?grid[x-0][y+1]:BULLET):NA);
				vision.push_back(isInGrid(x+1, y+1)?(!isThereABullet(x+1, y+1)?grid[x+1][y+1]:BULLET):NA);
				vision.push_back(isInGrid(x+1, y-0)?(!isThereABullet(x+1, y-0)?grid[x+1][y-0]:BULLET):NA);
				vision.push_back(isInGrid(x+1, y-1)?(!isThereABullet(x+1, y-1)?grid[x+1][y-1]:BULLET):NA);
				vision.push_back(isInGrid(x-0, y-1)?(!isThereABullet(x-0, y-1)?grid[x-0][y-1]:BULLET):NA);
			}
			
			
			return vision;
		}

		//Checks if there's a bullet in a given coord 
		bool isThereABullet(int x, int y)
		{
			for(auto bullet : bullets)
			{
				if(bullet->x == x && bullet->y == y)
				{
					return true;
				}
			}
			return false;
		}

		int freeSpaceCount(int minX, int maxX, int minY, int maxY)
		{
			int count = 0;
			for(int i = minX; i <= maxX; i++)
			{
				for(int j = minY; j <= maxY; j++)
				{
					if(grid[i][j] == EMPTY && !isAgentThere(i, j))
						count++;
				}
			}
			return count;
		}

		//Places a food chunk for team_id
		void placeFood(int team_id)
		{
			int minX = 0, maxX = 0;
			int minY = 0, maxY = 0;
			int i = team_id - 1;
			if(i == 0) //Team1 NorthWest Team Cliff
			{
				minX = 0;
				minY = 0;
				maxX = gridSize/4;
				maxY = gridSize/4;
			}else if(i == 1) //Team2 NorthEast Team Darren
			{
				minX = 0;
				minY = gridSize-gridSize/4;
				maxX = gridSize/4;
				maxY = gridSize;
			}else if(i == 2) //Team3 SouthWest Team eLe
			{
				minX = gridSize-gridSize/4;
				minY = 0;
				maxX = gridSize;
				maxY = gridSize/4;
			}else if(i == 3) //Team4 SouthEast Team Vinny
			{
				minX = gridSize-gridSize/4;
				minY = gridSize-gridSize/4;
				maxX = gridSize;
				maxY = gridSize;
			}

			//ToDo:: There are a lot better algorithms in order to do this
			vector<int> Xs;
			vector<int> Ys;
			
			
			//Find all the empty spaces
			for(int i = minX; i <= maxX; i++)
			{
				for(int j = minY; j <= maxY; j++)
				{
					if(grid[i][j] == EMPTY && !isAgentThere(i, j))
					{
						Xs.push_back(i);
						Ys.push_back(j);
					}
				}
			}

			if(Xs.size() == 0)
				return;

			//Choose a random empty coord
			int randomEmptyPos = Random::getInt(0, Xs.size()-1);
			//Double check if the space is empty
			if(grid[Xs[randomEmptyPos]][Ys[randomEmptyPos]] != EMPTY || isAgentThere(Xs[randomEmptyPos], Ys[randomEmptyPos]))
			{
				cout<<"For some reason a space which should have been EMPTY is not"<<endl;
			}
			//Place the food
			grid[Xs[randomEmptyPos]][Ys[randomEmptyPos]] = FOOD;
			findTeam(team_id)->hasFood = true;
			
		}

		//Initializes the world's cordoned off regions 
		void initGrid()
		{
			//Reset the whole grid to 0 (unavailable space)
			resetGrid();
			//Now we need to setup the regions for the four agent groups. I'm not gonna make everything parametrized cause it will make the world way too
			//complicated very quickly. Therefore, as a default, team regions are gonna be with the size of x/4 and y/4.
			for (int i = 0; i < gridSize/4; i++)
			{
				for (int j = 0; j < gridSize/4; j++)
				{
					grid[i][j] = EMPTY; //NorthWest
					grid[gridSize-i-1][j] = EMPTY; //SouthWest
					grid[i][gridSize-j-1] = EMPTY; //NorthEast
					grid[gridSize-i-1][gridSize-j-1] = EMPTY; //SouthWest 
				}
			}
			// visualizeGrid();
		}

		//Defines agents and gives random position to them and places the four food chunks!
		void initTeams()
		{
			for(int i = 0; i < 4; i++)
			{
				int minX = 0, maxX = 0;
				int minY = 0, maxY = 0;
				if(i == 0) //Team1 NorthWest Team Cliff
				{
					minX = 0;
					minY = 0;
					maxX = gridSize/4;
					maxY = gridSize/4;
				}else if(i == 1) //Team2 NorthEast Team Darren
				{
					minX = 0;
					minY = gridSize-gridSize/4;
					maxX = gridSize/4;
					maxY = gridSize;
				}else if(i == 2) //Team3 SouthWest Team eLe
				{
					minX = gridSize-gridSize/4;
					minY = 0;
					maxX = gridSize;
					maxY = gridSize/4;
				}else if(i == 3) //Team4 SouthEast Team Vinny
				{
					minX = gridSize-gridSize/4;
					minY = gridSize-gridSize/4;
					maxX = gridSize;
					maxY = gridSize;
				}

				//Place food for every team
				while(true)
				{
					int x = Random::getInt(minX, maxX-1);
					int y = Random::getInt(minY, maxY-1);
					
					if(grid[x][y] == EMPTY && !isAgentThere(x, y))
				 	{
				 		grid[x][y] = FOOD; 
				 		break;
				 	}
				}

				shared_ptr<struct Team> team { new struct Team };
				for(int j = 0; j < 3; j++)
				{
					 shared_ptr<struct Agent> a { new struct Agent };
					 a->id = j+1;
					 a->task = j+1; //give the agents an initial task
					 if(j == 0)
					 	a->task = HARVESTER;
					 else if(j == 1)
					 	a->task = BUILDER;
					 else if(j == 2)
					 	a->task = ATTACKER;
					 while(true) //Iterates untill it finds an empty space for the agent. 
					 {
					 	int x = Random::getInt(minX, maxX-1);
					 	int y = Random::getInt(minY, maxY-1);
					 	if(grid[x][y] == EMPTY && !isAgentThere(x, y))
					 	{
					 		a->x = x;
					 		a->y = y;
					 		int temp_facingX = 0;
					 		int temp_facingY = 0;
					 		while(temp_facingY == 0 && temp_facingX == 0)
					 		{
					 			temp_facingX = Random::getInt(-1, 1);
					 			temp_facingY = Random::getInt(-1, 1);
					 		}
					 		a->facingX = temp_facingX;
					 		a->facingY = temp_facingY;
					 		a->team_id = i+1;
					 		grid[x][y] = AGENT;
					 		break;
					 	}
					 }
					 team->agents.push_back(a);
				}
				//add the agents to the teams
				team->id = i+1;
				team->hasFood = true;
				teams.push_back(team);
			}
		}
};

CoopWorld::CoopWorld(shared_ptr<ParametersTable> _PT) :
		AbstractWorld(_PT) {

	// columns to be added to ave file
	popFileColumns.clear();
	popFileColumns.push_back("score");
	popFileColumns.push_back("score_VAR"); // specifies to also record the variance (performed automatically because _VAR)
	popFileColumns.push_back("totalHarvest");
	popFileColumns.push_back("totalBuilt");
	popFileColumns.push_back("totalShots");
	popFileColumns.push_back("totalSwitch");
	popFileColumns.push_back("accuracy");
	popFileColumns.push_back("totalFF");
	popFileColumns.push_back("totalStructGotHit");
	popFileColumns.push_back("totalStructFF");
	popFileColumns.push_back("totalStructHit");
	popFileColumns.push_back("totalSuccessfulShots");
	popFileColumns.push_back("totalGotHit");
	popFileColumns.push_back("sGained");
	popFileColumns.push_back("sLost");
}

void CoopWorld::evaluate(map<string, shared_ptr<Group>>& groups, int analyze, int visualize, int debug) {
	// cout<<endl<<endl<<endl<<"evaluation starts: "<<endl<<endl<<endl;
	auto population = groups[groupNamePL->get(PT)]->population;
	if (population.size()%4 != 0)
	{
		cout << "Population size for the Coop world should be dividable by 4. Exiting!" << endl;
 	    exit(1);
	}
	int exp_number = 0;
	for (int i = 0; i < population.size(); i=i+4)
	{
		population[i]->brains[brainNamePL->get(PT)]->resetBrain();
		// cout<<"starts giving pop thingies\n";
		//Each four organisms compete against eachother. I made four variables instead of a vector of vectors for easier understanding of the code
		//Otherwise it would be too complicated to read/debug
		vector<shared_ptr<Organism>> p1;
		vector<shared_ptr<Organism>> p2;
		vector<shared_ptr<Organism>> p3;
		vector<shared_ptr<Organism>> p4;


		p1.push_back(population[i]);
		p2.push_back(population[i+1]);
		p3.push_back(population[i+2]);
		p4.push_back(population[i+3]);

		//This way it's easy to give the agents three different brains
		for (int i = 0; i < 2; i++)
		{
			p1.push_back(population[i]->makeCopy());
			p2.push_back(population[i+1]->makeCopy());
			p3.push_back(population[i+2]->makeCopy());
			p4.push_back(population[i+3]->makeCopy());
		}

		exp_number++;
		Game g = Game(p1, p2, p3, p4, brainNamePL, PT, exp_number);	
	}
}


