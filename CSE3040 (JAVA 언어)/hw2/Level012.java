package hw1;

/**
 * CSE3040 HW2 Level012.java Purpose: Prints decimal number in binary form
 * 
 * @version 1.0 11/3/2019
 * @author Minseok Kang
 */

import java.util.Scanner;	//imports Scanner class to get input from user

/**
 * interface that is subtype of BinarySequenceStr 
 */
interface IntSequenceStr {
	/**
	 * method that returns true if there is next element, otherwise returns false
	 * @param args not used
	 * @return boolean data
	 */
	boolean hasNext();
	
	/**
	 * method that returns next element
	 * @param args not used
	 * @return integer data
	 */
	int next();	
}

/**
 * class that implements IntSequenceStr. Makes Decimal number in binary form one by one.
 */
class BinarySequenceStr implements IntSequenceStr{
	private int num;	//variable that stores input
	private int n=0;	//variable that stores binary digits of input number
	
	/**
	 * Constructor that initializes variable num, n
	 * @param integer data num
	 */
	BinarySequenceStr(int num){
		this.num = num;	//initialize variable num
		while(true) {	//loop that calculates binary digits of input number
			n++;
			if(Math.pow(2, n)<=num && num<Math.pow(2, n+1))
				break;
		}
		n++;
	}
	
	/**
	 * Method that determines whether there is next element
	 * @param args not Used
	 * @returns boolean data
	 */
	public boolean hasNext() {
		if(n > 0) {	//returns true until digit is bigger than zero
			n--;
			return true;	
		}
		else	//returns false if there is no more digit left
			return false;
	}
	
	/**
	 * Method that returns binary value of input
	 * @param args not used
	 * @returns integer data(1 or 0 of binary digit)
	 */
	public int next() {
		if(Math.pow(2,  n)<=num && num<Math.pow(2,  n+1)) {	//if num is bigger than Math.pow(2, n), returns 1
			num -= Math.pow(2,  n);
			return 1;
		}		
		else	//if num is smaller than Math.pow(2, n), returns 0
			return 0;		
	}
}

/**
 *public class that gets input from user and prints in binary form
 */

public class Level012 {
	public static void main(String args[]) {
		Scanner in = new Scanner(System.in);	//create an object of Scanner class
		System.out.print("Enter a positive integer: ");
		String str = in.nextLine();	//gets input from user
		int num = Integer.parseInt(str);	//change string type into integer value
		in.close();
		System.out.println("Integer: " + num);
		IntSequenceStr seq = new BinarySequenceStr(num);	//create an instance of BinarySequenceStr with parameter input
		System.out.print("Binary number: ");
		while (seq.hasNext())	//loop continues until there is no more digit
			System.out.print(seq.next());
		System.out.println("");
	}
}
