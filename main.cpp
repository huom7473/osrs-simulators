#include "weapons.h"
#include "simulations.h"
#include "bloat_simulator.h"
#include <iostream>
#include <cstdio>
#include <string>

int main(int argc, char **argv) {
    int trials;
    std::string test_name;
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <trials> <test_name>" << std::endl;
        return 1;
    }
    else {
        trials = atoi(argv[1]);
        test_name = argv[2];
    }


    if(test_name == "bloat")
        bloat(trials);
    else if (test_name == "da")
        casino(trials);

}
