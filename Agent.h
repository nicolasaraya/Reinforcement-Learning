#ifndef AGENT_H
#define AGENT_H
#include <iostream>

using namespace std;

class Agent{
    public:
        Agent();
        ~Agent();   
        pair<int,int> getPos();
        pair<int, int> getPrevPos();
        void setPos(int, int);
    
    private:
        int x_pos;
        int y_pos;
        int x_prev;
        int y_prev; 
        int reward; 
};

#endif