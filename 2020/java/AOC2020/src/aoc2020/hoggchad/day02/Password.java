package aoc2020.hoggchad.day02;

/**
 * A password read from a corrupted file, with a weird policy.
 * 
 * @author Chad Hogg
 */
public class Password {

	/** The minimum number of occurrences in the policy / earliest index position. */
	private int minTimes;
	/** The maximum number of occurrences in the policy / latest index position. */
	private int maxTimes;
	/** The character that must appear according to the policy. */
	private char required;
	/** The password itself. */
	private String text;
	
	/**
	 * Constructs a new Password.
	 * 
	 * @param minTimes The minimum number of occurrences in the policy / earliest index position.
	 * @param maxTimes The maximum number of ocucrrences in the policy / latest index position.
	 * @param required The character that must appear according to the policy.
	 * @param text The password itself.
	 */
	public Password(int minTimes, int maxTimes, char required, String text) {
		this.minTimes = minTimes;
		this.maxTimes = maxTimes;
		this.required = required;
		this.text = text;
	}
	
	/**
	 * Tests whether or not this password is legal according to the part 1 policy interpretation.
	 * 
	 * @return True if the required characters occurs at least minTimes and no more than maxTimes.
	 */
	public boolean isLegalPart1() {
		int count = 0;
		for(int index = 0; index < text.length(); index++) {
			if(text.charAt(index) == required) {
				count++;
			}
		}
		return count >= minTimes && count <= maxTimes;
	}
	
	/**
	 * Tests whether or not this password is legal according to the part 2 policy interpretation.
	 * 
	 * @return True if the required character occurs exactly at either 1-based index minTimes or 1-based index maxTimes but not both.
	 */
	public boolean isLegalPart2() {
		int count = 0;
		if(text.charAt(minTimes - 1) == required) {
			count++;
		}
		if(text.charAt(maxTimes - 1) == required) {
			count++;
		}
		return count == 1;
	}
}
