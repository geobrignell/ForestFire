#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <assert.h>
#include <omp.h>
#include <fstream>
#include <tuple>

/* This code implements a simple C++ version of the forest fire model, as described in the week 9 video.
Recall that each point on the grid represents a possible site for a tree:
- 0: no tree
- 1: tree is alive
- 2: tree is burning
- 3: tree is burnt*/

// define arguments of the forest fire function
std::tuple<int, int> forest_fire(int N, double p, int wind);

bool bottom_reached(int N, std::vector<std::vector<int>> last_grid);

int main(int argc, char **argv)
{
  // read the size of the (square) grid, the probability of filling a grid point with a tree and the random seed

  // check that we have 3 arguments (in addition to the program name)
  assert (argc == 4); 
  int N = atoi(argv[1]);
  double p = atof(argv[2]);
  int wind = atoi(argv[3]); // wind = {0:off, 1:up, 2:right, 3:down, 4:left}

  // check that wind is valid
  if (wind == 0 || wind == 1 || wind ==2 || wind ==3 || wind ==4){}
  else{
    std::cout << "Enter a valid wind value";
    return 0;
  }

  //setting up file locations
  std::string folder_name = "wind_data/wind_dir_" + std::to_string(wind);

  std::ofstream myfile;
  myfile.open(folder_name + "/p_N" + std::to_string(p) +"_" + std::to_string(N) + ".csv");
  myfile << "seed" << "," << "steps" << "," << "bool" << "\n";


  // loop through seeds and input data to file
  //#pragma omp parallel for
  for (int seed = 0; seed < 50; seed++){
    myfile << seed << ",";
    srand(seed);
    myfile << std::get<0>(forest_fire(N, p, wind)) << "," << std::get<1>(forest_fire(N, p, wind)) << "\n";
    }


  myfile.close();
  // end timer
  // clock_gettime(CLOCK_MONOTONIC, &finish);
  // double time_elapsed = (finish.tv_sec - start.tv_sec);
  // time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  // std::cout << "Simulation took " << time_elapsed << " s" << std::endl;
        
  return 0;
}
  
// function which implements the forest fire model
std::tuple<int, int> forest_fire(int N, double p, int wind){

  /* we want 2 grids - the currently active one, and the one from the previous time step -
     we call those grid_new and grid_old
     note: we could have generalised this to be a single 3 dimensional array/vector, with the 3rd index over time*/  
  std::vector < std::vector < int > > grid_old;
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
  std::vector < std::vector < int > > grid_new;
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
   
    // we want the following variables to be private:
    // i, j, left_burning, right_burning, above_burning and below_burning
    // and the following to be shared:
    // n, grid_old, grid_new, burning
    // by default, loop iteration variables and variables declared locally within the parallel loop are private
    // -> this covers all the private variables
    // variables declared outside the loop are shared
    // -> this covers all the shared variables
    // therefore we do not need to be explicit about any of the variables
    #pragma omp parallel for reduction (||:burning)
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
          if (j > 0){
            left_burning = (grid_old[i][j-1] == 2);
            }
            
          bool right_burning = false;
          if (j < N-1){
            right_burning = (grid_old[i][j+1] == 2);
          }

          bool below_burning = false;
          if (i < N-1){
            below_burning = (grid_old[i+1][j] == 2);
          }

          bool above_burning = false;
          if (i > 0){
            above_burning = (grid_old[i-1][j] == 2);
          }
          
          //adding in the wind conditions
          // basically just extends the statements above but for an extra tile
          
          bool down_down_burning = false;
          bool up_up_burning = false;
          bool left_left_burning = false;
          bool right_right_burning = false;

          if (wind == 1){
            if (i < N-2){
              down_down_burning = (grid_old[i+2][j] ==2);
            }
          }

          if (wind == 3){
            if (i > 1){
              up_up_burning = (grid_old[i-2][j] ==2);
            }
          }

          if (wind == 2){
            if (j>1){
              left_left_burning = (grid_old[i][j-2]==2);
            }
          }

          if (wind == 4){
            if (j<N-2){
              right_right_burning = (grid_old[i][j+2]==2);
            }
          }
        

          
          // if any of the neighbours are burning, set this tree on fire
          if (left_burning || right_burning || above_burning || below_burning || down_down_burning || up_up_burning || left_left_burning || right_right_burning){
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
    // the OMP region ends here
    
    // increase the time counter
    t++;
    
    // the current grid will become the old grid in the next step
    grid_old = grid_new;
  
  } 

  bool reached_bottom = bottom_reached(N, grid_old);
  // if (reached_bottom){
  //   std::cout << "Fire reached the end!" << std::endl; 
  // }
  // else{
  //   std::cout << "Didn't reach end!" << std::endl;
  // }
  
  // std::cout << "Fire burned out after " << t << " steps" << std::endl;

  // return tuple of steps and result
  return std::make_tuple(t, reached_bottom);
}

bool bottom_reached(int N, std::vector<std::vector<int>> last_grid){
  
  for (unsigned int i = 0; i < N; i++){
    
    if (last_grid[N-1][i] == 2 || last_grid[N-1][i] == 3){
      return true;
    }
  }

  return false;
}