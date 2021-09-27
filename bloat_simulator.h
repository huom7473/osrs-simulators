//
// Created by huom on 9/6/20.
//

#ifndef COMBAT_SIMULATOR_BLOAT_SIMULATOR_H
void simulate_bloat(int (*func) (int&, int&, int&), int trials);
int bloat_1bgs(int &htk, int &hp1, int &hp2);
int bloat_2bgs(int &htk, int &hp1, int &hp2);
int bloat_chally(int &htk, int &hp1, int &hp2);
int bloat_prebgs(int &htk, int &hp1, int &hp2);
int bloat_prebgs_rdefense(int &htk, int &hp1, int &hp2);
int bloat_prebgs_miss(int &htk, int &hp1, int &hp2);
int bloat_2prebgs_rdefense(int &htk, int &hp1, int &hp2);
#define COMBAT_SIMULATOR_BLOAT_SIMULATOR_H

#endif //COMBAT_SIMULATOR_BLOAT_SIMULATOR_H
