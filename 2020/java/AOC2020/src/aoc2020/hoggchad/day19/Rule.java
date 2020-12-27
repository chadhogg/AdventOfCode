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
	
	/** The nonterminal that can be replaced. */
	private String head;
	/** A list of replacement strings. */
	private List<List<String>> options;

	/**
	 * Constructs a rule from a string like '4: 4 5 6 | "a"'.
	 * 
	 * @param line The string.
	 */
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
	
	/**
	 * Tests whether or not a token is a terminal.
	 * 
	 * @param token The token.
	 * @return True if the token is a terminal.
	 */
	public static boolean isTerminal(String token) {
		return token.charAt(0) == '\"';
	}
	
	/**
	 * Tests whether or not a token is a nonterminal.
	 * 
	 * @param token The token.
	 * @return True if the token is a terminal.
	 */
	public static boolean isNonTerminal(String token) {
		return !isTerminal(token);
	}
	
	/**
	 * Gets the index of the first token in a list that is a nonterminal.
	 * 
	 * @param string The list of tokens.
	 * @return The index, or -1 if there are no nonterminals.
	 */
	public static int indexOfFirstNonTerminal(List<String> string) {
		for(int index = 0; index < string.size(); index++) {
			if(isNonTerminal(string.get(index))) {
				return index;
			}
		}
		return -1;
	}
	
	/**
	 * Gets the head of the rule.
	 * 
	 * @return The head of the rule.
	 */
	public String getHead() {
		return head;
	}
	
	/**
	 * Gets the replacements for the rule.
	 * 
	 * @return The replacements.
	 */
	public List<List<String>> getOptions() {
		return Collections.unmodifiableList(options);
	}
}
