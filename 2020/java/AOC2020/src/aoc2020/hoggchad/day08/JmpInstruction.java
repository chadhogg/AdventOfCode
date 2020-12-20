package aoc2020.hoggchad.day08;

/**
 * An instruction that does nothing to the accumulator, but chooses the next instruction.
 * 
 * @author Chad Hogg
 */
public class JmpInstruction extends Instruction {

	/**
	 * Constructs a jump instruction.
	 * 
	 * @param argument The program counter offset.
	 */
	public JmpInstruction(int argument) {
		super(argument);
	}
	
	@Override
	public int getAccumChange() {
		return 0;
	}
	
	@Override
	public int getPCChange() {
		return getArgument();
	}
}
