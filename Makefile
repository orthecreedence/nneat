SHELL		= /bin/bash
MAKE		= make

TARGET		= nneat
SUBDIRS		= 
LIBS		= -lpthread -lglut32 -lglu32 -lopengl32
INCLUDE		= 

CXX			= g++ 
CXXFLAGS	= -O2 -g -Wall -fmessage-length=0 $(INCLUDE) 

OBJS		= nneat.o draw.o population.o animal.o food.o config.o NEAT/gene.o NEAT/genome.o NEAT/innovation.o NEAT/link.o NEAT/neat.o NEAT/network.o NEAT/nnode.o NEAT/organism.o NEAT/population.o NEAT/species.o NEAT/trait.o

$(TARGET):	recursive $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(INCLUDE)
	

recursive:
	@for i in $(SUBDIRS); do (cd $$i; $(MAKE) all); done

all: $(TARGET)

config.o: config.cpp

clean:
	@for i in $(SUBDIRS); do (cd $$i; $(MAKE) clean); done
	rm -f *.o ${TARGET} *.stackdump

