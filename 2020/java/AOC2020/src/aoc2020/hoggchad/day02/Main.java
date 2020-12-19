package aoc2020.hoggchad.day02;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * Solves day 2 of Advent Of Code 2020.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day02/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be found.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Password> passwords = readInput();
		System.out.println(part1(passwords));
		System.out.println(part2(passwords));
	}
	
	/**
	 * Counts the number of valid passwords according to the first policy.
	 * 
	 * @param passwords A list of passwords that might or might not be valid.
	 * @return The number of valid passwords.
	 */
	private static int part1(List<Password> passwords) {
		int count = 0;
		for(Password pw : passwords) {
			if(pw.isLegalPart1()) {
				count++;
			}
		}
		return count;
	}
	
	/**
	 * Counts the number of valid passwords according to the second policy.
	 * 
	 * @param passwords A list of passwords that might or might not be valid.
	 * @return The number of vlaid passwords.
	 */
	private static int part2(List<Password> passwords) {
		int count = 0;
		for(Password pw : passwords) {
			if(pw.isLegalPart2()) {
				count++;
			}
		}
		return count;
	}
	
	/**
	 * Reads a password file.
	 * 
	 * @return A list of passwords.
	 * @throws FileNotFoundException If the input file cannot be found.
	 */
	private static List<Password> readInput() throws FileNotFoundException {
		List<Password> passwords = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			String line = fileScanner.nextLine();
			int hyphenLocation = line.indexOf('-');
			int spaceLocation = line.indexOf(' ', hyphenLocation);
			int min = Integer.parseInt(line.substring(0, hyphenLocation));
			int max = Integer.parseInt(line.substring(hyphenLocation + 1, spaceLocation));
			char required = line.charAt(spaceLocation + 1);
			String text = line.substring(spaceLocation + 4);
			passwords.add(new Password(min, max, required, text));
		}
		return passwords;
	}
}
