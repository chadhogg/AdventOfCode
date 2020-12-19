package aoc2020.hoggchad.day05;

/**
 * A boarding pass, with BSP-encoded row and seat.
 * 
 * @author Chad Hogg
 */
public class BoardingPass {
	/** The lowest possible row number. */
	public static final int MIN_ROW = 0;
	/** The highest possible row number. */
	public static final int MAX_ROW = 127;
	/** The lowest possible seat number. */
	public static final int MIN_COL = 0;
	/** The highest possible seat number. */
	public static final int MAX_COL = 7;

	/** The text of the boarding pass. */
	private String text;
	
	/**
	 * Constructs a new BoardingPass.
	 * 
	 * @param text A 10-letter code for the seat.
	 */
	public BoardingPass(String text) {
		this.text = text;
		if(text.length() != 10) {
			throw new IllegalArgumentException("Boarding pass was not 10 characters long.");
		}
	}
	
	/**
	 * Gets the row of this BoardingPass.
	 * 
	 * @return The result of BSP-unencoding the first 7 characters.
	 */
	public int getRow() {
		int low = MIN_ROW;
		int high = MAX_ROW;
		for(int index = 0; index < 7; index++) {
			int mid = (low + high) / 2;
			char letter = text.charAt(index);
			if(letter == 'F') {
				high = mid;
			}
			else if(letter == 'B') {
				low = mid + 1;
			}
			else {
				throw new IllegalStateException("One of first 7 letters was not F or B.");
			}
		}
		if(low != high) {
			throw new IllegalStateException("BSP did not reduce to one row.");
		}
		return low;
	}
	
	/**
	 * Gets the column of this BoardingPass.
	 * 
	 * @return The result of BSP-unencoding the last 3 characters.
	 */
	public int getCol() {
		int low = MIN_COL;
		int high = MAX_COL;
		for(int index = 7; index < 10; index++) {
			int mid = (low + high) / 2;
			char letter = text.charAt(index);
			if(letter == 'L') {
				high = mid;
			}
			else if(letter == 'R') {
				low = mid + 1;
			}
			else {
				throw new IllegalStateException("One of last 3 letters was not R or L.");
			}
		}
		if(low != high) {
			throw new IllegalStateException("BSP did not reduce to one column.");
		}
		return low;
	}
	
	/**
	 * Gets the seat ID.
	 * 
	 * @return Eight times the row plus the column.
	 */
	public int getSeatID() {
		return getRow() * 8 + getCol();
	}
}
