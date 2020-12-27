package aoc2020.hoggchad.day20;

import java.util.Map;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Set;
import java.util.HashSet;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 20.
 * It could use a lot of refactoring, as I introduced classes late in the development cycle and didn't adjust working code.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day20/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Map<Integer, Tile> tiles = readInput();
		System.out.println(part1(tiles));
		System.out.println(part2(tiles));
	}
	
	/**
	 * Finds a way to arrange tiles so that their borders match
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return The product of the IDs of the four corner tiles.
	 */
	private static long part1(Map<Integer, Tile> tiles) {
		OrientedTile[][] solution = solvePuzzle(tiles);
		return (long)(solution[0][0].getTileID()) * solution[0][solution.length - 1].getTileID() * solution[solution.length - 1][0].getTileID() * solution[solution.length - 1][solution.length - 1].getTileID();
	}

	/**
	 * Arranges the tiles into a complete image, then looks for sea monsters in it.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return The number of black pixels not part of a sea monster.
	 */
	private static int part2(Map<Integer, Tile> tiles) {
		OrientedTile[][] solution = solvePuzzle(tiles);
		boolean[][] image = stitchImage(solution, tiles);
		int allTrues = countNonSeamonsterTrues(image, new boolean[image.length][image[0].length]);
		boolean[][] seaMonster = new boolean[3][20];
		seaMonster[0][18] = seaMonster[1][0] = seaMonster[1][5] = seaMonster[1][6] = seaMonster[1][11] = seaMonster[1][12] = seaMonster[1][17] = seaMonster[1][18] = seaMonster[1][19] = seaMonster[2][1] = seaMonster[2][4] = seaMonster[2][7] = seaMonster[2][10] = seaMonster[2][13] = seaMonster[2][16] = true;
		for(Orientation orientation : Orientation.values()) {
			boolean[][] orientedImage = new boolean[image.length][image.length];
			for(int row = 0; row < image.length; row++) {
				for(int col = 0; col < image.length; col++) {
					orientedImage[Orientation.getNewRow(orientation, row, col, image.length, image.length)][Orientation.getNewCol(orientation, row, col, image.length, image.length)] = image[row][col];
				}
			}
			boolean[][] monsters = findCopies(orientedImage, seaMonster);
			int count = countNonSeamonsterTrues(orientedImage, monsters);
			if(count != allTrues) {
				return allTrues - countSeaMonsterTiles(monsters);
			}
		}
		return -1;
	}
	
	private static int countSeaMonsterTiles(boolean[][] monsters) {
		int count = 0;
		for(int row = 0; row < monsters.length; row++) {
			for(int col = 0; col < monsters[0].length; col++) {
				if(monsters[row][col]) {
					count++;
				}
			}
		}
		// Because 1/4 of the pixels in a block are sea monster pixels.  I originally thought all the #s in the block were.
		// I would design the entire system differently with the correct interpretation, but will just use this hack.
		return count / 4;
	}
	
	/**
	 * Orients and places tiles into a grid such that their borders match.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return A 2D array showing the location and orientation of all of the tiles.
	 */
	private static OrientedTile[][] solvePuzzle(Map<Integer, Tile> tiles) {
		// Because we are not putting it anywhere in particular, we can choose an arbitrary tile to start without loss of generality.
		int firstTileID = tiles.keySet().iterator().next();
		// We start with a 1x1 grid, because the recursive function will try expanding in each direction.
		OrientedTile[][] initial = new OrientedTile[1][1];
		// Because of symmetry we can choose an arbitrary orientation for the first tile without loss of generality.
		initial[0][0] = new OrientedTile(firstTileID, Orientation.STANDARD);
		Map<Integer, Set<OrientedTile>> available = buildOrientations(tiles);
		available.remove(firstTileID);
		return solvePuzzle(initial, tiles, available);
	}
	
	/**
	 * Creates a collection containing all orientations of all tiles.
	 * 
	 * @param tiles A map from tile ID to Tile object.
	 * @return A map from tile ID to set of OrientedTile objects.
	 */
	private static Map<Integer, Set<OrientedTile>> buildOrientations(Map<Integer, Tile> tiles) {
		Map<Integer, Set<OrientedTile>> allOrientations = new HashMap<>();
		for(int tileID : tiles.keySet()) {
			Set<OrientedTile> tileOrientations = new HashSet<>();
			for(Orientation orientation : Orientation.values()) {
				tileOrientations.add(new OrientedTile(tileID, orientation));
			}
			allOrientations.put(tileID, tileOrientations);
		}
		return allOrientations;
	}

	/**
	 * Tries to expand a partially solved puzzle.
	 * 
	 * @param soFar A grid of oriented tiles already placed, with nulls where no tile yet exists.
	 * @param tiles A map from tile ID to Tile object, for all tiles that exist.
	 * @param available A map from tile ID to set of OrientedTile object, for tiles that have not yet been placed in the puzzle.
	 * @return A grid containing a complete puzzle solution, or null if no extension of the partial solution works.
	 */
	private static OrientedTile[][] solvePuzzle(OrientedTile[][] soFar, Map<Integer, Tile> tiles, Map<Integer, Set<OrientedTile>> available) {
		// Doing this one first reduces the branching factor.  In the extreme, it can allow us to backtrack immediately.
		Position mostConstrained = getMostConstrainedPosition(soFar, tiles, available);
		if(mostConstrained.row == -1) {
			// If there are no empty spaces, either the puzzle is solved or we need to expand the grid.
			if(soFar.length * soFar[0].length == tiles.size()) {
				return soFar;
			}
			else {
				return tryGrowing(soFar, tiles, available);
			}
		}
		else {
			// Try each oriented tile that could go in that position, one at a time.
			Set<OrientedTile> options = getOptionsForPosition(soFar, mostConstrained, tiles, available);
			for(OrientedTile choice : options) {
				soFar[mostConstrained.row][mostConstrained.col] = choice;
				Set<OrientedTile> thatTile = available.remove(choice.getTileID());
				OrientedTile[][] result = solvePuzzle(soFar, tiles, available);
				// If any works, we have our solution.
				if(result != null) {
					return result;
				}
				// Un-do any changes so that we can properly backtrack.
				available.put(choice.getTileID(), thatTile);
				soFar[mostConstrained.row][mostConstrained.col] = null; 
			}
		}
		// We were not able to complete the partial solution.
		return null;
	}
	
	/**
	 * Searches a grid to find the empty position on it with the fewest available oriented tiles that could go there.
	 * 
	 * @param soFar The existing grid.
	 * @param tiles A map from tile ID to Tile object, for all tiles.
	 * @param available A map from tile ID to set of OrientedTile object, for only tiles that have not yet been used.
	 * @return The position on the grid with the fewest options, or the position (-1,-1) if there are no empty positions.
	 */
	private static Position getMostConstrainedPosition(OrientedTile[][] soFar, Map<Integer, Tile> tiles, Map<Integer, Set<OrientedTile>> available) {
		Position mostConstrained = new Position(-1, -1);
		int choices = Integer.MAX_VALUE;
		for(int row = 0; row < soFar.length; row++) {
			for(int col = 0; col < soFar[0].length; col++) {
				if(soFar[row][col] == null) {
					Set<OrientedTile> options = getOptionsForPosition(soFar, new Position(row, col), tiles, available);
					if(options.size() < choices) {
						choices = options.size();
						mostConstrained.row = row;
						mostConstrained.col = col;
					}
				}
			}
		}
		return mostConstrained;
	}
	
	/**
	 * Gets the set of oriented tiles that could fit in a position.
	 * 
	 * @param soFar The partially-filled grid.
	 * @param pos A position on the gird that contains null.
	 * @param tiles A map from tile ID to Tile object, for all tiles.
	 * @param available A map from tile ID to set of OrientedTile objects, for only tiles that have not yet been used.
	 * @return The set of all oriented tiles that could go in that position.
	 */
	private static Set<OrientedTile> getOptionsForPosition(OrientedTile[][] soFar, Position pos, Map<Integer, Tile> tiles, Map<Integer, Set<OrientedTile>> available) {
		Set<OrientedTile> options = new HashSet<>();
		boolean[] aboveBottomBorder = null;
		if(pos.row > 0 && soFar[pos.row - 1][pos.col] != null) {
			aboveBottomBorder = soFar[pos.row - 1][pos.col].getBottomBorder(tiles);
		}
		boolean[] belowTopBorder = null;
		if(pos.row < soFar.length - 1 && soFar[pos.row + 1][pos.col] != null) {
			belowTopBorder = soFar[pos.row + 1][pos.col].getTopBorder(tiles);
		}
		boolean[] beforeRightBorder = null;
		if(pos.col > 0 && soFar[pos.row][pos.col - 1] != null) {
			beforeRightBorder = soFar[pos.row][pos.col - 1].getRightBorder(tiles);
		}
		boolean[] afterLeftBorder = null;
		if(pos.col < soFar[0].length - 1 && soFar[pos.row][pos.col + 1] != null) {
			afterLeftBorder = soFar[pos.row][pos.col + 1].getLeftBorder(tiles);
		}
		for(int tileID : available.keySet()) {
			Set<OrientedTile> orientations = available.get(tileID);
			for(OrientedTile orientation : orientations) {
				boolean works = true;
				if(works && aboveBottomBorder != null) {
					if(!Arrays.equals(aboveBottomBorder, orientation.getTopBorder(tiles))) {
						works = false;
					}
				}
				if(works && belowTopBorder != null) {
					if(!Arrays.equals(belowTopBorder, orientation.getBottomBorder(tiles))) {
						works = false;
					}
				}
				if(works && beforeRightBorder != null) {
					if(!Arrays.equals(beforeRightBorder, orientation.getLeftBorder(tiles))) {
						works = false;
					}
				}
				if(works && afterLeftBorder != null) {
					if(!Arrays.equals(afterLeftBorder, orientation.getRightBorder(tiles))) {
						works = false;
					}
				}
				if(works) {
					options.add(orientation);
				}
			}
		}
		return options;
	}

	/**
	 * Try expanding a partially solved puzzle by growing the grid in one direction or another.
	 * 
	 * @param soFar The existing puzzle grid, which is full.
	 * @param tiles A map from tile ID to Tile object, for all tiles.
	 * @param available A map from tile ID to OrientedTile object, for only unused tiles.
	 * @return A solution that involves an expanded grid, or null if no solution uses the existing grid.
	 */
	private static OrientedTile[][] tryGrowing(OrientedTile[][] soFar, Map<Integer, Tile> tiles, Map<Integer, Set<OrientedTile>> available) {
		OrientedTile[][] result = null;
		if(soFar.length * soFar.length < tiles.size() && result == null) {
			// Try adding a new row on top of the grid.
			OrientedTile[][] addTop = new OrientedTile[soFar.length + 1][soFar[0].length];
			for(int row = 0; row < soFar.length; row++) {
				for(int col = 0; col < soFar[0].length; col++) {
					addTop[row + 1][col] = soFar[row][col];
				}
			}
			result = solvePuzzle(addTop, tiles, available);
		}
		if(soFar.length * soFar.length < tiles.size() && result == null) {
			// Try adding a new row on the bottom of the grid.
			OrientedTile[][] addBottom = new OrientedTile[soFar.length + 1][soFar[0].length];
			for(int row = 0; row < soFar.length; row++) {
				for(int col = 0; col < soFar[0].length; col++) {
					addBottom[row][col] = soFar[row][col];
				}
			}
			result = solvePuzzle(addBottom, tiles, available);
		}
		if(soFar[0].length * soFar[0].length < tiles.size() && result == null) {
			// Try adding a new column on the left of the grid.
			OrientedTile[][] addLeft = new OrientedTile[soFar.length][soFar[0].length + 1];
			for(int row = 0; row < soFar.length; row++) {
				for(int col = 0; col < soFar[0].length; col++) {
					addLeft[row][col + 1] = soFar[row][col];
				}
			}
			result = solvePuzzle(addLeft, tiles, available);
		}
		if(soFar[0].length * soFar[0].length < tiles.size() && result == null) {
			// Try adding a new column on the right of the grid.
			OrientedTile[][] addRight = new OrientedTile[soFar.length][soFar[0].length + 1];
			for(int row = 0; row < soFar.length; row++) {
				for(int col = 0; col < soFar[0].length; col++) {
					addRight[row][col] = soFar[row][col];
				}
			}
			result = solvePuzzle(addRight, tiles, available);
		}
		return result;
	}
	
	/**
	 * Puts an image of tiles together, throwing away their borders.
	 * 
	 * @param puzzle A 2D array of oriented tiles.
	 * @param tiles A map from tile ID to Tile object.
	 * @return A 2D array of pixels.
	 */
	private static boolean[][] stitchImage(OrientedTile[][] puzzle, Map<Integer, Tile> tiles) {
		Tile arbitraryTile = tiles.values().iterator().next();
		int tileSize = arbitraryTile.getHeight();
		boolean[][] image = new boolean[puzzle.length * (tileSize - 2)][puzzle[0].length * (tileSize - 2)];
		for(int outRow = 0; outRow < puzzle.length; outRow++) {
			for(int outCol = 0; outCol < puzzle[0].length; outCol++) {
				Tile baseTile = tiles.get(puzzle[outRow][outCol].getTileID());
				Tile orientedTile = new Tile(baseTile.getTileID(), baseTile.getImage().afterOrientation(puzzle[outRow][outCol].getOrientation()));
				for(int inRow = 0; inRow < tileSize - 2; inRow++) {
					for(int inCol = 0; inCol < tileSize - 2; inCol++) {
						image[outRow * (tileSize - 2) + inRow][outCol * (tileSize - 2) + inCol] = orientedTile.getValue(inRow + 1, inCol + 1);
					}
				}
			}
		}
		return image;
	}
	
	/**
	 * Finds instances of a partially-specified subimage within a larger image.
	 * 
	 * @param wholeImage The entire black and white image.
	 * @param subImage The subimage.  For matches, black areas of this must also be black on the whole image.
	 * @return A 2D array that is true where that pixel is part of a subimage copy.
	 */
	private static boolean[][] findCopies(boolean[][] wholeImage, boolean[][] subImage) {
		boolean[][] seaMonsterParts = new boolean[wholeImage.length][wholeImage[0].length];
		for(int startRow = 0; startRow < wholeImage.length - subImage.length; startRow ++) {
			for(int startCol = 0; startCol < wholeImage[0].length - subImage[0].length; startCol++) {
				boolean possibleMatch = true;
				for(int rowOffset = 0; rowOffset < subImage.length && possibleMatch; rowOffset++) {
					for(int colOffset = 0; colOffset < subImage[0].length && possibleMatch; colOffset++) {
						if(subImage[rowOffset][colOffset] && !wholeImage[startRow + rowOffset][startCol + colOffset]) {
							possibleMatch = false;
						}
					}
				}
				if(possibleMatch) {
					for(int rowOffset = 0; rowOffset < subImage.length; rowOffset++) {
						for(int colOffset = 0; colOffset < subImage[0].length; colOffset++) {
							seaMonsterParts[startRow + rowOffset][startCol + colOffset] = true;
						}
					}
				}
			}
		}
		return seaMonsterParts;
	}

	
	/**
	 * Counts the number of pixels in an image that are black and not marked as part of a subimage copy.
	 * This turned out not to be useful, because I misunderstood exactly what we were supposed to be counting.
	 * 
	 * @param image The image.
	 * @param monsterParts A 2D array of whether or not the pixel is part of a monster.
	 * @return The number of non-monster black pixels.
	 */
	private static int countNonSeamonsterTrues(boolean[][] image, boolean[][] monsterParts) {
		int count = 0;
		for(int row = 0; row < image.length; row++) {
			for(int col = 0; col < image[0].length; col++) {
				if(image[row][col] && !monsterParts[row][col]) {
					count++;
				}
			}
		}
		return count;
	}

	/**
	 * Reads a set of tiles from an input file.
	 * 
	 * @return A map from tile ID to tile.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static Map<Integer, Tile> readInput() throws FileNotFoundException {
		final int TILE_HEIGHT = 10;
		final int TILE_WIDTH = 10;
		Map<Integer, Tile> tiles = new HashMap<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			String header = fileScanner.nextLine();
			if(!header.isEmpty()) {
				int tileID = Integer.parseInt(header.substring(header.indexOf(' ') + 1, header.indexOf(':')));
				boolean[][] data = new boolean[TILE_HEIGHT][TILE_WIDTH];
				for(int row = 0; row < TILE_HEIGHT; row++) {
					String line = fileScanner.nextLine();
					if(line.length() != TILE_WIDTH) {
						throw new IllegalStateException("Unexpected line length: " + line);
					}
					for(int col = 0; col < TILE_WIDTH; col++) {
						if(line.charAt(col) == '#') {
							data[row][col] = true;
						}
						else if(line.charAt(col) == '.') {
							data[row][col] = false;
						}
						else {
							throw new IllegalStateException("Unexpected character: " + line.charAt(col));
						}
					}
				}
				tiles.put(tileID, new Tile(tileID, data));
			}
		}
		fileScanner.close();
		return tiles;
	}
	
	/**
	 * Converts a grid to a string, for debugging.
	 * 
	 * @param puzzle The grid.
	 * @return A string showing which tiles are at which locations in the grid.
	 */
	private static String puzzleToString(OrientedTile[][] puzzle) {
		StringBuilder output = new StringBuilder();
		for(int row = 0; row < puzzle.length; row++) {
			for(int col = 0; col < puzzle[0].length; col++) {
				if(puzzle[row][col] != null) {
					output.append(puzzle[row][col].getTileID());
				}
				else {
					output.append("null");
				}
				output.append(" ");
			}
			output.append("\n");
		}
		return output.toString();
	}
	
}
