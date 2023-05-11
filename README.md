# ForestFire

A project to investigate the forest fire model such as the time it takes to reach a boundary and the amount of steps until burn out.

Although you could in principle use the python code to perform some of the calculations involved in this project,
it would take too long time to run all of the necessary simulations.
Therefore C++ code with OpenMP parallelisation was used for all sections of this project.

The forest fire model is a simple case of stochastic spreading whereby its evolution is determined by the properties and behaviour of its nearest neighbours. A forest fire model simulates a forest via a grid of trees and spaces. Fire is then implemented and is allowed to spread through the forest under certain rules.
A report of the investigation is found in ForestFire_Optimisation.pdf
