package aoc2020.hoggchad.day24;

import java.util.Set;
import java.util.HashSet;
import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent of Code 2020, day 24.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day24/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<String> strings = readInput();
		System.out.println(part1(strings));
		System.out.println(part2(strings));
	}
	
	/**
	 * Gets the number of black tiles following the coordinates to do flips.
	 * 
	 * @param strings The list of strings that tell you which tiles to flip.
	 * @return The number of black tiles.
	 */
	private static int part1(List<String> strings) {
		return doInitialization(strings).size();
	}
	
	/**
	 * Gets the number of black tiles following a hexagonal cellular automaton.
	 * 
	 * @param strings The list of strings that tell you which tiles to flip initially.
	 * @return The final number of black tiles.
	 */
	private static int part2(List<String> strings) {
		Set<Coordinate> black = doInitialization(strings);
		for(int day = 1; day <= 100; day++) {
			black = update(black);
			System.out.println("Day " + day + ": " + black.size());
		}
		return black.size();
	}
	
	/**
	 * Follows the instructions about which tiles to flip.
	 * 
	 * @param strings The instructions.
	 * @return A set of coordinates containing black tiles.
	 */
	private static Set<Coordinate> doInitialization(List<String> strings) {
		Set<Coordinate> black = new HashSet<>();
		for(String string : strings) {
			Coordinate c = new Coordinate(string);
			if(black.contains(c)) {
				black.remove(c);
			}
			else {
				black.add(c);
			}
		}
		return black;
	}
	
	/**
	 * Do a cellular automaton update step.
	 * 
	 * @param oldBlack A set of black coordinates.
	 * @return A new set of black coordinates.
	 */
	private static Set<Coordinate> update(Set<Coordinate> oldBlack) {
		Set<Coordinate> newBlack = new HashSet<>();
		int furthestEast = Integer.MIN_VALUE;
		int furthestWest = Integer.MAX_VALUE;
		int furthestNorth = Integer.MIN_VALUE;
		int furthestSouth = Integer.MAX_VALUE;
		for(Coordinate c : oldBlack) {
			if(c.east > furthestEast) {
				furthestEast = c.east;
			}
			if(c.east < furthestWest) {
				furthestWest = c.east;
			}
			if(c.north > furthestNorth) {
				furthestNorth = c.north;
			}
			if(c.north < furthestSouth) {
				furthestSouth = c.north;
			}
		}
		for(int row = furthestSouth - 1; row <= furthestNorth + 1; row++) {
			for(int col = furthestWest - 2; col <= furthestEast + 2; col++) {
				Coordinate c = new Coordinate(row, col);
				int neighborhood = countNeighborhood(c, oldBlack);
				if(oldBlack.contains(c)) {
					if(neighborhood == 1 || neighborhood == 2) {
						newBlack.add(c);
					}
				}
				else {
					if(neighborhood == 2) {
						newBlack.add(c);
					}
				}
			}
		}
		return newBlack;
	}
	
	/**
	 * Calculates the neighborhood size for a hexagonal cell.
	 * 
	 * @param c The coordinates of the cell.
	 * @param oldBlack The set of coordinates of currently black tiles.
	 * @return THe number of adjacent cells that are black.
	 */
	private static int countNeighborhood(Coordinate c, Set<Coordinate> oldBlack) {
		Set<Coordinate> neighbors = new HashSet<>();
		neighbors.add(new Coordinate(c.north + 1, c.east + 1));
		neighbors.add(new Coordinate(c.north, c.east + 2));
		neighbors.add(new Coordinate(c.north - 1, c.east + 1));
		neighbors.add(new Coordinate(c.north - 1, c.east - 1));
		neighbors.add(new Coordinate(c.north, c.east - 2));
		neighbors.add(new Coordinate(c.north + 1, c.east - 1));
		neighbors.retainAll(oldBlack);
		return neighbors.size();
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of strings telling you which tiles to initially flip.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<String> readInput() throws FileNotFoundException {
		List<String> strings = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			strings.add(fileScanner.nextLine());
		}
		fileScanner.close();
		return strings;
	}
}
