#include <map>

#include "random"
#include <iostream>
#include <string>
#include <vector>

#include "mpi.h"

double f1(double x)
{
    return x * x;
}
double f2(double x)
{
    return exp(-x * x);
}

bool sanitizeArgMap(const std::map<std::string, std::string> &map)
{

    // Check for each pair in the map...
    for (const auto &pair : map)
    {
        if (pair.first == "-P")
        {
            if (pair.second != "1" && pair.second != "2")
            {
                std::cout << "Invalid input for " << pair.first << ": Value must be 1 or 2. Aborting!\n";
                return false;
            }
        }
        if (pair.first == "-N")
        {
            try
            {
                auto test = std::stoll(pair.second);
            }
            // If it fails, print an error message and abort
            catch (...)
            {
                std::cout << "Invalid input for " << pair.first << ": Value must cast to a long. Aborting!\n";
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // maps arg types to their arg
    std::map<std::string, std::string> argMap = {{"-P", "1"}, {"-N", "10000000"}};

    // For each arg passed to the command line...
    for (int i = 1; i < argc; i++)
    {
        // Cast to a string
        std::string arg = argv[i];

        // If the arg contains a "-", the next arg should specify a value. Add it to the map and increment the argc
        // counter
        if (arg.find("-") != std::string::npos)
        {
            argMap[arg] = argv[i + 1];
            i++;
        }
        // If the arg doesn't contain "-", the user did not format their arguments correctly. Pass a message to the
        // console and move on
        else
        {
            std::cerr << "Unexpected input " << arg << ", ignoring...\n";
        }
    }

    // Check for clean inputs
    bool clean = sanitizeArgMap(argMap);
    if (!clean)
    {
        // There was an error sanitizing the map. Abort
        return -1;
    }

    int P = std::stoi(argMap["-P"]);
    long long N = std::stoll(argMap["-N"]);

    // Broadcast N and P so all ranks know them
    MPI_Bcast(&P, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    std::default_random_engine generator(rank + 12345);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double localSum = 0.0;
    auto g = (argMap["-P"] == "1" ? f1 : f2);

    long long local_N = N / size; // integer division
    for (long long i = 0; i < local_N; i++)
    {
        double x = dist(generator);
        localSum += g(x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&localSum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double estimate = global_sum / N;
        std::cout << "Estimate = " << estimate << std::endl;
        std::cout << "Bye!" << std::endl;
    }

    MPI_Finalize();

    return 0;
}
