//PROLOGUE: This is my first GA - where the optimum solution must be found that will move from (0,0) to the DESTINATION.
//          Every 2 digits in the chromosome represents 1 of 4 movements: up, down, left or right.
//DECLARATIONS
#include <iostream>
#include <string>
#include <cstdlib> //for rand() and srand()
#include <ctime> //for time()
#include <math.h> //for pow()
#include <sstream> //for stringstream

#define NPOP            20
//#define XOVER_RATE      0.7 //WHY IS THIS EVEN NEEDED?
#define CHROMO_LENGTH   8
#define MAX_GEN         10
#define DESTINATION_X   4 //must ensure it is possible to reach in the CHROMO_LENGTH!
#define DESTINATION_Y   4
#define THRES_FIT       1    //this is the threshold of the total fitness that the chromosome must overcome to survive
//DEVNOTE: this should be in percentile, not absolute: BECAUSE, if SD is low of the population (i.e., all have mean fitness, then all will die)
//THRES_FIT of 1 will mean as long as chromosome is fitter than mean, it will survive
#define X_SPLICE_LENGTH 1   //minimum is 1 for crossover to happen
#define GENE_BASE       5  //base 2 contains 1 and 0, base 10 is 0123456789
#define MUTATION_RATE   0.1 //one out of every 10 genes should mutate
using namespace std;
//END OF DECLARATIONS

struct Chromosome{
    string genes;
    double fitness;
};

// END OF PROTOTYPES

string GetRandGenes(int clength);
double GetFitness(string genes);
string Cross(int n, Chromosome* Zulu);

double total_fitness;
double threshold_fitness;
bool Solution_Found = false;
Chromosome Alpha[NPOP]; //the working population of chromosomes
Chromosome OldGen[NPOP]; //however, the number of OldGen selected will be <NPOP
// END OF FUNCTIONS (PRE-CALLED)

int main(){ //includes output text, crossing and mutation

    srand(time(0));
    cout << "Hello!" << endl;
    cout << "Population Size: " << NPOP << endl;
    cout << "Chromosome Length:" << CHROMO_LENGTH << endl;



    for (int i=0;i<NPOP;i++){ //this is to initialize the population
        //Alpha[i].genes = "0";
        Alpha[i].genes = GetRandGenes(CHROMO_LENGTH);
        Alpha[i].fitness = GetFitness(Alpha[i].genes);
    }

    for (int g=0;g<MAX_GEN&&!Solution_Found;g++){    //g: generation number
        total_fitness = 0;
        threshold_fitness = 0;
        cout << "==GENERATION: "<< g << "==" << endl;

        cout << "No.|Chromosome" << endl;
        for (int i=0; i<NPOP; i++){
            cout << i << "   " << Alpha[i].genes << "   " << Alpha[i].fitness << endl;
            total_fitness += Alpha[i].fitness;
            threshold_fitness = total_fitness/NPOP*THRES_FIT;
        }
        cout << "Total Fitness: " << total_fitness << endl;

        int nsurvive=0;
        for (int i=0; i<NPOP; i++){     //SELECTION: Random selection
            //int randtest = (rand()%100);
            if(Alpha[i].fitness >= threshold_fitness){
                OldGen[nsurvive]=Alpha[i];
                nsurvive++; //because increment is AFTER Selection, nsurvive reflects the actual number of chromosomes surviving
            //cout << "The fitness:" << Alpha[i].fitness << " and the threshold: " << threshold_fitness << endl;
            }
        }
            cout << "After Selection "<< nsurvive << " chromosomes remain" << endl;
        for (int i=0; i<nsurvive; i++){
            cout << i << "   " << OldGen[i].genes << "   " << OldGen[i].fitness << endl;
        }

        for (int i=0; i<NPOP; i++){ //CROSSING: OldGen Chromes  will be used to popualte the next generation
                Alpha[i].genes=Cross(nsurvive,OldGen);

                if ((rand()%100)/100 < MUTATION_RATE){ //MUTATION: Should randomly choose a gene to mutate
                    int mut_point = rand()%(CHROMO_LENGTH);
                    stringstream mut_gene;
                    stringstream oldgene;
                    oldgene << Alpha[i].genes[mut_point];
                    //istringstream (Alpha[i].genes[mut_point]) >> oldgene;
                    do {
                    mut_gene << rand()%GENE_BASE;
                    }while(mut_gene.str()==oldgene.str());

                    Alpha[i].genes.replace(mut_point,1,mut_gene.str());

                }

                Alpha[i].fitness=GetFitness(Alpha[i].genes);
                if (Alpha[i].fitness == 999){
                    cout << "Solution has been found!" << endl;
                    cout << Alpha[i].genes << "   " << Alpha[i].fitness << endl;
                    Solution_Found = true;
                    break;
                }
        }
            cout << "Next Generation has been populated!" << endl;

    }

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

double GetFitness(string genes){ //Get Fitness Number from ONE chromosome
    int x=0, y=0;
    //string s = "11000110";
    string s = "0123456789";
    for(int j=0; j<(CHROMO_LENGTH); j+=1){

        if (genes.compare(j,1,s,0,1)==0){ //check for 0
            //do nothing: rest
        }
        else if (genes.compare(j,1,s,1,1)==0){ //check for up
            y+=1;
        }
        else if (genes.compare(j,1,s,2,1)==0){ //check if dw
            y-=1;
        }
        else if (genes.compare(j,1,s,3,1)==0){ //check if lf
            x-=1;
        }
        else if (genes.compare(j,1,s,4,1)==0){ //check if rg
            x+=1;
        }

    }
    double invfitness = pow((x-DESTINATION_X),2)+ pow((y-DESTINATION_Y),2);

    if (invfitness!=0){
    return 1/invfitness;
    }
    else{
    cout << "Solution Found!" << endl;
    return 999;  //solution is found!
    }

}

string Cross(int n,Chromosome* Zulu){ //n is the number of survivors of the OldGen
    int r1,r2;
    int splice_point = rand()%(CHROMO_LENGTH); //this is the splice point of each chromosome
    int splice_length = X_SPLICE_LENGTH;           //this is the splice length

    do{
    r1 = rand()%n;
    r2 = rand()%n;
    } while (r1==r2); //generates unique integers r1 and r2 < n

    string c = Zulu[r1].genes;
    return c.replace(splice_point,splice_length,Zulu[r2].genes,splice_point,splice_length);

}
