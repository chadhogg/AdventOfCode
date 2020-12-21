package aoc2020.hoggchad.day12;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent of Code 2020, day 12.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day12/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<String> directions = readInput();
		System.out.println(part1(directions));
		System.out.println(part2(directions));
	}
	
	/**
	 * Follows a series of directions assuming that they control the ship itself.
	 * 
	 * @param directions The list of directions.
	 * @return The Manhattan distance traveled.
	 */
	private static int part1(List<String> directions) {
		int x = 0;
		int y = 0;
		int heading = 270; // 0 north, 90 west, 180 south, 270 east
		for(String direction : directions) {
			char type = direction.charAt(0);
			int amount = Integer.parseInt(direction.substring(1));
			switch(type) {
			case 'N': y += amount; break;
			case 'E': x += amount; break;
			case 'S': y -= amount; break;
			case 'W': x -= amount; break;
			case 'R': heading = (heading - amount + 360) % 360; break;
			case 'L': heading = (heading + amount) % 360; break;
			case 'F': 
				switch(heading) {
				case 0: y += amount; break;
				case 90: x -= amount; break;
				case 180: y -= amount; break;
				case 270: x += amount; break;
				default: throw new IllegalStateException("Heading is off of grid: " + heading);
				}
				break;
			default: throw new IllegalStateException("Unexpected direction: " + type);
			}
		}
		return Math.abs(x) + Math.abs(y);
	}
	
	/**
	 * Follows a series of directions assuming that they control the location of a waypoint / the ship.
	 * 
	 * @param directions The list of directions.
	 * @return The Manhattan distance traveled.
	 */
	private static int part2(List<String> directions) {
		int shipY = 0;
		int shipX = 0;
		int wpY = 1;
		int wpX = 10;
		for(String direction : directions) {
			char type = direction.charAt(0);
			int amount = Integer.parseInt(direction.substring(1));
			switch(type) {
			case 'N': wpY += amount; break;
			case 'E': wpX += amount; break;
			case 'S': wpY -= amount; break;
			case 'W': wpX -= amount; break;
			case 'F': shipY += wpY * amount; shipX += wpX * amount; break;
			case 'R':
			case 'L':
				double angle = (type == 'R' ? -amount : amount) * Math.PI / 180.0;
				int sin = (int)Math.round(Math.sin(angle));
				int cos = (int)Math.round(Math.cos(angle));
				int newX = wpX * cos - wpY * sin;
				int newY = wpX * sin + wpY * cos;
				wpY = newY;
				wpX = newX;
				break;
			default: throw new IllegalStateException("Unexpected direction: " + type);
			}
		}
		return Math.abs(shipX) + Math.abs(shipY);
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of directions.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<String> readInput() throws FileNotFoundException {
		List<String> directions = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNext()) {
			directions.add(fileScanner.next());
		}
		return directions;
	}
}
