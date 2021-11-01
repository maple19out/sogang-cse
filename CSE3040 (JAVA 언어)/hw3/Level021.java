package hw3;

/**
 * CSE3040 HW3 Level021.java Purpose: Build a program that fetches a URL on the Internet and finds the information we need.
 * 
 * @version 1.0 12/11/2019
 * @author Minseok Kang
 */

import java.io.*;	//import to handle input/output exception
import java.util.*;	//import to use ArrayList
import java.net.*;	//import to use URL

public class Level021 {
	static ArrayList<String> lines = new ArrayList<String>();	//use ArrayList to store every source line in URL

	public static void main(String[] args) {
		URL url = null;
		BufferedReader input = null;
		
		//target URL address
		String address = "https://aladin.co.kr/shop/common/wbest.aspx?BestType=Bestseller&BranchType=1&CID=0";
		String line = "";
		try {
			url = new URL(address);
			input = new BufferedReader(new InputStreamReader(url.openStream()));	//Read line by line
			//Loop below adds only valid sentence to ArrayList
			while ((line = input.readLine()) != null) {
				if (line.trim().length() > 0)
					lines.add(line);
			}
			input.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		int rank = 1;	//variable that shows rank of the book
		int status = 0;	//variable that properly handle raw URL source lines
		String time = "";	//Object that stores current Year/Month/Week shown in URL
		
		//Loop below extracts information of current Year/Month/Week in URL
		for (int i = 0; i < lines.size(); i++) {
			String l = lines.get(i);
			if (l.contains("<div class=\"")) {
				if (l.contains("Layer_Year")) {
					int begin = l.indexOf(")\">") + ")\">".length();
					int end = l.indexOf("<img");
					time += (l.substring(begin, end) + " ");
				} else if (l.contains("Layer_Month")) {
					int begin = l.indexOf(")\">") + ")\">".length();
					int end = l.indexOf("<img");
					time += (l.substring(begin, end) + " ");
				} else if (l.contains("Layer_Week")) {
					int begin = l.indexOf(")\">") + ")\">".length();
					int end = l.indexOf("<img");
					time += l.substring(begin, end);
					break;
				}
			}
		}
		System.out.println("[" + time + "]");
		
		//Loop below extracts information of bestseller books and their author.
		for (int i = 0; i < lines.size(); i++) {
			String l = lines.get(i);

			if (status == 0) {
				if (l.contains("<div class=\"ss_book_list\">")) {
					status = 1;
				}
			} else if (status == 1) {
				//Extracts title of the book.
				if (l.contains("class=\"bo3\"")) {
					int begin = l.indexOf("<b>") + "<b>".length();
					int end = l.indexOf("</b></a>");
					System.out.print(rank + "À§: " + l.substring(begin, end) + " ");
					status = 2;
				}
			} else if (status == 2) {
				//Extracts author of the book.
				if (l.contains("a href")) {
					int begin = l.indexOf("\">") + "\">".length();
					int end = l.indexOf("</a>");
					System.out.println("(" + l.substring(begin, end) + ")");
					rank++;
					status = 0;
				}
			}
		}
	}
}