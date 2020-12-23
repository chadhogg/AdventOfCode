package aoc2020.hoggchad.day17;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent of Code 2020, day 17.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day17/input";
	/** The number of cycles to run. */
	private static final int CYCLES = 6;

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		boolean[][] initial = readInput();
		System.out.println(part1(initial));
		System.out.println(part2(initial));
	}

	/**
	 * Runs 6 cycles of a 3D cellular automaton.
	 * 
	 * @param initial The initial 2D slice.
	 * @return The number of active cubes in the final state.
	 */
	private static int part1(boolean[][] initial) {
		boolean[][][] state = new boolean[initial.length][initial[0].length][1];
		for(int x = 0; x < initial.length; x++) {
			for(int y = 0; y < initial[x].length; y++) {
				state[x][y][0] = initial[x][y];
			}
		}
		
		for(int cycle = 1; cycle <= CYCLES; cycle++) {
			state = update(state);
		}
		return countActive(state);
	}
	
	/**
	 * Does one update to a 3D cellular automaton.
	 * 
	 * @param current The current state.
	 * @return The next state.
	 */
	private static boolean[][][] update(boolean[][][] current) {
		boolean[][][] copy = new boolean[current.length + 2][current[0].length + 2][current[0][0].length + 2];
		for(int x = 0; x < current.length; x++) {
			for(int y = 0; y < current[0].length; y++) {
				for(int z = 0; z < current[0][0].length; z++) {
					copy[x + 1][y + 1][z + 1] = current[x][y][z];
				}
			}
		}
		boolean[][][] next = new boolean[current.length + 2][current[0].length + 2][current[0][0].length + 2];
		for(int x = 0; x < next.length; x++) {
			for(int y = 0; y < next[x].length; y++) {
				for(int z = 0; z < next[x][y].length; z++) {
					int count = countNeighborhood(copy, x, y, z);
					if(copy[x][y][z]) {
						if(count == 2 || count == 3) {
							next[x][y][z] = true;
						}
						else {
							next[x][y][z] = false;
						}
					}
					else {
						if(count == 3) {
							next[x][y][z] = true;
						}
						else {
							next[x][y][z] = false;
						}
					}
				}
			}
		}
		return next;
	}
	
	/**
	 * Counts the size of the neighborhood (not counting itself) around a 3D cell.
	 * 
	 * @param current The current state.
	 * @param x The x-coordinate of the cell.
	 * @param y The y-coordinate of the cell.
	 * @param z The z-coordinate of the cell.
	 * @return The number of adjacent active cells.
	 */
	private static int countNeighborhood(boolean[][][] current, int x, int y, int z) {
		int count = 0;
		for(int theX = x - 1; theX <= x + 1; theX++) {
			if(theX >= 0 && theX < current.length) {
				for(int theY = y - 1; theY <= y + 1; theY++) {
					if(theY >= 0 && theY < current[theX].length) {
						for(int theZ = z - 1; theZ <= z + 1; theZ++) {
							if(theZ >= 0 && theZ < current[theX][theY].length) {
								if(x != theX || y != theY || z != theZ) {
									if(current[theX][theY][theZ]) {
										count++;
									}
								}
							}
						}
					}
				}
			}
		}
		return count;
	}
	
	/**
	 * Counts the total number of active cells in a 3D cellular automaton.
	 * 
	 * @param current The current state.
	 * @return The number of active cells.
	 */
	private static int countActive(boolean[][][] current) {
		int count = 0;
		for(int x = 0; x < current.length; x++) {
			for(int y = 0; y < current[x].length; y++) {
				for(int z = 0; z < current[x][y].length; z++) {
					if(current[x][y][z]) {
						count++;
					}
				}
			}
		}
		return count;
	}
	
	/**
	 * Prints a 3D cellular automaton.
	 * 
	 * @param state The current state.
	 */
	private static void printState(boolean[][][] state) {
		for(int z = 0; z < state[0][0].length; z++) {
			System.out.println("z=" + (z - state[0][0].length / 2));
			for(int x = 0; x < state.length; x++) {
				for(int y = 0; y < state[x].length; y++) {
					if(state[x][y][z]) {
						System.out.print("#");
					}
					else {
						System.out.print(".");
					}
				}
				System.out.println();
			}
			System.out.println();
		}
		System.out.println();
	}
	
	/**
	 * Runs 6 cycles of a 4D cellular automaton.
	 * 
	 * @param initial A 2D slice of the initial state.
	 * @return The number of active cells in the final state.
	 */
	private static int part2(boolean[][] initial) {
		boolean[][][][] state = new boolean[initial.length][initial[0].length][1][1];
		for(int x = 0; x < initial.length; x++) {
			for(int y = 0; y < initial[x].length; y++) {
				state[x][y][0][0] = initial[x][y];
			}
		}
		
		for(int cycle = 1; cycle <= CYCLES; cycle++) {
			state = update2(state);
		}
		return countActive2(state);
	}

	/**
	 * Updates a 4D cellular automaton.
	 * 
	 * @param current The current state.
	 * @return The new state.
	 */
	private static boolean[][][][] update2(boolean[][][][] current) {
		boolean[][][][] copy = new boolean[current.length + 2][current[0].length + 2][current[0][0].length + 2][current[0][0][0].length + 2];
		for(int x = 0; x < current.length; x++) {
			for(int y = 0; y < current[0].length; y++) {
				for(int z = 0; z < current[0][0].length; z++) {
					for(int w = 0; w < current[0][0][0].length; w++) {
						copy[x + 1][y + 1][z + 1][w + 1] = current[x][y][z][w];
					}
				}
			}
		}
		boolean[][][][] next = new boolean[current.length + 2][current[0].length + 2][current[0][0].length + 2][current[0][0][0].length + 2];
		for(int x = 0; x < next.length; x++) {
			for(int y = 0; y < next[x].length; y++) {
				for(int z = 0; z < next[x][y].length; z++) {
					for(int w = 0; w < next[x][y][z].length; w++) {
						int count = countNeighborhood2(copy, x, y, z, w);
						if(copy[x][y][z][w]) {
							if(count == 2 || count == 3) {
								next[x][y][z][w] = true;
							}
							else {
								next[x][y][z][w] = false;
							}
						}
						else {
							if(count == 3) {
								next[x][y][z][w] = true;
							}
							else {
								next[x][y][z][w] = false;
							}
						}
					}
				}
			}
		}
		return next;
	}

	/**
	 * Counts the neighborhood size of a cell in a 4D cellular automaton.
	 * 
	 * @param current The current state.
	 * @param x The x-coordinate of the cell.
	 * @param y The y-coordinate of the cell.
	 * @param z The z-coordinate of the cell.
	 * @param w The w-coordinate of the cell.
	 * @return The number of adjacent active cells.
	 */
	private static int countNeighborhood2(boolean[][][][] current, int x, int y, int z, int w) {
		int count = 0;
		for(int theX = x - 1; theX <= x + 1; theX++) {
			if(theX >= 0 && theX < current.length) {
				for(int theY = y - 1; theY <= y + 1; theY++) {
					if(theY >= 0 && theY < current[theX].length) {
						for(int theZ = z - 1; theZ <= z + 1; theZ++) {
							if(theZ >= 0 && theZ < current[theX][theY].length) {
								for(int theW = w - 1; theW <= w + 1; theW++) {
									if(theW >= 0 && theW < current[theX][theY][theZ].length) { 
										if(x != theX || y != theY || z != theZ || w != theW) {
											if(current[theX][theY][theZ][theW]) {
												count++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return count;
	}

	/**
	 * Counts the total active cells in a 4D cellular automaton.
	 * 
	 * @param current The current state.
	 * @return The number of active cells.
	 */
	private static int countActive2(boolean[][][][] current) {
		int count = 0;
		for(int x = 0; x < current.length; x++) {
			for(int y = 0; y < current[x].length; y++) {
				for(int z = 0; z < current[x][y].length; z++) {
					for(int w = 0; w < current[x][y][z].length; w++) {
						if(current[x][y][z][w]) {
							count++;
						}
					}
				}
			}
		}
		return count;
	}

	/**
	 * Reads the input.
	 * 
	 * @return A 2-D array of boolean.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static boolean[][] readInput() throws FileNotFoundException {
		List<String> lines = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			lines.add(fileScanner.nextLine());
		}
		fileScanner.close();
		boolean[][] initial = new boolean[lines.size()][lines.get(0).length()];
		for(int row = 0; row < lines.size(); row++) {
			for(int col = 0; col < lines.get(row).length(); col++) {
				if(lines.get(row).charAt(col) == '.') {
					initial[row][col] = false;
				}
				else if(lines.get(row).charAt(col) == '#') {
					initial[row][col] = true;
				}
				else {
					throw new IllegalStateException("Unexpected character: " + lines.get(row).charAt(col));
				}
			}
		}
		return initial;
	}
}
