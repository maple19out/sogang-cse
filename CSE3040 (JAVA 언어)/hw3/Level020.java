package hw3;

/**
 * CSE3040 HW3 Level020.java Purpose: Read input from a file and prints in price-asecending order (if same, in alphabetic order)
 * 
 * @version 1.0 12/8/2019
 * @author Minseok Kang
 */

import java.io.BufferedReader;	//import BufferedReader to get input line by line
import java.io.FileReader;	//import to get input
import java.io.IOException;	//import to handle file input output exception
import java.util.Iterator;	//import to use Iterator
import java.util.ArrayList;	//import to use ArrayList
import java.util.Collections;	//import to use sort method in Collections
import java.util.HashMap;	//import to use HashMap
import java.util.Map;	//import to use Map

/**
 * class Element refers to elements in ArrayList list. 
 */
class Element implements Comparable<Element>{	//implements Comparable interface to set comparing criteria
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
}

/**
 * Class that extends HashMap to Override toString method properly
 */
class MyMap<K, V> extends HashMap<K, V> {
	ArrayList<Element> list = new ArrayList<>();	//use ArrayList object in order to utilize sorting method in Level018

	/**
	 * Override toString to properly print Output
	 * 
	 * @param none
	 * @return String
	 */
	public String toString() {
		String s = ""; // starts with blank String
		Iterator<Element> it = list.iterator();
		// Loop below iterates keys in newMap and adds keys and values to String s
		while (it.hasNext()) {
			Element e = it.next();
			s += e.item + " " + e.price;
			if(it.hasNext())
				s += "\n";
		}
		return s; // return final output
	}

	/**
	 * Method that adds element into list
	 * 
	 * @param item  : String Object(fruits)
	 * @param price : Double Object(price)
	 */
	public void makeList(String item, Double price) {
		Element element = new Element();
		element.item = item;
		element.price = price;
		list.add(element);
	}
}

public class Level020 {
	/**
	 * Creates an object of user-defined class and returns it.
	 * @return MyMap object
	 */
	public static MyMap<String, Double> InitializeMap() {
		MyMap<String, Double> map = new MyMap<>();
		return map;
	}

	public static int readDataFromFile(String str, Map<String, Double> map) {
		try (BufferedReader br = new BufferedReader(new FileReader(str))) { // read line by line using
																			// try-with-resources
			while (true) {
				String line = br.readLine();
				if (line == null) // read until end of file
					break;
				int i;
				// this loop separates item and price in a sentence.
				for (i = 0; i < line.length(); i++)
					if (line.charAt(i) == ' ')
						break;
				//add contents to list by using makeList method in MyMap class.
				((MyMap<String, Double>) map).makeList(line.substring(0, i),  Double.parseDouble(line.substring(i+1, line.length())));
			}
			//sort by using new criteria(price-ascending order(if same, alphabetic order))
			Collections.sort(((MyMap<String, Double>)map).list);
		} catch (IOException e) {
			return 1;
		}
		return 0;
	}

	public static void main(String[] args) throws Exception {
		Map<String, Double> map = InitializeMap();
		int rv = readDataFromFile("./input.txt", map);
		if (rv == 1) {
			System.out.println("input file not found!");
			return;
		}
		System.out.println(map);
	}
}