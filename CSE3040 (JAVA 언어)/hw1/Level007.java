/**
 * CSE3040 HW1
 * Level007.java
 * Purpose: Counts how many given letter exists in a input text 
 * 
 * @version 1.0 10/7/2019
 * @author Minseok Kang
 */

import java.util.Scanner;	//import a Scanner class to get a input from the user

public class Level007 {
	public static void main(String[] args) {
		Scanner obj = new Scanner(System.in);	//Scanner object that gets input text, letter
		String text;	//String class that stores user input(text)
		char letter;	//Character variable that stores user input(letter)
		int number=0;	//saves how many letter is in the text
		
		System.out.print("Enter a text: ");	//require a text input
		text = obj.nextLine();	//gets an input in String class
		
		System.out.print("Enter a letter: ");	//require a letter input
		letter = (obj.nextLine()).charAt(0);	//gets an input in String class and just store first element of the input
		
		//search from start to end of String text. If letter matches text[i], variable number increments by 1
		for(int i=0; i<text.length(); i++)
			if(text.charAt(i) == letter)
				number++;
		
		//prints final result 
		System.out.printf("There are %d %c's in the text\n",  number, letter);		
	}
}
