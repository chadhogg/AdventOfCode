package aoc2020.hoggchad.day21;

import java.util.Set;
import java.util.Map;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;

/**
 * A relationship between a set of ingredients and a set of allergens.
 * Exactly one ingredient must contain each allergen.
 * 
 * @author Chad Hogg
 */
public class Rule {

	/** The set of ingredients. */
	private final Set<String> ingredients;
	/** The set of allergens. */
	private final Set<String> allergens;
	
	/**
	 * Constructs a rule.
	 * 
	 * @param line A string of the format "mxmxvkd kfcds sqjhc nhms (contains dairy, fish)".
	 */
	public Rule(String line) {
		int lparen = line.indexOf('(');
		String firstHalf = line.substring(0, lparen);
		String secondHalf = line.substring(lparen + 10, line.length() - 1);
		ingredients = new HashSet<>();
		allergens = new HashSet<>();
		ingredients.addAll(Arrays.asList(firstHalf.split(" ")));
		allergens.addAll(Arrays.asList(secondHalf.split(", ")));
	}
	
	/**
	 * Gets the ingredients.
	 * 
	 * @return The ingredients.
	 */
	public Set<String> getIngredients() {
		return Collections.unmodifiableSet(ingredients);
	}
	
	/**
	 * Gets the allergens.
	 * 
	 * @return The allergens.
	 */
	public Set<String> getAllergens() {
		return Collections.unmodifiableSet(allergens);
	}
	
	/**
	 * Checks whether or not this rule is satisfied by a partial map of allergens to the (unique) ingredients that contain them.
	 * 
	 * @param mapping A map of allergen -> ingredient, which may not be complete.
	 * @return True if, for each allergen that this rule contains and that exists in the mapping, we have the mapped ingredient.
	 */
	public boolean isSatisfied(Map<String, String> mapping) {
		for(String allergen : allergens) {
			if(mapping.containsKey(allergen)) {
				if(!ingredients.contains(mapping.get(allergen))) {
					return false;
				}
			}
		}
		return true;
	}
	
	@Override
	public String toString() {
		return ingredients.toString() + " -> " + allergens.toString();
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj == null) {
			return false;
		}
		else if(obj.getClass() == this.getClass()) {
			Rule other = (Rule)obj;
			return ingredients.equals(other.ingredients) && allergens.equals(other.allergens);
		}
		else {
			return false;
		}
	}
	
	@Override
	public int hashCode() {
		return ingredients.hashCode() + 31 * allergens.hashCode();
	}
}
