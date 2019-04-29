# Parallel-Merge-Sort

Don't forget to enable OpenMP when compiling: -fopenmp

if compiling on Mac OS X, might get "unsupported option '-fopenmp'". Then try:

```
brew install cmake gcc
cmake -DCMAKE_CXX_COMPILER=g++-6 CMakeLists.txt
make
```
