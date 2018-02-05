# Mars Lander Neural Network Project

The Learning algorithm used is based on the original implementation of NEAT as follows:

>The NEAT software is based on the real-time NeuroEvolution of
>Augmenting Topologies method of evolving artificial neural networks, by
>Stanley and Miikkulainen (2002).
>
>Source code included in this release is originally written by Kenneth
>Stanley (kstanley@cs.ucf.edu).  The official distribution site is:
>
>http://nn.cs.utexas.edu/keyword?neat-c

This project is based on modifying the original c++ NEAT package described above to work with the CUED
Mars Lander project. It makes some changes and fixes some bugs to the NEAT package as a nice side effect.
Included are a few pre-trained networks for some of the 5 included scenarios under different conditions
including engine lag.

When compiled with make, the package includes two binaries, one for running the lander simulation (called lander),
and one for running the neural network (called neat). In order to begin training, run ./neat with the settings file (.ne ending)
as a parameter, followed by one of the stargen files. When finished, you can run ./lander with any one of the training files to load that AI, or with no
parameters to not use any AI. In order to use the newly trained parameter, go into the genome file and make sure
to delete all but one of the genomes you want to use, as the neural network stores the entire population of that
generation. The premade networks all start with name Scen followed by a number (representing the scenario they are trained with).
Further training can be done on multiple scenarios to make a more general AI capable of handling many situations, but this
would take a lot of time/processing power. Currently the mars lander simulation does not support parallel processing, making it difficult.

In order to change the way the simulation itself works (e.g. adding engine lag), lander.cpp and/or lander.h must be edited
and the program recompiled. To change the way training is done, experiments.cpp is the file to change.

Note: All training files ending in _old were trained using an older, more simple and quicker to train setup, but are less powerful.
The lander program will recognize those files as using the old method, and will work as expected. The newer setup takes much longer
to train a working solution, but can handle more complicated scenarios.

Additionally, if you want to restart training from an already existing genome, simply use that instead of startgene, and add an extra
third parameter to ./neat, e.g:
      ./neat mars.ne startgenmars c
