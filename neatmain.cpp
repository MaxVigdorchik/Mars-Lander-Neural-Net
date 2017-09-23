/*
 Copyright 2001 The University of Texas at Austin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//#include <iomanip>
//#include <sstream>
//#include <list>
//#include <vector>
//#include <algorithm>
//#include <cmath>
//#include <iostream.h>
//#include "networks.h"
//#include "neat.h"
//#include "genetics.h"
//#include "experiments.h"
//#include "neatmain.h"
#include <iostream>
#include <vector>
#include "neat.h"
#include "population.h"
#include "experiments.h"
using namespace std;


//  double testdoubval() {
//    return *testdoub;
//  }

//  double testdoubset(double val) {
//    *testdoub=val;
//  }

int main(int argc, char *argv[]) {

  //list<NNode*> nodelist;

  int pause;

  NEAT::Population *p;

  /* GTKMM */
//    myapp=new Gtk::Main(argc, argv);

//    testdoub=&val;

  //***********RANDOM SETUP***************//
  /* Seed the random-number generator with current time so that
      the numbers will be different every time we run.    */
  srand( (unsigned)time( NULL ) );
  if (argc < 3) {
      cerr << "A NEAT parameters file (.ne file) and a starting genes file is required!" << endl;
    return -1;
  }
  bool checkpoint = argc ==4;
  //Load in the params
  NEAT::load_neat_params(argv[1],true);

  cout<<"loaded"<<endl;

  //Genome *test = new_Genome_auto(12,2,0,0,"teststartgenes");
  //test->print_to_filename("teststartgene");

  int generations;
  cout<<"Input number of generations to run experiment: "<<endl;

  cin>>generations;
  p = lander_test(generations, argv[2], checkpoint);
  if (p)
    delete p;

  return(0);
}

