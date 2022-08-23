# Database-Simplfied
A sub project taken from my old website. It's a *database* storing data related to the paths in a grid. 

## Contents

* [Goal](#high-level-goal)
* [Results](#result)
    * [Functionality](#functionality)
    * [Efficiency](#efficiency)
    * [Flaws](#flaws)
    * [Goal Reflection](#goal-reflection)

## High Level Goal
I wanted to add a project which would have an interactive a grid with statistical data. In planning the project, I noticed the values of the required numbers would exceed any integral types. Arbitrary precision numbers were needed. I thought of going with python, and using a library or framework, but where's the fun in that. I ended up with making the project seen here.

## Result
### Functionality
It is cross platform as needed. I can run it on all my devices at home (x86_64) and on a hosting sever (x86_32) with no modifications. The database as a whole works and provided the services necessary in a timely manner.

### Efficiency
Efficiency I am happy with.
* Is single threaded, causing the database to be the bottle-neck in the data path. Requests are about 4-30ms (SSD & HDD) to be handled when using the API.
* Files are kept on disk instead of memory mapped. They are accessed through disk operations.
* Algorithmic complexity is `(2 disk access + data read time)` which turns into `O(log(n))`. Since disk access in this case is logarithmic at most and modern disks will fetch small enough blocks, which the data is, in constant time.

### Flaws
* Complete disregard for every database concept in existence.
* Can store only incredibly specific data.
* Read only.

### Goal Reflection
Achieved quite well. Produced what was needed. Could have been done much better, but everyone starts somewhere.

## Note
This uploaded code is **not** self contained.  
Javascript code is dependant on missing code, since this project was taken from a larger project (my wesbite).  
C++ code must be built with node-gyp, within the larger project to be able to run.
