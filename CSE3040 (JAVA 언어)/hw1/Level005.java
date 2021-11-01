/**
 * CSE3040 HW1
 * Level005.java
 * Purpose: User needs to find randomly generated number until it matches
 * 
 * @version 1.0 10/7/2019
 * @author Minseok Kang
 */

import java.util.Scanner;	//import a Scanner class to get a input from the user

public class Level005 {
	public static void main(String[] args) {
		int number = ((int)(Math.random()*100)+1);	//set a random number range in 1~100
		int userNumber;	//user input number
		Scanner obj = new Scanner(System.in);	//create a Scanner object to get a input
		
		//loop continues until the user guesses a generated number correctly
		while(true) {
			System.out.print("Guess a number (1-100): ");	//require a number range in 1~100
			userNumber = obj.nextInt();	//get an input in type Int
			if(userNumber < number)	//if userNumber is smaller than number, prints "Too small!"
				System.out.println("Too small!");
			else if(userNumber > number)	//if userNumber is larger than number, prints "Too large!"
				System.out.println("Too large!");
			else {	//if userNumber equals number, prints "Correct!" and breaks the loop
				System.out.println("Correct!");
				break;
			}			
		}
	}
}
