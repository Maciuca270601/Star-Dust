Name: Maciuca Alexandru Petru
Group: 314CA
Course: PC
Title: Star Dust

November 2020

## First Task
    
    I allocated a matrix of integers along with a vector that holds
    the number of elements of each row of the matrix.
    
    I wrote each element of the matrix in its unfolded form
    with the help of a char type vector so that I can then calculate the amount later.
    
    I noticed that on the first line and on line 'n-1' all 4 bytes values are added
    but for the other lines only the first and the last bytes are 
    so I deduced a formula for the sum and the number of surfaces.
    
    I displayed the result of the get_shield function in a double variable
    to keep the precision of 7 decimals. (get_shield returns the arithmetic_average)

## Second Task
	  
    For this task I choose also to use the same shift on bytes so I
    could extract from the matrix an int element and divide it in 4 bytes.

    For the swap and delete function, I performed various necessary operations
    on the vector of bytes in order to obtain the desired result.
    Later, I had converted the 4 bytes from the vector back to an element of type int.
    
    For the modify function, I performed the same reasoning but firstly I have checked
    if the operation is realizable.
    If I had place in the matrix, I performed it similarly as with the other 2 functions
    but if I didn't, I had to use a helper function which checks the date type 
    and how many elements have to be added and then reallocate the memory if necessary.
