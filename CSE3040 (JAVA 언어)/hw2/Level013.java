package hw1;

/**
 * CSE3040 HW2 Level013.java Purpose: calculate the sum of area of multiple shapes
 * 
 * @version 1.0 11/3/2019
 * @author Minseok Kang
 */


/**
 * Class Shape has variable area and related methods setArea, getArea
 */
class Shape{
	private double area;	//private instance variable area
	
	/**
	 * Method that sets area 
	 * @param double area
	 */
	public void setArea(double area) {
		this.area = area;
	}
	
	/**
	 * Method that returns area
	 * @return double type data area
	 */
	public double getArea() {
		return area;
	}
}

/**
 * class Circle that extends class Shape
 */
class Circle extends Shape{
	/**
	 * Constructor with parameter calls method in superclass Shape
	 * @param double type data radius (radius of Circle)
	 */
	Circle(double radius){
		setArea(radius*radius*Math.PI);
	}
}

/**
 * class Square that extends class Shape
 */
class Square extends Shape{
	/**
	 * Constructor with parameter calls method in superclass Shape
	 * @param double type data x (length of Square)
	 */
	Square(double x){
		setArea(x*x);
	}
}

/**
 * class Rectangle that extends class Shape
 */
class Rectangle extends Shape{
	/**
	 * Constructor with parameter calls method in superclass Shape
	 * @param x (length of horizontal side)
	 * @param y (length of vertical side)
	 */
	Rectangle(double x, double y){
		setArea(x*y);
	}
}


/**
 * public class that prints sum of total Shape
 */
public class Level013 {
	public static void main(String args[]) {
		Shape[] arr = {new Circle(5.0), new Square(4.0), new Rectangle(3.0, 4.0), new Square(5.0)};	//create instance of subclass of Shape
		System.out.println("Total area of the shapes is: " + sumArea(arr));
	}
	
	/**
	 * method that adds total area of array Shape
	 * @param Shape[] arr
	 * @return double type sum(total area)
	 */
	public static double sumArea(Shape[] arr) {
		double sum=0;
		for(int i=0; i<arr.length; i++)	//iterate until i reaches length of arr.
			sum += arr[i].getArea();
		return sum;
	}
}
