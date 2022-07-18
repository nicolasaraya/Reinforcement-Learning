#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include "Env.h"
#include "Agent.h"
#include "Qlearning.h"
#include "Sarsa.h"
#include "pbPlots.hpp"
#include "supportLib.hpp"

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

int make_plot(vector<double> rewards);

int main(int argc, char* argv[]){
    srand(time(NULL));

    int current_episode; 
    int action_taken;
    int cum_reward; 
    vector<double> rewards;
    
    ofstream reward_output;

    reward_output.open("Rewards.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    reward_output.clear();

    Agent* agent = new Agent();
    Environment* env = new Environment(environment, stochastic_actions, agent);

    if(algorithm == 1){
        rewards = Qlearning(env, 
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
        rewards = Sarsa(env, 
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


    if(make_plot(rewards)) cout << "Plot error" << endl;
    env->Multi_print_grid();
    reward_output.close();
    //delete(agent);
    //delete(env);
    return 0;
}


int make_plot(vector<double> rewards){
    bool success;
    StringReference *errorMessage = new StringReference();
	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

    string path = "./plots/";
    vector<double> num_episodes;
    for(int i = 0; i < rewards.size(); i++) num_episodes.push_back(i);
    

    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = &num_episodes;
	series->ys = &rewards;
	series->linearInterpolation = false;
	series->pointType = toVector(L"dots");
    series->lineType = toVector(L"solid");
	series->color = CreateRGBColor(0, 0, 1);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 1000;
	settings->height = 600;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
    
    time_t rawtime; 
    struct tm* info; 
    time(&rawtime);
    info = localtime(&rawtime);
    string pathaux = "_" + to_string(info->tm_hour) + "_" + to_string(info->tm_min) + "_" + to_string(info->tm_sec) + "__"  + to_string(info->tm_year+1900) +  "-" + to_string(info->tm_mon) + "-" + to_string(info->tm_mday)  ;
     
    if(algorithm == 1 && environment == 1){
        settings->title = toVector(L"Q-Learning Train in Small World");
        path += "QLearningSW" + pathaux + ".png";
    }
    else if(algorithm == 1 && environment == 2){
        settings->title = toVector(L"Q-Learning Train in CliffWalking");
        path += "QLearningCW" + pathaux + ".png";
    }
    else if(algorithm == 2 && environment == 1){
        settings->title = toVector(L"Sarsa Train in Small World");
        path += "SarsaSW" + pathaux + ".png";
    }
    else if(algorithm == 2 && environment == 2){
        settings->title = toVector(L"Sarsa Train in CliffWalking");
        path += "SarsaCW" + pathaux + ".png";
    }
    
	settings->xLabel = toVector(L"Episodes");
	settings->yLabel = toVector(L"Rewards");
	settings->scatterPlotSeries->push_back(series);

	success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

    if(success){
        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, path);
        DeleteImage(imageReference->image);
	}else{
	    cerr << "Error: ";
        for(wchar_t c : *errorMessage->string){
            wcerr << c;
        }
        cerr << endl;
	}

	return success ? 0 : 1;

}

