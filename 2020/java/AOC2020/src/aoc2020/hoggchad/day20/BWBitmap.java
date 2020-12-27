package aoc2020.hoggchad.day20;

import java.util.Arrays;

/**
 * A black and white image.
 * 
 * @author Chad Hogg
 */
public class BWBitmap {
	
	/**
	 * Makes a deep copy of a 2D array of booleans.
	 * 
	 * @param original The original 2D array.
	 * @return A deep copy of it.
	 */
	public static boolean[][] deepCopy(boolean[][] original) {
		boolean[][] copy = new boolean[original.length][original[0].length];
		for(int row = 0; row < original.length; row++) {
			for(int col = 0; col < original[row].length; col++) {
				copy[row][col] = original[row][col];
			}
		}
		return copy;
	}

	/** The pixel values. (0,0) is upper-left corner. */
	private final boolean[][] values;
	
	/**
	 * Constructs a new BWBitmap.
	 * 
	 * @param values The pixel values.
	 */
	public BWBitmap(boolean[][] values) {
		this.values = deepCopy(values);
	}
	
	/**
	 * Gets the height of the image.
	 * 
	 * @return The number of rows.
	 */
	public int getHeight() {
		return values.length;
	}
	
	/**
	 * Gets the width of the image.
	 * 
	 * @return The number of columns.
	 */
	public int getWidth() {
		return values[0].length;
	}
	
	/**
	 * Gets a pixel value.
	 * 
	 * @param row The row of the pixel.
	 * @param col The column of the pixel.
	 * @return The value of that pixel.
	 */
	public boolean getValue(int row, int col) {
		return values[row][col];
	}
	
	/**
	 * Produces a copy of this image in a certain orientation.
	 * 
	 * @param orientation The orientation.
	 * @return A copy of this image in the new orientation.
	 */
	public BWBitmap afterOrientation(Orientation orientation) {
		boolean[][] copy = new boolean[getWidth()][getHeight()];
		for(int row = 0; row < copy.length; row++) {
			for(int col = 0; col < copy[0].length; col++) {
				copy[Orientation.getNewRow(orientation, row, col, getHeight(), getWidth())][Orientation.getNewCol(orientation, row, col, getHeight(), getWidth())] = values[row][col];
			}
		}
		return new BWBitmap(copy);
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for(int row = 0; row < values.length; row++) {
			for(int col = 0; col < values[0].length; col++) {
				if(values[row][col]) {
					sb.append("#");
				}
				else {
					sb.append(".");
				}
			}
			sb.append("\n");
		}
		return sb.toString();
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj == null) {
			return false;
		}
		else if(obj.getClass() == this.getClass()) {
			BWBitmap other = (BWBitmap)obj;
			return Arrays.deepEquals(values, other.values);
		}
		else {
			return false;
		}
	}
	
	@Override
	public int hashCode() {
		return Arrays.deepHashCode(values);
	}
}
