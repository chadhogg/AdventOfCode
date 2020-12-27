package aoc2020.hoggchad.day20;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class Tests {
	
	private static final int IMAGE_SIZE = 8;
	
	private BWBitmap getOriginalImage() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[i][i] = true;
		}
		image[0][1] = true;
		image[1][0] = true;
		image[2][0] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageRotated90() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[IMAGE_SIZE - 1 - i][i] = true;
		}
		image[IMAGE_SIZE - 2][0] = true;
		image[IMAGE_SIZE - 1][1] = true;
		image[IMAGE_SIZE - 1][2] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageRotated180() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[i][i] = true;
		}
		image[IMAGE_SIZE - 1][IMAGE_SIZE - 2] = true;
		image[IMAGE_SIZE - 2][IMAGE_SIZE - 1] = true;
		image[IMAGE_SIZE - 3][IMAGE_SIZE - 1] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageRotated270() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[IMAGE_SIZE - 1 - i][i] = true;
		}
		image[1][IMAGE_SIZE - 1] = true;
		image[0][IMAGE_SIZE - 2] = true;
		image[0][IMAGE_SIZE - 3] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageFlippedHorizontally() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[IMAGE_SIZE - 1 - i][i] = true;
		}
		image[0][IMAGE_SIZE - 2] = true;
		image[1][IMAGE_SIZE - 1] = true;
		image[2][IMAGE_SIZE - 1] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageFlippedVertically() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[IMAGE_SIZE - 1 - i][i] = true;
		}
		image[IMAGE_SIZE - 1][1] = true;
		image[IMAGE_SIZE - 2][0] = true;
		image[IMAGE_SIZE - 3][0] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageRotated90UpsideDown() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[i][i] = true;
		}
		image[1][0] = true;
		image[0][1] = true;
		image[0][2] = true;
		return new BWBitmap(image);
	}
	
	private BWBitmap getImageRotated270UpsideDown() {
		boolean[][] image = new boolean[IMAGE_SIZE][IMAGE_SIZE];
		for(int i = 0; i < IMAGE_SIZE; i++) {
			image[i][i] = true;
		}
		image[IMAGE_SIZE - 2][IMAGE_SIZE - 1] = true;
		image[IMAGE_SIZE - 1][IMAGE_SIZE - 2] = true;
		image[IMAGE_SIZE - 1][IMAGE_SIZE - 3] = true;
		return new BWBitmap(image);
	}

	@Test
	public void testOrientImage() {
		BWBitmap original = getOriginalImage();
		BWBitmap rot90 = getImageRotated90();
		BWBitmap rot180 = getImageRotated180();
		BWBitmap rot270 = getImageRotated270();
		BWBitmap flippedHorz = getImageFlippedHorizontally();
		BWBitmap flippedVert = getImageFlippedVertically();
		BWBitmap rot90UD = getImageRotated90UpsideDown();
		BWBitmap rot270UD = getImageRotated270UpsideDown();
		BWBitmap afterStandard = original.afterOrientation(Orientation.STANDARD);
		BWBitmap afterRot90 = original.afterOrientation(Orientation.ROT90);
		BWBitmap afterRot180 = original.afterOrientation(Orientation.ROT180);
		BWBitmap afterRot270 = original.afterOrientation(Orientation.ROT270);
		BWBitmap afterFlipHorz = original.afterOrientation(Orientation.FLIPHORZ);
		BWBitmap afterFlipVert = original.afterOrientation(Orientation.FLIPVERT);
		BWBitmap afterRot90UD = original.afterOrientation(Orientation.ROT90UPSIDEDOWN);
		BWBitmap afterRot270UD = original.afterOrientation(Orientation.ROT270UPSIDEDOWN);
		assertEquals("Standard", original, afterStandard);
		assertEquals("Rot90", rot90, afterRot90);
		assertEquals("Rot180", rot180, afterRot180);
		assertEquals("Rot270", rot270, afterRot270);
		assertEquals("FlipHorz", flippedHorz, afterFlipHorz);
		assertEquals("FlipVert", flippedVert, afterFlipVert);
		assertEquals("Rot90UD", rot90UD, afterRot90UD);
		assertEquals("Rot270UD", rot270UD, afterRot270UD);
		
	}
}
