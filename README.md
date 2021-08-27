# n-Link Pendulum

A simulation of an n-link pendulum system, demonstrating the use of the Runga Kutta method on mass-spring-damper systems.

The meat and potatoes of the simulation happens in the c++ code.

The c++ function computes timeseries simulation data and writes it out to a binary file. The python script consumes the binary file and plots the simulation data on a tkinter canvas.

Only one timestep (approximately) is held in memory at a time, allowing this simulation/visualization to be extremely resource efficient.

## Screenshots

2-link pendulum

![Image of 2-link pendulum](img/2-link.png)

5-link pendulum

![Image of 5-link pendulum](img/5-link.png)

## Quickstart

No special packages are required. The c++ file builds with `g++` and `make`, and the python script is run with standard `python 3`.

To run the entire simulation, clone the repository and run `make clean build sim plot`.
This command rebuilds the simulation code, runs the simulation, and plots the resultant data.

Change simulation parameters such as simulation time or link node properties in the c++ `main` function.
