package aoc2020.hoggchad.day25;

/**
 * A solution to Advent Of Code 2020, day 25.
 * 
 * @author Chad Hogg
 */
public class Main {

	/** The example card public key. */
	private static final long EXAMPLE_CARD_PK = 5764801;
	/** The example door public key. */
	private static final long EXAMPLE_DOOR_PK = 17807724;
	/** My input card public key. */
	private static final long INPUT_CARD_PK = 14012298;
	/** My input door public key .*/
	private static final long INPUT_DOOR_PK = 74241;
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		System.out.println(part1V2(INPUT_CARD_PK, INPUT_DOOR_PK));
	}
	
	/**
	 * A first brute-force attempt that worked easily for the example, but was too slow for my input.
	 * Upon further reflection, this involves repeating the same steps over and over again.
	 * 
	 * @param cardPK The card's public key.
	 * @param doorPK The door's public key.
	 * @return The encryption key.
	 */
	private static long part1(long cardPK, long doorPK) {
		final long SUBJECT = 7L;
		long cardLoopSize = -1;
		long doorLoopSize = -1;
		for(long loopSize = 0; cardLoopSize == -1 || doorLoopSize == -1;loopSize++) {
			long pk = generatePK(SUBJECT, loopSize);
			if(cardLoopSize == -1 && pk == cardPK) {
				System.out.println("Found card loop size!");
				cardLoopSize = loopSize;
			}
			if(doorLoopSize == -1 && pk == doorPK) {
				System.out.println("Found door loop size!");
				doorLoopSize = loopSize;
			}
		}
		long cardEK = generatePK(doorPK, cardLoopSize);
		long doorEK = generatePK(cardPK, doorLoopSize);
		if(cardEK != doorEK) {
			throw new IllegalStateException("Encryption keys do not match.");
		}
		return cardEK;
	}
	
	/**
	 * An improved version that doesn't senselessly recompute the same things over and over again.
	 * 
	 * @param cardPK The card's public key.
	 * @param doorPK The door's public key.
	 * @return The encryption key.
	 */
	private static long part1V2(long cardPK, long doorPK) {
		final long SUBJECT = 7L;
		long currentPK = 1L;
		long cardLoopSize = 1L;
		long doorLoopSize = 1L;
		for(long loopSize = 1L; cardLoopSize == -1 || doorLoopSize == 1; loopSize++) {
			currentPK = iterativePK(SUBJECT, currentPK);
			if(cardPK == currentPK) {
				cardLoopSize = loopSize;
			}
			if(doorPK == currentPK) {
				doorLoopSize = loopSize;
			}
		}
		long cardEK = generatePK(doorPK, cardLoopSize);
		long doorEK = generatePK(cardPK, doorLoopSize);
		if(cardEK != doorEK) {
			throw new IllegalStateException("Encryption keys do not match.");
		}
		return cardEK;
	}
	
	/**
	 * Generates a public key.
	 * 
	 * @param subjectNumber The subject number.
	 * @param loopSize The number of repetitions.
	 * @return The public key.
	 */
	private static long generatePK(long subjectNumber, long loopSize) {
		final long DIV = 20201227L;
		long pk = 1L;
		for(long count = 0; count < loopSize; count++) {
			pk *= subjectNumber;
			pk %= DIV;
		}
		return pk;
	}
	
	/**
	 * Generates the public key for the next loop size.
	 * 
	 * @param previousValue The public key for the previous loop size.
	 * @param subjectNumber The subject number.
	 * @return The public key for the next loop size.
	 */
	private static long iterativePK(long previousValue, long subjectNumber) {
		final long DIV = 20201227L;
		previousValue *= subjectNumber;
		previousValue %= DIV;
		return previousValue;
	}
}
