package aoc2020.hoggchad.day11;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Arrays;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 11.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day11/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be found.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		char[][] layout = readInput();
		System.out.println(part1(layout));
		System.out.println(part2(layout));
	}
	
	/**
	 * Runs a cellular automata until it reaches equilibrium.
	 * 
	 * @param layout The initial grid.
	 * @return The number of occupied seats in the equilibrium map.
	 */
	private static int part1(char[][] layout) {
		char[][] previousLayout = new char[layout.length][layout[0].length];
		char[][] currentLayout = deepCopy(layout);
		while(!Arrays.deepEquals(previousLayout, currentLayout)) {
			previousLayout = deepCopy(currentLayout);
			for(int row = 0; row < layout.length; row++) {
				for(int col = 0; col < layout[0].length; col++) {
					if(previousLayout[row][col] == 'L' && neighborhoodSize(previousLayout, row, col) == 0) {
						currentLayout[row][col] = '#';
					}
					else if(previousLayout[row][col] == '#' && neighborhoodSize(previousLayout, row, col) >= 4) {
						currentLayout[row][col] = 'L';
					}
					else {
						currentLayout[row][col] = previousLayout[row][col];
					}
				}
			}
		}
		return countOccupied(currentLayout);
	}
	
	/**
	 * Makes a copy of a 2-d array of chars.
	 * 
	 * @param array The array to copy.
	 * @return A deep copy of it.
	 */
	private static char[][] deepCopy(char[][] array) {
		char[][] copy = new char[array.length][array[0].length];
		for(int row = 0; row < array.length; row++) {
			copy[row] = Arrays.copyOf(array[row], array[row].length);
		}
		return copy;
	}
	
	/**
	 * Counts the number of occupied seats adjacent to a map location.
	 * 
	 * @param layout The map.
	 * @param row The row of the viewer.
	 * @param col The column of the viewer.
	 * @return The number of adjacent occupied seats.
	 */
	private static int neighborhoodSize(char[][] layout, int row, int col) {
		int neighborhood = 0;
		if(row > 0) {
			if(col > 0 && layout[row - 1][col - 1] == '#') {
				neighborhood++;
			}
			if(layout[row - 1][col] == '#') {
				neighborhood++;
			}
			if(col < layout[row].length - 1 && layout[row - 1][col + 1] == '#') {
				neighborhood++;
			}
		}
		if(col > 0 && layout[row][col - 1] == '#') {
			neighborhood++;
		}
		if(col < layout[row].length -1 && layout[row][col + 1] == '#') {
			neighborhood++;
		}
		if(row < layout.length - 1) {
			if(col > 0 && layout[row + 1][col - 1] == '#') {
				neighborhood++;
			}
			if(layout[row + 1][col] == '#') {
				neighborhood++;
			}
			if(col < layout[row].length - 1 && layout[row + 1][col + 1] == '#') {
				neighborhood++;
			}			
		}
		return neighborhood;
	}
	
	/**
	 * Counts the number of occupied seats on the map.
	 * 
	 * @param layout The map.
	 * @return The number of occupied seats.
	 */
	private static int countOccupied(char[][] layout) {
		int count = 0;
		for(int row = 0; row < layout.length; row++) {
			for(int col = 0; col < layout[row].length; col++) {
				if(layout[row][col] == '#') {
					count++;
				}
			}
		}
		return count;
	}
	
	/**
	 * Performs a certain variant of cellular automata until reaching an equilibrium.
	 * 
	 * @param layout The initial grid.
	 * @return The number of occupied seats in the equilibrium.
	 */
	private static int part2(char[][] layout) {
		char[][] previousLayout = new char[layout.length][layout[0].length];
		char[][] currentLayout = deepCopy(layout);
		while(!Arrays.deepEquals(previousLayout, currentLayout)) {
			previousLayout = deepCopy(currentLayout);
			for(int row = 0; row < layout.length; row++) {
				for(int col = 0; col < layout[0].length; col++) {
					if(previousLayout[row][col] == 'L' && weirdNeighborhoodSize(previousLayout, row, col) == 0) {
						currentLayout[row][col] = '#';
					}
					else if(previousLayout[row][col] == '#' && weirdNeighborhoodSize(previousLayout, row, col) >= 5) {
						currentLayout[row][col] = 'L';
					}
					else {
						currentLayout[row][col] = previousLayout[row][col];
					}
				}
			}
		}
		return countOccupied(currentLayout);		
	}	
	
	/**
	 * Counts how many of the 8 cardinal directions lead to an occupied seat.
	 * 
	 * @param layout The map.
	 * @param row The row the viewer is sitting in.
	 * @param col THe column the viewer is sitting in.
	 * @return The number of occupied seats seen.
	 */
	private static int weirdNeighborhoodSize(char[][] layout, int row, int col) {
		int neighborhood = 0;
		for(int yOffset = -1; yOffset <= 1; yOffset++) {
			for(int xOffset = -1; xOffset <= 1; xOffset++) {
				if(yOffset != 0 || xOffset != 0) {
					if(seesOccupied(layout, row, col, yOffset, xOffset)) {
						neighborhood++;
					}
				}
			}
		}
		return neighborhood;
	}
	
	/**
	 * Checks whether or not a person sees an occupied seat before seeing an empty seat / edge of the world when looking in a direction.
	 * 
	 * @param layout The map of where seats are.
	 * @param row The row of the person's location.
	 * @param col The column of the person's location.
	 * @param yOffset Whether they are looking up, down, or neutral.
	 * @param xOffset Whether they are looking left, right, or neutral.
	 * @return True if they encounter an occupied seat before finding an empty seat or the edge of the map.
	 */
	private static boolean seesOccupied(char[][] layout, int row, int col, int yOffset, int xOffset) {
		int distance = 1;
		while(true) {
			if(row + yOffset * distance < 0 || row + yOffset * distance > layout.length - 1 || col + xOffset * distance < 0 || col + xOffset * distance > layout[row].length - 1) {
				return false;
			}
			else if(layout[row + yOffset * distance][col + xOffset * distance] == '#') {
				return true;
			}
			else if(layout[row + yOffset * distance][col + xOffset * distance] == 'L') {
				return false;
			}
			distance++;
		}
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A 2-D array containing the layout.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static char[][] readInput() throws FileNotFoundException {
		List<String> lines = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			lines.add(fileScanner.nextLine());
		}
		char[][] layout = new char[lines.size()][lines.get(0).length()];
		for(int row = 0; row < lines.size(); row++) {
			for(int col = 0; col < lines.get(row).length(); col++) {
				layout[row][col] = lines.get(row).charAt(col);
			}
		}
		return layout;
	}
}
