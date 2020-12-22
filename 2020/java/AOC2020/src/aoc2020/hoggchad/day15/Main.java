package aoc2020.hoggchad.day15;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 15.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day15/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Integer> starting = readInput();
		System.out.println(getLastNumber(new ArrayList<>(starting), 2020));
		System.out.println(getLastNumber(new ArrayList<>(starting), 30000000));
	}
	
	/**
	 * Gets the last number in the sequence.
	 * 
	 * @param initial The initial numbers in the sequence.
	 * @param count How many times the sequence should repeat before stopping.
	 * @return The last number generated.
	 */
	private static int getLastNumber(List<Integer> initial, int count) {
		// We only need to remember the most recent turn at which each number was spoken.
		Map<Integer, Integer> valueToLastTurn = new HashMap<>();
		for(int index = 0; index < initial.size() - 1; index++) {
			valueToLastTurn.put(initial.get(index), index + 1);
		}
		
		int previous = initial.get(initial.size() - 1);
		for(int currentTurn = initial.size(); currentTurn < count; currentTurn++) {
			if(valueToLastTurn.containsKey(previous)) {
				int next = currentTurn - valueToLastTurn.get(previous);
				valueToLastTurn.put(previous, currentTurn);
				previous = next;
			}
			else {
				valueToLastTurn.put(previous, currentTurn);
				previous = 0;
			}
		}
		return previous;
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of the starting numbers.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<Integer> readInput() throws FileNotFoundException {
		List<Integer> numbers = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		String[] input = fileScanner.nextLine().split(",");
		for(String s : input) {
			numbers.add(Integer.parseInt(s));
		}
		return numbers;
	}
}
