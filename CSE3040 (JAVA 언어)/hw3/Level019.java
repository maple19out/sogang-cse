package hw3;

/**
 * CSE3040 HW3 Level019.java Purpose: Read input from a file and prints in alphabetic order.
 * 
 * @version 1.0 12/4/2019
 * @author Minseok Kang
 */

import java.util.*;	//import to use ColletionFramework
import java.io.FileReader;	//import to get input
import java.io.BufferedReader;	//import BufferedReader to get input line by line
import java.io.IOException;	//import to handle exception

/**
 * Class that extends TreeMap to Override toString method properly & 
 */
class MyMap<K, V> extends TreeMap<K, V>{
	TreeMap<K, V> newMap = new TreeMap<>();	//use TreeMap variable to get advantage of internal sorting of keys.
	
	/**
	 * Override toString to properly print Output
	 * @param none
	 * @return String
	 */
	public String toString() {
		String s = "";	//starts with blank String
		Iterator<Map.Entry<K, V>> it = newMap.entrySet().iterator();
		//Loop below iterates keys in newMap and adds keys and values to String s
		while(it.hasNext()) {
			Map.Entry<K, V> entry = (Map.Entry<K, V>)it.next();
			s += (entry.getKey() + " " + entry.getValue());
			if(it.hasNext())
				s += '\n';
		}
		return s;	//return final output
	}
	
	/**
	 * Method that adds element into newMap
	 * @param item : String Object(fruits)
	 * @param price : Double Object(price)
	 */
	public void makeTree(K item, V price) {
		newMap.put(item, price);
	}
}

public class Level019 {
	/**
	 * Creates an object of user-defined class and returns it.
	 * @return MyMap object
	 */
	public static MyMap<String, Double> InitializeMap() {
		MyMap<String, Double> map = new MyMap<>();
		return map;
	}
	
	public static int readDataFromFile(String str, Map<String, Double> map) {		
		try (BufferedReader br = new BufferedReader(new FileReader(str))){	//read line by line using try-with-resources
			while(true) {
				String line = br.readLine();
				if(line == null)	//read until end of file
					break;
				int i;
				//this loop separates item and price in a sentence.
				for(i=0; i<line.length(); i++)
					if(line.charAt(i) == ' ')
						break;
				//Statement below : Type-Casting needed to use method 'makeTree'
				((MyMap<String, Double>) map).makeTree(line.substring(0, i),  Double.parseDouble(line.substring(i+1, line.length())));
			}
		}
		catch (IOException e) {
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