package aoc2020.hoggchad.day07;

import java.util.Map;
import java.util.TreeMap;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, Day 07.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day07/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Map<String, Bag> bags = readInput();
		System.out.println(part1(bags));
		System.out.println(part2(bags));
	}
	
	/**
	 * Counts the number of bags that can contain a shiny gold bag.
	 * 
	 * @param bags A map of all bags.
	 * @return The number of bags that can (recursively) contain it.
	 */
	private static int part1(Map<String, Bag> bags) {
		int count = 0;
		for(Bag bag : bags.values()) {
			if(bag.canContain("shiny gold", bags)) {
				count++;
			}
		}
		return count;
	}
	
	/**
	 * Counts the number of bags that can be held in a shiny gold bag.
	 * 
	 * @param bags A map of all bags.
	 * @return The number of bags that can be held in a shiny gold bag.
	 */
	private static int part2(Map<String, Bag> bags) {
		return bags.get("shiny gold").countContents(bags);
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A map of names to bag objects.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static Map<String, Bag> readInput() throws FileNotFoundException {
		Map<String, Bag> bags = new TreeMap<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			Bag bag = new Bag(fileScanner.nextLine());
			bags.put(bag.getType(), bag);
		}
		return bags;
	}
	
}
