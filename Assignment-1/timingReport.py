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
for i in range(10,110,10):
    time.sleep(5)
    x=pexpect.spawn("python3 timing.py -c "+str(i)+ " -p "+args.parllel)
    print("For "+ str(i)+ " clients," + str(x.readline()))
    x.close()
sys.exit()