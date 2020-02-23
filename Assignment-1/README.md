# Assignment - 1
## Description

```
timing.py -c [userCount] -p [1 if parllel else 0]
    > spawns server and clients' executables of the given userCount.
    > if parllel flag is set, make all users except last 2 spawned, to keep spamming the server. else, no parllel convos.
    > prints the time taken to send from last but one user spawned to last user spawned. 
```
```
timingReport.py -p [1 if parllel else 0]
    > prints the time taken for each user count in [10,50,100,200,400,500] using timing.py . 
```
## Results

---------------------------------------------------
No   No of Clients   No of parallel Users Time taken
-----------------------------------------------------
1.        10                  3             400ms
------------------------------------------------------
2.        20                  4              410ms
----------------------------------------------------
3.        30                  5             420ms
---------------------------------------------------
9.        90                  5             900ms
-------------------------------------------------------
10.       100                5              900ms
