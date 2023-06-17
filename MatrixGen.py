import numpy as np
import sys

LONG_LONG_MAX =  9223372036854775807

def genMatrix():
    n = int(sys.argv[1])
    m = int(sys.argv[2])
    #f =  open("Matrix_out.txt")
    mat = np.random.randint(0,1000,size=(n,m))
    np.savetxt("in1.txt",mat,fmt='%i')
    
    m2 = int(sys.argv[3])

    mat2 = np.random.randint(0,1000,size=(m,m2))

    np.savetxt("in2.txt",mat2,fmt='%i')

    matRes = np.matmul(mat,mat2)
    if (matRes>LONG_LONG_MAX).sum() > 0:
        genMatrix()
    np.savetxt("matrixres.txt",matRes,fmt='%i')

if __name__ == "__main__":
    genMatrix()
