package aoc2020.hoggchad.day20;

/**
 * An orientation of a tile.
 * 
 * @author Chad Hogg
 */
public enum Orientation {
	STANDARD,
	ROT90,
	ROT180,
	ROT270,
	FLIPHORZ,
	FLIPVERT,
	ROT90UPSIDEDOWN,
	ROT270UPSIDEDOWN;
	
	/**
	 * Gets the row in which you would look up a pixel after applying an orientation change.
	 * 
	 * @param orientation The new orientation.
	 * @param row The row the pixel had been in in standard orientation.
	 * @param col The column the pixel had been in in standard orientation.
	 * @param height The height of the image in standard orientation.
	 * @param width The width of the image in standard orientation.
	 * @return The new row.
	 */
	public static int getNewRow(Orientation orientation, int row, int col, int height, int width) {
		switch(orientation) {
		case STANDARD:
			return row;
		case ROT90:
			return width - 1 - col;
		case ROT180:
			return height - 1 - row;
		case ROT270:
			return col;
		case FLIPHORZ:
			return row;
		case FLIPVERT:
			return height - 1 - row;
		case ROT90UPSIDEDOWN:
			return col;
		case ROT270UPSIDEDOWN:
			return width - 1 - col;
		default:
			throw new IllegalStateException("Unknown orientation");
		}
	}
	
	/**
	 * Gets the column in which you would look up a pixel after applying an orientation change.
	 * 
	 * @param orientation The new orientation.
	 * @param row The row the pixel had been in in standard orientation.
	 * @param col The column the pixel had been in in standard orientation.
	 * @param height The height of the image in standard orientation.
	 * @param width The width of the image in standard orientation.
	 * @return The new column.
	 */
	public static int getNewCol(Orientation orientation, int row, int col, int height, int width) {
		switch(orientation) {
		case STANDARD:
			return col;
		case ROT90:
			return row;
		case ROT180:
			return width - 1 - col;
		case ROT270:
			return height - 1 - row;
		case FLIPHORZ:
			return width - 1 - col;
		case FLIPVERT:
			return col;
		case ROT90UPSIDEDOWN:
			return row;
		case ROT270UPSIDEDOWN:
			return height - 1 - row;
		default:
			throw new IllegalStateException("Unknown orientation");
		}
	}

}