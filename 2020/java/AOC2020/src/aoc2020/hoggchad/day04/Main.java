package aoc2020.hoggchad.day04;

import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.Map;
import java.util.TreeMap;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, Day 04.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day04/input";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		List<Map<String, String>> passports = readInput();
		System.out.println(part1(passports));
		System.out.println(part2(passports));
	}
	
	/**
	 * Counts how many passports contain fields "byr", "iyr", "eyr", "hgt", "hcl", "ecl", and "pid".
	 * 
	 * @param passports A list of passports (each of which is a key-value map of fields).
	 * @return The number that have each required field.
	 */
	private static int part1(List<Map<String, String>> passports) {
		Set<String> fields = Set.of("byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid");
		return countPassportsWithRequiredFields(fields, passports);
	}
	
	/**
	 * Counts how many passports contain all fields in a set.
	 * 
	 * @param fields The set of required fields.
	 * @param passports A list of passports.
	 * @return The number of passports that have every field.
	 */
	private static int countPassportsWithRequiredFields(Set<String> fields, List<Map<String, String>> passports) {
		int count = 0;
		for(Map<String, String> passport : passports) {
			if(hasFields(fields, passport)) {
				count++;
			}
		}
		return count;
	}
	
	/**
	 * Checks whether or not a passport contains all fields in a set.
	 * 
	 * @param fields The set of required fields.
	 * @param passport The passport.
	 * @return True if the passport contains each of the fields.
	 */
	private static boolean hasFields(Set<String> fields, Map<String, String> passport) {
		return passport.keySet().containsAll(fields);
	}

	/**
	 * Counts the number of passports that have valid values for all required fields.
	 * 
	 * @param passports A list of passports.
	 * @return The number of them that are valid.
	 */
	private static int part2(List<Map<String, String>> passports) {
		int count = 0;
		for(Map<String, String> passport : passports) {
			if(!byrTest(passport.get("byr"))) {
				continue;
			}
			if(!iyrTest(passport.get("iyr"))) {
				continue;
			}
			if(!eyrTest(passport.get("eyr"))) {
				continue;
			}
			if(!hgtTest(passport.get("hgt"))) {
				continue;
			}
			if(!hclTest(passport.get("hcl"))) {
				continue;
			}
			if(!eclTest(passport.get("ecl"))) {
				continue;
			}
			if(!pidTest(passport.get("pid"))) {
				continue;
			}
			count++;
		}
		return count;
	}
	
	/**
	 * Tests whether or not a birth year field exists and is a 4-digit number between 1920 and 2002.
	 * 
	 * @param byr A birth year.
	 * @return True if it is valid.
	 */
	private static boolean byrTest(String byr) {
		if(byr == null) {
			return false;
		}
		if(byr.length() != 4) {
			return false;
		}
		if(!isAllDigits(byr)) {
			return false;
		}
		int year = Integer.parseInt(byr);
		if(year < 1920 || year > 2002) {
			return false;
		}
		return true;
	}
	
	/**
	 * Tests whether or not an issue year field exists and is a 4-digit number between 2010 and 2020.
	 * 
	 * @param iyr An issue year.
	 * @return True if it is valid.
	 */
	private static boolean iyrTest(String iyr) {
		if(iyr == null) {
			return false;
		}
		if(iyr.length() != 4) {
			return false;
		}
		if(!isAllDigits(iyr)) {
			return false;
		}
		int year = Integer.parseInt(iyr);
		if(year < 2010 || year > 2020) {
			return false;
		}
		return true;
	}

	/**
	 * Tests whether or not an expiration year field exists and is a 4 digit number between 2020 and 2030.
	 * 
	 * @param eyr An expiration year.
	 * @return True if it is valid.
	 */
	private static boolean eyrTest(String eyr) {
		if(eyr == null) {
			return false;
		}
		if(eyr.length() != 4) {
			return false;
		}
		if(!isAllDigits(eyr)) {
			return false;
		}
		int year = Integer.parseInt(eyr);
		if(year < 2020 || year > 2030) {
			return false;
		}
		return true;
	}
	
	/**
	 * Tests whether or not a height field exists and is either between 150 and 193 cm or between 49 and 76 in.
	 * 
	 * @param hgt A height.
	 * @return True if it is valid.
	 */
	private static boolean hgtTest(String hgt) {
		if(hgt == null) {
			return false;
		}
		if(hgt.endsWith("in")) {
			double num = Double.parseDouble(hgt.substring(0, hgt.length() - 2));
			return (num >= 59 && num <= 76);
		}
		else if(hgt.endsWith("cm")) {
			double num = Double.parseDouble(hgt.substring(0, hgt.length() - 2));
			return (num >= 150 && num <= 193);
		}
		else {
			return false;
		}
	}
	
	/**
	 * Tests whether or not an hcl field exists and consists of a # and 6-hexit number.
	 * 
	 * @param hcl A hair color.
	 * @return True if it is valid.
	 */
	private static boolean hclTest(String hcl) {
		if(hcl == null) {
			return false;
		}
		if(hcl.length() != 7) {
			return false;
		}
		if(hcl.charAt(0) != '#') {
			return false;
		}
		for(int index = 1; index < 7; index++) {
			if(!(Character.isDigit(hcl.charAt(index)) || (hcl.charAt(index) >= 'a' && hcl.charAt(index) <= 'f'))) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * Tests whether or not an ecl field exists and consists of either "amb", "blu", "brn", "gry", "grn", "hzl", or "oth".
	 * 
	 * @param ecl A eye color.
	 * @return True if it is valid.
	 */
	private static boolean eclTest(String ecl) {
		if(ecl == null) {
			return false;
		}
		Set<String> colors = Set.of("amb", "blu", "brn", "gry", "grn", "hzl", "oth");
		return colors.contains(ecl);
	}
	
	/**
	 * Tests whether or not a pid field exists and consists of 9 digits.
	 * 
	 * @param pid The passport ID.
	 * @return True if it is valid.
	 */
	private static boolean pidTest(String pid) {
		if(pid == null) {
			return false;
		}
		if(pid.length() != 9) {
			return false;
		}
		if(!isAllDigits(pid)) {
			return false;
		}
		return true;
	}
	
	/**
	 * Tests whether or a String contains only digits.
	 * 
	 * @param number A string.
	 * @return True if that string contains only digits.
	 */
	private static boolean isAllDigits(String number) {
		for(int index = 0; index < number.length(); index++) {
			if(!Character.isDigit(number.charAt(index))) {
				return false;
			}
		}
		return true;
	}
		
	/**
	 * Reads a file of passpoart data.
	 * 
	 * @return A list of passports (field name -> value maps).
	 * @throws FileNotFoundException If the input file cannot be found.
	 */
	private static List<Map<String, String>> readInput() throws FileNotFoundException {
		List<Map<String, String>> passports = new ArrayList<>();
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		Map<String, String> currentPassport = new TreeMap<>();
		while(fileScanner.hasNextLine()) {
			String line = fileScanner.nextLine();
			if(line.length() == 0) {
				passports.add(currentPassport);
				currentPassport = new TreeMap<>();
			}
			else {
				addFieldsToPassport(line, currentPassport);
			}
		}
		if(currentPassport.size() > 0) {
			passports.add(currentPassport);
		}
		return passports;
	}
	
	/**
	 * Extends a passport with additional fields.
	 * 
	 * @param line A string containing space-separated fields, each of which has the form key:value.
	 * @param passport A passport to which those key-value pairs should be added.
	 */
	private static void addFieldsToPassport(String line, Map<String, String> passport) {
		String[] fields = line.split(" ");
		for(String field : fields) {
			int colonPos = field.indexOf(':');
			String key = field.substring(0, colonPos);
			String value = field.substring(colonPos + 1);
			if(passport.containsKey(key)) {
				throw new IllegalStateException("Multiple fields with the same key in a passport.");
			}
			passport.put(key, value);
		}
	}
}
