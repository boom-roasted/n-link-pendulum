# n-Link Pendulum

A simulation of an n-link pendulum.

The simulation demonstrates the use of the Runge Kutta method for numerical analysis.

### Interface

![Image of Interface](img/3-link.png)

### Quickstart

To start the simulation, clone the repository and run the following command
```console
make run
```

### Keyboard Shortcuts

| Command | Keyboard |
| --- | --- |
| Play/Pause | SPACE |
| Play/Pause | K |
| Restart | R |
| Frame Back | ARROW_LEFT |
| Frame Back | J |
| Frame Forward | ARROW_RIGHT |
| Frame Forward | L |
| Zoom In | PLUS |
| Zoom Out | MINUS |
| Zoom to Fit | ZERO |
| Options | ESCAPE |

### Simulation Options

Found in the Options menu

* Number of pendulum links
* Simulation time
* Node mass
* Link length
* Spring constant
* Dampening constant

### Build dependencies:

* `make`
* `g++`
* `SDL2 SDL2_image SDL2_ttf`
