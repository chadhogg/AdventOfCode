package aoc2020.hoggchad.day05;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, Day 05.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day05/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<BoardingPass> passes = readInput();
		System.out.println(part1(passes));
		System.out.println(part2(passes));
	}
	
	/**
	 * Finds the largest seat ID among all boarding passes.
	 * 
	 * @param passes A list of boarding passes.
	 * @return The largest seat ID to be found in that list.
	 */
	private static int part1(List<BoardingPass> passes) {
		int max = passes.get(0).getSeatID();
		for(int index = 1; index < passes.size(); index++) {
			int id = passes.get(index).getSeatID();
			if(id > max) {
				max = id;
			}
		}
		return max;
	}
	
	/**
	 * Finds the unique seat ID that does not exist, when the IDs before and after it do.
	 * 
	 * @param passes A list of boarding passes.
	 * @return The ID of the empty middle seat.
	 */
	private static int part2(List<BoardingPass> passes) {
		int max = part1(passes);
		boolean[] exists = new boolean[max + 1];
		for(BoardingPass pass : passes) {
			int id = pass.getSeatID();
			if(exists[id]) {
				throw new IllegalStateException("Two passes have the same ID.");
			}
			exists[id] = true;
		}
		for(int index = 1; index < max - 1; index++) {
			if(!exists[index] && exists[index-1] && exists[index+1]) {
				return index;
			}
		}
		return -1;
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of boarding passes.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<BoardingPass> readInput() throws FileNotFoundException {
		List<BoardingPass> passes = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNext()) {
			passes.add(new BoardingPass(fileScanner.next()));
		}
		return passes;
	}
}
