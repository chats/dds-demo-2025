# CycloneDDS Example Project

This project demonstrates a basic publish-subscribe system using Eclipse CycloneDDS.

## Prerequisites
- CMake (>= 3.10)
- CycloneDDS
- CycloneDDS IDL compiler

## Building
```bash
mkdir build && cd build
cmake ..
make
```
## Configuration (optional)
```
export CYCLONEDDS_URI="file://$HOME/path/to/configuration.xml"
```

## Running
Terminal 1: `./subscriber`
Terminal 2: `./publisher`