#ifndef ENV_H
#define ENV_H
#include <iostream>
#include <vector>
#include "Agent.h"

using namespace std;

class Environment{
    public:
        //var

        //methods
        Environment(int, int, Agent*);
        ~Environment();
        void step(int); 
        void printQTable();
        void Multi_print_grid();
        void reset();
        void epsilonUpdate();
        int typeEnv(); 
        bool isDone(); 
        pair<int, int> getSizeGrid();
        pair<int, int> getGoal();
        double getReward(int, int);
        double getEpsilon();
    private: 
        //var
        int environment;
        int stochastic_actions;
        int height_grid;
        int width_grid; 
        int goalx;
        int goaly;
        int init_x_pos;
        int init_y_pos;
        int action_sel;
        int blocked[100][100];
        Agent* agent;
        double reward[100][100];
        double stochastic_prob = 0.2; 
        //methods

};






#endif 