# LUP Decomposition CA Assignment - 5

Name : Anand Kadale
PRN: 21620003

Assignment 5 Comments :
In this code I have implimented program to solve 'Ax = b' matrix, where A and b is Known and x has
to find.

Observations :
1) This code is slight more complicated because we are calculating matrix values.
2) This code is also little bit prone to have some errors as we try rectangular matrix.

* For making it parallel :
1) In this we can parallely compute the L and U matrix which will give us a very less execution time as
compared to this implementation.





Comment on Time Complexity of this Algorithm :

Here for decomposition this matrix I have used nested loop and with this I also doing some constant
work
So far this nested loop will take θ(n^3) time and the constant work will take O(1) time.
We will get,

θ(n^3) + O(1) - recurrence

after solving this we will get ,

O( n^3 ) as the final Complexity of time.
