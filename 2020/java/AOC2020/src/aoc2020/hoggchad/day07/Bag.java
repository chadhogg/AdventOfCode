package aoc2020.hoggchad.day07;

import java.util.Map;
import java.util.TreeMap;

/**
 * A type of bag.
 * 
 * @author Chad Hogg
 */
public class Bag {
	
	/** A textual description of this bag's color. */
	private String type;
	
	/** The bags that can be held in in. */
	private Map<String, Integer> contents;
	
	/**
	 * Constructs a new bag.
	 * 
	 * @param line A line of text describing the bag and its contents.
	 */
	public Bag(String line) {
		int containIndex = line.indexOf(" bags contain ");
		type = line.substring(0, containIndex);
		String remainder = line.substring(containIndex + " bags contain ".length());
		contents = new TreeMap<>();
		if(!remainder.equals("no other bags.")) {
			while(!remainder.isEmpty()) {
				int commaIndex = remainder.indexOf(',');
				int spaceIndex = remainder.indexOf(' ');
				int bagIndex = remainder.indexOf(" bag");
				int count = Integer.parseInt(remainder.substring(0, spaceIndex));
				contents.put(remainder.substring(spaceIndex + 1, bagIndex), count);
				if(commaIndex == -1) {
					remainder = "";
				}
				else {
					remainder = remainder.substring(commaIndex + 2);
				}
			}
		}
	}
	
	/**
	 * Gets the type of this bag.
	 * 
	 * @return The type of the bag.
	 */
	public String getType() {
		return type;
	}
	
	/**
	 * Tests whether or not this bag can contain another specific bag.
	 * 
	 * @param description The one we would like to contain.
	 * @param allBags A map of names to bags.
	 * @return True if this bag can (recursively) contain the description.
	 */
	public boolean canContain(String description, Map<String, Bag> allBags) {
		for(String content : contents.keySet()) {
			if(content.equals(description)) {
				return true;
			}
			if(allBags.get(content).canContain(description, allBags)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Counts the total number of bags that can be contained within this.
	 * 
	 * @param allBags A map of names to bags.
	 * @return The number of bags this can (recursively) hold.
	 */
	public int countContents(Map<String, Bag> allBags) {
		int count = 0;
		for(String content : contents.keySet()) {
			Bag bag = allBags.get(content);
			count += contents.get(content) * (1 + bag.countContents(allBags));
		}
		return count;
	}
}
