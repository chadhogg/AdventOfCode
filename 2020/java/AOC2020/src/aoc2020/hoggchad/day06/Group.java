package aoc2020.hoggchad.day06;

import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.TreeSet;

/**
 * A group of people who have answered yes/no questions.
 * 
 * @author Chad Hogg
 */
public class Group {

	/** A list of the responses for each member; a response is a list of question letters answered yes. */
	private List<String> members;
	
	/**
	 * Constructs a new, empty group.
	 */
	public Group() {
		members = new ArrayList<String>();
	}
	
	/**
	 * Adds a member to the group.
	 * 
	 * @param member The new member's yes list.
	 */
	public void addMember(String member) {
		members.add(member);
	}
	
	/**
	 * Gets the number of members.
	 * 
	 * @return The number of members.
	 */
	public int getMemberCount() {
		return members.size();
	}
	
	/**
	 * Gets the set of questions answered yes by at least one group member.
	 * 
	 * @return That set.
	 */
	public Set<Character> getAnyYesAnswers() {
		Set<Character> yesses = new TreeSet<>();
		for(String member : members) {
			for(int index = 0; index < member.length(); index++) {
				yesses.add(member.charAt(index));
			}
		}
		return yesses;
	}
	
	/**
	 * Gets the set of questions answered yes by every group member.
	 * 
	 * @return That set.
	 */
	public Set<Character> getAllYesAnswers() {
		Set<Character> possible = new TreeSet<>();
		for(int index = 0; index < members.get(0).length(); index++) {
			possible.add(members.get(0).charAt(index));
		}
		Set<Character> yesses = new TreeSet<>();
		for(Character question : possible) {
			boolean foundInAll = true;
			for(String member : members) {
				if(member.indexOf(question) == -1) {
					foundInAll = false;
				}
			}
			if(foundInAll) {
				yesses.add(question);
			}
		}
		return yesses;
	}
}
