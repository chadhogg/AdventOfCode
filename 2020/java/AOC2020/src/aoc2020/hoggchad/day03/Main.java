package aoc2020.hoggchad.day03;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020 Day 03.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day03/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file is not found.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		RepeatingMap map = readInput();
		System.out.println(part1(map));
		System.out.println(part2(map));
	}
	
	/**
	 * Counts the number of trees encountered when going down 1, right 3.
	 * 
	 * @param map The map.
	 * @return The number of trees encountered.
	 */
	private static long part1(RepeatingMap map) {
		return countTrees(map, 3, 1);
	}
	
	/**
	 * Counts the number of trees encountered on 5 different paths and multiplies them together.
	 * 
	 * @param map The map.
	 * @return The product of the number of trees encountered.
	 */
	private static long part2(RepeatingMap map) {
		return countTrees(map, 1, 1) * countTrees(map, 1, 3) * countTrees(map, 1, 5) * countTrees(map, 1, 7) * countTrees(map, 2, 1);
	}
	
	/**
	 * Counts the number of trees encountered on a downward path through the map.
	 * 
	 * @param map The map.
	 * @param dy The number of spaces jumped downward each time.
	 * @param dx The number of spaces jumped rightward each time.
	 * @return The number of trees encountered.
	 */
	private static long countTrees(RepeatingMap map, int dy, int dx) {
		int row = 0;
		int col = 0;
		long treeCount = 0;
		while(row < map.getHeight()) {
			if(map.isTree(row, col)) {
				treeCount++;
			}
			row += dy;
			col += dx;
		}
		return treeCount;		
	}
	
	/**
	 * Reads the input file.
	 * 
	 * @return A map of trees / not trees.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static RepeatingMap readInput() throws FileNotFoundException {
		int lines = 0;
		List<Boolean> list = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			lines++;
			String line = fileScanner.nextLine();
			for(int index = 0; index < line.length(); index++) {
				if(line.charAt(index) == '#') {
					list.add(true);
				}
				else if(line.charAt(index) == '.') {
					list.add(false);
				}
				else {
					throw new IllegalStateException("File contained unexpected character.");
				}
			}
		}
		if(list.size() % lines != 0) {
			throw new IllegalStateException("File did not seem to contain a square grid.");
		}
		int width = list.size() / lines;
		boolean[][] grid = new boolean[lines][width];
		for(int row = 0; row < lines; row++) {
			for(int col = 0; col < width; col++) {
				grid[row][col] = list.get(row * width + col);
			}
		}
		return new RepeatingMap(grid);
	}
}
