#ifndef PENDULUM_PENDULUMOPTIONS_H
#define PENDULUM_PENDULUMOPTIONS_H

struct PendulumOptions
{
    int numLinks = 3;        // Size of pendulum
    double simTime = 20;     // Simulation time, seconds
    int saveFrameStep = 500; // Computed frames between every output frame

    double m = 0.25;   // Node mass
    double l = 3;      // Link length
    double k = 100e3;  // Link spring constant
    double c = 0.0001; // Link dampening constant
};

#endif // PENDULUM_PENDULUMOPTIONS_H
