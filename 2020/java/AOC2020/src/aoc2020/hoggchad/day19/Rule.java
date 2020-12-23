package aoc2020.hoggchad.day19;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

/**
 * A production rule in a context-free grammar.
 * 
 * @author Chad Hogg
 */
public class Rule {
	
	private String head;
	private List<List<String>> options;

	public Rule(String line) {
		int colonPos = line.indexOf(":");
		head = line.substring(0, colonPos);
		options = new ArrayList<>();
		String rest = line.substring(colonPos + 2);
		String[] productions = rest.split("\\|");
		for(String production : productions) {
			String[] tokens = production.trim().split(" ");
			options.add(Arrays.asList(tokens));
		}
	}
	
	public static boolean isTerminal(String token) {
		return token.charAt(0) == '\"';
	}
	
	public static boolean isNonTerminal(String token) {
		return !isTerminal(token);
	}
	
	public static int indexOfFirstNonTerminal(List<String> string) {
		for(int index = 0; index < string.size(); index++) {
			if(isNonTerminal(string.get(index))) {
				return index;
			}
		}
		return -1;
	}
	
	public String getHead() {
		return head;
	}
	
	public List<List<String>> getOptions() {
		return Collections.unmodifiableList(options);
	}
}
