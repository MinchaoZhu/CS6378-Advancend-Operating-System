# Project 1


## Compile

```bash
> chmod +x build.sh
> ./build.sh
```
## Clean

```bash
> chmod +x clean.sh
> ./clean.sh
```

## Run

After compilation, the executable file is in ```./build``` folder.
```bash
> chmod +x launcher.sh
```

There are four arguments for program:

1. node id
2. is initially active
    - yes: 0
    - no: 1
3. conf path
4. snapshots times
    - starter node: snapshots times
    - others: 0


The node should runs on server(dcXX) defined in conf file.

For initially active node and snapshot from this node for 6 times:

```bash
./launcher.sh 0 0 main.txt 6
```


For inactive node:
```bash
./launcher.sh 2 1 main.txt 0
```


## Result

### Part 1

After launch all nodes, 

nodes will send and receive and print messages on terminal:

```
nodeNumbers: 5
minPerActive: 10
maxPerActive: 15
minSendDelay: 100
snapshotDelay: 2000
maxNumber: 150
id: 0
hostName: localhost
port: 1234
neighbors: 
 1 4
4: client starts
1: client starts
Connections Established: 2 neighbors.
msg: {0 1 0 0 0}
msg: {3 5 1 2 1}
msg: {3 7 2 2 1}
msg: {5 5 1 2 4}
msg: {3 9 4 4 2}
msg: {8 7 2 2 6}
msg: {11 11 4 4 6}
msg: {8 7 2 2 7}
msg: {8 7 2 2 8}
...
```


### Part 2

After receive ```marker``` : ```ctl: marker 6 reveived```, the snapshot will be recorded on ```output.out``` file.

### Part 3

Sample output for conf:

```
5 6 10 100 2000 150
0 dc0 1234 
1 dc1 1235
2 dc2 1236
3 dc3 1237
4 dc4 1238
1 4 
0 2 3 
1 3 
1 2 4
0 3
```

main-0.out:
```
34 28 8 30 33
73 63 8 61 71
109 87 8 87 96
114 87 8 87 96
```

main-1.out:
```
32 33 8 27 31
70 64 8 61 68
98 94 8 86 92
98 96 8 86 92
```

main-2.out:
```
30 31 32 32 30
68 60 67 65 70
98 93 100 97 98
98 94 105 98 98
```

main-3.out:
```
30 31 8 33 30
68 56 8 66 70
98 93 8 98 98
98 95 8 104 98
```

main-4.out:
```
28 28 8 30 34
68 56 8 61 72
98 89 8 95 102
98 89 8 95 108
```

### Part 4

Finnaly, nodes will halt after MAP terminates.

