# Yet Another XML Parser

## Build

### Simple

In the project root:

``` bash
mkdir build
cd build
cmake ..
make
```

### Build with doxygen documentation

In the build directory:

``` bash
cmake .. -DENABLE_DOXYGEN=TRUE
make
make doxygen-docs
```

The documentation can be found in the ```html``` directory. 
To view the documentation just open ```html/index.html``` with any browser.

## Run

### Run tests

``` bash
cd build
ctest
```

### Use the eval tool

``` bash
cd build/src
./eval ../../data/data.xml ../../data/operations.xml > results.xml
```

## Things that can be improved

* The tests are very verbose and could benefit from implementing a comparison
    operator for the XML elements and the XML document,
* The tests currently read from the file system which is an unnecessary
  dependency on hardware. Better: read from a hardcoded string or similar,
* Tests for the evaluation function are missing.
