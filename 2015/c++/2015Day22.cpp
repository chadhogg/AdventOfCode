/// \file 2015Day22.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-22.

#include <iostream>
#include <string>
#include <cassert>
#include <set>


struct Character
{
  int hitPoints;
  int damage;
  int armor;
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
  boss.armor = 0;
  return boss;
}

const int STARTING_HP = 50;
const int STARTING_MANA = 500;
const int MAGIC_MISSILE_COST = 53;
const int MAGIC_MISSILE_DAMAGE = 4;
const int DRAIN_COST = 73;
const int DRAIN_DAMAGE = 2;
const int DRAIN_HEALING = 2;
const int SHIELD_COST = 113;
const int SHIELD_TURNS = 6;
const int SHIELD_ARMOR = 7;
const int POISON_COST = 173;
const int POISON_TURNS = 6;
const int POISON_DAMAGE = 3;
const int RECHARGE_COST = 229;
const int RECHARGE_TURNS = 5;
const int RECHARGE_MANA = 101;
constexpr int MIN_SPELL_COST = std::min ({MAGIC_MISSILE_COST, DRAIN_COST, SHIELD_COST, POISON_COST, RECHARGE_COST});

struct State
{
  int myHP;
  int bossHP;
  int turnsTaken;
  int currentMana;
  int shieldLength;
  int poisonLength;
  int rechargeLength;
  bool myTurn;

  auto operator<=> (const State& s) const = default;
};

std::ostream&
operator<< (std::ostream& out, const State& state)
{
  out << "{" << state.myHP << ", " << state.bossHP << ", " << state.turnsTaken << ", " << state.currentMana << ", " << state.shieldLength << ", " << state.poisonLength << ", " << state.rechargeLength << ", " << state.myTurn << "}";
  return out;
}

struct StateWithUsedMana
{
  int usedMana;
  State state;

  auto operator<=> (const StateWithUsedMana& s) const = default;
};

std::ostream&
operator<< (std::ostream& out, const StateWithUsedMana& swum)
{
  out << swum.usedMana << ": " << swum.state;
  return out;
}

StateWithUsedMana
afterMagicMissile (const StateWithUsedMana& current)
{
  assert (current.state.currentMana >= MAGIC_MISSILE_COST);
  assert (current.state.myTurn);
  return {current.usedMana + MAGIC_MISSILE_COST,
    {current.state.myHP,
    current.state.bossHP - MAGIC_MISSILE_DAMAGE,
    current.state.turnsTaken + 1,
    current.state.currentMana - MAGIC_MISSILE_COST,
    current.state.shieldLength,
    current.state.poisonLength,
    current.state.rechargeLength,
    false}
  };
}

StateWithUsedMana
afterDrain (const StateWithUsedMana& current)
{
  assert (current.state.currentMana >= DRAIN_COST);
  assert (current.state.myTurn);
  return {current.usedMana + DRAIN_COST,
    {current.state.myHP + DRAIN_HEALING,
    current.state.bossHP - DRAIN_DAMAGE,
    current.state.turnsTaken + 1,
    current.state.currentMana - DRAIN_COST,
    current.state.shieldLength,
    current.state.poisonLength,
    current.state.rechargeLength,
    false}
  };
}

StateWithUsedMana
afterShield (const StateWithUsedMana& current)
{
  assert (current.state.currentMana >= SHIELD_COST);
  assert (current.state.myTurn);
  assert (current.state.shieldLength == 0);
  return {current.usedMana + SHIELD_COST,
    {current.state.myHP,
    current.state.bossHP,
    current.state.turnsTaken + 1,
    current.state.currentMana - SHIELD_COST,
    SHIELD_TURNS,
    current.state.poisonLength,
    current.state.rechargeLength}
  };
}

StateWithUsedMana
afterPoison (const StateWithUsedMana& current)
{
  assert (current.state.currentMana >= POISON_COST);
  assert (current.state.myTurn);
  assert (current.state.poisonLength == 0);
  return {current.usedMana + POISON_COST,
    {current.state.myHP,
    current.state.bossHP,
    current.state.turnsTaken + 1,
    current.state.currentMana - POISON_COST,
    current.state.shieldLength,
    POISON_TURNS,
    current.state.rechargeLength}
  };
}

StateWithUsedMana
afterRecharge (const StateWithUsedMana& current)
{
  assert (current.state.currentMana >= RECHARGE_COST);
  assert (current.state.myTurn);
  assert (current.state.rechargeLength == 0);
  return {current.usedMana + RECHARGE_COST,
    {current.state.myHP,
    current.state.bossHP,
    current.state.turnsTaken + 1,
    current.state.currentMana - RECHARGE_COST,
    current.state.shieldLength,
    current.state.poisonLength,
    RECHARGE_TURNS}
  };
}

StateWithUsedMana
findCheapest (const Character& boss, bool hardMode)
{
  std::set<StateWithUsedMana> frontier;
  frontier.insert ({0, {STARTING_HP, boss.hitPoints, 0, STARTING_MANA, 0, 0, 0, true}});
  while (!frontier.empty ()) {
    StateWithUsedMana current = *frontier.begin ();
    frontier.erase (current);
//    std::cout << "Considering " << current << "\n";
    if (hardMode && current.state.myTurn) { --current.state.myHP; }
    int yourArmor = (current.state.shieldLength > 0 ? SHIELD_ARMOR : 0);
    current.state.shieldLength = std::max (0, current.state.shieldLength - 1);
    if (current.state.poisonLength > 0) {
      current.state.bossHP -= POISON_DAMAGE;
      --current.state.poisonLength;
    }
    if (current.state.rechargeLength > 0) {
      current.state.currentMana += RECHARGE_MANA;
      --current.state.rechargeLength;
    }
    if (current.state.myHP <= 0) { continue; }
    else if (current.state.bossHP <= 0) { return current; }

    if (current.state.myTurn) {
      if (current.state.currentMana < MIN_SPELL_COST) { continue; }
      if (current.state.currentMana >= MAGIC_MISSILE_COST) {
        frontier.insert (afterMagicMissile (current));
      }
      if (current.state.currentMana >= DRAIN_COST) {
        frontier.insert (afterDrain (current));
      }
      if (current.state.currentMana >= SHIELD_COST && current.state.shieldLength == 0) {
        frontier.insert (afterShield (current));
      }
      if (current.state.currentMana >= POISON_COST && current.state.poisonLength == 0) {
        frontier.insert (afterPoison (current));
      }
      if (current.state.currentMana >= RECHARGE_COST && current.state.rechargeLength == 0) {
        frontier.insert (afterRecharge (current));
      }
    }
    else {
      current.state.myHP -= std::max (1, boss.damage - yourArmor);
      ++current.state.turnsTaken;
      current.state.myTurn = true;
      frontier.insert (current);
    }
  }
  throw "Impossible";
}


/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Character boss = readInput ();
  std::cout << findCheapest (boss, false).usedMana << "\n";
  std::cout << findCheapest (boss, true).usedMana << "\n";
  return 0;
}
