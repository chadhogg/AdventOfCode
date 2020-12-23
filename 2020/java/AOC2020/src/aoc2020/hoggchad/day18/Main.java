package aoc2020.hoggchad.day18;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 18.
 * We don't need no stinkin' shift-reduce parsers!
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day18/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<String> lines = readInput();
		System.out.println(part1(lines));
		System.out.println(part2(lines));
	}
	
	/**
	 * Evaluates many expressions under the assumption that addition and multiplication have equal precedence.
	 * 
	 * @param lines A list of expressions.
	 * @return The sum of the values of the expressions.
	 */
	private static long part1(List<String> lines) {
		long sum = 0L;
		for(String line : lines) {
			sum += solveExpression(line);
		}
		return sum;
	}
	
	/**
	 * Evaluates many expressions under the assumption that addition has higher precedence than multiplication.
	 * 
	 * @param lines A list of expressions.
	 * @return The sum of the values of the expressions.
	 */
	private static long part2(List<String> lines) {
		long sum = 0L;
		for(String line : lines) {
			sum += solveExpression2(line);
		}
		return sum;
	}

	/**
	 * Evaluates a mathematical expression involving only addition and multiplication, with equal precedence.
	 * 
	 * @param exp A string containing the expression.
	 * @return Its value.
	 */
	private static long solveExpression(String exp) {
		int lastOpenIndex = exp.lastIndexOf('(');
		if(lastOpenIndex != -1) {
			// If there are any subexpressions, solve one that does not contain any subexpression, replace subexpression with result, and then evaluate.
			int matchingCloseIndex = exp.indexOf(')', lastOpenIndex);
			long subResult = solveExpression(exp.substring(lastOpenIndex + 1, matchingCloseIndex));
			return solveExpression(exp.substring(0, lastOpenIndex) + subResult + exp.substring(matchingCloseIndex + 1));
		}
		else {
			// No subexpressions and no precedence, so just process operators left-to-right.
			String[] tokens = exp.split(" ");
			long result = Long.parseLong(tokens[0]);
			for(int index = 1; index < tokens.length; index += 2) {
				if(tokens[index].equals("+")) {
					result += Long.parseLong(tokens[index + 1]);
				}
				else if(tokens[index].equals("*")) {
					result *= Long.parseLong(tokens[index + 1]);
				}
				else {
					throw new IllegalStateException("Unexpected token: " + tokens[index]);
				}
			}
			return result;
		}
	}
	
	/**
	 * Evaluates a mathematical expression involving only addition and multiplication, with addition having higher precedence.
	 * 
	 * @param exp A string containing the expression.
	 * @return Its value.
	 */
	private static long solveExpression2(String exp) {
		int lastOpenIndex = exp.lastIndexOf('(');
		if(lastOpenIndex != -1) {
			// If there are any subexpressions, solve one that does not contain any subexpression, replace subexpression with result, and then evaluate.
			int matchingCloseIndex = exp.indexOf(')', lastOpenIndex);
			long subResult = solveExpression2(exp.substring(lastOpenIndex + 1, matchingCloseIndex));
			return solveExpression2(exp.substring(0, lastOpenIndex) + subResult + exp.substring(matchingCloseIndex + 1));
		}
		else {
			// No subexpressions; do all addition followed by all multiplication.
			List<String> tokens = new ArrayList<>(Arrays.asList(exp.split(" ")));
			for(int index = 0; index < tokens.size(); index++) {
				if(tokens.get(index).equals("+")) {
					long result = Long.parseLong(tokens.get(index - 1)) + Long.parseLong(tokens.get(index + 1));
					tokens.set(index - 1, Long.toString(result));
					tokens.remove(index);
					tokens.remove(index);
					index--;
				}
			}
			for(int index = 0; index < tokens.size(); index++) {
				if(tokens.get(index).equals("*")) {
					long result = Long.parseLong(tokens.get(index - 1)) * Long.parseLong(tokens.get(index + 1));
					tokens.set(index - 1, Long.toString(result));
					tokens.remove(index);
					tokens.remove(index);
					index--;
				}
			}
			if(tokens.size() != 1) {
				throw new IllegalStateException("Bad list: " + tokens);
			}
			return Long.parseLong(tokens.get(0));
		}		
	}
	
	/**
	 * Reads the input.
	 * 
	 * @return A list of strings, each of which contains an expression.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static List<String> readInput() throws FileNotFoundException {
		List<String> expressions = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		while(fileScanner.hasNextLine()) {
			expressions.add(fileScanner.nextLine());
		}
		fileScanner.close();
		return expressions;
	}
}
