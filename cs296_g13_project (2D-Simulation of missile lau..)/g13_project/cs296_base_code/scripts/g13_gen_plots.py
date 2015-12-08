import matplotlib.pyplot as plt
import numpy as np
import subprocess
import random
import re
subprocess.call(['mkdir','-p','plots'])
itnum=500
renum=50
randnum=5#number of random reruns to be picked for each iteration value. I am not taking it 15 as renum value has been reduced from 150 to 50
f = open('./data/g13_lab09data.csv', 'r')
data=[[],[],[],[],[],[],[]]#iteration value, rerun number, step time, collision time, velocity time, position time, loop time

for line in f:
	inp=re.findall(r'[0-9.]+', line)
	data[0].append(int(inp[0]))
	data[1].append(int(inp[1]))
	data[2].append(float(inp[2]))
	data[3].append(float(inp[3]))
	data[4].append(float(inp[4]))
	data[5].append(float(inp[5]))
	data[6].append(float(inp[6]))

avg=([],[],[],[],[],[],[])#iteration value, step time, collision time, velocity time, position time, loop time, sum of collision,position update and vel update
stdev=[]#standard deviation for step time
for it in range(0,itnum):
	avg[0].append(it+1)
	for feature in range(2,7):
		tempavg=np.array(data[feature][it*renum:(it+1)*renum])
		avg[feature-1].append(np.mean(tempavg))
	avg[6].append(avg[2][it]+avg[3][it]+avg[4][it])
	tempstd=np.array(data[2][it*renum:(it+1)*renum])
	stdev.append(np.std(tempstd))				

numbins=10
data_54=data[2][53*renum:54*renum]#step time for highest roll number
data_54_freq=np.histogram(np.array(data_54),numbins)
data_54_cum=np.cumsum(data_54_freq[0])

randavg=list()

for it in range(0,itnum):
	templist=random.sample(data[2][it*renum:(it+1)*renum],randnum)
	randavg.append(np.mean(templist))

mrand,crand=np.polyfit(range(1,itnum+1),randavg,1)
m,c=np.polyfit(range(1,itnum+1),avg[1],1)
randl=[mrand+crand,mrand*itnum+crand]
norml=[m+c,m*itnum+c]
print(m,c,mrand,crand)

fig = plt.figure()
p1 = fig.add_subplot(111)
p1.set_title('Step time and Loop time vs Number of iterations')    
p1.set_xlabel('Number of Iterations')
p1.set_ylabel('Averaged time over iterations (ms)')
p1.bar(avg[0], avg[1],facecolor='r',edgecolor='white',label='Average Step Time')
p1.plot(avg[0],avg[5],label='Average Total Loop Time')
p1.legend(loc=2)
fig.savefig('./plots/g13_plot01.png')

fig = plt.figure()
p2 = fig.add_subplot(111)
p2.set_title('Time vs Number of iterations')    
p2.set_xlabel('Number of Iterations')
p2.set_ylabel('Averaged time over iterations (ms)')
p2.plot(avg[0], avg[1],color='r',label='Average Step Time')
p2.plot(avg[0], avg[2],color='b',label='Average Collision Time')
p2.plot(avg[0], avg[3],color='g',label='Average Velocity update Time')
p2.plot(avg[0], avg[4],color='c',label='Average Position update Time')
p2.plot(avg[0], avg[6],color='m',label='Sum of Collision, Velocity and Position update Time')
p2.legend()
fig.savefig('./plots/g13_plot02.png')

fig = plt.figure()
p3 = fig.add_subplot(111)
p3.set_title('Step Time vs Number of iterations')    
p3.set_xlabel('Number of Iterations')
p3.set_ylabel('Averaged time over iterations (ms)')
p3.errorbar(avg[0], avg[1],stdev,color='r',ecolor='g',label='Error Bars')
p3.plot(avg[0], avg[1],color='r',label='Average Step Time')
p3.legend()
fig.savefig('./plots/g13_plot03.png')

fig = plt.figure()
p4 = fig.add_subplot(111)
p4.set_title('Step Time Frequency Plot')    
p4.set_xlabel('Step Time (ms)')
p4.set_ylabel('Frequency')
p4.hist(data_54,numbins,color='g',label='Step Time Frequency')
p4.plot(data_54_freq[1][1:],data_54_cum,color='r',label='Cumulative Step Time Frequency')
p4.legend(loc=2)
fig.savefig('./plots/g13_plot04.png')

fig = plt.figure()
p5 = fig.add_subplot(111)
p5.set_title('Step time vs Number of iterations')    
p5.set_xlabel('Number of Iterations')
p5.set_ylabel('Averaged time over iterations (ms)')
p5.scatter(avg[0],avg[1],color='r',label='Step time for all reruns of iterations')
p5.scatter(avg[0],randavg,color='g',label='Step time for random reruns of iterations')
p5.plot([1,itnum],norml,color='m',label='Model fit for all reruns')
p5.plot([1,itnum],randl,color='b',label='Model fit for random reruns')
p5.legend()
#plt.show()
fig.savefig('./plots/g13_plot05.png')			


