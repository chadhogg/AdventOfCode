package aoc2020.hoggchad.day08;

/**
 * An instruction in an extremely simple instruction set.
 * 
 * @author Chad Hogg
 */
public abstract class Instruction {
	
	/** The argument to the instruction. */
	private int argument;
	
	/**
	 * Constructs a new instruction.
	 * 
	 * @param argument The number associated with the instruction.
	 */
	public Instruction(int argument) {
		this.argument = argument;
	}
	
	/**
	 * Gets the argument.
	 * 
	 * @return The argument.
	 */
	public int getArgument() {
		return argument;
	}

	/**
	 * Gets the change to the accumulator from this instruction.
	 * 
	 * @return The change to the accumulator from this instruction.
	 */
	public abstract int getAccumChange();
	
	/**
	 * Gets the change to the program counter from this instruction.
	 * 
	 * @return The change to the program counter from this instruction.
	 */
	public abstract int getPCChange();
}
