package hw1;

/**
 * CSE3040 HW2
 * Level011.java
 * Purpose: Prints the Fibonacci sequence
 * 
 * @version 1.0 11/3/2019
 * @author Minseok Kang
 */


/**
 * interface IntSequence (Subtype of FibonacciSequence)
 * 
 */
interface IntSequence {
	/**
	 * Method that returns true/false if there exists next element 
	 * @param args Not used
	 * @return boolean data
	 */
	boolean hasNext();
	
	/**
	 * Method that returns next element
	 * @param args Not Used
	 * @return next element
	 */
	int next();	
}


/**
 * class FibonacciSequence implements methods in IntSequence
 * calculates next Fibonacci number
 */
class FibonacciSequence implements IntSequence {
	private int i = 0;	// variable that stores 1st Fibo number
	private int j = 1;	// variable that stores 2nd Fibo number
	private int num = -1;	//variable that stores n-th index number of Fibo
	
	
	/**
	 * Method that returns true/false if there exists next element
	 * @param args Not used
	 * @returns boolean data
	 */
	public boolean hasNext() { return true; }
	
	/**
	 * Method that calculates next Fibo number and returns it
	 * @param args Not used
	 * @returns next Fibo number
	 */
	public int next() {
		num++;	//increases by 1 after method is called.
		if(num == 0)	//1st Fibo number
			return 0;
		else if(num == 1)	//2nd Fibo number
			return 1;
		else {	//n-th Fibo number(n>=2)
			int temp;	//local variable that temporarily stores i
			temp = i;
			i = j;
			j = temp + j;
			return j;	//returns sum of previous two number i, j;			
		}
	}
}

/**
 * public class shows 20 Fibonacci sequences
 */
public class Level011 {
	public static void main(String[] args) {
		IntSequence seq = new FibonacciSequence();	//create an instance of FibonacciSequence
		for(int i=0; i<20; i++) {	//iterate 20 times to print 20 Fibo sequences
			if(seq.hasNext() == false) break;
			System.out.print(seq.next() + " ");
		}
		System.out.println(" ");
	}
}
