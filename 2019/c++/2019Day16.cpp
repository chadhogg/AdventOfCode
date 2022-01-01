/// \file 2019Day16.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/16.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>
#include <unordered_map>

#include "utilities.hpp"

/* A reddit post I wrote about this problem.

I completed AoC 2020 in December 2020 and AoC 2021 in December 2021, and because I just couldn't get enough and because I've heard so much about IntCode that I wanted to see it for myself, I started AoC 2019 the day after Christmas.  It's all been fairly smooth sailing until Day 16, Part 2, where I hit a bigger brick wall than I remember in either of the more recent years.  After about 24 hours of banging my head against it, I finally have a working solution.  I thought it would be worth documenting, for others who come along behind me, and so that other people can tell me where I could have saved some time and frustration.  In case it's not obvious, the entire rest of this post is a spoiler.

## Introduction

If you don't recall, 2019 Day 16 is the Flawed Frequency Transmission protocol.  We are given a digit sequence and need to send it through 100 phases.  In each phase, each digit is replaced by a new one that is computed by summing all of the existing digits multiplied by either 0, -1, 0, or 1.  Specifically, when computing the new value for the first digit you use the repeating pattern <1, 0, -1, 0> for the multipliers.  For the second digit you use <0, 1, 1, 0, 0, -1, -1, 0>.  For the third digit you use <0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1, 0>, and so forth.  The result of each phase is still a digit sequence, so we discard everything except the final digit of a sum.

In part 1 we are given a relatively short digit sequence to which we are to apply this process (mine was 651 characters long).  And a straightforward implementation of the described procedure runs through the 100 phases quite quickly to show you the final output.  In part 2 you are given a sequence that is 10,000 times longer, and an offset into it of the short segment you are required to type in as your answer.  Now that quadratic algorithm isn't looking so good.

After a few hours of making no real progress, I searched reddit for a hint.  What I discovered was lots of people saying to note that the offset you are given is always somewhere in the second half of the sequence, and that this would make your life a lot easier.  [Aside: I'm one of those people who hates puzzles that want you to write code that only works for your input and not any legal input.  Moving on.]

## Discovery

So I started looking at what happens in the back half of a sequence.  In particular, I started at the very end.  In the analysis below, w, x, y, and z refer to the last 4 elements of the sequence, while z(4) refers to the new value of z after 4 phases.  The key thing that makes the second half interesting is that the repeating pattern is going to contain all 0s up to your own position, then all 1s from there on out.  So I set up some equations for how to calculate the last few elements after a single phase:

- w(1) = w + x + y + z
- x(1) =     x + y + z
- y(1) =         y + z
- z(1) =             z

That's interesting, and simple enough.  What about if we go to two phases?

- w(2) = w(1) + x(1) + y(1) + z(1) = w + 2x + 3y + 4z
- x(2) =        x(1) + y(1) + z(1) =      x + 2y + 3z
- y(2) =               y(1) + z(1) =           y + 2z
- z(2) =                      z(1) =                z

That also looks like a pretty regular pattern, but I don't yet see how to generalize it to any number of phases yet.  Let's do 3 phases:

- w(3) = w(2) + x(2) + y(2) + z(2) = w + 3x + 6y + 10z
- x(3) =        x(2) + y(2) + z(2) =      x + 3y +  6z
- y(3) =               y(2) + z(2) =           y +  3z
- z(3) =                      z(2) =                 z

And 4 phases:

- w(4) = w(3) + x(3) + y(3) + z(3) = w + 4x + 10y + 20z
- x(4) =        x(3) + y(3) + z(3) =      x +  4y + 10z
- y(4) =               y(3) + z(3) =            y +  4z
- z(4) =                      z(4) =                  z

The problem gives us a sample input of size 8 and shows us the results after 1, 2, 3, and 4 phases.  We can use that to confirm that all of the formulas above are correct (not bothering to compute the first half of the output).  Note that our actual answer is the result of these formulas modulus 10.  Since the only operations in these formulas are addition and multiplication it doesn't matter that we are saving the modulus until the end -- we would get the same answer if we did it in each step.  I still didn't see a general formula, so I did 5 phases:

- w(5) = w(4) + x(4) + y(4) + z(4) = w + 5x + 15y + 35z
- x(5) =        x(4) + y(4) + z(4) =      x +  5y + 15z
- y(5) =               y(4) + z(4) =            y +  5z
- z(5) =                      z(5) =                  z

And a sixth as well:

- w(6) = w(5) + x(5) + y(5) + z(5) = w + 6x + 21y + 56z
- x(6) =        x(5) + y(5) + z(5) =      x +  6y + 21z
- y(6) =               y(5) + z(5) =            y +  6z
- z(6) =                      z(5) =                  z

At this point we can start working on a general formula for any number of phases, p.  Starting with the easiest one:

- z(p) = z

The next one is pretty easy too:

- y(p) = y + pz

Next we have to think a bit.  If we look at the z coefficients in the various x(n) formulas, we see 1, 3, 6, 10, 15, 21.  I know that sequence!  It's the triangular numbers:

- x(p) = x + py + p(p+1)z/2

And now it gets really tricky.  The z coefficients for the various w(n) formulas are 1, 4, 10, 20, 35, 56.  That sequence did not look familiar to me.  But it did to Google, who pointed me to a wikipedia page on the tetrahedral numbers.  And that article had an interesting tidbit: "The pattern found for triangular numbers (n + 1 choose 2) and for tetrahedral numbers (n + 2 choose 3) can be generalized.  This leads to the formula (n + k - 1 choose k)."  I didn't have examples for the 5th-from-last digit in a sequence, but it seemed quite probable that it would use the formula (n + 3 choose 4) as its coefficient for z, and that the rest would continue in the same way.

## An idea that didn't work out

So I now had a completely general formula for finding the value of any second-half digit n after p phases:

- (p-1 choose 0) * nth digit + (p choose 1) * n+1th digit + (p+1 choose 2) * n+2th digit + (p+2 choose 3) * n+3th digit + (p+3 choose 4) * n+4th digit ... until you run out of digits.

So I wrote some code to accomplish that, started it, and ... got the wrong answer.  You see, when your sequence has 6,500,000 digits in it, the last term in that summation looks something like (p+3,250,000 choose 3,250,000).  I'm sorry to say that that's a very large number -- far too big to fit in any integral type.

Modular arithmetic to the rescue, right?!  If (n % 10 choose k % 10) % 10 is the same as (n choose k) % 10, then I don't need to compute any large numbers.  Unfortunately, even some basic example trying shows that this is not true.  And it makes sense that it wouldn't be, since the choose operation involves division.  I spent several hours trying to find some way to insert a modulus somewhere in the operation without affecting the result, without any success.  I found Lucas's Theorem that sounded promising initially, but only works for prime numbers.

## Another idea

I decided to look back at the information I had about tetrahedral numbers and noticed something that should have been obvious about them originally: the nth tetrahedral number is the sum of the first n triangular numbers, in the same way that the nth triangular number is the sum of the first n natural numbers, and the nth natural number is the sum n copies of 1.  By expanding the binomial coefficient formula I had discovered earlier for what I'm calling the meta-tetrahedral numbers I confirmed that the nth one of them is the sum of the first n tetrahedral numbers.

Why does this matter?  Because now all I am doing is addition.  And if all I am doing is addition, I *can* safely move my modulus 10 operation as deep into the calculation as necessary, which means no large numbers and no overflow.  Hooray!  This is going to require a lot of calculations, though.  And many of them will be repeated many times, so it will be worth memoizing any results.  I need to build a table like this, though I only store the values modulus 10:

|           | phase 1 | phase 2 | phase 3 | phase 4 | phase 5 | ... |
|-----------|---------|---------|---------|---------|---------|-----|
|always 1   |        1|        1|        1|        1|        1| ... |
|-----------|---------|---------|---------|---------|---------|-----|
|nat num    |        1|        2|        3|        4|        5| ... |
|-----------|---------|---------|---------|---------|---------|-----|
|triangular |        1|        3|        6|       10|       15| ... |
|-----------|---------|---------|---------|---------|---------|-----|
|tetrahedral|        1|        4|       10|       20|       35| ... |
|-----------|---------|---------|---------|---------|---------|-----|
|no name?   |        1|        5|       15|       35|       70| ... |
|-----------|---------|---------|---------|---------|---------|-----|
|...        |      ...|      ...|      ...|      ...|      ...| ... |

Other than the first row and column, any value in this table is just the sum of the values above and left of it, so adding another entry to a table that has already been filled up to it is trivial.  Unfortunately, it has 100 columns and *gulp* up to 3,250,000 rows.  My first attempt to memoize it failed because it used more than 16GB of memory.  I was able to cut that down dramatically by being smarter about how I stored the table.

To compute the value of the first element in the second half of the sequence after 100 phases, we just do 1st row 100th column * that element + 2nd row 100th column * next element + 3rd row 100th column * element after that ... until we run out of elements.

Once again, I tried this on the sample input, and was delighted to find that it worked.  So I ran it on my real input and found that it was still very, very slow.

## Re-evaluating life decisions

It was about this time that I realized I'd spent probably 12 hours optimizing a simple 100n^2 algorithm into a complicated n^2 algorithm.  Who cares about the 100 phases?  Eliminating the need to repeat a process 100 times was going to have a negligible impact on my runtime when that thing that I'm doing once instead of 100 times is still extremely expensive.

What I needed to do was to attack that n, and it was obvious how to.  I was still computing the final value of every element in the second half of the sequence even though I only needed to know 8 of them.  Once I change my outer loop to iterate over only the 8 elements whose values I cared about I had code that ran in less than 2 seconds, which is certainly good enough for me.

And just when I was feeling like I'd wasted an incredible amount of time I suddenly realized that I hadn't after all!  Because I have a formula for computing final values from initial values without generating all of the phases in between, it's safe for me to only compute final values for the 8 elements I care about.  If I didn't have that formula I would be calculating one phase at a time, and I *would* need intermediate values for all of the elements after the ones in question.  Huzzah!

Other than the long unproductive turn into trying to efficiently calculate binomial coefficients modulus 10, where could I have saved myself some time and energy here?
*/

using Sequence = std::vector<int>;

const Sequence BASE_PATTERN = {0, 1, 0, -1};

Sequence getPatternForIndex (unsigned int index) {
    static std::vector<Sequence> patterns;
    if (index < patterns.size ()) {
        return patterns.at (index);
    }
    assert (patterns.size () == index);
    Sequence pattern;
    for (int num : BASE_PATTERN) {
        for (unsigned int count {0U}; count <= index; ++count) {
            pattern.push_back (num);
        }
    }
    patterns.push_back (pattern);
    return pattern;
}

Sequence doPhase (Sequence input) {
    Sequence output;
    output.reserve (input.size ());
    for (unsigned int index {0U}; index < input.size (); ++index) {
        Sequence pattern = getPatternForIndex (index);
        unsigned int patternIndex = 1U;
        int sum {0};
        for (int num : input) {
            sum += num * pattern[patternIndex];
            patternIndex = (patternIndex + 1) % pattern.size ();
        }
        output.push_back (std::abs (sum) % 10);
    }
    return output;
}

Sequence doMultiplePhases (Sequence const& input, unsigned int toDo) {
    Sequence output = input;
    for (unsigned int count {0U}; count < toDo; ++count) {
        output = doPhase (output);
    }
    return output;
}

Sequence getInput () {
    std::string line = read<std::string> ();
    Sequence result;
    for (char c : line) {
        result.push_back (c - '0');
    }
    return result;
}

unsigned long sublist (Sequence const& sequence, unsigned int offset, unsigned int length) {
    unsigned long result = 0;
    for (unsigned int count {0U}; count < length; ++count) {
        result *= 10;
        result += sequence.at (offset + count);
    }
    return result;
}

std::string strlist (Sequence const& sequence, unsigned int offset, unsigned int length) {
    std::string str = "";
    for (unsigned int count {0U}; count < length; ++count) {
        str += std::to_string (sequence.at (offset + count));
    }
    return str;
}

Sequence expand (Sequence const& original, unsigned int duplications) {
    Sequence result;
    result.reserve (duplications * original.size ());
    for (unsigned int count {0U}; count < duplications; ++count) {
        for (int i : original) {
            result.push_back (i);
        }
    }
    return result;
}

using Type = int;
using Index = int;

constexpr long MIN_TYPE = -1;
constexpr long MAX_TYPE = 3250000;
constexpr long MIN_NUMBER = 1;
constexpr long MAX_NUMBER = 100;
constexpr char INVALID = 10;
constexpr long ENTRY_LENGTH = MAX_NUMBER - MIN_NUMBER + 1;

struct TriangularNumberLikeThingMemo {
    std::vector<char> answers;

    TriangularNumberLikeThingMemo () {
        answers.resize ((MAX_TYPE - MIN_TYPE + 1) * (MAX_NUMBER - MIN_NUMBER + 1), INVALID);
    }
    
    bool contains (Type type, Index number) const {
        return answers[type * ENTRY_LENGTH + number] != INVALID;
    }

    int get (Type type, Index number) const {
        return answers[type * ENTRY_LENGTH + number];
    }

    void set (Type type, Index number, char value) {
        answers[type * ENTRY_LENGTH + number] = value;
    }
};

int getTriangularNumberLikeThing (Type type, Index number) {
    static TriangularNumberLikeThingMemo memo;
    assert (number > 0);
    if (!memo.contains (type, number)) {
        if (type == -1) { memo.set (type, number, 1); }
        else if (type == 0) { memo.set (type, number, number % 10); }
        else if (type == 1) { memo.set (type, number, (number * (number + 1) / 2) % 10); }
        else if (number == 1) { memo.set (type, number, 1); }
        else { memo.set (type, number, (getTriangularNumberLikeThing (type, number - 1) + getTriangularNumberLikeThing (type - 1, number)) % 10); }
    }
    return memo.get (type, number);
}

void preCalculateTriangularNumberThings (int max_type, int max_number) {
    for (int i = 0; i <= max_type; ++i) {
        for (int j = 1; j <= max_number; ++j) {
            getTriangularNumberLikeThing (i, j);
        }
    }
}

std::string justGiveUsPartWeCareAbout (Sequence const& original, unsigned int phases, unsigned int startIndex, unsigned int length) {
    std::string result = "";
    //preCalculateTriangularNumberThings (original.size () / 2, phases);
    for (unsigned int index = startIndex; index < startIndex + length; ++index) {
        int sum {0};
        for (unsigned int offset = 0; index + offset < original.size (); ++offset) {
            sum = (sum + getTriangularNumberLikeThing (offset - 1, phases) * original[index + offset]) % 10;
        }
        result += ('0' + sum);
    }
    return result;
}


constexpr unsigned int PHASES = 100U;

int main () {
    Sequence input = getInput ();
    Sequence after100 = doMultiplePhases (input, PHASES);
    std::cout << sublist (after100, 0U, 8U) << "\n";
    Sequence expanded = expand (input, 10000U);
    std::cout << justGiveUsPartWeCareAbout (expanded, PHASES, sublist (input, 0U, 7U), 8U) << "\n";
    return 0;
}
