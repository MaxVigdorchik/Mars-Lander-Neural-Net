CC = g++ -O3 
CCSW = -DLANDER_MAIN #Used to distinguish bettwen neatmain and main in lander_graphics
PLATFORM = `uname`
#LIBS = -L/usr/lib /usr/lib/libqthreads.so.0 -lguile -ldl -lreadline -ltermcap -lm

INCLUDES = -I/usr/include/g++-2 -I/usr/lib/sigc++/include -I./lib -I./src

CFLAGS = -g -fopenmp -Wall -Wno-return-type $(INCLUDES) -DSWIG_GLOBAL -Wno-deprecated-declarations 
#CFLAGS = -g -Wall -Werror
#CFLAGS = -g -Wall 

all: neat lander

neat: neat.o network.o nnode.o link.o trait.o gene.o genome.o innovation.o organism.o species.o population.o lander.o lander_graphics.o experiments.o neatmain.o
	$(CC) $(CFLAGS) $(LIBS) build/neat.o build/network.o build/nnode.o build/link.o build/trait.o build/gene.o build/genome.o build/innovation.o build/organism.o build/species.o build/population.o build/lander.o build/lander_graphics.o build/experiments.o build/neatmain.o -o neat  

lander: neat.o network.o nnode.o link.o trait.o gene.o genome.o innovation.o organism.o species.o population.o lander_main.o lander_graphics_main.o experiments.o
	$(CC) $(CFLAGS) $(LIBS) $(CCSW) build/neat.o build/network.o build/nnode.o build/link.o build/trait.o build/gene.o build/genome.o build/innovation.o build/organism.o build/species.o build/population.o build/lander_main.o build/lander_graphics_main.o build/experiments.o -o lander  -lGL -lGLU -lglut

########################

lander_graphics_main.o: src/lander_graphics.cpp src/lander.h
	$(CC) $(CFLAGS) $(CCSW) -c src/lander_graphics.cpp -o build/lander_graphics_main.o

lander_main.o: src/lander.cpp src/lander.h
	$(CC) $(CFLAGS) $(CCSW) -c src/lander.cpp -o build/lander_main.o

lander_graphics.o: src/lander_graphics.cpp src/lander.h
	$(CC) $(CFLAGS) -c src/lander_graphics.cpp -o build/lander_graphics.o

lander.o: src/lander.cpp src/lander.h
	$(CC) $(CFLAGS) -c src/lander.cpp -o build/lander.o

neat.o: lib/neat.cpp lib/neat.h
	  $(CC) $(CFLAGS) -c lib/neat.cpp -o build/neat.o

network.o: lib/network.cpp lib/network.h lib/neat.h neat.o  
	$(CC) $(CFLAGS)  -c lib/network.cpp -o build/network.o

nnode.o: lib/nnode.cpp lib/nnode.h    
	$(CC) $(CFLAGS) -c lib/nnode.cpp -o build/nnode.o

link.o: lib/link.cpp lib/link.h
	  $(CC) $(CFLAGS) -c lib/link.cpp -o build/link.o

trait.o: lib/trait.cpp lib/trait.h
	  $(CC) $(CFLAGS) -c lib/trait.cpp -o build/trait.o

gene.o: lib/gene.cpp lib/gene.h
	  $(CC) $(CFLAGS) -c lib/gene.cpp -o build/gene.o

genome.o: lib/genome.cpp lib/genome.h
	  $(CC) $(CFLAGS) -c lib/genome.cpp -o build/genome.o

innovation.o: lib/innovation.cpp lib/innovation.h
	  $(CC) $(CFLAGS) -c lib/innovation.cpp -o build/innovation.o

organism.o: lib/organism.cpp lib/organism.h    
	$(CC) $(CFLAGS) -c lib/organism.cpp -o build/organism.o

species.o: lib/species.cpp lib/species.h lib/organism.h
	  $(CC) $(CFLAGS) -c lib/species.cpp -o build/species.o

population.o: lib/population.cpp lib/population.h lib/organism.h
	  $(CC) $(CFLAGS) -c lib/population.cpp -o build/population.o

experiments.o: src/experiments.cpp src/experiments.h lib/network.h lib/species.h
	$(CC) $(CFLAGS) -c src/experiments.cpp -o build/experiments.o

neatmain.o: src/neatmain.cpp src/neatmain.h lib/neat.h lib/population.h
	$(CC) $(CFLAGS) -c src/neatmain.cpp -o build/neatmain.o


########################

clean:
	rm -f build/* lander neat
