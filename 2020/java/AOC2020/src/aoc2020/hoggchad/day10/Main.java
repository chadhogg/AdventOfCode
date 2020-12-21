package aoc2020.hoggchad.day10;

import java.util.SortedSet;
import java.util.Stack;
import java.util.TreeSet;
import java.util.Scanner;
import java.util.Set;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent of Code 2020, day 10.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day10/input";
	
	/** The largest boost that an adaptor can provide. */
	private static final long MAX_DIFF = 3;
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		SortedSet<Long> adaptors = readInput();
		System.out.println(part1(adaptors));
		System.out.println(part2(adaptors));
	}

	/**
	 * Finds how many 1-difference and how many 3-difference gaps there are, and multiplies them together.
	 * 
	 * @param adaptors A sorted set of adaptor ratings.
	 * @return The product of the number of 1-differences and 3-differences.
	 */
	private static long part1(SortedSet<Long> adaptors) {
		long oneDifferences = 0;
		long twoDifferences = 0;
		long threeDifferences = 1;
		Iterator<Long> iter = adaptors.iterator();
		long previousNumber = 0;
		while(iter.hasNext()) {
			long currentNumber = iter.next();
			if(currentNumber - previousNumber == 1) {
				oneDifferences++;
			}
			else if(currentNumber - previousNumber == 2) {
				twoDifferences++;
			}
			else if(currentNumber - previousNumber == 3) {
				threeDifferences++;
			}
			else {
				throw new IllegalStateException("Found a difference not 1, 2, or 3.");
			}
			previousNumber = currentNumber;
		}
		return oneDifferences * threeDifferences;
	}
	
	/**
	 * Counts the number of legal ways to arrange adaptors to reach maximum power.
	 * 
	 * @param adaptors The sorted set of adaptor ratings.
	 * @return The answer to the combinatorics problem.
	 */
	private static long part2(SortedSet<Long> adaptors) {
		List<Long> numbers = new ArrayList<>();
		numbers.add(0L);
		numbers.addAll(adaptors);
		numbers.add(numbers.get(numbers.size() - 1) + MAX_DIFF);
		return countPossibilities(numbers);
	}
	
	/**
	 * Counts the number of possible subsequences that can be taken from a sequence without any differences higher than MAX_DIFF.
	 * A subsequence in this sense is a copy of the sequence in which 0 or more non-terminal elements have been removed.
	 * 
	 * @param numbers The original sequence.
	 * @return The number of legal subsequences.
	 */
	private static long countPossibilities(List<Long> numbers) {
		if(numbers.size() == 1) {
			// Since we cannot remove the first / last, there is one way through a sequence of 1 element.
			return 1;
		}
		else if(numbers.size() == 2) {
			if(numbers.get(1) - numbers.get(0) <= MAX_DIFF) {
				// Since we cannot remove the first / last, there is one way through a sequence of 2 elements not spaced too far apart.
				return 1;
			}
			else {
				throw new IllegalArgumentException("This is not supposed to be possible.");
			}
		}
		else {
			int middleIndex = numbers.size() / 2;
			List<Long> firstHalf = numbers.subList(0,  middleIndex + 1);
			List<Long> secondHalf = numbers.subList(middleIndex, numbers.size());
			long waysToGetToMiddle = countPossibilities(firstHalf);
			long waysToGetFromMiddle = countPossibilities(secondHalf);
			// Total possibilities that use middle element are all ways to get to it * all ways to get from it.
			long waysIncludingMiddle = waysToGetToMiddle * waysToGetFromMiddle;
			long waysWithoutMiddle;
			if(numbers.get(middleIndex + 1) - numbers.get(middleIndex - 1) <= MAX_DIFF) {
				List<Long> withoutMiddle = new ArrayList<>(firstHalf.subList(0, middleIndex));
				withoutMiddle.addAll(secondHalf.subList(1, secondHalf.size()));
				// What would it be if this element were not available?
				waysWithoutMiddle = countPossibilities(withoutMiddle);
			}
			else {
				// Can't skip middle because elements on either side are too far apart.
				waysWithoutMiddle = 0L;
			}
			return waysIncludingMiddle + waysWithoutMiddle;
		}		
	}

	/**
	 * What it might look like to solve part 2 with brute force.  As expected, takes far too long.
	 * 
	 * @param adaptors A sorted set of adaptor ratings.
	 * @return The number of possible ways to use them.
	 */
	private static long bruteForcePart2(SortedSet<Long> adaptors) {
		List<Long> numbers = new ArrayList<>();
		numbers.add(0L);
		numbers.addAll(adaptors);
		numbers.add(numbers.get(numbers.size() - 1) + MAX_DIFF);
		Stack<List<Integer>> incompleteIndexSequences = new Stack<>();
		Set<List<Integer>> completeIndexSequences = new HashSet<>();
		incompleteIndexSequences.add(List.of(0));
		while(!incompleteIndexSequences.isEmpty()) {
			List<Integer> partialSequence = incompleteIndexSequences.pop();
			int lastIndex = partialSequence.get(partialSequence.size() - 1);
			if(lastIndex == numbers.size() - 1) {
				completeIndexSequences.add(partialSequence);
			}
			else {
				for(int plus = 1; plus <= MAX_DIFF && lastIndex + plus < numbers.size(); plus++) {
					if(numbers.get(lastIndex + plus) - numbers.get(lastIndex) <= MAX_DIFF) {
						List<Integer> longer = new ArrayList<>(partialSequence);
						longer.add(lastIndex + plus);
						incompleteIndexSequences.add(longer);
					}
				}
			}
		}
		return completeIndexSequences.size();
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A sorted set of adaptor ratings.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static SortedSet<Long> readInput() throws FileNotFoundException {
		SortedSet<Long> adaptors = new TreeSet<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLong()) {
			adaptors.add(fileScanner.nextLong());
		}
		return adaptors;
	}
}
