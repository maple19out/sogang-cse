package hw3;

/**
 * CSE3040 HW3 Level018.java Purpose: Read input from a file and prints in ascending order(price) / if same, in alphabetical order (item)
 * 
 * @version 1.0 12/4/2019
 * @author Minseok Kang
 */

import java.util.*;	//import to use ColletionFramework
import java.io.FileReader;	//import to get input
import java.io.BufferedReader;	//import BufferedReader to get input line by line
import java.io.IOException;	//import to handle exception

/**
 * class Element refers to elements in ArrayList list. 
 */
class Element implements Comparable<Element>{	//implements Comparable interface to set comparing criteria
	String sentence;	//variable that stores raw lines of input file
	String item;	//variable that stores item
	double price;	//variable that stores price value;
	
	/**
	 * Method that sets comparing criteria : sort by price-ascending order
	 * @param Element e
	 * @return integer
	 */
	public int compareTo(Element e) {
		if(this.price > e.price) return 1;
		else if(this.price == e.price) {
			if(this.item.compareTo(e.item) > 0) return 1;
		}
		return -1;
	}
	/**
	 * Method that sets comparing criteria : sort by alphabetical order (if prices are same)
	 * @param String s
	 * @return integer
	 */
	public int compareTo(String s) {
		int sum=0;	//variable that stores difference between two items.
		int i=0, j=0;
		for(i=0; i<this.item.length() && j<s.length(); i++) {
			sum += (item.charAt(i)-s.charAt(j));
			j++;
		}
		if(i > j)	//case that first element is longer
			sum = 1;
		else if(i < j)	//case that second element is longer
			sum = -1;
		return sum;
	}
	
	/**
	 *Method that returns main content of class Element 
	 */
	public String toString() {
		return this.sentence;	//return sentence which contains items with price
	}
}


/**
 * Class thatRead input from a file and prints in ascending order(price) / if same, in alphabetical order (item)
 */
public class Level018 {
	/**
	 * Method that read data from input file
	 * @param str : file name
	 * @param list : Element added to list
	 * @return integer(1: error occur / 0: read complete
	 */
	public static int readDataFromFile(String str, ArrayList<Element> list) {		
		try (BufferedReader br = new BufferedReader(new FileReader(str))){	//read line by line using try-with-resources
			while(true) {
				String line = br.readLine();
				if(line == null)	//read until end of file
					break;
				Element element = new Element();	//create an object of class Element
				int i;
				//this loop separates item and price in a sentence.
				for(i=0; i<line.length(); i++)
					if(line.charAt(i) == ' ')
						break;
				element.sentence = line;
				element.item = line.substring(0, i);
				element.price = Double.parseDouble(line.substring(i+1, line.length()));
				list.add(element);
			}
		}
		catch (IOException e) {
			return 1;
		}
		return 0;
	}
	
	public static void main(String[] args) throws Exception {
		ArrayList<Element> list = new ArrayList<>();
		int rv = readDataFromFile("./input.txt", list);
		if(rv == 1) {
			System.out.println("input file not found!");
			return;
		}
		Collections.sort(list);
		Iterator<Element> it = list.iterator();
		while(it.hasNext()) System.out.println(it.next());
	}
}