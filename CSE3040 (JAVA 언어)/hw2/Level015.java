package hw1;

/**
 * CSE3040 HW2 Level015.java Purpose: compare two instances of class Points
 * 
 * @version 1.0 11/3/2019
 * @author Minseok Kang
 */

/**
 * class Points stores array of double numbers;
 */
class Points{
	double[] arr;	//double array not initialized
	double sum=0;	//sum of arr elements
	
	/**
	 * Constructor that initializes variable arr, sum;
	 * @param double[] arr
	 */
	Points(double[] arr){
		this.arr = new double[arr.length];	//allocate length of array
		for(int i=0; i<arr.length; i++) {	//copy array
			this.arr[i] = arr[i];
			sum += arr[i];	//adds all of the elements in array to sum	
		}
	}
	
	/**
	 * method that overrides equals method
	 * @param Object p
	 * @return boolean data
	 */
	@Override
	public boolean equals(Object p) {
		if(this == p) return true;	//return true if same instance
		if(p == null) return false;	//return false if p is null
		if(this.getClass() != p.getClass()) return false;	//return false is two instances are from diffrent class
		Points other = (Points)p;	//(typecasting p to Points)
		return this.sum == other.sum;	//return true if sum of elements in arr is same
	}
	
	/**
	 * method that overrides toString method
	 * @param args not used
	 * @return String that shows sum of points
	 */
	@Override
	public String toString() {
		return "[sum of points: "+ this.sum + "]";		
	}
}

/**
 * class that shows equality of two Points
 */
public class Level015 {
	public static void main(String[] args) {
		Points p1 = new Points(new double[] { 1.0, 2.0, 3.0 });	//create an instance
		Points p2 = new Points(new double[] { 4.0, 5.0, 6.0 });	//create an instance
		System.out.println(p1);	//print sum of p1(overriden method)
		System.out.println(p2);	//print sum of p2(overriden method)
		System.out.println(p1.equals(p2));	//print equality of p1, p2(overriden method)
		Points p3 = new Points(new double[] { 1.0, 4.0, 7.0 });	//create an instance
		Points p4 = new Points(new double[] { 3.0, 9.0 });	//create an instance
		System.out.println(p3);
		System.out.println(p4);
		System.out.println(p3.equals(p4));
		Points p5 = new Points(new double[] { 1.0, 2.0 });	//create an instance
		Points p6 = null;
		System.out.println(p5);
		System.out.println(p6);
		System.out.println(p5.equals(p6));
	}
}