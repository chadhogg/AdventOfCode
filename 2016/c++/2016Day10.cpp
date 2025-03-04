/// \file 2016Day10.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-10.

#include <iostream>
#include <string>
#include <cassert>
#include <regex>
#include <map>
#include <vector>

using Chip = int;
using BotNum = int;

const Chip NONE = -1;
const std::string BOT = "bot";
const std::string OUTPUT = "output";

struct Bot {
  BotNum number;
  Chip first;
  Chip second;
};

struct Instruction {
  BotNum number;
  std::string lowerType;
  BotNum lowerNum;
  std::string higherType;
  BotNum higherNum;
};

struct Problem {
  std::map<BotNum, Instruction> botInstructions;
  std::map<BotNum, Bot> activeBots;
  std::map<BotNum, Bot> finishedBots;
  std::map<BotNum, std::vector<Chip>> outputs;
};

void
giveToRobot (Problem & prob, BotNum botNum, Chip chip) {
  assert (!prob.finishedBots.contains (botNum));
  if (prob.activeBots.contains (botNum)) {
    assert (prob.activeBots[botNum].first != NONE);
    assert (prob.activeBots[botNum].second == NONE);
    prob.activeBots[botNum].second = chip;
  }
  else {
    prob.activeBots[botNum] = {botNum, chip, NONE};
  }
}

Problem
readInput ()
{
  Problem prob;
  std::basic_regex valueRegex ("^value (\\d+) goes to bot (\\d+)$");
  std::basic_regex actionRegex ("^bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)");
  std::string line;
  while (std::getline (std::cin, line)) {
    std::smatch smatch;
    if (std::regex_match (line, smatch, valueRegex)) {
      BotNum botNum = atoi (smatch[2].str ().c_str ());
      Chip chip = atoi (smatch[1].str ().c_str ());
      giveToRobot (prob, botNum, chip);
    }
    else if (std::regex_match (line, smatch, actionRegex)) {
      BotNum botNum = atoi (smatch[1].str ().c_str ());
      std::string lowType = smatch[2].str ();
      BotNum lowNum = atoi (smatch[3].str ().c_str ());
      std::string highType = smatch[4].str ();
      BotNum highNum = atoi (smatch[5].str ().c_str ());
      assert (!prob.botInstructions.contains (botNum));
      prob.botInstructions[botNum] = {botNum, lowType, lowNum, highType, highNum};
    }
    else {
      assert (false);
    }
  }
  return prob;
}


Problem
simulate (Problem prob) {
  while (!prob.activeBots.empty()) {
    BotNum botNum = NONE;
    for (auto iter = prob.activeBots.begin (); iter != prob.activeBots.end () && botNum == NONE; ++iter) {
      if (iter->second.first != NONE && iter->second.second != NONE) {
        botNum = iter->first;
      }
    }
    assert (botNum != NONE);
    Bot active = prob.activeBots[botNum];
    prob.activeBots.erase (botNum);
    prob.finishedBots[botNum] = active;
    Chip lower = std::min (active.first, active.second);
    Chip higher = std::max (active.first, active.second);
    if (prob.botInstructions[botNum].lowerType == BOT) {
      giveToRobot (prob, prob.botInstructions[botNum].lowerNum, lower);
    }
    else {
      assert (prob.botInstructions[botNum].lowerType == OUTPUT);
      if (!prob.outputs.contains (prob.botInstructions[botNum].lowerNum)) {
        prob.outputs[prob.botInstructions[botNum].lowerNum] = {};
      }
      prob.outputs[prob.botInstructions[botNum].lowerNum].push_back (lower);
    }
    if (prob.botInstructions[botNum].higherType == BOT) {
      giveToRobot (prob, prob.botInstructions[botNum].higherNum, higher);
    }
    else {
      assert (prob.botInstructions[botNum].higherType == OUTPUT);
      if (!prob.outputs.contains (prob.botInstructions[botNum].higherNum)) {
        prob.outputs[prob.botInstructions[botNum].higherNum] = {};
      }
      prob.outputs[prob.botInstructions[botNum].higherNum].push_back (higher);
    }
  }
  return prob;
}

BotNum
findBot (const Problem& prob, Chip a, Chip b) {
  for (auto iter = prob.finishedBots.cbegin (); iter != prob.finishedBots.cend (); ++iter) {
    if (iter->second.first == a && iter->second.second == b) { return iter->first; }
    if (iter->second.second == a && iter->second.first == b) { return iter->first; }
  }
  assert (false);
  return NONE;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Problem prob = readInput ();
  Problem finished = simulate (prob);
  std::cout << findBot (finished, 61, 17) << "\n";
  std::cout << finished.outputs[0].front () * finished.outputs[1].front () * finished.outputs[2].front () << "\n";
  return 0;
}

