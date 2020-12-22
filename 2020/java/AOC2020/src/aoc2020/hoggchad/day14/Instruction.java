package aoc2020.hoggchad.day14;

/**
 * An instructions in a highly restricted architecture.
 * 
 * @author Chad Hogg
 */
public abstract class Instruction {

	/**
	 * Runs this instruction in a version 1 decoder chip.
	 * 
	 * @param computer The decoder chip.
	 */
	public abstract void run(Computer computer);
	
	/**
	 * Runs this instruction in a version 2 decoder chip.
	 * 
	 * @param computer The decoder chip.
	 */
	public abstract void run2(Computer computer);
}
