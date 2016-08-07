#ifndef MAZEFUNCTION_HPP_INCLUDED
#define MAZEFUNCTION_HPP_INCLUDED



void PrintGrid(Space* Grid,int col, int row){
    n_rows = row;
    n_cols = col;
    for (int i=0;i<(n_rows);i++){ //print the grid
        for (int j=0;j<n_cols;j++){
            cout <<Grid[(n_cols*n_rows)-n_cols*i-n_cols+j].property; //maximum grid number is 49-1=48
            //cout << Grid[0].property;
        }
        cout<<endl;
    }
}

void SearchGrid(Space* Grid){
    int grid_no = 0;
    do{
    cin >> grid_no;
    cout << endl;
    cout << "(" << Grid[grid_no].col.str() << "," << Grid[grid_no].row.str() << ")     " << Grid[grid_no].property << endl;
    } while(grid_no != 999);
    cin.get();
}

int EnvSensed(Space* Grid){
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
        env += Grid[bot_position-1+n_cols].identity*nw;
        env += Grid[bot_position+n_cols].identity*n;
        env += Grid[bot_position+n_cols+1].identity*ne;
        env += Grid[bot_position+1].identity*e;
        env += Grid[bot_position-n_cols+1].identity*se;
        env += Grid[bot_position-n_cols].identity*s;
        env += Grid[bot_position-n_cols-1].identity*sw;

    return env;

}

void MoveBot(Chromosome* Zulu, Space* Grid, int env, bool ActiveMutation){
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
    stringstream activated_gene_parser; //parse from char to int
    activated_gene_parser << Zulu[active_chromo].genes.str()[env];
    activated_gene_parser >> active_gene;
    //cout << "Active Gene: " << active_gene << " | Env: " << env << endl;
    int movement_dir = (bot_orientation_int + active_gene)%4;
    //cout << "Movement Direction:" << bot_orientation_int << "+" << active_gene << "=" << movement_dir << endl;
    //Check For Obstacle
    //If None, Move
    int shift_position;
      switch(movement_dir){
        case 0: shift_position = n_cols; bot_orientation = "N"; break; //NORTH
        case 1: shift_position = 1; bot_orientation = "E"; break; //EAST
        case 2: shift_position = -n_cols; bot_orientation = "S"; break; //SOUTH
        case 3: shift_position = -1; bot_orientation = "W"; break; //WEST
        default: cout << "SHIFT_POSITION_ERROR" << endl;    break;
        }

    int next_position = bot_position + shift_position;
    if (next_position==prev_position && ActiveMutation){
        //Bot_Stuck = true;
        //cout << "Bot Oscillating" << endl;
        //cout << "Mutating Gene from " << Alpha[active_chromo].genes.str()[env] << endl;
        if(ActiveMutation){Mutate_Active_Gene(env);}
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
            if(ActiveMutation){Mutate_Active_Gene(env);}
            //cout << "To  :" << Alpha[active_chromo].genes.str() << endl;
            break;
        case 2:
            Exit_Reached = true;
            break;
    }
    step_count++;
}

void PopulateSpace(Space* Grid, int col, int row, int n_exit, int n_bot, int obs){ //set n_exit =0 and n_bot =0 if you want random values

    n_cols = col;
    n_rows = row;
    obs_density = obs;
    bool rand_exit = (n_exit==0);
    bool rand_bot = (n_bot==0);
    int n_wallmax = 2*n_rows+2*n_cols-4;
    int n_wall =0;

    if (rand_exit){
        while((n_exit==0) || (n_exit==n_wallmax-1)||(n_exit==n_wallmax-n_cols)||(n_exit==n_cols-1)){
        n_exit = rand()%n_wallmax;
        }} //exit cannot be at 4 corners

    int n_innermax = (n_rows-2)*(n_cols-2);
    int n_inner=0;
    if (rand_bot){n_bot =rand()%n_innermax;}

    for (int i=0; i<(n_cols*n_rows); i++){ //START POPULATION
        Grid[i].row << i/n_cols;
        Grid[i].col << i%n_cols;

        if ((i/n_cols==0)||(i%n_cols==0)||(i/n_cols==n_rows-1)||(i%n_cols==n_cols-1)){
            if (n_exit == n_wall && rand_exit){ //exit
                Grid[i].property = "e";
                Grid[i].identity = 2;}
            else{                               //wall
                Grid[i].property = "#";
                Grid[i].identity = 1;}
            n_wall++;}

        else{if(n_inner==n_bot && rand_bot){ //bot
                Grid[i].property = "b";
                Grid[i].identity = 10;
                bot_position = i;
                switch(rand()%4){
                    case (0): bot_orientation="N"; break;
                    case (1): bot_orientation="E"; break;
                    case (2): bot_orientation="S"; break;
                    case (3): bot_orientation="W"; break;}}
            else { //empty space
                if (rand()%100 < obs_density){
                    Grid[i].property = "#";
                    Grid[i].identity = 1;}
                else{
                    Grid[i].property = "-";
                    Grid[i].identity = 0;}}
            n_inner++;}
        }

    if(!rand_exit){
        Grid[n_exit].property = "e";
        Grid[n_exit].identity = 2;
    }
    if(!rand_bot){
        Grid[n_bot].property = "b";
        Grid[n_bot].identity = 10;
        bot_position = n_bot;
        bot_orientation = "N";
    }
}


Space* ChooseSpace(int s_code){
    switch(s_code){
        case 10: //Dojo
            PopulateSpace(Dojo,3,3,0,4,0);
            return Dojo;
            break;
        case 11: //Sanctuary
            PopulateSpace(Sanctuary,4,4,0,5,0);
            return Sanctuary;
            break;

        default:
            cout << "INVALID SPACE CODE" << endl;
            break;

    }

}

void RunThrough(Chromosome* Zulu, Space* Grid, int env, bool ActiveMutation, bool StepByStep){
    step_count = 0;
    prev_position = 0;
    Bot_Stuck = false;
    Zulu[active_chromo].fitness = 0;
    Zulu[active_chromo].survival = false;
    while(!Exit_Reached && (step_count < STEP_TOLERANCE) && !Bot_Stuck){
                MoveBot(Zulu,Grid, env, ActiveMutation);
                if(StepByStep){PrintGrid(Grid,n_cols,n_rows);}
                }

            if(Exit_Reached){
                cout<< "Exit Reached in " << step_count << "steps!" << endl;
                Zulu[active_chromo].fitness ++;
                Zulu[active_chromo].survival = true;
                Zulu[active_chromo].fitness_rank = n_survivors;
                n_survivors++;
                //cout<< "Fitness: " << Alpha[active_chromo].fitness << endl;
                total_fitness ++;
                //cout << "Total Fitness: " << total_fitness << endl;
                Exit_Reached = false;
            }

}

void DisplayFittest (Chromosome* Zulu, int chromo_index, Space* Grid, int s_code){
    //remember to specify active_chromo
    RunThrough(Zulu,Grid,EnvSensed(Grid),false,true);

}

Space* WhichGrid(){
    PopulateSpace(Dojo,3,3,0,4,0);
    cout << "returning dojo" << endl;
    return Dojo;
}



#endif // MAZEFUNCTION_HPP_INCLUDED
