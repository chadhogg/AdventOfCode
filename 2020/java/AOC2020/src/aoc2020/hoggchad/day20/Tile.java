package aoc2020.hoggchad.day20;

/**
 * A rectangular sub-image.
 * 
 * @author Chad Hogg
 */
public class Tile {

	/** The numerical ID of the tile. */
	private final int tileID;
	/** The black-and-white image. */
	private final BWBitmap image;
	
	/**
	 * Constructs a tile.
	 * 
	 * @param tileID The numerical ID of the tile.
	 * @param image The black and white image.
	 */
	public Tile(int tileID, boolean[][] image) {
		this.tileID = tileID;
		this.image = new BWBitmap(image);
	}
	
	/**
	 * Constructs a tile.
	 * 
	 * @param tileID The numerical ID of the tile.
	 * @param image The image.
	 */
	public Tile(int tileID, BWBitmap image) {
		this.tileID = tileID;
		this.image = image;
	}
	
	/**
	 * Gets the tile ID.
	 * 
	 * @return The tile ID.
	 */
	public int getTileID() {
		return tileID;
	}
	
	/**
	 * Gets the image.
	 * 
	 * @return The image.
	 */
	public BWBitmap getImage() {
		return image;
	}
	
	/**
	 * Gets the top border of the tile.
	 * 
	 * @return The top border of the tile.
	 */
	public boolean[] getTopBorder() {
		boolean[] row = new boolean[image.getWidth()];
		for(int col = 0; col < row.length; col++) {
			row[col] = image.getValue(0, col);
		}
		return row;
	}
	
	/**
	 * Gets the bottom border of the tile.
	 * 
	 * @return The bottom border of the tile.
	 */
	public boolean[] getBottomBorder() {
		boolean[] row = new boolean[image.getWidth()];
		for(int col = 0; col < row.length; col++) {
			row[col] = image.getValue(image.getHeight() - 1, col);
		}
		return row;
	}
	
	/**
	 * Gets the left border of the tile.
	 * 
	 * @return The left border of the tile.
	 */
	public boolean[] getLeftBorder() {
		boolean[] col = new boolean[image.getHeight()];
		for(int row = 0; row < col.length; row++) {
			col[row] = image.getValue(row, 0);
		}
		return col;
	}
	
	/**
	 * Gets the right border of the tile.
	 * 
	 * @return The right border of the tile.
	 */
	public boolean[] getRightBorder() {
		boolean[] col = new boolean[image.getHeight()];
		for(int row = 0; row < col.length; row++) {
			col[row] = image.getValue(row, image.getWidth() - 1);
		}
		return col;
	}
	
	/**
	 * Gets the black/white value stored at a location in the image.
	 * 
	 * @param row The 0-based row.
	 * @param col The 0-based column.
	 * @return
	 */
	public boolean getValue(int row, int col) {
		return image.getValue(row, col);
	}
	
	/**
	 * Gets the number of rows in the image.
	 * 
	 * @return The height.
	 */
	public int getHeight() {
		return image.getHeight();
	}
	
	/**
	 * Gets the number of columns in the image.
	 * 
	 * @return The width.
	 */
	public int getWidth() {
		return image.getWidth();
	}
}
