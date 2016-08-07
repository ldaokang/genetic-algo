//PROLOGUE: This is my second GA - where the bot must find its way out of the maze!
//DECLARATIONS
#include <iostream>
#include <string>
#include <cstdlib> //for rand() and srand()
#include <ctime> //for time()
#include <math.h> //for pow()
#include <sstream> //for stringstream

#define N_POP           100
#define XOVER_RATE      0.8 //WHY IS THIS EVEN NEEDED?
#define CHROMO_LENGTH   243 //this is the total number of cases of trinary system of length 5 (3^5)
#define MAX_GEN         30
#define THRES_FIT       0.9    //this is the threshold of the total fitness that the chromosome must overcome to survive
//DEVNOTE: this should be in percentile, not absolute: BECAUSE, if SD is low of the population (i.e., all have mean fitness, then all will die)
//THRES_FIT of 1 will mean as long as chromosome is fitter than mean, it will survive
#define X_SPLICE_LENGTH 1   //minimum is 1 for crossover to happen
#define N_XOVER         40
#define GENE_BASE       4  //base 2 contains 1 and 0, base 10 is 0123456789
#define MUTATION_RATE   0.1 //one out of every 10 genes should mutate
int obs_density = 10; //from 0 to 100
#define PROB_CHOSEN     0.4
#define EXP_N_SURVIVE   10 //number of chromosomes expected to survive each round

int n_cols = 4;
int n_rows = 4;
int STEP_TOLERANCE = 1;

using namespace std;
//END OF DECLARATIONS

struct Chromosome{
    stringstream genes;
    double fitness;
    bool survival;
    int fitness_rank;
    double pselection;
};

struct Space{   //this is to create the grid
    stringstream col;
    stringstream row;
    string property;
    int identity;
};

// END OF PROTOTYPES
void Mutate_Active_Gene(int env);

int EnvSensed();
void MoveBot(int env);

double total_fitness;
double threshold_fitness;
bool Solution_Found = false;
bool Exit_Reached = false;
bool Bot_Stuck = false;
int bot_position;
int prev_position;
double step_count;
int active_chromo; //chromosome being tested at the moment
int n_survivors;
int order = 0;
int space_code;
string bot_orientation; //NSEW
Chromosome Alpha[N_POP]; //the working population of chromosomes
Chromosome OldGen[N_POP]; //however, the number of OldGen selected will be <N_POP
Space Dojo[9];
Space Sanctuary[16];
Space Austria[25];
Space Brazil[36];

//HEADERS
#include "geneticA.hpp"
#include "mazefunction.hpp"

void Train (Chromosome* Zulu, int s_code, bool StepByStep);

int main(){ //includes output text, crossing and mutation
    stringstream progress;
    srand(time(0));
    //system("./intro.sh");
    cout << "Population Size: " << N_POP << endl << "Chromosome Length:" << CHROMO_LENGTH << endl;

    for (int i=0;i<N_POP;i++){ //Initialize Random Chromosomes
        Alpha[i].genes.str(GetRandGenes(CHROMO_LENGTH));
    }

    while (space_code!=999 && STEP_TOLERANCE != 999){
    cout << "SPACE CODES:" << endl;
    cout << "10: Dojo [3x3] (no obs)" << endl << "11: Sanctuary [4x4] (no obs)" << endl << "12: Brazil [6x6] (no obs)" << endl;
    cin >> space_code;
    cout << "STEP_TOLERANCE:";
    cin >> STEP_TOLERANCE;
    Train (Alpha,space_code,true);
    }
    /*--- DOJO --- */
    /*for (int g=0; g< MAX_GEN; g++){
        cout << "GENERATION " << g << endl;
        n_survivors = 0;
        total_fitness = 0;

        for (active_chromo=0;active_chromo<N_POP;active_chromo++){
        cout << "CHROMOSOME " << active_chromo << endl;
            Space* PlaceHolder = ChooseSpace(space_code);
            RunThrough(Alpha,PlaceHolder,EnvSensed(PlaceHolder),true,true);
        }
        //progress << n_survivors << ",";
        //AssignFitRank(Alpha);
        //n_survivors = FitRankSelection(Alpha,OldGen);

        cout << "Number of Survivors: " << n_survivors << endl;
        cout << "preparing to Cross..." << endl;
        for (int i = 0; i < N_POP; i++){ Alpha[i].genes.str(Cross(n_survivors,Alpha)); }
        cout << "Preparing to Mutate... " << endl;
        for (int i=0; i<N_POP; i++){ Alpha[i].genes.str(Rand_Mutation(Alpha,i)); }
    cin.get();
    }
    */







    cin.get();

return 0;}



void Train(Chromosome* Zulu, int s_code, bool StepByStep){
        for (int g=0; g< MAX_GEN; g++){
        cout << "GENERATION " << g << endl;
        n_survivors = 0;
        total_fitness = 0;

        for (active_chromo=0;active_chromo<N_POP;active_chromo++){
        cout << "CHROMOSOME " << active_chromo << endl;
            Space* PlaceHolder = ChooseSpace(s_code);
            RunThrough(Alpha,PlaceHolder,EnvSensed(PlaceHolder),true, StepByStep);
        }
        //progress << n_survivors << ",";
        //AssignFitRank(Alpha);
        //n_survivors = FitRankSelection(Alpha,OldGen);

        cout << "Number of Survivors: " << n_survivors << endl;
        cout << "preparing to Cross..." << endl;
        for (int i = 0; i < N_POP; i++){ Zulu[i].genes.str(Cross(n_survivors,Zulu)); }
        cout << "Preparing to Mutate... " << endl;
        for (int i=0; i<N_POP; i++){ Zulu[i].genes.str(Rand_Mutation(Zulu,i)); }
        cin.get();
        }
}

void PrintSurvivalStats(){
    cout << "==SURVIVAL STATS ==" << endl;
        for (int i=0; i<N_POP;i++){
        if (Alpha[i].survival){
            //cout << Alpha[i].fitness_rank <<": No." << i << "|F("<<i<<")="<< Alpha[i].fitness << endl;
            n_survivors++;
            if (Alpha[i].fitness_rank<5){
            cout << "Testing algorithm of Rank:" << Alpha[i].fitness_rank << endl;
            active_chromo = i;

            cin.get();}
        }
        }

}







