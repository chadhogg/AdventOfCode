package aoc2020.hoggchad.day22;

import java.util.Set;
import java.util.HashSet;
import java.util.List;
import java.util.Collections;
import java.util.LinkedList;

/**
 * A game of Combat (or Recursive Combat).
 * 
 * @author Chad Hogg
 */
public class Game {
	
	/** The result of a round in the recursive version of the game. */
	public enum Result { P1WINS, P2WINS, NOTDONE };
	
	/** The first player's deck, ordered from top to bottom. */
	private LinkedList<Integer> player1;
	/** The second player's deck, ordered from top to bottom. */
	private LinkedList<Integer> player2;
	/** A set of previous states of this game, for use in the recursive version. */
	private Set<Game> previousIterations;
	
	/**
	 * Constructs a new game.
	 * 
	 * @param p1 The first player's deck.
	 * @param p2 The second player's deck.
	 */
	public Game(List<Integer> p1, List<Integer> p2) {
		player1 = new LinkedList<>(p1);
		player2 = new LinkedList<>(p2);
		previousIterations = new HashSet<>();
	}
	
	/**
	 * Gets the first player's deck.
	 * 
	 * @return An unmodifiable view of the deck.
	 */
	public List<Integer> getPlayer1() {
		return Collections.unmodifiableList(player1);
	}
	
	/**
	 * Gets the second player's deck.
	 * 
	 * @return An unmodifiable view of the deck.
	 */
	public List<Integer> getPlayer2() {
		return Collections.unmodifiableList(player2);
	}
	
	/**
	 * Plays one round of normal Combat.
	 */
	public void playRound() {
		if(player1.peek() > player2.peek()) {
			player1.add(player1.remove());
			player1.add(player2.remove());
		}
		else {
			player2.add(player2.remove());
			player2.add(player1.remove());
		}
	}
	
	/**
	 * Plays one game of normal Combat.
	 */
	public void playGame() {
		while(!player1.isEmpty() && !player2.isEmpty()) {
			playRound();
		}
	}

	/**
	 * Plays one round of Recursive Combat.
	 * 
	 * @return P1WINS if infinite recursion was detected, otherwise NOTDONE.
	 */
	public Result playRecursiveRound() {
		System.out.println("Round between " + player1 + " and " + player2);
		if(previousIterations.contains(this)) {
			System.out.println("Player 1 wins the round on infinite looping.");
			return Result.P1WINS;
		}
		previousIterations.add(new Game(player1, player2));
		if(player1.peek() > player1.size() - 1 || player2.peek() > player2.size() - 1) {
			playRound();
			System.out.println("It was just a normal round.");
			return Result.NOTDONE;
		}
		else {
			int p1Card = player1.remove();
			int p2Card = player2.remove();
			Game subGame = new Game(player1.subList(0, p1Card), player2.subList(0, p2Card));
			Result result = subGame.playRecursiveGame();
			if(result == Result.P1WINS) {
				player1.add(p1Card);
				player1.add(p2Card);
			}
			else {
				player2.add(p2Card);
				player2.add(p1Card);
			}
			System.out.println("It was a recursion round.");
			return Result.NOTDONE;
		}
	}
	
	/**
	 * Plays one game of Recursive Combat.
	 * 
	 * @return P1WINS if player 1 won (for any reason), or P2WINS if player 2 won (for any reason).
	 */
	public Result playRecursiveGame() {
		System.out.println("Playing a game between " + player1 + " and " + player2);
		Result result = Result.NOTDONE;
		while(result == Result.NOTDONE) {
			result = playRecursiveRound();
			if(player1.isEmpty()) {
				result = Result.P2WINS;
				System.out.println("Player 1 wins the game!");
			}
			else if(player2.isEmpty()) {
				result = Result.P1WINS;
				System.out.println("Player 2 wins the game!");
			}
		}
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if(obj == null || this.getClass() != obj.getClass()) {
			return false;
		}
		else {
			Game other = (Game)obj;
			return player1.equals(other.player1) && player2.equals(other.player2);
		}
	}
	
	@Override
	public int hashCode() {
		return player1.hashCode() + 31 * player2.hashCode();
	}
}
