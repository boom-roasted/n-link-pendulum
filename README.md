# n-Link Pendulum

A simulation of an n-link pendulum system, demonstrating the use of the Runga Kutta method on mass-spring-damper systems.

The meat and potatoes of the simulation happens in the c++ code.
The c++ script serializes the timeseries timulation data into a binary file, which the python script consumes and plots on a tkinter canvas.

## Screenshot

TODO

## Quickstart

No special packages are required, other than `g++`, `make`, and `python 3`.

To run the entire simulation, clone the repository and run `make clean build run plot`.
This command rebuilds the simulation code, re-runs the simulation, and plots the resultant data.

Change simulation parameters such as simulation time or link node properties in the `main` function.
