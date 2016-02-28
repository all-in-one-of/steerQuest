#!/usr/bin/python

import random
f = open('./sim_data.txt','w')

for time in range(100):
	for sheep in range(100):
		f.writelines( str(time)+' '+str(sheep)+' '+ str(random.randint(0,1000)) +' '+ str(random.randint(1,1000)) +'\n' )

f.close()
