package aoc2020.hoggchad.day22;

import java.util.List;
import java.util.LinkedList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * A solution to Advent of Code 2020, day 22.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The path to the input file. */
	private static final String FILE_PATH = "../../inputs/day22/input";
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	public static void main(String[] args) throws FileNotFoundException {
		Game game = readInput(FILE_PATH);
		System.out.println(part1(new Game(game.getPlayer1(), game.getPlayer2())));
		System.out.println(part2(new Game(game.getPlayer1(), game.getPlayer2())));
	}
	
	/**
	 * Plays a game of Combat.
	 * 
	 * @param game The initial state of the game.
	 * @return The winning player's score.
	 */
	private static int part1(Game game) {
		game.playGame();
		List<Integer> winner = game.getPlayer1();
		if(winner.isEmpty()) {
			winner = game.getPlayer2();
		}
		return calculateScore(winner);
	}
	
	/**
	 * Plays a game of Recursive Combat.
	 * 
	 * @param game The initial state of the game.
	 * @return The winning player's score.
	 */
	private static int part2(Game game) {
		Game.Result result = game.playRecursiveGame();
		List<Integer> winner = null;
		if(result == Game.Result.P1WINS) {
			winner = game.getPlayer1();
		}
		else {
			winner = game.getPlayer2();
		}
		return calculateScore(winner);
	}
	
	/**
	 * Calculates the score of a deck of cards.
	 * 
	 * @param winner The deck.
	 * @return The sum of the products of the cards values and their distances from the bottom.
	 */
	private static int calculateScore(List<Integer> winner) {
		int score = 0;
		for(int index = 0; index < winner.size(); index++) {
			score += winner.get(index) * (winner.size() - index);
		}
		return score;		
	}
	
	/**
	 * Reads the input.
	 * 
	 * @param path The file path.
	 * @return A game.
	 * @throws FileNotFoundException If the input file cannot be read.
	 */
	private static Game readInput(String path) throws FileNotFoundException {
		List<Integer> player1 = new LinkedList<>();
		List<Integer> player2 = new LinkedList<>();
		Scanner fileScanner = new Scanner(new File(path));
		boolean stillFirst = true;
		fileScanner.nextLine(); // "Player 1:"
		while(fileScanner.hasNextLine()) {
			String line = fileScanner.nextLine();
			if(line.startsWith("Player 2:")) {
				stillFirst = false;
			}
			else if(!line.isEmpty()) {
				if(stillFirst) {
					player1.add(Integer.parseInt(line));
				}
				else {
					player2.add(Integer.parseInt(line));
				}
			}
		}
		fileScanner.close();
		return new Game(player1, player2);
	}
}
