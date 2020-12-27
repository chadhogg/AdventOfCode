package aoc2020.hoggchad.day21;

import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 21.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day21/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Set<Rule> rules = readInput(FILE_PATH);
		Set<String> ingredients = getAllIngredients(rules);
		Set<String> allergens = getAllAlergens(rules);
		System.out.println(part1(rules, ingredients, allergens));
		System.out.println(part2(rules, ingredients, allergens));
	}

	/**
	 * Looks for ingredients that cannot possibly contain allergens.
	 * 
	 * @param rules A set of rules.
	 * @param ingredients A set of ingredients.
	 * @param allergens A set of allergens.
	 * @return The number of times that a proven safe ingredient appears in a rule.
	 */
	private static int part1(Set<Rule> rules, Set<String> ingredients, Set<String> allergens) {
		Set<Map<String, String>> possibleMappings = findPossibleMappings(rules, ingredients, allergens);
		if(possibleMappings.size() != 1) {
			throw new IllegalStateException("It appeared that there would be exactly one possible matching, but there is " + possibleMappings.size());
		}
		Map<String, String> mapping = possibleMappings.iterator().next();
		return countSafeIngredients(rules, ingredients, mapping);
	}
	
	/**
	 * Gets a list of dangerous ingredients.
	 * 
	 * @param rules A set of rules.
	 * @param ingredients A set of ingredients.
	 * @param allergens A set of allergens.
	 * @return The list of dangerous ingredients, separated by commas and ordered alphabetically by the allergen they contain.
	 */
	private static String part2(Set<Rule> rules, Set<String> ingredients, Set<String> allergens) {
		Set<Map<String, String>> possibleMappings = findPossibleMappings(rules, ingredients, allergens);
		if(possibleMappings.size() != 1) {
			throw new IllegalStateException("It appeared that there would be exactly one possible matching, but there is " + possibleMappings.size());
		}
		Map<String, String> mapping = possibleMappings.iterator().next();
		List<String> sortedAllergens = new ArrayList<>(allergens);
		Collections.sort(sortedAllergens);
		StringBuilder sb = new StringBuilder();
		sb.append(mapping.get(sortedAllergens.get(0)));
		for(int index = 1; index < sortedAllergens.size(); index++) {
			sb.append(",");
			sb.append(mapping.get(sortedAllergens.get(index)));
		}
		return sb.toString();
	}
	
	/**
	 * Finds all possible mappings from allergen to [unique] ingredient.
	 * I expected that there might be many, but experience shows there is a unique mapping.
	 * 
	 * @param rules A set of rules.
	 * @param ingredients A set of ingredients.
	 * @param allergens A set of allergens.
	 * @return A set of complete mappings that satisfy every rule.
	 */
	private static Set<Map<String, String>> findPossibleMappings(Set<Rule> rules, Set<String> ingredients, Set<String> allergens) {
		Map<String, String> partial = new HashMap<>();
		return findPossibleMappings(rules, ingredients, allergens, partial);
	}
	
	/**
	 * Finds all possible mappings from allergen to [unique] ingredient.
	 * I expected that there might be many, but experience shows there is a unique mapping.
	 * 
	 * @param rules A set of rules.
	 * @param ingredients A set of ingredients.
	 * @param allergens A set of allergens.
	 * @param partial A partially-complete mapping that should be extended
	 * @return A set of complete mappings that satisfy every rule.
	 */
	private static Set<Map<String, String>> findPossibleMappings(Set<Rule> rules, Set<String> ingredients, Set<String> allergens, Map<String, String> partial) {
		Set<Map<String, String>> possibilities = new HashSet<>();
		if(allergens.isEmpty()) {
			possibilities.add(new HashMap<>(partial));
		}
		else {
			String nextAllergen = allergens.iterator().next();
			allergens.remove(nextAllergen);
			for(String ingredient : ingredients) {
				partial.put(nextAllergen, ingredient);
				if(allRulesSatisfied(rules, partial)) {
					Set<String> otherIngredients = new HashSet<>(ingredients);
					otherIngredients.remove(ingredient);
					possibilities.addAll(findPossibleMappings(rules, otherIngredients, allergens, partial));
				}
				partial.remove(nextAllergen);
			}
			allergens.add(nextAllergen);
		}
		return possibilities;
	}
	
	/**
	 * Counts the number of times that a safe ingredient appears in a rule.
	 * 
	 * @param rules The rules.
	 * @param ingredients The ingredients.
	 * @param mapping A complete mapping of allergens to ingredients.
	 * @return The count.
	 */
	private static int countSafeIngredients(Set<Rule> rules, Set<String> ingredients, Map<String, String> mapping) {
		int count = 0;
		for(String ingredient : ingredients) {
			if(!mapping.containsValue(ingredient)) {
				for(Rule rule : rules) {
					if(rule.getIngredients().contains(ingredient)) {
						count++;
					}
				}
			}
		}
		return count;
	}

	/**
	 * Tests whether or not all rules are satsified by a partial mapping.
	 * 
	 * @param rules The rules.
	 * @param partial A partial mapping of allergen to unique ingredient.
	 * @return True if all rules are satisfied by that partial mapping.
	 */
	private static boolean allRulesSatisfied(Set<Rule> rules, Map<String, String> partial) {
		for(Iterator<Rule> it = rules.iterator(); it.hasNext(); ) {
			Rule rule = it.next();
			if(!rule.isSatisfied(partial)) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * Gets all ingredients that appear in any rule.
	 * 
	 * @param rules The rules.
	 * @return The ingredients.
	 */
	private static Set<String> getAllIngredients(Set<Rule> rules) {
		Set<String> ingredients = new HashSet<>();
		for(Rule rule : rules) {
			ingredients.addAll(rule.getIngredients());
		}
		return ingredients;
	}
	
	/**
	 * Gets all allergens that appear in any rule.
	 * 
	 * @param rules The rules.
	 * @return The allergens.
	 */
	private static Set<String> getAllAlergens(Set<Rule> rules) {
		Set<String> allergens = new HashSet<>();
		for(Rule rule : rules) {
			allergens.addAll(rule.getAllergens());
		}
		return allergens;
	}
	
	/**
	 * Reads the input.
	 * 
	 * @param path The path to the input file.
	 * @return A set of rules.
	 * @throws FileNotFoundException If the input file cannot be found.
	 */
	private static Set<Rule> readInput(String path) throws FileNotFoundException {
		Set<Rule> rules = new HashSet<>();
		Scanner fileScanner = new Scanner(new File(path));
		while(fileScanner.hasNextLine()) {
			rules.add(new Rule(fileScanner.nextLine()));
		}
		fileScanner.close();
		return rules;
	}
}
