A CDCL SAT-Solver created for a seminar at TU Braunschweig
by Benjamin Hennies

### Build

To build with cmake, use

```
 cmake -DCMAKE_BUILD_TYPE=Release -S . -B build_dir
 cmake --build build_dir/
```
#### Additional Build Flags
Add `-DVERBOSE_LOGGING=ON` and/or `-DCOLLECT_STATS=ON` to the first build command to build the solver with
logging of the search process (only feasible for small instances) or collecting and printing statistics 
about the search process in csv-format to stdout.

### Usage
```
build_dir/SAT_Solver path_to_dimacs_file
```
The solver will print
```
SATISFIABLE/UNSATISFIABLE
(optional statistic-keys)
(optional statistic-values)
```
In case of satisfiable instance, the solver will also generate a file path_to_dimacs_file.sol, containing a satisfiying
assignment.





