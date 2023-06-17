# Multithreaded Round Robin Scheduler For Matrix Multiplication

This project aims to optimize an I/O and calculation heavy task such as matrix multiplication by the use of multithreading and round robin scheduling between the script reading the matrices and the one performing the calculation. Implemented in C

### Usage

 - Run `MatrixGen.py` to create two random matrices
	 - Usage: `python3 MatrixGen.py <A> <B> <C>` where A,B,C are positive integers
	 - This creates 2 random matrices, the first one of size AxB stored in `in1.txt`, and the second one of size BxC stored in `in2.txt`.
	 - It also creates the expected output of the multiplication of these matrices and stores the result in `matrixres.txt`
 - Run `transpose.py` to transpose the 2nd matrix for simplification of calculation
	 - Usage: `python3 transpose.py`
 - Update the `run.sh` file's 5th line
	 - Usage: `time ./c <A> <B> <C> <input_file1> <input_file2> <output_file>` where A, B and C are the same dimensions as above. `input_file1`, `input_file2` and `output_file` can be left as is.
 - Run the above bash file. Output will be generated in the output file specified above.


We can see a significant improvement in overall time taken, especially on large improvements. Graphs about waiting times, turn around times, time taken to read/compute the inputs vs number of threads is present in `Images` folder. The data for the same is in `CSVs` folder.
