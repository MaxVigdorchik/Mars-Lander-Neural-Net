#include <iostream>
#include <iomanip>
#include <strstream>
#include <list>
#include <vector>
#include <algorithm>
#include <math.h>
#include "neat.h"
#include "population.h"
#include "visual.h"
#include "genome.h"

int main(int argc, char* argv[])
{
      /* GTKMM */
    myapp=new Gtk::Main(argc, argv);
    
    char* filename;
    if(argc != 2)
	cout << "Must include a genome file name to visualize" << endl;
    else
	filename = argv[1];

    cout << "Reading in Genome and converting to Network" << endl;

    Genome *genome = new_Genome_load(filename);
    Organism *org = new Organism(0.0, autopilot_genome, 1);
    Network *net = org->net;

    net->graph(1000,1000);
}
