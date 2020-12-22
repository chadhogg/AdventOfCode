package aoc2020.hoggchad.day16;

/**
 * The rules about a field on a ticket.
 * 
 * @author Chad Hogg
 */
public class Field {

	/** The name of the ticket. */
	private String name;
	/** The minimum value in the first allowable range. */
	private int range1Min;
	/** The maximum value in the first allowable range. */
	private int range1Max;
	/** The minimum value in the second allowable range. */
	private int range2Min;
	/** The maximum value in the second allowable range. */
	private int range2Max;
	
	/**
	 * Constructs a Field.
	 * 
	 * @param line A string formatted like "class: 0-1 or 4-19".
	 */
	public Field(String line) {
		int colonLoc = line.indexOf(':');
		int firstHyphen = line.indexOf('-');
		int or = line.indexOf(" or ");
		int secondHyphen = line.indexOf('-', or);
		name = line.substring(0, colonLoc);
		range1Min = Integer.parseInt(line.substring(colonLoc + 2, firstHyphen));
		range1Max = Integer.parseInt(line.substring(firstHyphen + 1, or));
		range2Min = Integer.parseInt(line.substring(or + 4, secondHyphen));
		range2Max = Integer.parseInt(line.substring(secondHyphen + 1));
	}
	
	/**
	 * Tests whether or not a number would be a valid value for this field.
	 * 
	 * @param number The number to check.
	 * @return True if it falls in one of the two allowable ranges.
	 */
	public boolean isValid(int number) {
		return (range1Min <= number && number <= range1Max) || (range2Min <= number && number <= range2Max);
	}
	
	/**
	 * Gets the name of this field.
	 * 
	 * @return The name.
	 */
	public String getName() {
		return name;
	}
}
