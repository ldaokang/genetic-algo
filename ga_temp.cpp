//PROLOGUE: This is a general template for a genetic algorithm
//DECLARATIONS
#include <iostream>
#include <string>
#include <cstdlib> //for rand() and srand()
#include <ctime> //for time()
#include <math.h> //for pow()
#include <sstream> //for stringstream

//DEFINITIONS
//I: GENERAL - INITIATION
#define N_POP            100
#define CHROMO_LENGTH   243 //this is the total number of cases of trinary system of length 5 (3^5)
#define MAX_GEN         20

//I: GENERAL - CHANGES
#define X_SPLICE_LENGTH 1   //1: SPLICES ONE GENE AT A TIME
#define N_XOVER         120 //number of genes that crossover
#define GENE_BASE       4  //depending on number of actions the automata is required to carry out (max: 9)
#define MUTATION_RATE   0.3 //3 out of every 10 genes should mutate
#define PROB_CHOSEN     0.4 //probability of the fittest chromosome to be chosen (FitRankSelection)
#define EXP_N_SURVIVE   10  //expected number of chromosomes to select from a fitness rank method
//#define XOVER_RATE      0.7
//#define THRES_FIT       0.9    //this is the threshold of the total fitness that the chromosome must overcome to survive
//DEVNOTE: this should be in percentile, not absolute: BECAUSE, if SD is low of the population (i.e., all have mean fitness, then all will die)
//THRES_FIT of 1 will mean as long as chromosome is fitter than mean, it will survive


//II: SPECIFIC TO GA
#define N_COLS          7
#define N_ROWS          7
#define STEP_TOLERANCE  12
#define OBS_DENSITY     10 //OBSTACLE DENSITY: From 0 to 100

using namespace std;

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
    string property; //e > exit, b > bot, # > wall, - > empty space
    int identity; //
};

//FUNCTIONS
//I: GENETICS
string GetRandGenes(int clength);
string Cross(int n, Chromosome* Zulu);
void Rand_Mutation();
void Mutate_Active_Gene(int env);
void FitRankSelection(); //FITNESS RANK SELECTION METHOD

//II: SPECIFIC - GRID INITIATION
void PopulateGrid();
void PrintGrid();
void SearchGrid();

//III: SPECIFIC - SENSING AND MOVEMENT
int EnvSensed();
void MoveBot(int env);

//VARIABLES
//I: GENERAL
double total_fitness;
double threshold_fitness;
bool Solution_Found = false;
int active_chromo; //chromosome being tested at the moment
int nsurvive; //number of chromosomes survived at the moment

//II: SPECIFIC
bool Exit_Reached = false;
bool Bot_Stuck = false;
int bot_position;
int prev_position;
double step_count;

string bot_orientation; //NSEW
Chromosome Alpha[N_POP]; //the working population of chromosomes
Chromosome OldGen[N_POP]; //however, the number of OldGen selected will be <N_POP
Space Grid[N_ROWS*N_COLS];
// END OF FUNCTIONS (PRE-CALLED)

int main(){ //includes output text, crossing and mutation
    stringstream progress; //measures the overall progress of the GA
    srand(time(0));
    system("./intro.sh"); //calls for intro to be played
    cout << "Population Size: " << N_POP << endl;
    cout << "Chromosome Length:" << CHROMO_LENGTH << endl;

    for (int i=0;i<N_POP;i++){ //this is to initialize the population
        //Alpha[i].genes.str() = "0";
        Alpha[i].genes.str(GetRandGenes(CHROMO_LENGTH));
        //cout << Alpha[i].genes.str() << endl;
        //Alpha[i].fitness = GetFitness(Alpha[i].genes.str());
    }

    cin.get();
    cout << "Press Enter to Start" << endl;

    for (int g=0; g< MAX_GEN; g++){
        nsurvive = 0;
        total_fitness = 0;

        for (active_chromo=0; active_chromo<N_POP; active_chromo++){
            PopulateGrid();
            //cout << "==CHROMOSOME " << active_chromo << " ==" << endl;
            step_count = 0;
            prev_position = 0;
            Bot_Stuck = false;
            Alpha[active_chromo].survival = false;
            while(!Exit_Reached && (step_count < STEP_TOLERANCE) && !Bot_Stuck){
                MoveBot(EnvSensed());
                //PrintGrid();
                step_count++;
                }

            if(Exit_Reached){
                //cout<< "Exit Reached in " << step_count << "steps!" << endl;
                Alpha[active_chromo].fitness = step_count;
                Alpha[active_chromo].survival = true;
                Alpha[active_chromo].fitness_rank = nsurvive;
                nsurvive++;
                //cout<< "Fitness: " << Alpha[active_chromo].fitness << endl;
                total_fitness += step_count;
                //cout << "Total Fitness: " << total_fitness << endl;

                Exit_Reached = false;
            }
            //cin.get();

        }
        /*threshold_fitness = total_fitness*THRES_FIT/
        cout << "After Running Maze, " <<  << " chromosomes are out" << endl;
        for(int i=0; i<N_POP;i++){
            if(Alpha[i].fitness >= threshold_fitness){
                OldGen[nsurvive].genes.str(Alpha[i].genes.str());
                OldGen[nsurvive].fitness = Alpha[i].fitness;
                nsurvive++;
                }
            }*/
        //cout << "A total of      "<< nsurvive << " made it out of the maze" << endl;
        /*cout << "==SURVIVAL STATS ==" << endl;
        for (int i=0; i<N_POP;i++){
        if (Alpha[i].survival){
            cout << Alpha[i].fitness_rank <<": No." << i << "|F("<<i<<")="<< Alpha[i].fitness << endl;
        }
        }*/
        progress << nsurvive << ",";
        FitRankSelection();
        //progress << nsurvive << ".";
        /*cout << "After Selection "<< nsurvive << " chromosomes remain" << endl;
        for (int i=0; i<nsurvive;i++){
            cout << OldGen[i].fitness_rank <<": No." << i << "|F("<<i<<")="<< OldGen[i].fitness << endl;
        }
        cout << "Total Fitness: " << total_fitness << endl;
        */
        for (int i = 0; i < N_POP; i++){
            Alpha[i].genes.str(Cross(nsurvive,OldGen));
        }
        Rand_Mutation();
    cout << "End of Generation " << g << endl;
    //cin.get();
    }
    cout << "Progress:" << progress.str() << endl;

   /* for (int g=0;g<MAX_GEN&&!Solution_Found;g++){    //BEGIN THE GENETIC ALGORITHM! g: generation number
        total_fitness = 0;
        threshold_fitness = 0;
        cout << "==GENERATION: "<< g << "==" << endl;

        cout << "No.|Chromosome" << endl;
        for (int i=0; i<N_POP; i++){
            cout << i << "   " << Alpha[i].genes.str() << "   " << Alpha[i].fitness << endl;
            total_fitness += Alpha[i].fitness;
            threshold_fitness = total_fitness/N_POP*THRES_FIT;
        }
        cout << "Total Fitness: " << total_fitness << endl;

        int nsurvive=0;
        for (int i=0; i<N_POP; i++){     //SELECTION: Random selection
            //int randtest = (rand()%100);
            if(Alpha[i].fitness >= threshold_fitness){
                OldGen[nsurvive]=Alpha[i];
                nsurvive++; //because increment is AFTER Selection, nsurvive reflects the actual number of chromosomes surviving
            //cout << "The fitness:" << Alpha[i].fitness << " and the threshold: " << threshold_fitness << endl;
            }
        }
            cout << "After Selection "<< nsurvive << " chromosomes remain" << endl;
        for (int i=0; i<nsurvive; i++){
            cout << i << "   " << OldGen[i].genes.str() << "   " << OldGen[i].fitness << endl;
        }

        for (int i=0; i<N_POP; i++){ //CROSSING: OldGen Chromes  will be used to popualte the next generation
                Alpha[i].genes.str()=Cross(nsurvive,OldGen);

                if ((rand()%100)/100 < MUTATION_RATE){ //MUTATION: Should randomly choose a gene to mutate
                    int mut_point = rand()%(CHROMO_LENGTH);
                    stringstream mut_gene;
                    stringstream oldgene;
                    oldgene << Alpha[i].genes.str()[mut_point];
                    //istringstream (Alpha[i].genes.str()[mut_point]) >> oldgene;
                    do {
                    mut_gene << rand()%GENE_BASE;
                    }while(mut_gene.str()==oldgene.str());

                    Alpha[i].genes.str().replace(mut_point,1,mut_gene.str());

                }

                Alpha[i].fitness=GetFitness(Alpha[i].genes.str());
                if (Alpha[i].fitness == 999){
                    cout << "Solution has been found!" << endl;
                    cout << Alpha[i].genes.str() << "   " << Alpha[i].fitness << endl;
                    Solution_Found = true;
                    break;
                }
        }
            cout << "Next Generation has been populated!" << endl;

    }
    */
    return 0;

}

string GetRandGenes(int clength){ //Make Random Chromosome

    string genes;
    for (int i=0;i<clength;i++){
        stringstream geneint;
        geneint << rand()%GENE_BASE;
        genes += geneint.str();
    }

return genes;
}

void PopulateGrid(){

    int n_wallmax = 2*N_ROWS+2*N_COLS-4;
    int n_wall =0;
    int n_exit = 0;
    do{
        n_exit = rand()%n_wallmax;
    }while((n_exit==0) || (n_exit==n_wallmax-1)||(n_exit==n_wallmax-N_COLS)||(n_exit==N_COLS-1));
    //exit cannot be located at the 4 corners

    int n_innermax = (N_ROWS-2)*(N_COLS-2);
    int n_inner=0;
    int n_bot =rand()%n_innermax;


    for (int i=0; i<(N_COLS*N_ROWS); i++){
        Grid[i].row << i/N_COLS;
        Grid[i].col << i%N_COLS;

        if ((i/N_COLS==0)||(i%N_COLS==0)||(i/N_COLS==N_ROWS-1)||(i%N_COLS==N_COLS-1)){
            if (n_exit == n_wall){
                Grid[i].property = "e";
                Grid[i].identity = 2;
            }
            else{
                Grid[i].property = "#";
                Grid[i].identity = 1;
            }
            n_wall++;
        }

        else{
            if(n_inner==n_bot){
                Grid[i].property = "b";
                Grid[i].identity = 10;
                bot_position = i;
                switch(rand()%4){
                    case (0): bot_orientation="N"; break;
                    case (1): bot_orientation="E"; break;
                    case (2): bot_orientation="S"; break;
                    case (3): bot_orientation="W"; break;
                }

            }
            else {
                if (rand()%100 < OBS_DENSITY){
                    Grid[i].property = "#";
                    Grid[i].identity = 1;

                }
                else{
                    Grid[i].property = "-";
                    Grid[i].identity = 0;
                }
            }
            n_inner++;
        }
        }
}

int EnvSensed(){
    int env = 0; //from left to right, precursor to int output
    int n (0),s(0),e(0),w(0),ne(0),nw(0),se(0),sw(0);
    if (bot_orientation == "N"){
        w=81; nw=27; n=9; ne=3; e=1;
    }
    if(bot_orientation == "S"){
        e=81; se=27; s=9; sw=3; w=1;
    }
    if(bot_orientation == "E"){
        n=81; ne=27; e=9; se=3; s=1;
    }
    if(bot_orientation=="W"){
        s=81; sw=27; w=9; nw=3; n=1;
    }
        env += Grid[bot_position-1].identity*w;
        env += Grid[bot_position-1+N_COLS].identity*nw;
        env += Grid[bot_position+N_COLS].identity*n;
        env += Grid[bot_position+N_COLS+1].identity*ne;
        env += Grid[bot_position+1].identity*e;
        env += Grid[bot_position-N_COLS+1].identity*se;
        env += Grid[bot_position-N_COLS].identity*s;
        env += Grid[bot_position-N_COLS-1].identity*sw;

    return env;

}

void MoveBot(int env){
    //Check Movement Direction
    string compass = "NESW";
    int bot_orientation_int = 0;
    for (int i=0;i<4;i++){ //parsing string to int
        if(bot_orientation[0] == compass[i]){
            bot_orientation_int = i;
            break;
        }
    }

    int active_gene = 0;
    stringstream activated_gene_parser;
    activated_gene_parser << Alpha[active_chromo].genes.str()[env];
    activated_gene_parser >> active_gene;
    //cout << "Active Gene: " << active_gene << " | Env: " << env << endl;
    int movement_dir = (bot_orientation_int + active_gene)%4;
    //cout << "Movement Direction:" << bot_orientation_int << "+" << active_gene << "=" << movement_dir << endl;
    //Check For Obstacle
    //If None, Move
    int shift_position;
      switch(movement_dir){
        case 0: shift_position = N_COLS; bot_orientation = "N"; break; //NORTH
        case 1: shift_position = 1; bot_orientation = "E"; break; //EAST
        case 2: shift_position = -N_COLS; bot_orientation = "S"; break; //SOUTH
        case 3: shift_position = -1; bot_orientation = "W"; break; //WEST
        default: cout << "SHIFT_POSITION_ERROR" << endl;    break;
        }

    int next_position = bot_position + shift_position;
    if (next_position==prev_position){
        //Bot_Stuck = true;
        //cout << "Bot Oscillating" << endl;
        //cout << "Mutating Gene from " << Alpha[active_chromo].genes.str()[env] << endl;
        Mutate_Active_Gene(env);
        //cout << "To " << Alpha[active_chromo].genes.str()[env] << endl;
    }
    prev_position = bot_position;
    switch (Grid[next_position].identity){
        case 0:
            Grid[next_position].identity = 10;
            Grid[next_position].property = "b";
            Grid[bot_position].identity = 0;
            Grid[bot_position].property = "-";
            bot_position = next_position;
            break;
        case 1:
            //Bot_Stuck=true;
            //cout << "Bot Stuck." << endl;
            //cout << "From:" << Alpha[active_chromo].genes.str() << endl;
            Mutate_Active_Gene(env);
            //cout << "To  :" << Alpha[active_chromo].genes.str() << endl;
            break;
        case 2:
            Exit_Reached = true;
            break;
    }

}
string Cross(int n,Chromosome* Zulu){ //n is the number of survivors of the OldGen
    int r1,r2;
    int splice_point;
    do{
    r1 = rand()%n;
    r2 = rand()%n;
    } while (r1==r2); //generates unique integers r1 and r2 < n
    string c = Zulu[r1].genes.str();

    for (int i =0; i<N_XOVER; i++){
        splice_point = rand()%(CHROMO_LENGTH); //this is the splice point of each chromosome
        c = c.replace(splice_point,X_SPLICE_LENGTH,Zulu[r2].genes.str(),splice_point,X_SPLICE_LENGTH);
    }

    return c;

}

void FitRankSelection(){

    int temp = 0;
    for (int i =0; i<N_POP; i++){ //assign fitness rank
        if (Alpha[i].survival){
            for (int j = 0; j<N_POP; j++){
            if (Alpha[j].survival){
                if (Alpha[i].fitness>Alpha[j].fitness&&Alpha[i].fitness_rank>Alpha[j].fitness_rank){
                    //cout << "Swap" << i << " and " << j << endl;
                    //cout << Alpha[i].fitness_rank << "&" << Alpha[j].fitness_rank << endl;
                    temp = Alpha[j].fitness_rank;
                    Alpha[j].fitness_rank = Alpha[i].fitness_rank;
                    Alpha[i].fitness_rank = temp;
                    //cout << Alpha[i].fitness_rank << "&" << Alpha[j].fitness_rank << endl;
                }
            }
            }
        }

    }
    for (int i = 0; i<N_POP; i++){ //assigning probabilites
        if(Alpha[i].survival){

        if(Alpha[i].fitness_rank==0){
            Alpha[i].pselection = PROB_CHOSEN;
        }
        else if(Alpha[i].fitness_rank==nsurvive){
            Alpha[i].pselection = pow(1-PROB_CHOSEN,nsurvive-1);
        }
        else{
            Alpha[i].pselection = PROB_CHOSEN*pow(1-PROB_CHOSEN,Alpha[i].fitness_rank);
        }
        }
    }
    /*cout << "==TERTIARY SURVIVAL STATS ==" << endl;
    for (int i=0; i<N_POP;i++){
        if (Alpha[i].survival){
            cout << Alpha[i].fitness_rank <<": No." << i << "|F("<<i<<")="<< Alpha[i].fitness << "|P(C)" << Alpha[i].pselection << endl;
        }
    }*/
    nsurvive=0;

    for (int i = 0; i<EXP_N_SURVIVE; i++){
    for (int i = 0; i<N_POP;i++){
        if(Alpha[i].survival){
        double r = rand()%100;
        //cout << r << "<" << Alpha[i].pselection*100 << endl;
        if ( r < (Alpha[i].pselection*100)){

            OldGen[nsurvive].genes.str(Alpha[i].genes.str());
            OldGen[nsurvive].fitness = Alpha[i].fitness;
            OldGen[nsurvive].fitness_rank = Alpha[i].fitness_rank;
            nsurvive++;
            }
        }

    }
    }

}
void Rand_Mutation(){
    for (int i=0; i<N_POP; i++){
    if ((rand()%100)/100 < MUTATION_RATE){ //MUTATION: Should randomly choose a gene to mutate
        int mut_point = rand()%(CHROMO_LENGTH);
        stringstream mut_gene;
        stringstream oldgene;
        oldgene << Alpha[i].genes.str()[mut_point];
        //istringstream (Alpha[i].genes.str()[mut_point]) >> oldgene;
        do {
        mut_gene << rand()%GENE_BASE;
        }while(mut_gene.str()==oldgene.str());

        Alpha[i].genes.str().replace(mut_point,X_SPLICE_LENGTH,mut_gene.str());
        }
    }

}

void Mutate_Active_Gene(int env){

    stringstream b,o;
    string s;
    int oldgene;
    int mutgene;
    s = Alpha[active_chromo].genes.str();
    b << s[env];
    b >> oldgene;
    mutgene = (oldgene+1)%4;
    o << mutgene;
    s[env] = o.str()[0];
    //cout << "In Mutate Function, " << oldgene << " to: "<< mutgene << o.str() << endl;
    Alpha[active_chromo].genes.str(s);
    step_count=step_count-0.5;
}
void PrintGrid(){
    for (int i=0;i<(N_ROWS);i++){ //print the grid
        for (int j=0;j<N_COLS;j++){
            cout <<Grid[(N_COLS*N_ROWS)-N_COLS*i-N_COLS+j].property; //maximum grid number is 49-1=48
            //cout << Grid[0].property;
        }
        cout<<endl;
    }
}

void SearchGrid(){
    int grid_no = 0;
    do{
    cin >> grid_no;
    cout << endl;
    cout << "(" << Grid[grid_no].col.str() << "," << Grid[grid_no].row.str() << ")     " << Grid[grid_no].property << endl;
    } while(grid_no != 999);
    cin.get();
}
