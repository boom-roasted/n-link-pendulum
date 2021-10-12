# Posterity

My original one-file rendition of an n-link pendulum simulation, with a one-file rendition of a python visualization.

Perhaps this is worth keeping around as a fun example project.

## Building and Running Everything

```console
make build sim plot
```

* `build` will build the executable.
* `sim` will run the executable, effectively running the simulation and creating the binary output file `data.bin`.
* `plot` will call the python script on the `data.bin` file, which pulls up a tkinter window and plots the simulation with a few options for pausing and re-starting.
