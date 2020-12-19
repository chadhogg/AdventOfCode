package aoc2020.hoggchad.day01;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * A program that solves Advent Of Code 2020, Day 01.
 * 
 * @author Chad Hogg
 */
public class Main {
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day01/input";
	/** The sum we are trying to find. */
	private static final int TARGET = 2020;

	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 * @throws FileNotFoundException If the input file does not exist.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Integer> entries = readInput();
		System.out.println(part1(entries));
		System.out.println(part2(entries));
	}

	/**
	 * Searches a list of integers to find the unique pair that sum to 2020, then returns their product.
	 * 
	 * @param entries The list of integers.
	 * @return The product of the two integers that sum to 2020.
	 */
	private static int part1(List<Integer> entries) {
		for(int outer = 0; outer < entries.size() - 1; outer++) {
			int numA = entries.get(outer);
			for(int inner = outer; inner < entries.size(); inner++) {
				int numB = entries.get(inner);
				if(numA + numB == TARGET) {
					return numA * numB;
				}
			}
		}
		throw new IllegalArgumentException("No pairs summed to 2020.");
	}
	
	/**
	 * Searches a list of integers to find the unique triple that sum to 2020, then returns their product.
	 * 
	 * @param entries The list of integers.
	 * @return The product of the three integers that sum to 2020.
	 */
	private static int part2(List<Integer> entries) {
		for(int outer = 0; outer < entries.size() - 2; outer++) {
			int numA = entries.get(outer);
			for(int middle = outer; middle < entries.size() - 1; middle++) {
				int numB = entries.get(middle);
				for(int inner = middle; inner < entries.size(); inner++) {
					int numC = entries.get(inner);
					if(numA + numB + numC == TARGET) {
						return numA * numB * numC;
					}
				}
			}
		}
		throw new IllegalArgumentException("No triples summed to 2020.");
	}
	
	/**
	 * Reads a file of integers.
	 * 
	 * @return A list of those integers.
	 * @throws FileNotFoundException If the input file does not exist.
	 */
	private static List<Integer> readInput() throws FileNotFoundException {
		List<Integer> entries = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextInt()) {
			entries.add(fileScanner.nextInt());
		}
		return entries;
	}
}
