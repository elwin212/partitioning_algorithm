# Fiduccia-Mattheyses Partitioning Algorithm

The Fiduccia-Mattheyses partitioning algorithm is implemented in this program using a doubly linked list data structure. The program aims to partition circuits from the ISPD98 Circuit Benchmark into two subsets, producing a txt file named `partition_result` where the output will be written.

## ISPD98 Circuit Benchmark Testbenches

You can use the following links to download the testbenches for testing the program:

- [ibm01](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm01.tar.gz)
- [ibm02](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm02.tar.gz)
- [ibm03](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm03.tar.gz)
- [ibm04](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm04.tar.gz)
- [ibm05](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm05.tar.gz)
- [ibm06](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm06.tar.gz)
- [ibm07](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm07.tar.gz)
- [ibm08](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm08.tar.gz)
- [ibm09](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm09.tar.gz)
- [ibm10](https://vlsicad.ucsd.edu/UCLAWeb/cheese/circuits/ibm10.tar.gz)

## How to Use the Program

1. Download and extract the program files.
2. Compile the program to build the executable.
3. Place the testbench files under the "testbench" directory as the executable or specify the file paths as input parameters.
4. In main function, change the `fileName` accordingly.
5. Run the program, and it will perform the Fiduccia-Mattheyses partitioning algorithm on the provided testbenches.
6. The program will generate a txt file named `partition_result` where the output of the partitioning will be written.
