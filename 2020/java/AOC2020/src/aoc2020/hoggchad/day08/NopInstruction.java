package aoc2020.hoggchad.day08;

/**
 * An instruction that has no effect on the accumulator, increments the program counter.
 * 
 * @author Chad Hogg
 */
public class NopInstruction extends Instruction {
	
	/**
	 * Constructs a no-op instruction.
	 * 
	 * @param argument An associated number, which is meaningless.
	 */
	public NopInstruction(int argument) {
		super(argument);
	}
	
	@Override
	public int getAccumChange() {
		return 0;
	}
	
	@Override
	public int getPCChange() {
		return 1;
	}

}
