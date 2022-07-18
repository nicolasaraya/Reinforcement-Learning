#ifndef QLEARNING_H
#define QLEARNING_H

#include <iostream>
#include <fstream>
#include "Env.h"
#include "Agent.h"
#include "QValues.h"

using namespace std; 

vector<double> Qlearning(Environment* , Agent* , double , double , double , double , int , int, int,  ofstream* );
void initQTable(QValues* , Environment*);
void printQTable(QValues*, Environment* );
void update_q_prev_state(Environment*, Agent*, QValues*, int, double, double);
int action_selection(Agent*, int, double, QValues*);

vector<double> Qlearning(Environment* env, Agent* agent, double learn_rate, double disc_factor, double exp_rate, double exp_discount, int num_episodes, int max_steps, int action_sel, ofstream* reward_output){
    QValues Q_values;
    initQTable(&Q_values, env);
    //printQTable(&Q_values, env);
    int action;
    double cum_reward;
    vector<double> finalrw;
    for(int i = 0; i < num_episodes; i++){
        env->reset();
        cum_reward = 0;
        int count_steps  = 0;
        while( !env->isDone() ){
            action = action_selection(agent, action_sel, exp_rate, &Q_values);
            env->step(action);
            update_q_prev_state(env, agent, &Q_values, action, disc_factor, learn_rate);
            cum_reward += env->getReward(agent->getPos().first, agent->getPos().second);
            if(count_steps++ > max_steps) break; 
            
        }
        finalrw.push_back(cum_reward);
        string s = "Episode " + to_string(i) + " Total reward obtained: " + to_string(finalrw.back())  + " Epsilon: " + to_string(exp_rate) + "\n";
        if(exp_rate > 0.01) exp_rate -= exp_discount;
        *reward_output << s;
        //cout << s;
    }    
    return finalrw; 
}

void initQTable(QValues* Q_values, Environment* env){
    int width_grid = env->getSizeGrid().first;
    int heigth_grid = env->getSizeGrid().second;
    for(int i = 0; i < width_grid ; i++){
        for(int j = 0; j < heigth_grid; j++){
            for(int k = 0; k < 4; k++){
                Q_values->values[i][j][k]=rand()%10;
                cout << "Initial Q value of cell [" <<i << ", " <<j << "] action " << k << " = " << Q_values->values[i][j][k] << "\n";
            }
        }
    }
    
    
}

void printQTable(QValues* Q_values, Environment* env){
    int width_grid = env->getSizeGrid().first;
    int height_grid = env->getSizeGrid().second;
    for(int i = 0; i < width_grid; i++){
        for(int j = 0; j < height_grid; j++){
            cout << "||" << Q_values->values[i][j][0] << " " << Q_values->values[i][j][1] << " " <<  Q_values->values[i][j][2] << " " << Q_values->values[i][j][3] << "||   ";
        }
        cout <<endl;
    }
}

void update_q_prev_state(Environment* env, Agent* agent, QValues* q_values, int action_taken, double disc_factor, double learn_rate){ //Updates the Q value of the previous state
    int x_pos = agent->getPos().first;
    int y_pos = agent->getPos().second;
    int goalx = env->getGoal().first;
    int goaly = env->getGoal().second;
    int environment = env->typeEnv(); 
    int prev_x_pos = agent->getPrevPos().first;
    int prev_y_pos = agent->getPrevPos().second;
    double reward = env->getReward(agent->getPos().first, agent->getPos().second);



    int best_action = 0;
    int valueBestAction = q_values->values[x_pos][y_pos][0];

    for(int i = 1; i < 4; i++){
        if(q_values->values[x_pos][y_pos][i] > valueBestAction){
            valueBestAction = q_values->values[x_pos][y_pos][i];
            best_action = i; 
        }
    }

    //Determine the max_a(Qvalue[x_pos][y_pos])
    //Update the Q value of the previous state and action if the agent has not reached a terminal state
    if(!( ((x_pos==goalx)&&(y_pos==goaly)) ||((environment==1)&&(x_pos==goalx)&&(y_pos==(goaly-1)))||((environment==2)&&(x_pos>0)&&(x_pos<goalx)&&(y_pos==0))) ){
        //Qvalues[prev_x_pos][prev_y_pos][action_taken] +=1;      
        q_values->values[prev_x_pos][prev_y_pos][action_taken]= q_values->values[prev_x_pos][prev_y_pos][action_taken] + (learn_rate * (reward + (disc_factor * q_values->values[x_pos][y_pos][action_taken]) - q_values->values[prev_x_pos][prev_y_pos][action_taken])); //How should the Q values be updated?
    }
    else {//Update the Q value of the previous state and action if the agent has reached a terminal state
        q_values->values[prev_x_pos][prev_y_pos][action_taken]=  q_values->values[prev_x_pos][prev_y_pos][action_taken] ;
    }
}


int action_selection(Agent* agent, int action_sel, double exp_rate, QValues* q_values){
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