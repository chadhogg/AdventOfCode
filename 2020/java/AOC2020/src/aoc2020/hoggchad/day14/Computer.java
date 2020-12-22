package aoc2020.hoggchad.day14;

import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;

/**
 * A decoder chip.
 * 
 * @author Chad Hogg
 */
public class Computer {

	/** The current bitmask. */
	private char[] bitMask;
	/** The memory values as address-value pairs. */
	private Map<Long, Long> memory;
	
	/**
	 * Constructs a new computer with a blank bitmask and nothing in memory.
	 */
	public Computer() {
		bitMask = new char[BitArray.LENGTH];
		Arrays.fill(bitMask, 'X');
		memory = new TreeMap<>();
	}
	
	/**
	 * Gets the current bitmask.
	 * 
	 * @return The current bitmask.
	 */
	public char[] getBitMask () {
		return bitMask;
	}
	
	/**
	 * Sets the bitmask.
	 * 
	 * @param bitMask The new bitmask.
	 */
	public void setBitMask(char[] bitMask) {
		this.bitMask = Arrays.copyOf(bitMask, BitArray.LENGTH);
	}
	
	/**
	 * Changes a value in memory.
	 * 
	 * @param address The location to write to.
	 * @param value The number to write.
	 */
	public void setMemory(long address, long value) {
		memory.put(address, value);
	}
	
	/**
	 * Gets the sum of the values stored at all memory locations.
	 * 
	 * @return That sum.
	 */
	public long getMemorySum() {
		long sum = 0;
		for(long address : memory.keySet()) {
			sum += memory.get(address);
		}
		return sum;
	}
}
