# osex3basicTest
tester for HUJI OS 2020 exercise 3
## Instructions:
- substitute SampleClient.cpp in your project folder for the one in the tester folder. (If you want to use it again later then back it up by renaming it or whatever)
- put the randomstring.txt somewhere on your disk
- substitute the path to randomstring.txt in the PATH_TO_RANDOM_STRING variable which resides in line 10 of "SampleClient.cpp".
- Rebuild the project. If it still runs the old executable make sure to recompile SampleClient.cpp (right click it in clion in the project context menu, and select "Recompile SampleClient.cpp")
- Run


### Example cmakelists.txt file:

(project specific)
```
cmake_minimum_required(VERSION 3.12)
project(ex3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_STANDARD 11)
add_executable(ex3 SampleClient.cpp Barrier.cpp Barrier.h MapReduceClient.h MapReduceFramework.cpp MapReduceFramework.h)
```
### Remarks:
- Running valgrind on the test as it is will detect memory leaks, since the bigfile test exits without freeing resources. If you want to run valgrind, comment out that test and run just the first three.
