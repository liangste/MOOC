// to compile
// 				javac -encoding UTF-8 EnergyValues.java
// to run from current directory
// 				java -cp ./ EnergyValues

import java.io.*;

public class EnergyValues {
	public static void main (String[] args) {
		try {
			System.out.println("Hello, World");
			int n = System.in.read();
			System.out.format("%d", n);
		} catch(IOException e){
			System.out.print("Exception");
		} finally {
		}
	}
}
