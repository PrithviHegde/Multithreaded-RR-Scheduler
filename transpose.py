import sys
fname = sys.argv[1]
f = open(fname,'r')
matrix = []
while True:
    line = f.readline()
    if not line:
        break
    row = list(map(int, line.split()))
    matrix.append(row)
f.close()
x,y = len(matrix), len(matrix[0])
m = [[0 for i in range(x)] for j in range(y)]
for i in range(len(matrix)):
    for j in range(len(matrix[0])):
        m[j][i] = matrix[i][j]
f = open(fname,'w')
for i in range(len(m)):
    for j in range(len(m[0])):
        if j != len(m[0])-1: f.write(str(m[i][j]) + ' ')
        else: f.write(str(m[i][j]) + '\n')
f.close()

