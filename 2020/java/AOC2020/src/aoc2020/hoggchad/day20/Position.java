package aoc2020.hoggchad.day20;

/**
 * A position within a 2D array.
 * 
 * @author Chad Hogg
 */
public class Position {
	/** The row. */
	public int row;

	/** The column. */
	public int col;
	
	/**
	 * Constructs a position.
	 * 
	 * @param row The row.
	 * @param col The column.
	 */
	public Position(int row, int col) {
		this.row = row;
		this.col = col;
	}
}