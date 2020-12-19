package aoc2020.hoggchad.day03;

/**
 * A map of trees / not trees that repeats infinitely to the right.
 * 
 * @author Chad Hogg
 */
public class RepeatingMap {
	
	/** The map, in which true indicates a tree. */
	private boolean[][] treeMap;

	/**
	 * Constructs a map by copying the underlying 2-D array.
	 * 
	 * @param treeMap A 2-d array in which true indicates the presence of a tree.
	 */
	public RepeatingMap(boolean[][] treeMap) {
		this.treeMap = new boolean[treeMap.length][treeMap[0].length];
		for(int row = 0; row < treeMap.length; row++) {
			for(int col = 0; col < treeMap[row].length; col++) {
				this.treeMap[row][col] = treeMap[row][col];
			}
		}
	}
	
	/**
	 * Checks whether or not a location on the map contains a tree.
	 * 
	 * @param row The row, which must be in the range [0, getHeight() - 1];
	 * @param col The column, which must be at least 0.
	 * @return True if that location contains a tree.
	 */
	public boolean isTree(int row, int col) {
		if(row < 0 || row > treeMap.length - 1 || col < 0) {
			throw new IllegalArgumentException("Invalid row or col.");
		}
		return treeMap[row][col % treeMap[row].length];
	}
	
	/**
	 * Gets the height of the map.
	 * 
	 * @return The number of rows in the map.
	 */
	public int getHeight() {
		return treeMap.length;
	}
}
