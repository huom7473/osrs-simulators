//
// Created by huom on 9/6/20.
//
#include "weapons.h"
#include "bloat_simulator.h"
#include <iostream>
#include <omp.h>
static constexpr int BLOAT_HP = 1800;

void simulate_bloat(int (*func) (int&, int&, int&), int trials) {
    int htk{}, hp1{}, hp2{};
    double ahtk{}, ahp1{}, ahp2{}, atd{}; //accumulators
    #pragma omp parallel for private(htk, hp1, hp2) reduction(+:ahtk, ahp1, ahp2, atd)
    for (int i = 0; i < trials; ++i) {
        func(htk, hp1, hp2);
        ahtk += htk;
        ahp1 += hp1;
        ahp2 += hp2;
        if (hp2 <= 0) ++atd;
    }
    // std::cout << "Results after " << trials << " simulated runs:\n";
    std::cout << "Two Down Probability: " << atd/trials << "\nAvg. HTK: " << ahtk/trials<<
              "\nAvg. Phase 1 HP: " << ahp1/trials << "\nAvg. Phase 2 HP: " << ahp2/trials << "\n" << std::endl;
}

int bloat_1bgs(int &htk, int &hp1, int &hp2) {
    int rem_hp{BLOAT_HP};
    int bgs_hits{}, current_hit{18}, new_defroll{}; //assumes no 1down
    bool dead_flag{false};
    //phase 1
    rem_hp -= claw_spec(9156);
    rem_hp -= claw_spec(9156);
    rem_hp -= (bgs_hits += bgs(9156));
    new_defroll = (109-bgs_hits)*84; //can't hit a 100 bgs anyway
    for(int i{}; i < 12; ++i) { //3 players * 4 scythes
        rem_hp -= scythe(new_defroll);
    }
    for(int i{}; i < 3; ++i) { //3 players * 1 chally spec
        rem_hp -= chally_spec(9156); //assuming chally is on normal defence
    }
    //current_hit = 18;
    hp1 = rem_hp;
    //phase 2 - begin checking for dead bloat
    for(int i{}; i < 15; ++i) { //3 players * 5 scythes
        rem_hp -= scythe(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    for(int i{}; i < 3; ++i) {
        rem_hp -= chally_spec(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    hp2 = rem_hp;
    //finish bloat
    if(rem_hp > 0) htk = current_hit;
    while(rem_hp > 0) {
        rem_hp -= scythe(9156);
        ++htk;
    }
    return 0;
}

int bloat_2bgs(int &htk, int &hp1, int &hp2) {
    int rem_hp{BLOAT_HP};
    int bgs_hits{}, current_hit{18}, new_defroll1{}, new_defroll2{}, temp{};
    bool dead_flag{false};
    //phase 1
    rem_hp -= claw_spec(9156);
    temp = bgs(9156);
    bgs_hits += temp;
    rem_hp -= temp;
    new_defroll1 = (109-bgs_hits)*84;
    temp = bgs(new_defroll1);
    bgs_hits += temp;
    rem_hp -= temp;
    new_defroll2 = (9+std::max(0,100-bgs_hits))*84;
    for(int i{}; i < 12; ++i) { //3 players * 4 scythes
        rem_hp -= scythe(new_defroll2);
    }
    for(int i{}; i < 3; ++i) { //3 players * 1 chally spec
        rem_hp -= chally_spec(9156); //assuming chally is on normal defence
    }
    //current_hit = 18;
    hp1 = rem_hp;
    //phase 2 - begin checking for dead bloat
    for(int i{}; i < 15; ++i) { //3 players * 5 scythes
        rem_hp -= scythe(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    for(int i{}; i < 3; ++i) {
        rem_hp -= chally_spec(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    hp2 = rem_hp;
    //finish bloat
    if(rem_hp > 0) htk = current_hit;
    while(rem_hp > 0) {
        rem_hp -= scythe(9156);
        ++htk;
    }
    return 0;
}

int bloat_chally(int &htk, int &hp1, int &hp2) {
    int rem_hp{BLOAT_HP};
    int current_hit{18}; //assumes no 1down
    bool dead_flag{false};
    for(int i{}; i < 6; ++i) { //2 challies * 3 players
        rem_hp -= chally_spec(9156);
    }
    for(int i{}; i < 12; ++i) { //3 players * 4 scythes
        rem_hp -= scythe(9156);
    }
    hp1 = rem_hp;
    //phase 2 - begin checking for dead bloat
    for(int i{}; i < 3; ++i) { //3 players * 1 claw spec
        rem_hp -= claw_spec(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    for(int i{}; i < 15; ++i) { //3 players * 5 scythes
        rem_hp -= scythe(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    hp2 = rem_hp;
    //finish bloat
    if(rem_hp > 0) htk = current_hit;
    while(rem_hp > 0) {
        rem_hp -= scythe(9156);
        ++htk;
    }
    return 0;
}

/* one player run-through bgsing at the beginning, then chally 4 scy chally from all 3 players,
 * then 2x playres have claw spec 2nd down -> scythe until dead */
int bloat_prebgs(int &htk, int &hp1, int &hp2) {
  int rem_hp{BLOAT_HP};
  int bgs_hits{}, current_hit{18}, new_defroll{}; //assumes no 1down
  bool dead_flag{false};
  //phase 1
  rem_hp -= (bgs_hits += bgs(9156))/2; //prebgs has damage halved
  new_defroll = (109-bgs_hits)*84; //can't hit a 100 bgs anyway
  for(int i{}; i < 6; ++i) { //2 challies * 3 players
    rem_hp -= chally_spec(new_defroll); //assume both challies are on reduced defense - not sure how this works.
    //it could be that 3 challies are on regular defense and 3 challies are on reduced.
  }
  for(int i{}; i < 12; ++i) { //3 players * 4 scythes
    rem_hp -= scythe(new_defroll);
  }
  hp1 = rem_hp;

  //phase 2 - begin checking for dead bloat
  for(int i{}; i < 2; ++i) { //2 players * 1 claw spec
    rem_hp -= claw_spec(9156);
    ++current_hit;
    if (!dead_flag and rem_hp <= 0) {
        htk = current_hit;
        dead_flag = true;
    }
  }
  for(int i{}; i < 16; ++i) { //2 players * 5 scythe + 1 player * 6 scythe
    rem_hp -= scythe(9156);
    ++current_hit;
    if (!dead_flag and rem_hp <= 0) {
        htk = current_hit;
        dead_flag = true;
    }
  }
  hp2 = rem_hp;

  //finish bloat if it's not already dead
  if(rem_hp > 0) htk = current_hit;
  while(rem_hp > 0) {
    rem_hp -= scythe(9156);
    ++htk;
  }
  return 0;
}

/* same thing as bloat_prebgs, but second round of challies on first down are on restored defense */
int bloat_prebgs_rdefense(int &htk, int &hp1, int &hp2) {
    int rem_hp{BLOAT_HP};
    int bgs_hits{}, current_hit{18}, new_defroll{}; //assumes no 1down
    bool dead_flag{false};
    //phase 1
    rem_hp -= (bgs_hits += bgs(9156))/2; //prebgs has damage halved
    new_defroll = (109-bgs_hits)*84; //can't hit a 100 bgs anyway
    for(int i{}; i < 6; ++i) { //2 challies * 3 players
        if (i < 3) rem_hp -= chally_spec(new_defroll);
        else rem_hp -= chally_spec(9156);
        //assume 3 challies are on regular defense and 3 challies are on reduced.
    }
    for(int i{}; i < 12; ++i) { //3 players * 4 scythes
        rem_hp -= scythe(new_defroll);
    }
    hp1 = rem_hp;

    //phase 2 - begin checking for dead bloat
    for(int i{}; i < 2; ++i) { //2 players * 1 claw spec
        rem_hp -= claw_spec(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    for(int i{}; i < 16; ++i) { //2 players * 5 scythe + 1 player * 6 scythe
        rem_hp -= scythe(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    hp2 = rem_hp;

    //finish bloat if it's not already dead
    if(rem_hp > 0) htk = current_hit;
    while(rem_hp > 0) {
        rem_hp -= scythe(9156);
        ++htk;
    }
    return 0;
}

/* 2x bloat_prebgs, second round of challies on first down are on restored defense */
int bloat_2prebgs_rdefense(int &htk, int &hp1, int &hp2) {
    int rem_hp{BLOAT_HP};
    int bgs_hits{}, current_hit{18}, new_defroll_temp{}, new_defroll{};
    bool dead_flag{false};
    //phase 1
    rem_hp -= (bgs_hits += bgs(9156))/2;
    new_defroll_temp = (109-bgs_hits)*84;
    rem_hp -= (bgs_hits += bgs(new_defroll_temp))/2;
    new_defroll = (9+std::max(0,100-bgs_hits))*84;
    //phase 1
    for(int i{}; i < 6; ++i) { //2 challies * 3 players
        if (i < 3) rem_hp -= chally_spec(new_defroll);
        else rem_hp -= chally_spec(9156);
        //3 challies are on regular defense and 3 challies are on reduced.
    }
    for(int i{}; i < 12; ++i) { //3 players * 4 scythes
        rem_hp -= scythe(new_defroll);
    }
    hp1 = rem_hp;

    //phase 2 - begin checking for dead bloat
    for(int i{}; i < 1; ++i) { //1 claw spec, since 2 bgsed (sadge loop but keeping for readability. gcc pls forgive)
        rem_hp -= claw_spec(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    for(int i{}; i < 17; ++i) { //2 players * 5 scythe + 2 players * 6 scythe
        rem_hp -= scythe(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    hp2 = rem_hp;

    //finish bloat if it's not already dead
    if(rem_hp > 0) htk = current_hit;
    while(rem_hp > 0) {
        rem_hp -= scythe(9156);
        ++htk;
    }
    return 0;
}

/* same thing as bloat_prebgs, but the bgs'er misses a scythe hit on first down to not die*/
int bloat_prebgs_miss(int &htk, int &hp1, int &hp2) {
    int rem_hp{BLOAT_HP};
    int bgs_hits{}, current_hit{17}, new_defroll{}; //assumes no 1down
    bool dead_flag{false};
    //phase 1
    rem_hp -= (bgs_hits += bgs(9156))/2; //prebgs has damage halved
    new_defroll = (109-bgs_hits)*84; //can't hit a 100 bgs anyway
    for(int i{}; i < 6; ++i) { //2 challies * 3 players
        rem_hp -= chally_spec(new_defroll); //assume both challies are on reduced defense - not sure how this works.
        //it could be that 3 challies are on regular defense and 3 challies are on reduced.
    }
    for(int i{}; i < 11; ++i) { //2 players * 4 scythes + 1 player * 3 scythes
        rem_hp -= scythe(new_defroll);
    }
    hp1 = rem_hp;

    //phase 2 - begin checking for dead bloat
    for(int i{}; i < 2; ++i) { //2 players * 1 claw spec
        rem_hp -= claw_spec(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    for(int i{}; i < 16; ++i) { //2 players * 5 scythe + 1 player * 6 scythe
        rem_hp -= scythe(9156);
        ++current_hit;
        if (!dead_flag and rem_hp <= 0) {
            htk = current_hit;
            dead_flag = true;
        }
    }
    hp2 = rem_hp;

    //finish bloat if it's not already dead
    if(rem_hp > 0) htk = current_hit;
    while(rem_hp > 0) {
        rem_hp -= scythe(9156);
        ++htk;
    }
    return 0;
}
