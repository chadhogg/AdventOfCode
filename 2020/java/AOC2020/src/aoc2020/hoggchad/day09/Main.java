package aoc2020.hoggchad.day09;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent of Code 2020, day 09.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day09/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Long> numbers = readInput();
		System.out.println(part1(numbers));
		System.out.println(part2(numbers));
	}
	
	/**
	 * Finds a number in a list that is not the sum of a pair of numbers in the 25 before it.
	 * 
	 * @param numbers The list of numbers.
	 * @return The number that is not a sum.
	 */
	private static long part1(List<Long> numbers) {
		final int PREAMBLE_SIZE = 25;
		for(int index = PREAMBLE_SIZE; index < numbers.size(); index++) {
			if(!isSumOfPrior(numbers, index, PREAMBLE_SIZE)) {
				return numbers.get(index);
			}
		}
		throw new IllegalStateException("Did not find a number with expected property.");
	}
	
	/**
	 * Tests whether or not a number in a list is the sum of two numbers earlier in the list.
	 * 
	 * @param numbers The list of numbers.
	 * @param findIndex The index of the number that might or might not be a sum.
	 * @param count The number of prior numbers to consider as addends.
	 * @return True if some pair of prior numbers adds to the value.
	 */
	private static boolean isSumOfPrior(List<Long> numbers, int findIndex, int count) {
		for(int indexA = findIndex - count; indexA < findIndex; indexA++) {
			for(int indexB = indexA + 1; indexB < findIndex; indexB++) {
				if(numbers.get(indexA) + numbers.get(indexB) == numbers.get(findIndex)) {
					return true;
				}
			}
		}
		return false;
	}
	
	/**
	 * Searches for a contiguous sequence of numbers that sums to a certain value.
	 * 
	 * @param numbers A list of numbers.
	 * @return The sum of the largest and smallest numbers in the sequence.
	 */
	private static long part2(List<Long> numbers) {
		long target = part1(numbers);
		for(int startIndex = 0; startIndex < numbers.size() - 1; startIndex++) {
			for(int endIndex = startIndex + 1; endIndex < numbers.size(); endIndex++) {
				long sum = 0;
				long smallest = Long.MAX_VALUE;
				long largest = Long.MIN_VALUE;
				for(int index = startIndex; index <= endIndex; index++) {
					sum += numbers.get(index);
					if(numbers.get(index) < smallest) {
						smallest = numbers.get(index);
					}
					if(numbers.get(index) > largest) {
						largest = numbers.get(index);
					}
				}
				if(sum == target) {
					return largest + smallest;
				}
			}
		}
		throw new IllegalStateException("Did not find a range with the expected property.");
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of numbers.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<Long> readInput() throws FileNotFoundException {
		List<Long> numbers = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLong()) {
			numbers.add(fileScanner.nextLong());
		}
		return numbers;
	}

}
