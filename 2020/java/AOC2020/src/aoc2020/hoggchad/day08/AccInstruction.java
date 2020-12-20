package aoc2020.hoggchad.day08;

/**
 * An instruction that adds to the accumulator and increments the program counter.
 * 
 * @author Chad Hogg
 */
public class AccInstruction extends Instruction {

	/**
	 * Creates an accumulating instruction.
	 * 
	 * @param argument The amount to add to the accumulator.
	 */
	public AccInstruction(int argument) {
		super(argument);
	}
	
	@Override
	public int getAccumChange() {
		return getArgument();
	}
	
	@Override
	public int getPCChange() {
		return 1;
	}
}
