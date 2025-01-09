/// \file 2015Day21.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-21.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <map>
#include <regex>
#include <list>

const int MY_HIT_POINTS = 100;
const int NUM_WEAPONS = 5;
const int NUM_ARMORS = 6;
const int NUM_RINGS = 7;
const int WEAPON_COSTS[] = {8, 10, 25, 40, 74};
const int WEAPON_DAMAGES[] = {4, 5, 6, 7, 8};
const int ARMOR_COSTS[] = {0, 13, 31, 53, 75, 102};
const int ARMOR_ARMORS[] = {0, 1, 2, 3, 4, 5};
const int RING_COSTS[] = {0, 25, 50, 100, 20, 40, 80};
const int RING_DAMAGES[] = {0, 1, 2, 3, 0, 0, 0};
const int RING_ARMORS[] = {0, 0, 0, 0, 1, 2, 3};

struct Character
{
  int hitPoints;
  int damage;
  int armor;
};

struct Equipment
{
  int weapon;
  int armor;
  int ring1;
  int ring2;
  int cost;
};

Character readInput ()
{
  Character boss;
  std::string word;
  std::cin >> word;
  assert (word == "Hit");
  std::cin >> word;
  assert (word == "Points:");
  std::cin >> boss.hitPoints;
  assert (boss.hitPoints > 0);
  std::cin >> word;
  assert (word == "Damage:");
  std::cin >> boss.damage;
  assert (boss.damage > 0);
  std::cin >> word;
  assert (word == "Armor:");
  std::cin >> boss.armor;
  assert (boss.armor > 0);
  return boss;
}

bool
wins (const Character& me, const Character& boss)
{
  int myHP = me.hitPoints;
  int bossHP = boss.hitPoints;
  while (myHP > 0 && bossHP > 0) {
    bossHP -= std::max (1, me.damage - boss.armor);
    if (bossHP > 0) {
      myHP -= std::max (1, boss.damage - me.armor);
    }
  }
  return myHP > 0;
}

bool
wins (const Equipment& me, const Character& boss)
{
  int myHP = MY_HIT_POINTS;
  int myDamage = WEAPON_DAMAGES[me.weapon] + RING_DAMAGES[me.ring1] + RING_DAMAGES[me.ring2];
  int myArmor = ARMOR_ARMORS[me.armor] + RING_ARMORS[me.ring1] + RING_ARMORS[me.ring2];
  return wins (Character {myHP, myDamage, myArmor}, boss);
}

int
bruteForceMinWinningCost (const Character& boss)
{
  Equipment cheapest = {0, 0, 0, 0, INT32_MAX};
  for (int weapon = 0; weapon < NUM_WEAPONS; ++weapon) {
    for (int armor = 0; armor < NUM_ARMORS; ++armor) {
      for (int ring1 = 0; ring1 < NUM_RINGS; ++ring1) {
        for (int ring2 = ring1; ring2 < NUM_RINGS; ++ring2) {
          if (ring2 > ring1 || (ring1 == 0 && ring2 == 0)) {
            int cost = WEAPON_COSTS[weapon] + ARMOR_COSTS[armor] + RING_COSTS[ring1] + RING_COSTS[ring2];
            if (cost < cheapest.cost && wins ({weapon, armor, ring1, ring2, cost}, boss)) {
              cheapest = {weapon, armor, ring1, ring2, cost};
            }
          }
        }
      }
    }
  }
  return cheapest.cost;
}

int
bruteForceMaxLosingCost (const Character& boss)
{
  Equipment costliest = {0, 0, 0, 0, 0};
  for (int weapon = 0; weapon < NUM_WEAPONS; ++weapon) {
    for (int armor = 0; armor < NUM_ARMORS; ++armor) {
      for (int ring1 = 0; ring1 < NUM_RINGS; ++ring1) {
        for (int ring2 = ring1; ring2 < NUM_RINGS; ++ring2) {
          if (ring2 > ring1 || (ring1 == 0 && ring2 == 0)) {
            int cost = WEAPON_COSTS[weapon] + ARMOR_COSTS[armor] + RING_COSTS[ring1] + RING_COSTS[ring2];
            if (cost > costliest.cost && !wins ({weapon, armor, ring1, ring2, cost}, boss)) {
              costliest = {weapon, armor, ring1, ring2, cost};
            }
          }
        }
      }
    }
  }
  return costliest.cost;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Character boss = readInput ();
  std::cout << bruteForceMinWinningCost (boss) << "\n";
  std::cout << bruteForceMaxLosingCost (boss) << "\n";
  return 0;
}

// 203 was too high