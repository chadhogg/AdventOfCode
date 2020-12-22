package aoc2020.hoggchad.day16;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 16.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day16/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Field> fields = new ArrayList<>();
		List<Integer> myTicket = null;
		List<List<Integer>> otherTickets = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		boolean finishedFields = false;
		boolean finishedMyTicket = false;
		while(fileScanner.hasNextLine()) {
			String line = fileScanner.nextLine();
			if(!finishedFields) {
				if(!line.isEmpty()) {
					fields.add(new Field(line));
				}
				else {
					finishedFields = true;
				}
			}
			else if(!finishedMyTicket) {
				if(!line.startsWith("your ticket:")) {
					throw new IllegalStateException("Unexpected string: " + line);
				}
				line = fileScanner.nextLine();
				myTicket = readTicket(line);
				line = fileScanner.nextLine();
				if(!line.isEmpty()) {
					throw new IllegalStateException("Unexpected string: " + line);
				}
				line = fileScanner.nextLine();
				if(!line.startsWith("nearby tickets:")) {
					throw new IllegalStateException("Unexpected string: " + line);
				}
				finishedMyTicket = true;
			}
			else {
				otherTickets.add(readTicket(line));
			}
		}
		fileScanner.close();
		
		System.out.println(part1(fields, otherTickets));
		System.out.println(part2(fields, myTicket, otherTickets));
	}
	
	/**
	 * Discards obviously invalid tickets and sums the values on them that make them invalid.
	 * 
	 * @param fields A list of fields.
	 * @param otherTickets A list of tickets that might be valid.
	 * @return The sum of the values on tickets that could not be valid for any field.
	 */
	private static long part1(List<Field> fields, List<List<Integer>> otherTickets) {
		long sum = 0;
		List<List<Integer>> invalidTickets = new ArrayList<>(); 
		for(List<Integer> ticket : otherTickets) {
			boolean anyInvalidField = false;
			for(int value : ticket) {
				boolean isValid = false;
				for(int fieldIndex = 0; fieldIndex < fields.size() && !isValid; fieldIndex++) {
					if(fields.get(fieldIndex).isValid(value)) {
						isValid = true;
					}
				}
				if(!isValid) {
					sum += value;
					anyInvalidField = true;
				}
			}
			if(anyInvalidField) {
				invalidTickets.add(ticket);
			}
		}
		otherTickets.removeAll(invalidTickets);
		return sum;
	}
	
	/**
	 * Determines which fields appear where on each ticket.
	 * 
	 * @param fields A list of fields.
	 * @param myTicket A ticket.
	 * @param validOtherTickets A list of valid tickets.
	 * @return The product of all fields whose names start with "departure" on my ticket.
	 */
	private static long part2(List<Field> fields, List<Integer> myTicket, List<List<Integer>> validOtherTickets) {
		// Start by finding for each field the set of possible positions they could be in.
		Map<String, Set<Integer>> fieldNameToPossiblePositions = new HashMap<>();
		Set<Integer> allPositions = new HashSet<>();
		for(int i = 0; i < myTicket.size(); i++) {
			allPositions.add(i);
		}
		for(Field field : fields) {
			fieldNameToPossiblePositions.put(field.getName(), new HashSet<>(allPositions));
		}
		for(Field field : fields) {
			Set<Integer> fieldSet = fieldNameToPossiblePositions.get(field.getName());
			for(List<Integer> ticket : validOtherTickets) {
				for(int index = 0; index < ticket.size(); index++) {
					if(!field.isValid(ticket.get(index))) {
						fieldSet.remove(index);
					}
				}
			}
		}
		// Hope that there is one field that can only be in one position, winnow it out, and repeat.
		Map<String, Integer> finalizedPositions = new HashMap<>();
		while(!fieldNameToPossiblePositions.isEmpty()) {
			for(String key : fieldNameToPossiblePositions.keySet()) {
				Set<Integer> fieldSet = fieldNameToPossiblePositions.get(key);
				if(fieldSet.size() == 1) {
					int value = fieldSet.iterator().next();
					finalizedPositions.put(key, value);
					fieldNameToPossiblePositions.remove(key);
					for(String key2 : fieldNameToPossiblePositions.keySet()) {
						fieldNameToPossiblePositions.get(key2).remove(value);
					}
					break;
				}
			}
		}
		// Compute product.
		long product = 1L;
		for(String fieldName : finalizedPositions.keySet()) {
			if(fieldName.startsWith("departure")) {
				product *= myTicket.get(finalizedPositions.get(fieldName));
			}
		}
		return product;
	}
	
	/**
	 * Reads a ticket into a list of integers.
	 * 
	 * @param line A string formatted like "11,12,13".
	 * @return A list of the integers in the line.
	 */
	private static List<Integer> readTicket(String line) {
		List<Integer> ticket = new ArrayList<>();
		for(String value : line.split(",")) {
			ticket.add(Integer.parseInt(value));
		}
		return ticket;
	}
}
