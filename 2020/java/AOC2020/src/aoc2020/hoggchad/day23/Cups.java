package aoc2020.hoggchad.day23;

import java.util.List;
import java.util.ArrayList;

/**
 * A circle of cups labeled with the numbers 1-9.
 * 
 * This isn't very efficient, but when your list is only 9 elements long I'll do as many linear time operations as you'd like.
 * 
 * @author Chad Hogg
 */
public class Cups {

	/** The cups. */
	private List<Integer> cups;
	/** Which is "current" in the game. */
	private int currentCup;
	/** The move number -- just for matching the example printing. */
	private int moveNum;
	
	/**
	 * Constructs a Cups circle.
	 * 
	 * @param line A string like "389125467".
	 */
	public Cups(String line) {
		cups = new ArrayList<>();
		for(String num : line.split("")) {
			cups.add(Integer.parseInt(num));
		}
		currentCup = cups.get(0);
		moveNum = 0;
	}
	
	/**
	 * Makes one move in the game.
	 */
	public void move() {
		// Some initial printing.
		moveNum++;
		System.out.println("-- move " + moveNum + " --");
		System.out.print("cups: ");
		for(int cup : cups) {
			if(cup == currentCup) {
				System.out.print("(" + cup + ") ");
			}
			else {
				System.out.print(cup + " ");
			}
		}
		
		// Pick up the three cups that come after the current cup.
		List<Integer> pickedUp = new ArrayList<>();
		int removeIndex = cups.indexOf(currentCup) + 1;
		pickedUp.add(cups.remove(removeIndex >= cups.size() ? 0 : removeIndex));
		pickedUp.add(cups.remove(removeIndex >= cups.size() ? 0 : removeIndex));
		pickedUp.add(cups.remove(removeIndex >= cups.size() ? 0 : removeIndex));
		System.out.println("\npick up: " + pickedUp.get(0) + ", " + pickedUp.get(1) + ", " + pickedUp.get(2));

		// The destination cup is the one whose label comes before the current cup's label.
		int destinationCup = currentCup == 1 ? cups.size() + 3: currentCup - 1;
		int destinationIndex = cups.indexOf(destinationCup);
		while(destinationIndex == -1) {
			destinationCup = destinationCup == 1 ? cups.size() + 3: destinationCup - 1;
			destinationIndex = cups.indexOf(destinationCup);
		}
		System.out.println("destination: " + destinationCup + "\n");

		// Put the picked-up cups after the destination cup.
		cups.add(destinationIndex + 1, pickedUp.get(2));
		cups.add(destinationIndex + 1, pickedUp.get(1));
		cups.add(destinationIndex + 1, pickedUp.get(0));

		// The new current cup is the one before the current cup.
		int currentCupIndex = (cups.indexOf(currentCup) + 1) % cups.size();
		currentCup = cups.get(currentCupIndex);
	}
	
	/**
	 * Gets a string showing the positions of the other cups after the cup labeled 1.
	 * 
	 * @return That string.
	 */
	public String getOrderAfter1() {
		int index = cups.indexOf(1);
		String result = "";
		for(int count = 0; count < cups.size() - 1; count++) {
			index = (index + 1) % cups.size();
			result += cups.get(index);
		}
		return result;
	}
	
}
