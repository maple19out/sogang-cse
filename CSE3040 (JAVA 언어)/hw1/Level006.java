/**
 * CSE3040 HW1
 * Level006.java
 * Purpose: find out how many numbers match between randomly generated 6 numbers and user-input 6 numbers
 * 
 * @version 1.0 10/7/2019
 * @author Minseok Kang
 */

import java.util.Scanner;	//import a Scanner class to get a input from the user

public class Level006 {
	public static void main(String[] args) {
		int []number = new int[6];	//Create an integer array of size six to store randomly generated six numbers.
		int []userNumber = new int[6];	//Create an integer array of size six to store user-input six numbers.
		Scanner obj = new Scanner(System.in);	//Create a Scanner object
		int match = 0;	//variable that records how many numbers match
		
		
		//This loop generates 6 different random number range 1 ~ 45
		for(int i=0; i<6; i++) {
			number[i] = (int)(Math.random()*45)+1;	//generate a number range 1 ~ 45
			for(int j=0; j<i; j++) {	//checks if the generated number overlaps
				while(number[i] == number[j])	//re-generate until it is different
					number[i] = (int)(Math.random()*45)+1;
			}
		}
		
		//This loop requires 6 different numbers from the user range 1 ~ 45
		for(int i=0; i<6; i++) {
			System.out.printf("[Lotto] Enter number #%d (1-45): ", i+1);	//require an input
			userNumber[i] = obj.nextInt();	//get an input in type integer
			
			//checks if there are any numbers that match user-input number
			for(int j=0; j<6; j++) {
				//if there is a same number, variable match increments 1
				if(userNumber[i] == number[j]) {
					match++;
					break;
				}
			}
		}
		
		//prints randomly generated numbers and value of variable match
		System.out.printf("This week's lotto numbers: %d %d %d %d %d %d\n", number[0], number[1], number[2], number[3], number[4], number[5]);
		System.out.printf("You matched %d numbers\n", match);			

	}
}
