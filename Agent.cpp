#include "Agent.h"


Agent::Agent(){
    x_pos = 0; 
    y_pos = 0;
}

Agent::~Agent(){
    
}

void Agent::setPos(int x, int y){
    x_prev = x_pos;
    y_prev = y_pos;
    x_pos = x;
    y_pos = y; 
}

pair<int,int> Agent::getPos(){
    return make_pair(x_pos, y_pos);
}

pair<int,int> Agent::getPrevPos(){
    return make_pair(x_prev, y_prev);
}