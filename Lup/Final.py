import numpy as np

def forward_sub(L, b):
    """Given a lower triangular matrix L and right-side vector b,
    compute the solution vector y solving Ly = b."""

    y = []
    for i in range(len(b)):
        y.append(b[i])
        for j in range(i):
            y[i]=y[i]-(L[i, j]*y[j])
        y[i] = y[i]/L[i, i]

    return y

def backward_sub(U, y):
    """Given a lower triangular matrix U and right-side vector y,
    compute the solution vector x solving Ux = y."""

    x = np.zeros_like(y)

    for i in range(len(x), 0, -1):
      x[i-1] = (y[i-1] - np.dot(U[i-1, i:], x[i:])) / U[i-1, i-1]

    return x


def lu_factor(A):

    #LU decompostion using Doolittles method

 L = np.zeros_like(A)
 U = np.zeros_like(A)

 N = np.size(A,0)

 for k in range(N):
        L[k, k] = 1
        U[k, k] = (A[k, k] - np.dot(L[k, :k], U[:k, k])) / L[k, k]
        for j in range(k+1, N):
            U[k, j] = (A[k, j] - np.dot(L[k, :k], U[:k, j])) / L[k, k]
        for i in range(k+1, N):
            L[i, k] = (A[i, k] - np.dot(L[i, :k], U[:k, k])) / U[k, k]

 return (L, U)

def lu_solve(L, U, b):
    # Step 1: Solve Uy = b using forward substitution
    y = forward_sub(L,b)

    # Step 2: Solve Lx = y using backward substitution
    x = backward_sub(U,y)

    return x


def linear_solve(A, b):

    L, U = lu_factor(A)
    x = lu_solve(L,U,b)
    return x

def main():

  A = np.array([
      [-2,8,1,10],
      [5,2,3,6],
      [3,-5,10,-2],
      [6,-2,4,5]])

  # A = [[-2, 8, 1, 10],
  #      [5, 2, 3, 6],
  #      [3, -5, 10, -2],
  #      [6, -2, 4, 5]]
  
  b = []

  ele = int(input('Enter No. of Urban total voters :'))
  b.append(ele)

  ele = int(input('Enter No. of Sub-Urban total voters :'))
  b.append(ele)

  ele = int(input('Enter No. of Rular total voters :'))
  b.append(ele)

  ele = int(input('Enter No. of Outsider total voters :'))
  b.append(ele)
  print("\n")
  print("Values of Voters are : " + str(b))

  res = np.array(linear_solve(A,b))
  print("Money to be sped on advertisement ( value of x in Ax = b) : " + str(res) + "\n")
   

  # this implementation is of last step of this assignment in which we have to print all the sum 
  
  # a = A[0, 0] + A[1, 0] + A[2, 0] + A[3, 0] * float(res[1])
  a = A[0, 0] + A[1, 0] + A[2, 0] + A[3, 0]
  print("Bulid Roads generated : " + str(np.dot(a, res[0])) + "k Votes \n")

  b = A[0, 1] + A[1, 1] + A[2, 1] + A[3, 1] 
  # a = A[0, 0] + A[1, 0] + A[2, 0] + A[3, 0]
  print("Infrastructure generated : " + str(np.dot(b, res[1])) + "k Votes \n")

  c = A[0, 2] + A[1, 2] + A[2, 2] + A[3, 2] 
  # a = A[0, 0] + A[1, 0] + A[2, 0] + A[3, 0]
  print("Farm Subsidies generated : " + str(np.dot(c, res[2])) + "k Votes \n")

  d = A[0, 3] + A[1, 3] + A[2, 3] + A[3, 3] 
  # a = A[0, 0] + A[1, 0] + A[2, 0] + A[3, 0]
  print("Fuel Price generated : " + str(np.dot(d, res[3])) + "k Votes \n ")

  """ Inputed b = [24, 30, 14, 48] """

if __name__=='__main__':
  main();
