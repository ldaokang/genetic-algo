#ifndef GENETICA_HPP_INCLUDED
#define GENETICA_HPP_INCLUDED

string GetRandGenes(int clength){ //Make Random Chromosome

    string genes;
    for (int i=0;i<clength;i++){
        stringstream geneint;
        geneint << rand()%GENE_BASE;
        genes += geneint.str();
    }

return genes;
}

string Cross(int n,Chromosome* Zulu){ //n is the number of survivors of the OldGen
    int r1,r2;
    int splice_point;
    do{
    r1 = rand()%n;
    r2 = rand()%n;
    } while (r1==r2 && !Zulu[r1].survival && !Zulu[r2].survival); //generates unique integers r1 and r2 < n
    string c = Zulu[r1].genes.str();

    if (rand()%100 < XOVER_RATE*100){
    for (int i =0; i<N_XOVER; i++){
        splice_point = rand()%(CHROMO_LENGTH); //this is the splice point of each chromosome
        c = c.replace(splice_point,X_SPLICE_LENGTH,Zulu[r2].genes.str(),splice_point,X_SPLICE_LENGTH);
    }}

    return c;

}


void AssignFitRank(Chromosome* Zulu){
    int temp = 0;
    for (int i =0; i<N_POP; i++){ //giving chromosomes correct fitness ranks
        if (Zulu[i].survival){
            for (int j = 0; j<N_POP; j++){
            if (Zulu[j].survival){
                if (Zulu[i].fitness>Zulu[j].fitness&&Zulu[i].fitness_rank>Zulu[j].fitness_rank){
                    //cout << "Swap" << i << " and " << j << endl;
                    //cout << Zulu[i].fitness_rank << "&" << Zulu[j].fitness_rank << endl;
                    temp = Zulu[j].fitness_rank;
                    Zulu[j].fitness_rank = Zulu[i].fitness_rank;
                    Zulu[i].fitness_rank = temp;
                    //cout << Zulu[i].fitness_rank << "&" << Zulu[j].fitness_rank << endl;
                    }
            }
            }
        }
    }
}

int FitRankSelection(Chromosome* Zulu, Chromosome* Yankee){ // Yankee is the Older Generation

    int n_selected;
/* ===================================== ASSIGNING PROBABILITIES =========================== */
    for (int i = 0; i<N_POP; i++){ //assigning probabilites
        if(Zulu[i].survival){

        if(Zulu[i].fitness_rank==0){
            Zulu[i].pselection = PROB_CHOSEN;
        }
        else if(Zulu[i].fitness_rank==n_survivors-1){ //fitness_rank begins from 0, so last member should be -1 no. of survivors
            Zulu[i].pselection = pow(1-PROB_CHOSEN,n_survivors-1);
        }
        else{
            Zulu[i].pselection = PROB_CHOSEN*pow(1-PROB_CHOSEN,Zulu[i].fitness_rank);
        }
        }
    }

/* ==================================== SELECTION OF CHROMOSOMES ================================== */
    n_selected=0;
    for (int i = 0; i<EXP_N_SURVIVE; i++){
    for (int i = 0; i<N_POP;i++){
        if(Zulu[i].survival){
        double r = rand()%100;
        //cout << r << "<" << Zulu[i].pselection*100 << endl;
        if ( r < (Zulu[i].pselection*100)){

            Yankee[n_survivors].genes.str(Zulu[i].genes.str());
            Yankee[n_survivors].fitness = Zulu[i].fitness;
            Yankee[n_survivors].fitness_rank = Zulu[i].fitness_rank;
            Yankee[n_survivors].survival = true;
            n_selected++;
            }
        }

    }
    }
return n_selected;
}

string Rand_Mutation(Chromosome* Zulu,int index){
    string m = Zulu[index].genes.str();
    if ((rand()%100)/100 < MUTATION_RATE){ //MUTATION: Should randomly choose a gene to mutate
        int mut_point = rand()%(CHROMO_LENGTH);
        stringstream mut_gene;
        stringstream oldgene;
        oldgene << m[mut_point];
        do {
        mut_gene.str("");
        mut_gene << rand()%GENE_BASE;
        }while(mut_gene.str()==oldgene.str());

        m.replace(mut_point,1,mut_gene.str());
        }

    return m;

}

void Mutate_Active_Gene(int env){

    stringstream b,o;
    string s;
    int oldgene;
    int mutgene;
    s = Alpha[active_chromo].genes.str();
    b << s[env];
    b >> oldgene;
    mutgene = (oldgene+1)%GENE_BASE;
    o << mutgene;
    s[env] = o.str()[0];
    //cout << "In Mutate Function, " << oldgene << " to: "<< mutgene << o.str() << endl;
    Alpha[active_chromo].genes.str(s);
}

#endif // GENETICA_HPP_INCLUDED
