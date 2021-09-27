//
// Created by huom on 4/9/21.
//

#include "duel_simulator.h"
#include "weapons.h"
#include <stdio.h>
#include <omp.h>

void simulate_duel(int (*func) (), int trials) {
    double p1_wins{};
    #pragma omp parallel for reduction(+:p1_wins)
    for (int i = 0; i < trials; ++i) {
        p1_wins += func();
    }
    printf("Player 1 Win Probability: %lf\n", p1_wins/trials);
}

// mace vs whip, both on accurate, max melees
// player 0 uses whip, player 1 uses mace
// returns winning player (1 for macer)
int mace_vs_whip_acc() {
    int p0hp{99}, p1hp{99}, poison_tick{-1}, tick_count{0}, temp{};
    // poison_tick mechanism: ticks before macer (player 1) is poisoned. poison hits when it hits 0
    // it starts negative and therefore won't hit 0 until it's set to 0

    // initial hit goes to macer if he has pid, otherwise we continue as normal and assume whipper gets first hit
    if (da_pid())
        p0hp -= da_mace();

    while (true) {
        if (--poison_tick == 0) { // assume poison hits first in a tick regardless of pid (not sure if this is true)
            p1hp -= 4; // macer gets poison damage
            poison_tick = 30; // and poison timer gets reset
        }

        if (tick_count % 4 == 0) {
            temp = da_whip();
            // if da_whip determines a poison hit and the target isn't already poisoned
            poison_tick = ((1 << 31) & temp) && poison_tick < 0 ? 30 : poison_tick;
            // clear MSB for actual value
            p1hp -= (temp & ~(1 << 31));

            p0hp -= da_mace();
        }

        // check for deaths at the end.
        // this should work because player 0 hits first in the loop, so even if both players die this hit
        // we assume that p0 wins, by checking for p1 dying first.
        if (p1hp <= 0)
            return 0;
        else if (p0hp <= 0)
            return 1;

        ++tick_count;
    }
}

// mace vs whip, macer brings ham/swift blade which is used if he does not have PID
int mace_and_ham_vs_whip_acc() {
    int p0hp{99}, p1hp{99}, poison_tick{-1}, tick_count{0}, temp{};
    // poison_tick mechanism: ticks before macer (player 1) is poisoned. poison hits when it hits 0
    // it starts negative and therefore won't hit 0 until it's set to 0

    // initial hit goes to macer if he has pid, otherwise we continue as normal and assume whipper gets first hit
    if (da_pid())
        p0hp -= da_mace();
    else
        p0hp -= da_box(); // ham instead since off pid

    while (true) {
        if (--poison_tick == 0) { // assume poison hits first in a tick regardless of pid (not sure if this is true)
            p1hp -= 4; // macer gets poison damage
            poison_tick = 30; // and poison timer gets reset
        }

        if (tick_count % 4 == 0) {
            temp = da_whip();
            // if da_whip determines a poison hit and the target isn't already poisoned
            poison_tick = ((1 << 31) & temp) && poison_tick < 0 ? 30 : poison_tick;
            // clear MSB for actual value
            p1hp -= (temp & ~(1 << 31));

            p0hp -= da_mace();
        }

        // check for deaths at the end.
        // this should work because player 0 hits first in the loop, so even if both players die this hit
        // we assume that p0 wins, by checking for p1 dying first.
        if (p1hp <= 0)
            return 0;
        else if (p0hp <= 0)
            return 1;

        ++tick_count;
    }
}

int mace_vs_whip_acc_debug() {
    int p0hp{99}, p1hp{99}, poison_tick{-1}, tick_count{0}, temp{}, dmg{};
    // poison_tick mechanism: ticks before macer (player 1) is poisoned. poison hits when it hits 0
    // it starts negative and therefore won't hit 0 until it's set to 0

    // initial hit goes to macer if he has pid, otherwise we continue as normal and assume whipper gets first hit
    if (da_pid()) {
        printf("Macer has PID.\n");
        dmg = da_mace();
        printf("TICK 0: Macer hit Whipper for %d. Whipper HP: %d\n", dmg, p0hp -= dmg);
    }
    else
        printf("Whipper has PID.\n");

    while (true) {
        if (--poison_tick == 0) { // assume poison hits first in a tick regardless of pid (not sure if this is true)
            p1hp -= 4;
            printf("TICK %d: Macer took 4 poison damage. Macer HP: %d\n", tick_count, p1hp -= 4);
            poison_tick = 30;
        }

        if (tick_count % 4 == 0) {
            temp = da_whip();
            dmg = temp & ~(1 << 31);
            // if da_whip determines a poison hit and the target isn't already poisoned
            printf("TICK %d: Whipper hit Macer for %d. Macer HP: %d\n", tick_count, dmg, p1hp -= dmg);
            if (((1 << 31) & temp) && poison_tick < 0) {
                printf("TICK %d: Macer was poisoned.\n", tick_count);
                poison_tick = 30;
            }

            dmg = da_mace();
            printf("TICK %d: Macer hit Whipper for %d. Whipper HP: %d\n", tick_count, dmg, p0hp -= dmg);
        }

        // check for deaths at the end.
        // this should work because player 0 hits first in the loop, so even if both players die this hit
        // we assume that p0 wins, by checking for p1 dying first.
        if (p1hp <= 0)
            return 0;
        else if (p0hp <= 0)
            return 1;

        ++tick_count;
    }
}