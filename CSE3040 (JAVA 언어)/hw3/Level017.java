package hw3;

/**
 * CSE3040 HW3 Level017.java Purpose: Read input from a file and prints the most expensive item, cheapest item, and average
 * 
 * @version 1.0 12/3/2019
 * @author Minseok Kang
 */

import java.io.BufferedReader;	//import BufferedReader to get input line by line
import java.io.FileReader;	//import to get an input
import java.io.IOException;	//import to handle file input exception
import java.util.*;	//import to use HashMap

public class Level017 {
	public static void main(String[] args) throws IOException {
		//Create HashMap object to store information. keys : items / values : price
		HashMap<String, Double> map = new HashMap<>();
		//get input from "./input.txt" file
		try ( BufferedReader br = new BufferedReader(new FileReader("./input.txt"))){
			//Read line by line until end of file
			while(true) {
				String line = br.readLine();
				if(line == null) break;
				int i;
				for(i=0; i<line.length(); i++)	//this loop separate item component and price component
					if(line.charAt(i) == ' ')
						break;
				map.put(line.substring(0, i), Double.parseDouble(line.substring(i+1, line.length())));	//use substring to store information separately	
			}	
		} catch(IOException e) {	//case that input file not found
			System.out.println("Input file not found!");
			e.printStackTrace();
		}
		
		Iterator<String> mapIter = map.keySet().iterator();	//use Iterator to iterate items in object map
		String maxItem=null, minItem=null;	//String object to store the most expensive item, cheapest item.
		double maxPrice=0, minPrice=0;	//Double variable to store the most expensive price, cheapest price
		double sum=0;	//variable to calculate average
		int num=0;	//variable that counts keys in object map
		if (mapIter.hasNext()) {	//initializing above variable
			String key = mapIter.next();
			maxItem = key;
			minItem = key;
			maxPrice = map.get(key);
			minPrice = map.get(key);
			sum += map.get(key);
			num++;
		}
		while(mapIter.hasNext()) {	//iterating process : update max, min item and price
			String key = mapIter.next();
			if(map.get(key) > maxPrice) {
				maxItem = key;
				maxPrice = map.get(key);
			}
			
			if(map.get(key) < minPrice) {
				minItem = key;
				minPrice = map.get(key);
			}
			sum += map.get(key);	//add price of key in every loop.
			num++;
		}
		sum /= num;
		//prints final result
		System.out.println("Summary");
		System.out.println("-------");
		System.out.printf("number of items: %d\n", num);
		System.out.printf("most expensive item: %s (%.2f)\n", maxItem, map.get(maxItem));
		System.out.printf("cheapest item: %s (%.2f)\n", minItem, map.get(minItem));
		System.out.printf("average price of items: %.2f\n",  sum);		
	}
}