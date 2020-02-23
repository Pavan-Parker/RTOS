import pexpect
import threading
import timeit
import time
import sys
import random
import argparse
from functools import reduce
parser = argparse.ArgumentParser()
parser.add_argument("-c","--clientsCount")
parser.add_argument("-p","--parllel")
args = parser.parse_args()

clientsCount=int(args.clientsCount)
port=random.randrange(11000,11100)
def spam(i):
    while(True):
        allSpawns[i].sendline("SENDGROUP 1 SUPBOIS")  
def observe(i):
    while(True):
        print(allSpawns[i].readline())

allSpawns=[]
threads=[]
allSpawns.append(pexpect.spawn("./server",args=[str(port)],use_poll=True))
for i in range(clientsCount):
    allSpawns.append(pexpect.spawn("./client",args=[str(i),str(port)],use_poll=True))

creationCommand=reduce((lambda x,y: str(x)+" "+str(y)),range(1,(clientsCount)+1))
allSpawns[-1].sendline("CREATEGROUP testgrp "+str(clientsCount)+" "+creationCommand)

if(args.parllel=='1'):
    for i in range(1,len(allSpawns)-2):
        x=(threading.Thread(target=spam,args=(i,),daemon=True))
        threads.append(x)
        x.start()
'''
y=(threading.Thread(target=observe,args=(-1,)))
threads.append(y)
z=(threading.Thread(target=observe,args=(0,)))
threads.append(z)
#map(lambda x: x.join(), threads)
'''
start=time.clock()
#start=timeit.timeit()
allSpawns[-2].sendline("SENDGROUP 1 SUPLASTONE")
if(allSpawns[-1].expect(['SUPLASTONE\r\n'],timeout=20)==0):
    end=time.clock()
    #end=timeit.timeit()
    print(str((end-start)*1000)+" ms")

map(lambda x:x._stop() , threads)
map(lambda x:x.close() , allSpawns)
sys.exit()