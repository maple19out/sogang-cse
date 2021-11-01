package hw1;

/**
 * CSE3040 HW2 Level016.java Purpose: Read text from a file and counts number of
 * occurrences for each character
 * 
 * @version 1.0 11/3/2019
 * @author Minseok Kang
 */

import java.io.FileInputStream;	//import FileInputStream to get input from file
import java.io.IOException;	//import IOException to handle Exception

/**
 * Text class gets input from file, counts character
 */
class Text{
	int [] arr = new int[128];	//array that counts how many characters appeared in the text.
	
	/**
	 * Method that reads File and counts how many characters appeared.
	 * @param str : File directory
	 * @return : void(none)
	 */
	public void readTextFromFile(String str) {
		for(int i=0; i<128; i++)	//initialize counting array
			arr[i] = 0;
		
		try (FileInputStream input = new FileInputStream(str)){	//use try with resources to handle close IOException
			
			byte[] b = new byte[1];	//read one byte at once
			while(input.read(b) != -1) {
				arr[b[0]]++;	//array element increases according to index		
			}
		}
		catch (IOException e) {	//if there is no file or error occured, print error message
			System.out.println("Error: file does not exist.");
		}

	}
	
	/**
	 * Method that returns count of Character
	 * @param ch : character value
	 * @return number of character occurrence stored in arr[]
	 */
	public int countChar(char ch){
		return arr[ch];
	}
}

/**
 * class that read file and prints number of characters occured
 */
public class Level016 {
	public static void main(String args[]) {
		Text t = new Text();	//create an instance
		t.readTextFromFile("src/hw1/input_Level016.txt");	//call method that reads file
		System.out.println("a: " + t.countChar('a'));	//print how many 'a' appeared in text
		System.out.println("b: " + t.countChar('b'));	//print how many 'b' appeared in text
		System.out.println("c: " + t.countChar('c'));	//print how many 'c' appeared in text
	}
}