package aoc2020.hoggchad.day08;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 08.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day08/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Instruction> program = readInput();
		System.out.println(part1(program));
		System.out.println(part2(program));
	}
	
	/**
	 * Runs a program until it reaches an infinite loop.
	 * 
	 * @param program The program to run.
	 * @return The final value of the accumulator.
	 */
	private static int part1(List<Instruction> program) {
		int accumulator = 0;
		int pc = 0;
		List<Boolean> visited = new ArrayList<>();
		for(int index = 0; index < program.size(); index++) {
			visited.add(false);
		}
		while(!visited.get(pc)) {
			visited.set(pc, true);
			accumulator += program.get(pc).getAccumChange();
			pc += program.get(pc).getPCChange();
		}
		return accumulator;
	}
	
	/**
	 * Runs various versions of a program, swapping out nop for jmp instructions until it finds one that does not infinite loop.
	 * 
	 * @param program The program to modify.
	 * @return The value of the accumulator from the version of the program that completes.
	 */
	private static int part2(List<Instruction> program) {
		int instToChange = 0;
		while(instToChange < program.size()) {
			if(program.get(instToChange) instanceof NopInstruction) {
				program.set(instToChange, new JmpInstruction(program.get(instToChange).getArgument()));
				int result = runUntilInfiniteLoopOrEnd(program);
				program.set(instToChange, new NopInstruction(program.get(instToChange).getArgument()));
				if(result != Integer.MIN_VALUE) {
					return result;
				}
			}
			else if(program.get(instToChange) instanceof JmpInstruction) {
				program.set(instToChange, new NopInstruction(program.get(instToChange).getArgument()));
				int result = runUntilInfiniteLoopOrEnd(program);
				program.set(instToChange, new JmpInstruction(program.get(instToChange).getArgument()));
				if(result != Integer.MIN_VALUE) {
					return result;
				}
			}
			instToChange++;
		}
		throw new IllegalStateException("Didn't find any instruction we could change to avoid infinite loop.");
	}
	
	/**
	 * Runs a program until reaching an infinite loop or executing the final instruction.
	 * 
	 * @param program The program to run.
	 * @return The value of the accumulator when the program ends, or Integer.MIN_VALUE if it loops.
	 */
	private static int runUntilInfiniteLoopOrEnd(List<Instruction> program) {
		int accumulator = 0;
		int pc = 0;
		List<Boolean> visited = new ArrayList<>();
		for(int index = 0; index < program.size(); index++) {
			visited.add(false);
		}
		while(!visited.get(pc)) {
			visited.set(pc, true);
			accumulator += program.get(pc).getAccumChange();
			pc += program.get(pc).getPCChange();
			if(pc == program.size()) {
				return accumulator;
			}
		}
		return Integer.MIN_VALUE;
	}
	
	/**
	 * Reads a list of instructions from a file.
	 * 
	 * @return The program.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<Instruction> readInput() throws FileNotFoundException {
		List<Instruction> program = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNext()) {
			String opcode = fileScanner.next();
			int argument = fileScanner.nextInt();
			if(opcode.equals("nop")) {
				program.add(new NopInstruction(argument));
			}
			else if(opcode.equals("acc")) {
				program.add(new AccInstruction(argument));
			}
			else if(opcode.equals("jmp")) {
				program.add(new JmpInstruction(argument));
			}
			else {
				throw new IllegalStateException("Unkonwn opcode: " + opcode);
			}
		}
		return program;
	}
}
