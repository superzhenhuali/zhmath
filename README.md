# zhmath

#### Description
This is a  C programming library for mathematics, especially for algebra structures.
Currently avalible for Hecke-Clifford superalgebra.

#### Examples:
The result of the center of Hecke-Clifford superalgebra of order 2,3,4 are in files:
	tex/example_2.pdf
	tex/example_3.pdf
	tex/example_4.pdf
Please go to the end of file to check it.


The function test_algebra_8() in  test.c is to calculate the center of Hecke-Clifford superalgebra of order n.
Add it to the main function, and 
	make clean; make 
Then run the following to calcute the case n=2:
	./test 2 > tex/output.tex 
The output is in tex/output.tex.  
Compiling it to PDF:
	cd tex/ ;
	pdflatex example.tex
You can open the file tex/example.pdf to see the result. 
