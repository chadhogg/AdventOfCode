package aoc2020.hoggchad.day23;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

/**
 * A much larger circle of cups.
 * 
 * The idea from Cups is now prohibitively slow.
 * Switching to LinkedList doesn't help that much, because you still need to do lookups.
 * Adding a cup label -> index map doesn't help much either, because there's still no fast way to jump to the node at index X.
 * Some kind of cup label -> iterator map seems like a better idea, but I couldn't figure out how to deal with those iterators becoming invalid.
 * So this idea dispenses with storing the list as a list at all, but instead stores the "links" in a map where it is easy to jump to an index.
 * 
 * @author Chad Hogg
 */
public class CupsV2 {

	/** A map from the label of a cup to the label of the cup that comes after it when traversing the circle clockwise. */
	private Map<Integer, Integer> cupToNextCup;
	/** The label on the "current" cup. */
	private int currentCup;
	
	/**
	 * Constructs a cup circle.
	 * 
	 * @param line A line like "389125467" showing the order of the first 9 cups.
	 */
	public CupsV2(String line) {
		List<Integer> cups = new ArrayList<>();
		for(String num : line.split("")) {
			cups.add(Integer.parseInt(num));
		}
		while(cups.size() < 1000000) {
			cups.add(cups.size() + 1);
		}
		currentCup = cups.get(0);
		cupToNextCup = new HashMap<>();
		for(int index = 0; index < cups.size() - 1; index++) {
			cupToNextCup.put(cups.get(index), cups.get(index + 1));
		}
		cupToNextCup.put(cups.get(cups.size() - 1), cups.get(0));
	}
	
	/**
	 * Makes a move in the game.
	 */
	public void move() {
		// Pick up the three cups that come after the current cup.
		List<Integer> pickedUp = new ArrayList<>();
		pickedUp.add(cupToNextCup.get(currentCup));
		pickedUp.add(cupToNextCup.get(pickedUp.get(0)));
		pickedUp.add(cupToNextCup.get(pickedUp.get(1)));
		cupToNextCup.put(currentCup, cupToNextCup.get(pickedUp.get(2)));
		cupToNextCup.remove(pickedUp.get(0));
		cupToNextCup.remove(pickedUp.get(1));
		cupToNextCup.remove(pickedUp.get(2));

		// The destination cup has the label before the curent cup's label.
		int destinationCup = currentCup == 1 ? cupToNextCup.size() + 3: currentCup - 1;
		while(pickedUp.contains(destinationCup)) {
			destinationCup = destinationCup == 1 ? cupToNextCup.size() + 3: destinationCup - 1;
		}

		// Put the picked up cups after the destination cup.
		cupToNextCup.put(pickedUp.get(2), cupToNextCup.get(destinationCup));
		cupToNextCup.put(pickedUp.get(1), pickedUp.get(2));
		cupToNextCup.put(pickedUp.get(0), pickedUp.get(1));
		cupToNextCup.put(destinationCup, pickedUp.get(0));
		
		// The new current cup is the one that comes after the current cup.
		currentCup = cupToNextCup.get(currentCup);
	}
	
	/**
	 * Computes the product of the labels of the two cups that come after the cup labeled 1 when traversing the circle clockwise.
	 * 
	 * @return That product.
	 */
	public long productAfter1() {
		return (long)(cupToNextCup.get(1)) * cupToNextCup.get(cupToNextCup.get(1));
	}

	/**
	 * Makes a string representation of the cup circle, for debugging purposes.
	 * 
	 * @return A string like "cups: (8) 3 5 2 1 4 7 6 9".
	 */
	private String cupsToString() {
		StringBuilder sb = new StringBuilder();
		sb.append("cups: (" + currentCup + ")");
		int nextCup = cupToNextCup.get(currentCup);
		while(nextCup != currentCup) {
			sb.append(" " + nextCup);
			nextCup = cupToNextCup.get(nextCup);
		}
		return sb.toString();
	}
}
