#include <map>

#include <iostream>
#include <string>
#include <vector>

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
                auto test = std::stoi(pair.second);
            }
            // If it fails, print an error message and abort
            catch (...)
            {
                std::cout << "Invalid input for " << pair.first << ": Value must cast to an int. Aborting!\n";
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
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

    return 0;
}
