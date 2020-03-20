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
**For no parllel users**

| No of Clients | Time taken |
| ------------- |-----------:|
| 10 | 2.582999999999988 ms |
| 50 | 3.1140000000000057 ms |
| 100 | 2.4020000000000152 ms |
| 200 | 2.5290000000000035 ms |
| 400 | 2.2719999999999962 ms |
| 500 | 2.694000000000085 ms |
| 550 | 2.634000000000025 ms |

**For (n-2) parllel users**
| No of Clients | Time taken |
| ------------- |-----------:|
| 10 | 3.144000000000008 ms|
| 50 | 13.469000000000008 ms |
| 100 | 14.872999999999998 ms |
| 200 | 128.9 ms |
| 400 | 225.71799999999996 ms |
| 500 | 314.9080000000001 ms |
| 550 | 369.51500000000016 ms |
## Notes and Observations
+    I've taken (n-2) parllel user convos case, as it is the worst case and variation with user count could be observed easily.

+    For no parllel convos case, it seems delay is almost invariant with user count
+    Where as in parllel convos case, it seems delay seems to be increasing exponentially w.r.t usercount. (just by eyeballing)
