//
// Created by huom on 9/6/20.
//

#include "weapons.h"
#include <random>
#include <iostream>
#include <cmath>

//static int p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
static int rng(int a, int b) { //generate random int [a, b]
    thread_local std::random_device rd;  //Will be used to obtain a seed for the random number engine
    thread_local std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> uni(a, b);
    return uni(gen);
}

int claw_spec(int defroll) {
    int rolls_needed{};
    do {
        ++rolls_needed;
    } while (rng(0, defroll) >= rng(0, 25568));
    switch (rolls_needed) {
        case 1:
            //if(!p1++) std::cout << "p1\n";
            return 2 * rng(24, 48) + 1;
        case 2:
            //if(!p2++) std::cout << "p2\n";
            return rng(36, 84) + 1;
        case 3:
            //if(!p3++) std::cout << "p3\n";
            return rng(24, 72) + 1;
        case 4:
            //if(!p4++) std::cout << "p4\n";
            return rng(12, 60);
        default:
            //if(!p5++) std::cout << "p5\n";
            return 2 * rng(0, 1);
    }
}

int chally_spec(int defroll) {
    int total_damage{};
    for (int i{}; i < 2; ++i) {
        if (rng(0, 26283) > rng(0, defroll))
            total_damage += rng(0, 73);
    }
    return total_damage;
}

int scythe(int defroll) {
    int total_damage{};
    for (int i{0}; i < 3; ++i) {
        if (rng(0, 35044) > rng(0, defroll))
            total_damage += rng(0, 54 / static_cast<int>(pow(2, i)));
    }
    return total_damage;
}

int bgs(int defroll) {
    if (rng(0, 77956) > rng(0, defroll))
        return rng(0, 84);
    return 0;
}

// decides who has pid
int da_pid() {
    return rng(0, 1);
}
// default assumes both are on accurate, so no defense bonus
// ignore most significant bit for the hit value; if the most significant bit is set, then poison is inflicted
int da_whip(int defroll) {
    if (rng(0, 16940) > rng(0, defroll))
        return rng(0, 25) | (!rng(0,3) << 31); // set MSB for 1/4 chance (poison)
    return 0;
}

// default assumes both are on accurate, so no defense bonus
int da_mace(int defroll) {
    if (rng(0, 17490) > rng(0, defroll))
        return rng(0, 26);
    return 0;
}

int da_box(int defroll) {
    if (rng(0, 7040) > rng(0, defroll))
        return rng(0, 11);
    return 0;
}

