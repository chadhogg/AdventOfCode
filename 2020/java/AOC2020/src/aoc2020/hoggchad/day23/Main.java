package aoc2020.hoggchad.day23;

/**
 * A solution to Advent Of Code 2020, day 23.
 * 
 * @author Chad Hogg
 */
public class Main {
	
	/** The example input. */
	private static final String EXAMPLE = "389125467";
	/** My personal input. */
	private static final String INPUT = "963275481";

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 */
	public static void main(String[] args) {
		System.out.println(part1(INPUT, 100));
		System.out.println(part2(INPUT, 10000000));
	}
	
	/**
	 * Runs the first simulation.
	 * 
	 * @param line The input.
	 * @param repetitions The number of moves to make.
	 * @return The final configuration.
	 */
	private static String part1(String line, int repetitions) {
		Cups cups = new Cups(line);
		for(int i = 0; i < repetitions; i++) {
			cups.move();
		}
		return cups.getOrderAfter1();
	}
	
	/**
	 * Runs the second simulation.
	 * 
	 * @param line The input.
	 * @param repetitions The number of moves to make.
	 * @return The product of the cup labels that come after 1.
	 */
	private static long part2(String line, long repetitions) {
		CupsV2 cups = new CupsV2(line);
		for(long i = 0; i < repetitions; i++) {
			cups.move();
		}
		return cups.productAfter1();
	}
}
