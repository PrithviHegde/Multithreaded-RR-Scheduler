import time
import os
import numpy as np
import matplotlib.pyplot as pl
import csv

cmdline1 = "gcc P2.c -o b"
cmdline2_1 = "./b"
cmdlinea = "gcc P1.c -o a"
cmdlinea_1 = "./a"
cmdline2_2 = " in1.txt in2.txt out.txt "
cmdlinea_2 = " in1.txt in2.txt "

f1 = open("in1.txt",'r')
f2 = open("in2.txt",'r')
rowf1 = 0
rowf2 = 0
colf2 = 0
for x in f1:
    rowf1+=1

for x in f2:
    rowf2+=1

f2.close()
f2 = open("in2.txt",'r')
a = f2.readline()
for i in range(len(a)):
    if a[i]==" ":
        colf2+=1

colf2 +=1
rowf2,colf2 = colf2,rowf2 
print(rowf1,rowf2,colf2)
cmdline2_2 = " " + str(rowf1) + " " + str(rowf2) + " " + str(colf2) + cmdline2_2
cmdlinea_2 = " " + str(rowf1) + " " + str(rowf2) + " " + str(colf2) + cmdlinea_2
numThreads = []
timeTaken = []
rows = []
min = 9999999999999
mini = 0
os.system(cmdlinea)
os.system(cmdline1)

#for i in range(1,max(rowf1, rowf2)+1):
for i in range(1,65):
    cmdline2_a = cmdlinea_1  + cmdlinea_2 + "16"
    cmdline2 = cmdline2_1 + cmdline2_2 + str(i)
    
    print(cmdline2_a)
    os.system(cmdline2_a)
    time1 = time.time_ns()
    os.system(cmdline2)
    time2 = time.time_ns()
    print("For",i,"threads, time taken is",time2-time1,"ns.")
    if (time2-time1<min):
        mini=i
        min = time2-time1
    numThreads.append(i)
    timeTaken.append(time2-time1)


with open('timedatap2.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['no.of threads','mult. time in ns'])
    [rows.append([numThreads[i],timeTaken[i]]) for i in range(len(numThreads))]
    writer.writerows(rows)


pl.plot(numThreads,timeTaken)
msg = "Min time taken is "+ str(min) + " ns,\n when "+str(mini)+" threads are used"
#pl.xticks(numThreads)
pl.xlabel("Number of threads used in multiplication")
pl.ylabel("Time taken to compute the product (in nanoseconds)")
pl.title("Plot of time vs. no.of threads with 1e9 Workload") 
pl.show()
