/**
 * CSE3040 HW1
 * Level004.java
 * Purpose: Prints the ASCII code of input alphabet
 * 
 * @version 1.0 10/7/2019
 * @author Minseok Kang
 */

import java.util.Scanner;	//import a Scanner class to get a input from the user

public class Level004 {
	public static void main(String[] args) {
		System.out.print("ASCII code teller. Enter a letter: ");	//require an input from the user
		Scanner obj = new Scanner(System.in);	//create a scanner object
		String str = obj.nextLine();	//gets an input from the user by String
		//final output is printed in both character form and integer form. Integer form needs type casting
		System.out.printf("The ASCII code of %c is %d\n", str.charAt(0), (int)(str.charAt(0)));
		
	}
}
