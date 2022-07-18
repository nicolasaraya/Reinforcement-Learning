#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include "Env.h"
#include "Agent.h"
#include "Qlearning.h"
#include "Sarsa.h"

using namespace std;

//////////////
//Setting value for learning parameters
int action_sel = 2; // 1 is greedy, 2 is e-greedy
int environment = 2; // 1 is small grid, 2 is Cliff walking
int algorithm = 2; //1 is Q-learning, 2 is Sarsa
int stochastic_actions = 0; // 0 is deterministic actions, 1 for stochastic actions
int num_episodes = 10000; //total learning episodes
int max_steps = 200; 
double learn_rate = 0.1; // how much the agent weights each new sample
double disc_factor = 0.99; // how much the agent weights future rewards
double exp_rate = 0.3; // how much the agent explores
double exp_discount = 0.0001;
///////////////


int main(int argc, char* argv[]){
    srand(time(NULL));

    int current_episode; 
    int action_taken;
    int cum_reward;
    
    ofstream reward_output;

    reward_output.open("Rewards.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    reward_output.clear();

    Agent* agent = new Agent();
    Environment* env = new Environment(environment, stochastic_actions, agent);

    if(algorithm == 1){
        Qlearning(env, 
            agent, 
            learn_rate,
            disc_factor, 
            exp_rate, 
            exp_discount,
            num_episodes, 
            max_steps, 
            action_sel, 
            &reward_output);
    }
    if(algorithm == 2){
        Sarsa(env, 
            agent, 
            learn_rate,
            disc_factor, 
            exp_rate, 
            exp_discount,
            num_episodes, 
            max_steps, 
            action_sel, 
            &reward_output);
    }


    env->Multi_print_grid();
    reward_output.close();
    //delete(agent);
    //delete(env);
    return 0;
}

