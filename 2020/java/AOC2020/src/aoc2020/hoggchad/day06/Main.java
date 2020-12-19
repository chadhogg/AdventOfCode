package aoc2020.hoggchad.day06;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, Day 06.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The input file path. */
	private static final String FILE_PATH = "../../inputs/day06/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Group> groups = readInput();
		System.out.println(part1(groups));
		System.out.println(part2(groups));
	}
	
	/**
	 * Sums, for each group, the number of questions answered yes by at least one group member.
	 * 
	 * @param groups The list of groups.
	 * @return That sum.
	 */
	private static int part1(List<Group> groups) {
		int count = 0;
		for(Group group : groups) {
			count += group.getAnyYesAnswers().size();
		}
		return count;
	}
	
	/**
	 * Sums, for each group, the number of questions answered yes by every group member.
	 * 
	 * @param groups The list of groups.
	 * @return That sum.
	 */
	private static int part2(List<Group> groups) {
		int count = 0;
		for(Group group : groups) {
			count += group.getAllYesAnswers().size();
		}
		return count;
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of groups.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<Group> readInput() throws FileNotFoundException {
		List<Group> groups = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		Group currentGroup = new Group();
		while(fileScanner.hasNextLine()) {
			String line = fileScanner.nextLine();
			if(line.length() == 0) {
				groups.add(currentGroup);
				currentGroup = new Group();
			}
			else {
				currentGroup.addMember(line);
			}
		}
		if(currentGroup.getMemberCount() > 0) {
			groups.add(currentGroup);
		}
		return groups;
	}
}
