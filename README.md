# Project OMG

A C++ (Visual Studio) project for a Software Construction and Design course. It was used to practise Git branching and merging.

## Contents

- `time_complexities_Algorithms.cpp`: implementations of common algorithms, with timing of each:
  - Sorting: insertion, bubble, merge, heap, quick, counting, radix, and bucket sort
  - Helpers: `getMax` and `getMin`
- `functions.h`: a small placeholder class used for branch and merge exercises
- `file2.txt`: a file added on a feature branch to practise merging
- `SCD_Project.sln` / `SCD_Project.vcxproj`: Visual Studio solution and project

## Build

Open `SCD_Project.sln` in Visual Studio and build the project, or compile the algorithms file directly:

```bash
g++ time_complexities_Algorithms.cpp -o algorithms
```
