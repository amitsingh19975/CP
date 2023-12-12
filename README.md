# C++ Competitive Programming Template Generator

## Command Table
| Command | Description |
|--|--|
| -n, --filename | name of the file to create |
| -jg, --just-generate | generate the file |
| -jc, --just-compile | compile the file |
| -r, --run | run the file |
| -fr, --force-run | compile and run the file |
| -conf, --config | config file name |
| -d, --debug | debug mode |
| -in, --input | input file name |
| -out, --output | output file name |
| -cp, --compile-path | path to the compile folder |
| -f, --force | force overwrite |
| -gin, --generate-input | generate input file |
| -gout, --generate-output | generate output file |
| -t, --test | test the program |
| -st, --whitespace-sensitive-test | test the program with whitespace |
| --show-chars | show n chars when test passes or fails |

## Usage
```bash
# Generate template with name "problem.cpp" in current directory
# with input and output file "problem.in" and "problem.out" respectively.
# -d is for debug mode
cpt -n=problem -d -jc -gin -gout

# Compile and run the program
# -fr is for force run; otherwise, it will not compile with -r flag, but run stale executable
cpt -n=problem -d -fr

# Test the program
cpt -n=problem -d -t
```
