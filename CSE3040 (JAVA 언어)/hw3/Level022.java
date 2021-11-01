package hw3;

/**
 * CSE3040 HW3 Level022.java Purpose: Build a program that fetches a URL on the Internet and finds the information we need (Using Jsoup)
 * 
 * @version 1.0 12/11/2019
 * @author Minseok Kang
 */

import java.io.IOException;	//import to handle input/output exception
// import utility below to use Jsoup to crawl information
import org.jsoup.Jsoup;	
import org.jsoup.nodes.Document;
import org.jsoup.select.Elements;


public class Level022 {
	public static void main(String[] args) throws Exception {
		String url = "https://aladin.co.kr/shop/common/wbest.aspx?BestType=Bestseller&BranchType=1&CID=0";	//target address
		Document doc = null;
		
		try {
			doc = Jsoup.connect(url).get();	//get data from url source using Jsoup
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
		
		Elements bestsellers = doc.select("div.ss_book_list");	//minimize information(exclude unnecessary information
		Elements titles = bestsellers.select("a.bo3");	//store title of bestsellers books 
		Elements author = bestsellers.select("li");	//store author of bestsellers books
		
		Elements year = doc.select("div.newbs_year");	//store current year
		Elements month = doc.select("div.newbs_month");	//store current month
		Elements week = doc.select("div.newbs_week");	//store current week
		
		//print out current time
		System.out.println("[" + year.eq(0).text() + " " + month.eq(0).text() + " " + week.eq(0).text() + "]");
		
		int j = 0;
		for (int i = 0; i < titles.size(); i++) {
			//print title of the books.
			System.out.print(i + 1 + "À§: " + titles.eq(i).text() + " ");
			//print author of the books.(As example output only prints out first author, string slicing needed)
			for(; j<author.size(); j++) {
				if(author.eq(j).toString().contains("AuthorSearch")) {
					int begin = author.eq(j).toString().indexOf("\">") + "\">".length();
					int end = author.eq(j).toString().indexOf("</a>");
					System.out.println("(" + author.eq(j).toString().substring(begin, end) + ")");
					j++;
					break;
				}
			}
		}
	}
}