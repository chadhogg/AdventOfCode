/// \file 2022Day02.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-02.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

const char OPPONENT_ROCK = 'A';
const char OPPONENT_PAPER = 'B';
const char OPPONENT_SCISSORS = 'C';

const char ME_ROCK = 'X';
const char ME_PAPER = 'Y';
const char ME_SCISSORS = 'Z';

const int SCORE_FOR_ROCK = 1;
const int SCORE_FOR_PAPER = 2;
const int SCORE_FOR_SCISSORS = 3;
const int SCORE_FOR_LOSS = 0;
const int SCORE_FOR_DRAW = 3;
const int SCORE_FOR_WIN = 6;

const int TRY_LOSE = 'X';
const int TRY_DRAW = 'Y';
const int TRY_WIN = 'Z';

using StrategyGuide = std::vector<std::pair<char, char>>;

StrategyGuide
getInput ()
{
  StrategyGuide guide;
  char x, y;
  while (std::cin)
  {
    std::cin >> x >> y;
    assert (x >= OPPONENT_ROCK && x <= OPPONENT_SCISSORS);
    assert (y >= ME_ROCK && y <= ME_SCISSORS);
    guide.push_back ({x, y});
  }
  guide.pop_back ();
  std::cout << guide.size () << "\n";
  return guide;
}

int
computeScore (const StrategyGuide& guide)
{
  int total = 0;
  for (const std::pair<char, char>& game : guide)
  {
    int score = 0;
    if (game.second == ME_ROCK) { score += SCORE_FOR_ROCK; }
    else if (game.second == ME_PAPER) { score += SCORE_FOR_PAPER; }
    else if (game.second == ME_SCISSORS) { score += SCORE_FOR_SCISSORS; }
    else { assert (false); }

    // I should use some fancy modular arithemtic here.  But why bother when the second part is likely to change the rules?
    if (game.first == game.second - (ME_ROCK - OPPONENT_ROCK)) { score += SCORE_FOR_DRAW; }
    else if (game.first == OPPONENT_ROCK && game.second == ME_SCISSORS) { score += SCORE_FOR_LOSS; }
    else if (game.first == OPPONENT_SCISSORS && game.second == ME_ROCK) { score += SCORE_FOR_WIN; }
    else if (game.first < game.second - (ME_ROCK - OPPONENT_ROCK)) { score += SCORE_FOR_WIN; }
    else if (game.first > game.second - (ME_ROCK - OPPONENT_ROCK)) { score += SCORE_FOR_LOSS; }
    else { assert (false); }
    //std::cout << game.first << " " << game.second << " " << score << "\n";
    total += score;
  }
  return total;
}

int
computeScore2 (const StrategyGuide& guide)
{
  int total = 0;
  for (const std::pair<char, char>& game : guide)
  {
    int score = 0;

    if (game.second == TRY_LOSE)
    {
      if (game.first == OPPONENT_ROCK) { score += SCORE_FOR_SCISSORS; }
      else if (game.first == OPPONENT_PAPER) { score += SCORE_FOR_ROCK; }
      else if (game.first == OPPONENT_SCISSORS) { score += SCORE_FOR_PAPER; }
      else { assert (false); }
      score += SCORE_FOR_LOSS;
    }
    else if (game.second == TRY_DRAW)
    {
      if (game.first == OPPONENT_ROCK) { score += SCORE_FOR_ROCK; }
      else if (game.first == OPPONENT_PAPER) { score += SCORE_FOR_PAPER; }
      else if (game.first == OPPONENT_SCISSORS) { score += SCORE_FOR_SCISSORS; }
      else { assert (false); }
      score += SCORE_FOR_DRAW;
    }
    else if (game.second == TRY_WIN)
    {
      if (game.first == OPPONENT_ROCK) { score += SCORE_FOR_PAPER; }
      else if (game.first == OPPONENT_PAPER) { score += SCORE_FOR_SCISSORS; }
      else if (game.first == OPPONENT_SCISSORS) { score += SCORE_FOR_ROCK; }
      else { assert (false); }
      score += SCORE_FOR_WIN;
    }
    else
    {
      assert (false);
    }
    
    total += score;

  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  StrategyGuide guide = getInput ();
  std::cout << computeScore (guide) << "\n";
  std::cout << computeScore2 (guide) << "\n";
  return 0;
}
