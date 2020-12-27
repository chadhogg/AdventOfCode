package aoc2020.hoggchad.day20;

import java.util.Map;

/**
 * The combination of a tile ID and orientation.
 * 
 * @author Chad Hogg
 */
public class OrientedTile {
	/** The ID of the tile. */
	private int tileID;
	/** The orientation of the tile. */
	private Orientation orientation;
	
	/**
	 * Constructs a new OrientedTile.
	 * 
	 * @param tileID The ID of the tile.
	 * @param orientation The orientation of the tile.
	 */
	public OrientedTile(int tileID, Orientation orientation) {
		this.tileID = tileID;
		this.orientation = orientation;
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
	 * Gets the orientation.
	 * 
	 * @return The orientation.
	 */
	public Orientation getOrientation() {
		return orientation;
	}
	
	/**
	 * Gets the border along the top after applying this orientation.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return The top border of the oriented tile.
	 */
	public boolean[] getTopBorder(Map<Integer, Tile> tiles) {
		Tile tile = tiles.get(tileID);
		switch(orientation) {
		case STANDARD:
			return tile.getTopBorder();
		case ROT90:
			return tile.getRightBorder();
		case ROT180:
			return reverse(tile.getBottomBorder());
		case ROT270:
			return reverse(tile.getLeftBorder());
		case FLIPHORZ:
			return reverse(tile.getTopBorder());
		case FLIPVERT:
			return tile.getBottomBorder();
		case ROT90UPSIDEDOWN:
			return tile.getLeftBorder();
		case ROT270UPSIDEDOWN:
			return reverse(tile.getRightBorder());
		default:
			throw new IllegalStateException("Unknown orientation.");
		}
	}
	
	/**
	 * Gets the border along the left after applying this orientation.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return The left border of the oriented tile.
	 */
	public boolean[] getLeftBorder(Map<Integer, Tile> tiles) {
		Tile tile = tiles.get(tileID);
		switch(orientation) {
		case STANDARD:
			return tile.getLeftBorder();
		case ROT90:
			return reverse(tile.getTopBorder());
		case ROT180:
			return reverse(tile.getRightBorder());
		case ROT270:
			return tile.getBottomBorder();
		case FLIPHORZ:
			return tile.getRightBorder();
		case FLIPVERT:
			return reverse(tile.getLeftBorder());
		case ROT90UPSIDEDOWN:
			return tile.getTopBorder();
		case ROT270UPSIDEDOWN:
			return reverse(tile.getBottomBorder());
		default:
			throw new IllegalStateException("Unknown orientation.");
		}
	}
	
	/**
	 * Gets the border along the bottom after applying this orientation.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return The bottom border of the oriented tile.
	 */
	public boolean[] getBottomBorder(Map<Integer, Tile> tiles) {
		Tile tile = tiles.get(tileID);
		switch(orientation) {
		case STANDARD:
			return tile.getBottomBorder();
		case ROT90:
			return tile.getLeftBorder();
		case ROT180:
			return reverse(tile.getTopBorder());
		case ROT270:
			return reverse(tile.getRightBorder());
		case FLIPHORZ:
			return reverse(tile.getBottomBorder());
		case FLIPVERT:
			return tile.getTopBorder();
		case ROT90UPSIDEDOWN:
			return tile.getRightBorder();
		case ROT270UPSIDEDOWN:
			return reverse(tile.getLeftBorder());
		default:
			throw new IllegalStateException("Unknown orientation.");
		}
	}
	
	/**
	 * Gets the border along the right after applying this orientation.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return The right border of the oriented tile.
	 */
	public boolean[] getRightBorder(Map<Integer, Tile> tiles) {
		Tile tile = tiles.get(tileID);
		switch(orientation) {
		case STANDARD:
			return tile.getRightBorder();
		case ROT90:
			return reverse(tile.getBottomBorder());
		case ROT180:
			return reverse(tile.getLeftBorder());
		case ROT270:
			return tile.getTopBorder();
		case FLIPHORZ:
			return tile.getLeftBorder();
		case FLIPVERT:
			return reverse(tile.getRightBorder());
		case ROT90UPSIDEDOWN:
			return tile.getBottomBorder();
		case ROT270UPSIDEDOWN:
			return reverse(tile.getTopBorder());
		default:
			throw new IllegalStateException("Unknown orientation.");
		}
	}
	
	/**
	 * Reverses an array of booleans.
	 * 
	 * @param array The original array, which will be modified.
	 * @return The same array, which has been reversed.
	 */
	private boolean[] reverse(boolean[] array) {
		for(int index = 0; index < array.length / 2; index++) {
			boolean temp = array[index];
			array[index] = array[array.length - 1 - index];
			array[array.length - 1 - index] = temp;
		}
		return array;
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj == null) {
			return false;
		}
		if(obj == this) {
			return true;
		}
		if(obj instanceof OrientedTile) {
			OrientedTile other = (OrientedTile)obj;
			return tileID == other.tileID && orientation == other.orientation;
		}
		return false;
	}
	
	@Override
	public int hashCode() {
		return tileID + 43 * orientation.hashCode();
	}
}
