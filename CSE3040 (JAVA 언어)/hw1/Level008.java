/**
 * CSE3040 HW1
 * Level008.java
 * Purpose: Counts how many given string exists in a input text
 * 
 * @version 1.0 10/7/2019
 * @author Minseok Kang
 */

import java.util.Scanner;	//import a Scanner class to get a input from the user

public class Level008 {
	public static void main(String[] args) {
		Scanner obj = new Scanner(System.in);	//Scanner object that gets input text, lettenr
		String text;	//String class that stores user input(text)
		String string;	//String class that stores user input(string)
		int number=0;	//saves how many string is in the text
		int i, j;	//variable for for loop
		
		System.out.print("Enter a text: ");	//require a text input
		text = obj.nextLine();	//gets an input in String class
		
		System.out.print("Enter a string: ");	//require a string input
		string = obj.nextLine();	//gets an input in String class and just store first element of the input
		
		
		//this loop checks how many instances are in the input text.
		for(i=0; i<text.length(); i++) {
			
			//check begins if (1)first letter of text and string matches (2)length of remaining text elements is not shorter than string 
			if((text.charAt(i) == string.charAt(0)) && (text.length()-i >= string.length())) {
				//checks every elements between sub-text and string
				for(j=0; j<string.length(); j++) {
					if(text.charAt(i+j) == string.charAt(j))	//if an element in text and string match, then continue
						continue;
					else	//if an element in text and string is different, then breaks the loop
						break;
				}
				//if j equals string.length() it implies matched completely(increments variable number by 1) otherwise match failed.
				if(j == string.length())
					number++;
			}
		}			
		
		//prints final result 
		System.out.printf("I have found %d instances of \"%s\".\n",  number, string);		
	}
}