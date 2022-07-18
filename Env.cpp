#include "env.h"
using namespace std;  


Environment::Environment(int environment, int stochastic_actions,  Agent* agent){
    this->environment = environment;
    this->stochastic_actions = stochastic_actions;
    this->agent = agent;

    if(environment==1){
        height_grid= 3;
        width_grid=4;
        goalx=3;
        goaly=2;
        init_x_pos=0;
        init_y_pos=0;
    }
    
    if(environment == 2){
        height_grid = 4;
        width_grid = 12;
        goalx = 11;
        goaly = 0;
        init_x_pos = 0;
        init_y_pos = 0;
    }
    
    for(int i = 0; i < width_grid; i++){
        for(int j = 0; j < height_grid; j++){ 
            if(environment==1){
                reward[i][j]=-0.04; //-1 if environment 2
                blocked[i][j]=0;  
            }
            if(environment==2){
                reward[i][j]=-1;
                blocked[i][j]=0;
            }
        }
    }

    if(environment==1){
        reward[goalx][goaly]=100;
        reward[goalx][(goaly-1)]=-100;
        blocked[1][1]=1;
    }
    
    if(environment==2){
        reward[goalx][goaly]=1;
        for(int h=1; h<goalx;h++){   
            reward[h][0]=-100; 
        }  
    }

    agent->setPos(init_x_pos, init_y_pos);   
}

Environment::~Environment(){

}

void Environment::step(int action){
    int x_pos = agent->getPos().first;
    int y_pos = agent->getPos().second;
    
    double rand_number = static_cast<float>(rand()) * static_cast<float>(1) / RAND_MAX;
    
    if(stochastic_actions && rand_number < stochastic_prob){ //stochastic prob == 0.2
        if(rand_number < stochastic_prob/2){
            action += 1; 
            if(action>3) action = 0; 
        }
        else{
            action-=1;
            if(action < 1) action = 3;
        }
    }
    
    if(action==0) { //up
        if((y_pos<(height_grid-1))&&(blocked[x_pos][y_pos+1]==0)){//If there is no wall or obstacle Up from the agent
            y_pos=y_pos+1;  //move up
        }
    }
    
    if(action==1) { //Right
        if((x_pos<(width_grid-1))&&(blocked[x_pos+1][y_pos]==0)){ //If there is no wall or obstacle Right from the agent
            x_pos=x_pos+1; //Move right
        }
    }
    if(action==2){  //Down
        if((y_pos>0)&&(blocked[x_pos][y_pos-1]==0)){ //If there is no wall or obstacle Down from the agent
            y_pos=y_pos-1; // Move Down
        }   
    }
    
    if(action==3) { //Left
        if((x_pos>0)&&(blocked[x_pos-1][y_pos]==0)){ //If there is no wall or obstacle Left from the agent
            x_pos=x_pos-1;//Move Left
        }  
    }

    agent->setPos(x_pos, y_pos);
}


void Environment::Multi_print_grid(){
    int x_pos = agent->getPos().first;
    int y_pos = agent->getPos().second;

    for(int y = (height_grid-1); y >=0 ; --y){
        for (int x = 0; x < width_grid; ++x){
            if(blocked[x][y]==1) {
                cout << " \033[42m# \033[0m";
            }else{
                if ((x_pos==x)&&(y_pos==y)){
                    cout << " \033[44m1 \033[0m";
                }else{
                    cout << " \033[31m0 \033[0m";   
                }
            }
        }
        printf("\n");
    }
}

void Environment::reset(){
    agent->setPos(init_x_pos, init_y_pos);
}


bool Environment::isDone(){
    int x_pos = agent->getPos().first;
    int y_pos = agent->getPos().second;

    bool meta = (x_pos == goalx) && (y_pos == goaly);
    bool meta2 = ((environment==1)&&(x_pos==goalx)&&(y_pos==(goaly-1))); 
    bool meta3 = ((environment==2)&&(x_pos>0)&&(x_pos<goalx)&&(y_pos==0));

    return meta || meta2 || meta3; 
}

pair<int, int> Environment::getSizeGrid(){
    return make_pair(width_grid, height_grid);
}

pair<int, int> Environment::getGoal(){
    return make_pair(goalx, goaly);
}

double Environment::getReward(int x_pos, int y_pos){
    return reward[x_pos][y_pos];
}

int Environment::typeEnv(){
    return environment;
}
