package aoc2020.hoggchad.day24;

/**
 * A coordinate in a hexagonal coordinate system.
 * 
 * From a point, those directly east or west from it are +2/-2 in the east direction.
 * From a point, those ne / se / nw / sw from it are +1/-1 in both directions.
 * 
 * @author Chad Hogg
 */
public class Coordinate {

	public int north;
	public int east;
	
	/**
	 * Constructs a coordinate from a string line "eneseew".
	 * 
	 * @param line That string.
	 */
	public Coordinate(String line) {
		north = 0;
		east = 0;
		for(int index = 0; index < line.length(); index++) {
			switch(line.charAt(index)) {
			case 'e': east += 2; break;
			case 'w': east -= 2; break;
			case 's': 
				north--;
				if(line.charAt(index + 1) == 'e') {
					east++;
				}
				else {
					east--;
				}
				index++;
				break;
			case 'n':
				north++;
				if(line.charAt(index + 1) == 'e') {
					east++;
				}
				else {
					east--;
				}
				index++;
				break;
			default:
				throw new IllegalStateException("Unexpected character: " + line.charAt(index));
			}
		}
	}
	
	/**
	 * Constructs a coordinate from its components.
	 * 
	 * @param north How far north.
	 * @param east How far east.
	 */
	public Coordinate(int north, int east) {
		this.north = north;
		this.east = east;
	}
	
	@Override
	public String toString() {
		return "(" + east + ", " + north + ")";
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj == null || this.getClass() != obj.getClass()) {
			return false;
		}
		else {
			Coordinate other = (Coordinate)obj;
			return other.north == north && other.east == east;
		}
	}
	
	@Override
	public int hashCode() {
		return north + 31 * east;
	}
}
