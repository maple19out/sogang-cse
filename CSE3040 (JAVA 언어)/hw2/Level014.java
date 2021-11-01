package hw1;

/**
 * CSE3040 HW2 Level014.java Purpose: calculate distance between two points in
 * N-dimensional space
 * 
 * @version 1.0 11/3/2019
 * @author Minseok Kang
 */


/**
 * class Point has variable of double array
 */
class Point{
	double[] arr;
	
	Point(double[] arr){
		this.arr = new double[arr.length];	//allocate according to input array length
		for(int i=0; i<arr.length; i++)	//copy array
			this.arr[i] = arr[i];
	}
}

/**
 * class EuclideanDistance calculates Euclidean distance of two points
 */
class EuclideanDistance{
	/**
	 * static method that calculates euclidean distance
	 * @param p1 : data of point 1
	 * @param p2 : data of point 2
	 * @return calculated sum or error(-1)
	 */
	static double getDist(Point p1, Point p2) {
		double sum=0;
		if(p1.arr.length != p2.arr.length)	//if dimension is different return -1
			return -1;
		for(int i=0; i<p1.arr.length; i++)	//calculating process
			sum += (p1.arr[i]-p2.arr[i])*(p1.arr[i]-p2.arr[i]);
		return Math.sqrt(sum);
	}
}

/**
 * class ManhattanDistance calculates Manhattan distance of two points
 */
class ManhattanDistance{
	/**
	 * static method that calculates manhattan distance
	 * @param p1 : data of point 1
	 * @param p2 : data of point 2
	 * @return calculated sum or error(-1)
	 */
	static double getDist(Point p1, Point p2) {
		double sum=0;
		if(p1.arr.length != p2.arr.length)	//if dimension is different return -1
			return -1;
		for(int i=0; i<p1.arr.length; i++)	//calculating process
			sum += Math.abs(p1.arr[i]-p2.arr[i]);
		return sum;
	}
}

/**
 * public class prints two types of distance between 2 points in n-dimensional space
 */
public class Level014 {
	public static void main(String[] args) {
		Point p1 = new Point(new double[] { 1.0, 2.0, 3.0 });	//initialize with 3-dimension
		Point p2 = new Point(new double[] { 4.0, 5.0, 6.0 });	//initialize with 3-dimension
		System.out.println("Euclidean Distance: " + EuclideanDistance.getDist(p1, p2));	//prints euclidean distance
		System.out.println("Manhattan Distance: " + ManhattanDistance.getDist(p1, p2));	//prints manhattan distance
		Point p3 = new Point(new double[] { 1.0, 2.0, 3.0 });	//initialize with 3-dimension
		Point p4 = new Point(new double[] { 4.0, 5.0 });	//initialize with 2-dimension
		System.out.println("Euclidean Distance: " + EuclideanDistance.getDist(p3, p4));	//prints euclidean distance(error expected)
		System.out.println("Manhattan Distance: " + ManhattanDistance.getDist(p3, p4));	//prints manhattan distance(error expected)
	}
}