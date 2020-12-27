package aoc2020.hoggchad.day20;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

/**
 * A brief program that transposes a text file, so that I can better debug my code.
 * 
 * @author Chad Hogg
 */
public class RotateFile {
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 * @throws FileNotFoundException If the input file cannot be read or output file cannot be written.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Scanner fileScanner = new Scanner(new File("../../inputs/day20/input"));
		PrintWriter writer = new PrintWriter(new File("../../inputs/day20/rotated"));
		List<String> originalLines = new ArrayList<>();
		while(fileScanner.hasNextLine()) {
			originalLines.add(fileScanner.nextLine());
		}
		int maxLength = 0;
		for(String line : originalLines) {
			if(line.length() > maxLength) {
				maxLength = line.length();
			}
		}
		for(int i = 0; i < maxLength; i++) {
			StringBuilder sb = new StringBuilder();
			for(int k = 0; k < originalLines.size(); k++) {
				if(originalLines.get(k).length() > i) {
					sb.append(originalLines.get(k).charAt(i));
				}
				else {
					sb.append(' ');
				}
			}
			writer.println(sb.toString().stripTrailing());
		}
		writer.close();
		fileScanner.close();
	}

}
