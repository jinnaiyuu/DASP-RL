# Makefile
#
# Author: Marlos C. Machado

ALE := ../../Arcade-Learning-Environment
#ALE := ..
# Set this to 1 to enable SDL and display_screen
USE_SDL     := 0

# -O3 Optimize code (urns on all optimizations specified by -O2 and also turns on the -finline-functions, -funswitch-loops, -fpredictive-commoning, -fgcse-after-reload, -ftree-loop-vectorize, -ftree-slp-vectorize, -fvect-cost-model, -ftree-partial-pre and -fipa-cp-clone options).
# -D__USE_SDL Ensures we can use SDL to see the game screen
# -D_GNU_SOURCE=1 means the compiler will use the GNU standard of compilation, the superset of all other standards under GNU C libraries.
# -D_REENTRANT causes the compiler to use thread safe (i.e. re-entrant) versions of several functions in the C library. # 
#FLAGS := -O3
FLAGS := -O3 -g
LIBS := -I$(ALE)/src -L$(ALE) -lale -lz
CXX := g++ -std=c++11
OUT_FILE := learner
# Search for library 'ale' and library 'z' when linking.
LDFLAGS := -lale -lz -lm


OBS := bin/mainBlobTime.o bin/Mathematics.o bin/Timer.o bin/Memory.o bin/Parameters.o bin/Features.o bin/Background.o bin/BlobTimeFeatures.o bin/RLLearner.o bin/SarsaLearner.o
DASPOBS := bin/Prior.o bin/Dasp.o bin/ActionSequenceDetection.o bin/bit_array.o bin/BreadthFirstSearch.o bin/IW1Search.o bin/UCTSearchTree.o bin/UCTTreeNode.o bin/PlayerAgent.o bin/SearchAgent.o bin/SearchTree.o bin/TreeNode.o bin/VertexCover.o
TRGOBS := bin/TriggerStrategy.o bin/EpisodeCount.o
SLTOBS := bin/SelectInitState.o bin/BackCount.o

ifeq ($(strip $(USE_SDL)), 1)
  FLAGS +=  -D__USE_SDL `sdl-config --cflags --libs`
  LDFLAGS += -lSDL -lSDL_gfx -lSDL_image
endif

all: learnerBlobTime

learnerBlobTime: $(OBS) bin/ActionPrior.o $(DASPOBS) $(TRGOBS) $(SLTOBS)
	$(CXX) $(FLAGS) $(OBS) bin/ActionPrior.o $(DASPOBS) $(TRGOBS) $(SLTOBS) -o learnerBlobTime $(LIBS)

bin/mainBlobTime.o: mainBlobTime.cpp
	$(CXX) $(FLAGS) -c mainBlobTime.cpp -o bin/mainBlobTime.o $(LIBS)

bin/Mathematics.o: common/Mathematics.cpp
	$(CXX) $(FLAGS) -c common/Mathematics.cpp -o bin/Mathematics.o $(LIBS)

bin/Timer.o: common/Timer.cpp
	$(CXX) $(FLAGS) -c common/Timer.cpp -o bin/Timer.o $(LIBS)

bin/Memory.o: common/Memory.cpp
	$(CXX) $(FLAGS) -c common/Memory.cpp -o bin/Memory.o $(LIBS)

bin/Parameters.o: common/Parameters.cpp
	$(CXX) $(FLAGS) -c common/Parameters.cpp -o bin/Parameters.o $(LIBS)

bin/Features.o: features/Features.cpp
	$(CXX) $(FLAGS) -c features/Features.cpp -o bin/Features.o $(LIBS)

bin/Background.o: features/Background.cpp
	$(CXX) $(FLAGS) -c features/Background.cpp -o bin/Background.o $(LIBS)

bin/BlobTimeFeatures.o: features/BlobTimeFeatures.cpp
	$(CXX) $(FLAGS) -c features/BlobTimeFeatures.cpp -o bin/BlobTimeFeatures.o $(LIBS)

bin/RLLearner.o: agents/rl/RLLearner.cpp
	$(CXX) $(FLAGS) -c agents/rl/RLLearner.cpp -o bin/RLLearner.o $(LIBS)

bin/SarsaLearner.o: agents/rl/sarsa/SarsaLearner.cpp
	$(CXX) $(FLAGS) -c agents/rl/sarsa/SarsaLearner.cpp -o bin/SarsaLearner.o $(LIBS)

### Action Prior
bin/ActionPrior.o: $(DASPOBS) $(TRGOBS) $(SLTOBS) dasp/ActionPrior.o
	$(CXX) $(FLAGS) -c dasp/ActionPrior.cpp -o bin/ActionPrior.o $(LIBS)

### DASPOBS: Pruning Strategy
bin/Prior.o: dasp/Prior.cpp
	$(CXX) $(FLAGS) -c dasp/Prior.cpp -o bin/Prior.o $(LIBS)

bin/Dasp.o: dasp/Dasp.cpp dasp/Prior.cpp
	$(CXX) $(FLAGS) -c dasp/Dasp.cpp -o bin/Dasp.o $(LIBS)

bin/ActionSequenceDetection.o: dasp/ActionSequenceDetection.cpp
	$(CXX) $(FLAGS) -c dasp/ActionSequenceDetection.cpp -o bin/ActionSequenceDetection.o $(LIBS)

bin/bit_array.o: dasp/bit_array.cxx
	$(CXX) $(FLAGS) -c dasp/bit_array.cxx -o bin/bit_array.o $(LIBS)

bin/BreadthFirstSearch.o: dasp/BreadthFirstSearch.cpp dasp/SearchTree.cpp
	$(CXX) $(FLAGS) -c dasp/BreadthFirstSearch.cpp -o bin/BreadthFirstSearch.o $(LIBS)

bin/IW1Search.o: dasp/IW1Search.cpp dasp/SearchTree.cpp
	$(CXX) $(FLAGS) -c dasp/IW1Search.cpp -o bin/IW1Search.o $(LIBS)

bin/UCTSearchTree.o: dasp/UCTSearchTree.cpp dasp/SearchTree.cpp
	$(CXX) $(FLAGS) -c dasp/UCTSearchTree.cpp -o bin/UCTSearchTree.o $(LIBS)

bin/UCTTreeNode.o: dasp/UCTTreeNode.cpp dasp/TreeNode.cpp
	$(CXX) $(FLAGS) -c dasp/UCTTreeNode.cpp -o bin/UCTTreeNode.o $(LIBS)

bin/PlayerAgent.o: dasp/PlayerAgent.cpp
	$(CXX) $(FLAGS) -c dasp/PlayerAgent.cpp -o bin/PlayerAgent.o $(LIBS)

bin/SearchAgent.o: dasp/SearchAgent.cpp dasp/PlayerAgent.cpp
	$(CXX) $(FLAGS) -c dasp/SearchAgent.cpp -o bin/SearchAgent.o $(LIBS)

bin/SearchTree.o: dasp/SearchTree.cpp
	$(CXX) $(FLAGS) -c dasp/SearchTree.cpp -o bin/SearchTree.o $(LIBS)

bin/TreeNode.o: dasp/TreeNode.cpp
	$(CXX) $(FLAGS) -c dasp/TreeNode.cpp -o bin/TreeNode.o $(LIBS)

bin/VertexCover.o: dasp/VertexCover.cpp
	$(CXX) $(FLAGS) -c dasp/VertexCover.cpp -o bin/VertexCover.o $(LIBS)

### TRGOBS: Trigger Strategy
bin/TriggerStrategy.o: dasp/TriggerStrategy.cpp
	$(CXX) $(FLAGS) -c dasp/TriggerStrategy.cpp -o bin/TriggerStrategy.o $(LIBS)
	
bin/EpisodeCount.o: dasp/EpisodeCount.cpp dasp/TriggerStrategy.cpp
	$(CXX) $(FLAGS) -c dasp/EpisodeCount.cpp -o bin/EpisodeCount.o $(LIBS)

### SLTOBS: Select init state strategy
bin/SelectInitState.o: dasp/SelectInitState.cpp
	$(CXX) $(FLAGS) -c dasp/SelectInitState.cpp -o bin/SelectInitState.o $(LIBS)

bin/BackCount.o: dasp/BackCount.cpp dasp/SelectInitState.cpp
	$(CXX) $(FLAGS) -c dasp/BackCount.cpp -o bin/BackCount.o $(LIBS)




clean:
	rm -rf ${OUT_FILE} bin/*.o
	rm -f learner*
	rm -f *.txt	


#This command needs to be executed in a osX before running the code:
#export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:../lib/ale_0_4"
