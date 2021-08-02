# Project 2

Group Member:

Minchao Zhu mxz190002 100%



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

### Running

```bash
> ./launcher.sh {id} {conf_path}
```

The program will execute a critical function then print its result to out0.txt-outn.txt.

The result is the logical clock time interval of its critical execution.

### Test

```bash
> ./launcher.sh -1 {conf_path}
```

When the first argument is -1, the program will enter test mode. It will do

1. Count lines of those output file. The amount of each file should be equal to ```requestSum``` defined in configuration.
2. Merge all intervals and check if there exist two intervals overlap. If no intervals overlap, test passes.



## Sample Output

```
out0.txt

257 260
309 310
352 353
388 389
421 422
457 458
491 492
526 527
558 559
593 594
626 627
667 668
699 700
735 736
768 769
802 803
841 842
873 874
908 909
942 943
```

```
out1.txt

10 11
26 27
42 43
58 59
74 75
90 91
106 107
122 123
138 139
154 155
170 171
186 187
202 203
233 234
301 302
344 345
379 380
412 413
449 450
483 484
```

```
out2.txt

18 19
34 35
50 51
66 67
82 83
98 99
114 115
130 131
146 147
162 163
178 179
194 195
210 211
269 272
328 329
360 361
396 397
429 430
467 468
500 501
```

```
out3.txt

220 223
289 290
336 337
371 372
404 405
439 440
475 476
508 509
542 543
575 576
610 611
646 647
683 684
715 716
751 752
784 785
823 824
857 858
892 893
```


## Results of Tests


1. conf1

```
4 20 10 1000
0 localhost 1234
1 localhost 1233
2 localhost 5233
3 localhost 1232
```

All test passed.


2. conf2
```
9 20 10 2000
0 localhost 1234
1 localhost 1235
2 localhost 1236
3 localhost 1237
4 localhost 1238
5 localhost 1239
6 localhost 1240
7 localhost 1241
8 localhost 1242
```

All test passed.


3. conf3

```
9 20 10 1000
0 dc02 1234
1 dc03 1233
2 dc06 5233
3 dc05 1232
4 dc10 1233
5 dc09 3455
6 dc15 6789
7 dc18 3467
8 dc23 2546
```

All test passed.