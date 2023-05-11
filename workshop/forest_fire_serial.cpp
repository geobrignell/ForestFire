#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <assert.h>

/* This code implements a simple C++ version of the forest fire model, as described in the week 9 video.
Recall that each point on the grid represents a possible site for a tree:
- 0: no tree
- 1: tree is alive
- 2: tree is burning
- 3: tree is burnt*/

// define arguments of the forest fire function
int forest_fire(int N, double p);

int main(int argc, char **argv)
{
  // read the size of the (square) grid, the probability of filling a grid point with a tree and the random seed
  // check that we have 3 arguments (in addition to the program name)
  assert (argc == 4);
   
  int N = atoi(argv[1]);
  double p = atof(argv[2]);
  int seed = atoi(argv[3]);
  
  // initialise the random number generator using a fixed seed for reproducibility
  // note: in real applications it may be better to e.g. use the system clock to initialise the random number generator
  srand(seed); 

  // start timer
  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // print a message so we know the commandline arguments were processed correctly
  std::cout << "Simulating forest fire with N = " << N << ", p = " << p << " and seed = " << seed << std::endl;  
  // call the forest fire function
  int nsteps = forest_fire(N, p);
  
  // end timer
  clock_gettime(CLOCK_MONOTONIC, &finish);
  double time_elapsed = (finish.tv_sec - start.tv_sec);
  time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  std::cout << "Simulation took " << time_elapsed << " s" << std::endl;
        
  return 0;
}
  
// function which implements the forest fire model
int forest_fire(int N, double p){

  /* we want 2 grids - the currently active one, and the one from the previous time step -
     we call those grid_new and grid_old
     note: we could have generalised this to be a single 3 dimensional array/vector, with the 3rd index over time*/  
  std::vector <std::vector<int>> grid_old;
  // randomly fill the initial grid with live trees, using probability p
  for (unsigned int i=0;i<N;i++){
    grid_old.push_back(std::vector<int>());

    for (unsigned int j=0;j<N;j++){
      // generate a random floating point number between 0 and 1
      // note: random number generation is non-trivial - in real applications this approach may not be sufficiently random
      double rn = ((float)rand()/(float)(RAND_MAX));
      
      // if the random number is less than our probability p, we fill the site with a tree
      if (rn <= p){
        grid_old[i].push_back(1);
      }
      // otherwise, the site remains empty
      else{
        grid_old[i].push_back(0);
      }
      
    }
  }
  
  // set the top row of trees on fire
  for (unsigned int i=0;i<N;i++){
    if (grid_old[0][i] == 1) grid_old[0][i] = 2;
  }
  
  // initialise the new grid to an empty array
  std::vector <std::vector<int>> grid_new;
  for (unsigned int i=0;i<N;i++){
    grid_new.push_back(std::vector<int>());
    for (unsigned int j=0;j<N;j++){
      grid_new[i].push_back(0);
    }
  }
  
  // loop over time - this loop will continue until there are no more burning trees
  int t = 0;
  bool burning = true;
  while (burning){
  
    // assume nothing is burning, unless we find otherwise below
    burning = false;
   
    // loop over grid points
    for (unsigned int i=0;i<N;i++){
      for (unsigned int j=0;j<N;j++){
      
        // first check if this is a previously burning tree, which should change from burning (2) to burnt (3)
        // note: this operation depends only on the tree itself, i.e. the data at this grid point only
        if (grid_old[i][j] == 2){
          grid_new[i][j] = 3;
          burning = true;
        }
        
        // now we check whether this tree is alive
        else if (grid_old[i][j] == 1){
          /* in this case, we need to check the status of its neighbours, paying attention to whether or not we are at the edge
            note: this operation depends on data from other cells          
            we first set each variable to false, to cover the case where we are at the edge of the cell*/
          bool left_burning = false;
          if (j > 0) left_burning = (grid_old[i][j-1] == 2);
            
          bool right_burning = false;
          if (j < N-1) right_burning = (grid_old[i][j+1] == 2);
          
          bool below_burning = false;
          if (i < N-1) below_burning = (grid_old[i+1][j] == 2);
          
          bool above_burning = false;
          if (i > 0) above_burning = (grid_old[i-1][j] == 2);
          
          // if any of the neighbours are burning, set this tree on fire
          if (left_burning || right_burning || above_burning || below_burning){
            grid_new[i][j] = 2;
          }
          else{
            grid_new[i][j] = 1;
          }
        }
        
        // if there is no tree or it is already burnt, the status remains the same
        else{
          grid_new[i][j] = grid_old[i][j];
        }
        
      }
    }
    
    // increase the time counter
    t++;
    
    // the current grid will become the old grid in the next step
    grid_old = grid_new;
  
  } 
  
  std::cout << "Fire burned out after " << t << " steps" << std::endl;

  return t;

}

