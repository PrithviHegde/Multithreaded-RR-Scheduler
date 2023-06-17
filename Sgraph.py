from matplotlib import pyplot as plt
import pandas as pd
import os,sys
import time

generateMatrix = "python3 MatrixGen.py "
transpose = "python3 preop.py in2.txt"
runScheduler = "in1.txt in2.txt out.txt"
turnaroundtimes = [[],[],[]]
waitingtimes = [[],[],[]]
vals = [1,5,10,50,100,500,1000]
cock = set()
for i in vals:
    for j in vals:
        for k in vals:
            if i*j*k in cock:continue
            cock.add(i*j*k)
            print(i,j,k)
            turnaroundtimes[0].append(i*j*k)
            waitingtimes[0].append(i*j*k)
            cmdgenmatrix = generateMatrix + ' ' + str(i) + ' ' + str(j) + ' ' + str(k)
            os.system(cmdgenmatrix)
            os.system(transpose)
            cmd = './c ' + str(i) + ' ' + str(j) + ' ' + str(k) + ' ' + runScheduler
            os.system(cmd)
t = 0
turnaroudndf = pd.read_csv("turnAround.csv")
waitingdf = pd.read_csv("waiting.csv")
cock = set()
for i in vals:
    for j in vals:
        for k in vals:        
            if i*j*k in cock:continue
            cock.add(i*j*k)
            row = turnaroudndf.iloc[t].to_numpy();
            turnaroundtimes[1].append(row[1])
            turnaroundtimes[2].append(row[2])
            row = waitingdf.iloc[t].to_numpy();
            waitingtimes[1].append(row[1])
            waitingtimes[2].append(row[2])
            t+=1
t,w = [],[]
for i in range(len(turnaroundtimes)):
    t.append((turnaroundtimes[0][i],turnaroundtimes[1][i],turnaroundtimes[2][i]))
    w.append((waitingtimes[0][i],waitingtimes[1][i],waitingtimes[2][i]))          
t.sort()
w.sort()
for i in range(len(t)):
    turnaroundtimes[0][i] = t[i][0]
    turnaroundtimes[1][i] = t[i][1]
    turnaroundtimes[2][i] = t[i][2]
for i in range(len(w)):
    waitingtimes[0][i] = w[i][0]
    waitingtimes[1][i] = w[i][1]
    waitingtimes[2][i] = w[i][2]

plt.title("Turnaround time vs Workload")
plt.xlabel("Workload")
plt.ylabel("Turn Around time")
plt.plot(turnaroundtimes[0], turnaroundtimes[1],label="P1")
plt.plot(turnaroundtimes[0], turnaroundtimes[2],label="P2")
plt.show()
plt.close()
plt.title("Waiting time vs Workload")
plt.xlabel("Workload")
plt.ylabel("Waiting time")
plt.plot(waitingtimes[0], waitingtimes[1],label="P1")
plt.plot(waitingtimes[0], waitingtimes[2],label="P2")
plt.show()
plt.close()
