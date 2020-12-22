package aoc2020.hoggchad.day14;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, Day 14.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day14/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		List<Instruction> instructions = new ArrayList<>();
		while(fileScanner.hasNextLine()) {
			String line = fileScanner.nextLine();
			if(line.startsWith("mask = ")) {
				instructions.add(new ChangeBitMask(line.substring("mask = ".length())));
			}
			else if(line.startsWith("mem[")) {
				int address = Integer.parseInt(line.substring(line.indexOf('[') + 1, line.indexOf(']')));
				long value = Long.parseLong(line.substring(line.indexOf("= ") + 2));
				instructions.add(new Store(address, value));
			}
		}
		
		System.out.println(part1(instructions));
		System.out.println(part2(instructions));
	}
	
	/**
	 * Runs the program simulating a version 1 decoder chip.
	 * 
	 * @param program The program to run.
	 * @return The sum of all memory values.
	 */
	private static long part1(List<Instruction> program) {
		Computer computer = new Computer();
		for(Instruction instruction : program) {
			instruction.run(computer);
		}
		return computer.getMemorySum();
	}
	
	/**
	 * Runs the program simulating a version 2 decoder chip.
	 * 
	 * @param program The program to run.
	 * @return The sum of all memory values.
	 */
	private static long part2(List<Instruction> program) {
		Computer computer = new Computer();
		for(Instruction instruction : program) {
			instruction.run2(computer);
		}
		return computer.getMemorySum();
	}
}
