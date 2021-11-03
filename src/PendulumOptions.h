#ifndef PENDULUM_PENDULUMOPTIONS_H
#define PENDULUM_PENDULUMOPTIONS_H

struct PendulumOptions
{
    int numLinks = 3;        // Size of pendulum
    double simTime = 20;     // Simulation time (seconds)
    int saveFrameStep = 500; // Computed frames between every output frame

    double m = 0.2;   // Node mass (kg)
    double l = 3;      // Link length (m)
    double k = 100e3;  // Link spring constant (N/m)
    double c = 100e-6; // Link dampening constant (N/m/m)
};

#endif // PENDULUM_PENDULUMOPTIONS_H
