//
// Created by huom on 4/9/21.
//

#include "simulations.h"
#include "bloat_simulator.h"
#include "duel_simulator.h"
#include <iostream>

void casino(int trials) {
    std::cout << "Mace vs Whip (Player 1 uses mace):\n";
    simulate_duel(mace_vs_whip_acc, trials);

    std::cout << "Mace + Ham vs Whip (Player 1 uses mace):\n";
    simulate_duel(mace_and_ham_vs_whip_acc, trials);
    // simulate_duel(mace_vs_whip_acc_debug, 100);
    // std::cout << std::endl;

    std::cout << "Trials run for each simulation: " << trials << "\n\n";
}

void bloat(int trials) {
    std::cout << "2x Pre-BGS:\n";
    simulate_bloat(bloat_2prebgs_rdefense, trials);
    std::cout << "1x Pre-BGS:\n";
    simulate_bloat(bloat_prebgs_rdefense, trials);
    /*std::cout << "Pre-BGS (all hits on drained def, but bgs'er misses a scythe hit 1st down):\n";
    simulate_bloat(bloat_prebgs_miss, NUM_TRIALS);*/
    std::cout << "Chally:\n";
    simulate_bloat(bloat_chally, trials);

    std::cout << "Trials run for each simulation: " << trials << "\n\n";
}
