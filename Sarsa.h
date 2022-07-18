#ifndef SARSA_H
#define SARSA_H
#include <iostream>
#include <fstream>
#include "Env.h"
#include "QValues.h"
#include "pbPlots.hpp"
#include "supportLib.hpp"


using namespace std; 

vector<double> Sarsa(Environment* , Agent* , double , double , double, double, int , int , int , ofstream* );
void initQTable(QValues* , Environment*);
void printQTable(QValues*, Environment* );
void update_q_prev_state_sarsa(Environment*, Agent*, QValues*, int, int, double, double);
int action_selection_sarsa(Agent*, int, double, QValues*);

void update_q_prev_state_sarsa(Environment* env, Agent* agent, QValues* Q_values, int action_taken, int action_taken2, double disc_factor, double learn_rate){
    int x_pos = agent->getPos().first;
    int y_pos = agent->getPos().second;
    int prev_x_pos = agent->getPrevPos().first;
    int prev_y_pos = agent->getPrevPos().second;
    int goalx = env->getGoal().first;
    int goaly = env->getGoal().second;
    int environment = env->typeEnv(); 
    double reward = env->getReward(agent->getPos().first, agent->getPos().second);
    
    //Update the Q value of the previous state and action if the agent has not reached a terminal state
    if(!( ((x_pos==goalx)&&(y_pos==goaly)) ||((environment==1)&&(x_pos==goalx)&&(y_pos==(goaly-1)))||((environment==2)&&(x_pos>0)&&(x_pos<goalx)&&(y_pos==0))    ) ){
       Q_values->values[prev_x_pos][prev_y_pos][action_taken] = Q_values->values[prev_x_pos][prev_y_pos][action_taken] + (learn_rate * (reward + (disc_factor * Q_values->values[x_pos][y_pos][action_taken2] )  - Q_values->values[prev_x_pos][prev_y_pos][action_taken] ));
        
    }
    else{//Update the Q value of the previous state and action if the agent has reached a terminal state
       //Q_values->values[prev_x_pos][prev_y_pos][action_taken]= Q_values->values[prev_x_pos][prev_y_pos][action_taken];
       Q_values->values[prev_x_pos][prev_y_pos][action_taken] = Q_values->values[prev_x_pos][prev_y_pos][action_taken] + (learn_rate * (reward + (disc_factor * Q_values->values[x_pos][y_pos][action_taken2] )  - Q_values->values[prev_x_pos][prev_y_pos][action_taken] ));
    } 
}


vector<double> Sarsa(Environment* env, Agent* agent, double learn_rate, double disc_factor, double exp_rate, double exp_discount, int num_episodes , int max_steps , int action_sel , ofstream* reward_output){
    QValues Q_values;
    initQTable(&Q_values, env);
    //printQTable(&Q_values, env);
    int action, action2;
    double cum_reward;
    vector<double> finalrw;
    for(int i = 0; i < num_episodes; i++){
        env->reset();
        cum_reward = 0;
        int count_steps  = 0;
        action = action_selection_sarsa(agent, action_sel, exp_rate, &Q_values);
        
        while( !env->isDone() ){
            env->step(action);
            action2 = action_selection_sarsa(agent, action_sel, exp_rate, &Q_values);
            update_q_prev_state_sarsa(env, agent, &Q_values, action, action2, disc_factor, learn_rate);
            cum_reward += env->getReward(agent->getPos().first, agent->getPos().second);
            if(count_steps++ > max_steps) break; 
            action = action2;
        }
        finalrw.push_back(cum_reward);
        string s = "Episode " + to_string(i) + " Total reward obtained: " + to_string(finalrw.back())  + " Epsilon: " + to_string(exp_rate) + "\n";
        if(exp_rate > 0.1) exp_rate -= exp_discount;
        *reward_output << s;
        //cout << s;
    }   //Add the reward obtained by the agent to the cummulative reward of the agent in the current episode

    return finalrw;
}

int action_selection_sarsa(Agent* agent, int action_sel, double exp_rate, QValues* q_values){
    double rand_number = static_cast<float>(rand()) * static_cast<float>(1) / RAND_MAX;
    int x_pos = agent->getPos().first;
    int y_pos = agent->getPos().second;
    int best_action = 0;
    int valueBestAction = q_values->values[x_pos][y_pos][0];

    for(int i = 1; i < 4; i++){
        if(q_values->values[x_pos][y_pos][i] > valueBestAction){
            valueBestAction = q_values->values[x_pos][y_pos][i];
            best_action = i; 
        }
    }

    if(action_sel==1) return best_action;//Greedy, always selects the action with the largest Q value
       
    if(action_sel==2){ //epsilon-greedy, selects the action with the largest Q value with prob (1-exp_rate) and a random action with prob (exp_rate)
        if(rand_number < exp_rate) return rand()%4; 
        else return best_action;    
    }
    return 0;
}


#endif