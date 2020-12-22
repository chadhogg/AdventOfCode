package aoc2020.hoggchad.day14;

import java.util.List;

/**
 * An instruction that stores a value into memory.
 * 
 * @author Chad Hogg
 */
public class Store extends Instruction {

	/** The address into which the value should be stored. */
	private long address;
	/** The new value that should be stored. */
	private long value;
	
	/**
	 * Constructs a new Store instruction.
	 * 
	 * @param address The address into which the value should be stored.
	 * @param value The value that should be stored.
	 */
	public Store(long address, long value) {
		this.address = address;
		this.value = value;
	}
	
	@Override
	public void run(Computer computer) {
		BitArray num = new BitArray(value);
		num.applyMask(computer.getBitMask());
		computer.setMemory(address, num.getValue());
	}
	
	@Override
	public void run2(Computer computer) {
		BitArray add = new BitArray(address);
		List<Long> addresses = add.applyMask2(computer.getBitMask());
		for(long a : addresses) {
			computer.setMemory(a, value);
		}
	}
}
