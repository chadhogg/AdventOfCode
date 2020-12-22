package aoc2020.hoggchad.day14;

/**
 * An instruction that changes the current bitmask.
 * 
 * @author Chad Hogg
 */
public class ChangeBitMask extends Instruction {

	/** The new bitmask. */
	private char[] bitMask;
	
	/**
	 * Constructs a new ChangeBitMask instruction.
	 * 
	 * @param text The text of the bitmask.
	 */
	public ChangeBitMask(String text) {
		bitMask = text.toCharArray();
	}
	
	@Override
	public void run(Computer computer) {
		computer.setBitMask(bitMask);
	}
	
	@Override
	public void run2(Computer computer) {
		run(computer);
	}
}
