package aoc2020.hoggchad.day13;

import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent Of Code 2020, day 13.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day13/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Scanner fileScanner = new Scanner(new File(FILE_PATH));
		int minDepartTime = fileScanner.nextInt();
		List<Integer> busIds = new ArrayList<>();
		List<Integer> idsWithZeroes = new ArrayList<>();
		fileScanner.nextLine();
		String[] stuff = fileScanner.nextLine().split(",");
		for(String possible : stuff) {
			if(!possible.equals("x")) {
				busIds.add(Integer.parseInt(possible));
				idsWithZeroes.add(Integer.parseInt(possible));
			}
			else {
				idsWithZeroes.add(0);
			}
		}
		
		System.out.println(part1(minDepartTime, busIds));
		System.out.println(part2SecondAttempt(idsWithZeroes));
	}
	
	/**
	 * Finds the id of the bus that leaves closest to (but not before) a certain time.
	 * 
	 * @param minDepartTime When you will arrive at the bus station.
	 * @param busIds A list of bus IDs.
	 * @return The product of the ID of the first bus you can catch and the number of minutes you will need to wait for it.
	 */
	private static int part1(int minDepartTime, List<Integer> busIds) {
		int bestId = -1;
		int bestTime = Integer.MAX_VALUE;
		for(int id : busIds) {
			int time = 0;
			while(time < minDepartTime) {
				time += id;
			}
			if(time < bestTime) {
				bestTime = time;
				bestId = id;
			}
		}
		return bestId * (bestTime - minDepartTime);
	}
	
	/**
	 * An attempt at brute-forcing part 2.  Even though it is only an O(n*m) algorithm, n is *very* large.
	 * 
	 * @param idsWithZeroes The list of bus IDs, with 0s inserted wherever we are not worrying about a bus.
	 * @return The earliest timestamp at which the busses all leave at consecutive timestamps (ignoring wildcards).
	 */
	private static long part2FirstAttempt(List<Integer> idsWithZeroes) {
		for(long timeStamp = 0; true; timeStamp++) {
			boolean allMatch = true;
			for(int offset = 0; offset < idsWithZeroes.size() && allMatch; offset++) {
				if(idsWithZeroes.get(offset) != 0) {
					if(!departsOn(timeStamp + offset, idsWithZeroes.get(offset))) {
						allMatch = false;
					}
				}
			}
			if(allMatch) {
				return timeStamp;
			}
		}
	}
	
	/**
	 * Searches for the first timestamp at which the busses all leave at consecutive timestamps (ignoring wildcards).
	 * 
	 * @param idsWithZeroes The list of bus IDs, with 0s inserted wherever we are not worrying about a bus.
	 * @return The timestamp.
	 */
	private static long part2SecondAttempt(List<Integer> idsWithZeroes) {
		if(idsWithZeroes.get(0) == 0) {
			throw new IllegalStateException("Didn't anticipate first value being a wildcard, would need to make the initialization more complicated.");
		}
		// Observation from lots of tedious example-drawing: 
		//   For a given sublist of the numbers n0, n1, ...nx:
		//      If you find the first timestamp that works for that sublist,
		//      Then all other timestamps that work will be that timestamp plus a multiple of n0 * n1 * ... nx.
		//   So when expanding the "solved" sublist to the next number, only consider such multiples.
		// There's probably even a direct way to find that first timestamp, but this is fast enough to not worry about it.
		long startPoint = idsWithZeroes.get(0);
		long jumpAmount = idsWithZeroes.get(0);
		for(int offset = 1; offset < idsWithZeroes.size(); offset++) {
			// The zeroes indicate wildcard positions we don't care about.
			if(idsWithZeroes.get(offset) != 0) {
				long timestamp = startPoint;
				// Because the jump amount is so large, this should be a pretty quick loop.
				while(!departsOn(timestamp + offset, idsWithZeroes.get(offset))) {
					timestamp += jumpAmount;
				}
				// This is now the earliest timestamp for which the first n busses follow the pattern.
				startPoint = timestamp;
				// Adding multiples of this gets you to other timestamps in which the first n busses follow the pattern.
				jumpAmount *= idsWithZeroes.get(offset);
			}
		}
		return startPoint;
	}
	
	/**
	 * Tests whether or not a bus departs at a certain time.
	 * 
	 * @param timeStamp The time.
	 * @param id The bus identifier.
	 * @return True if that bus departs at that timestamp.
	 */
	private static boolean departsOn(long timeStamp, long id) {
		return timeStamp % id == 0;
	}
}
