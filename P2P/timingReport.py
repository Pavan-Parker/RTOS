import pexpect
import threading
import timeit
import time
import sys
import random
import argparse
from functools import reduce
parser = argparse.ArgumentParser()
parser.add_argument("-p","--parllel")
args = parser.parse_args()

if(args.parllel=='1'): print("******PARLLEL********")
else:print("******NORMAL********")
print("| No of Clients | Time taken |")
print("| ------------- |-----------:|")
for i in [10,50,100,200,400,500]:
    time.sleep(5)
    x=pexpect.spawn("python3 timing.py -c "+str(i)+ " -p "+args.parllel)
    print("| "+ str(i)+ " | " + str(x.readline().decode('utf-8'))+" |")
    x.close()
sys.exit()