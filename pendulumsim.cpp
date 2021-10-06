#include <iostream>
#include <fstream>

#include "chain.h"

int main(int argc, char *argv[])
{
    // Main modify-able simulation parameters
    const int numLinks = 2; // Size of chain
    const std::string fp = "data.bin"; // Output data file
    const double simTime = 20; // Simulation time, seconds

    // Default node properties
    const double m = 0.25;
    const double l = 3;
    const double k = 1e5;
    const double c = 0.0001;

    const double deltaT = 1.0 / 200.0 * 1.0 / std::sqrt(k / m); // Time step increment
    const int iterations = std::lround(simTime / deltaT);

    auto chain = Chain::Create(numLinks, m, l, k, c, Chain::Layout::Line);

    // Data storage
    auto fout = std::ofstream(fp, std::ios::out | std::ios::binary);
    if (!fout)
    {
        std::cout << "Cannot open file for writing!" << std::endl;
        return 1;
    }

    // Write initial state to file
    chain.Serialize(fout);

    // Main simulation loop
    for (int i = 0; i < iterations; i++)
    {
        // Increment time
        chain.RungeKuttaSecondOrder(deltaT);

        // Write state to file
        chain.Serialize(fout);

        // Display progress
        if (i % 1000 == 0)
        {
            double progress = i / static_cast<double>(iterations);
            const int barWidth = 70;
            std::cout << "[";
            int barPosition = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i <= barPosition) std::cout << "#";
                else std::cout << " ";
            }
            std::cout << "] " << int(std::lround(progress * 100.0)) << " %\r";
            std::cout.flush();
        }
    }

    // Finish progress bar
    std::cout << std::endl;

    // Close data file
    fout.close();

    // Read output file and print resultant data to demonstrate integrety
    constexpr bool printResult = false;
    if (printResult)
    {
        const auto resultChains = Chain::Deserialize(fp);

        for (const auto& c : resultChains)
        {
            std::cout << "r: ";
            c.PrintState();
        }
    }

    return 0;
}
