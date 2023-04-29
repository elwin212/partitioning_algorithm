#pragma once
#ifndef HGRAPH_H_INCLUDED
#define HGRAPH_H_INCLUDED

#include "dlinkedlist.h"

#define other(num) ((num)?0:1) //if num = 0 -> 1 else ->0
#define LOCKED -10000

extern int SEED; // random seed for initial partitioning
extern double RATIO; // ratio of balance between partitions

typedef LinkedList* ll;   // ptr to a linked list
struct Net;       // predefine Net

struct Cell {
        int number;       // index # of Cell in Cells array
        LinkedList first;         // ll of nets on Cell
        node* gainbucket; // pointer to Cell in the bucket lists
        int block;        // which block (partition) the Cell is in
        int gain;         // gain of Cell
        int gainfromlock; // gain from moving and locking Cell
        Cell(int num, int side);    // constructor
        Cell() {}         // default constructor
};

struct Net {
        int number;     // number of Cells on Net
        LinkedList first;       // ll of Cells on Net
        int unlock[2];  // # of unlocked Cells in each block
        int lock[2];    // # of locked Cells in each block
        Net(int num);   // constructor, assigns num to number
        Net() {}        // default constructor
};

class Parthgraph
{
    
private:
        int numCells;   // number of Cells in hgraph
        int numnets;    // number of nets in hgraph
        double ratio;   // ratio of balance between the blocks
        int minsize[2]; //
        Cell* Cells;    // array of Cells in hgraph
        Net* nets;      // array of nets in hgraph
        int pmax;       // max number of pins per Cell
        ll buckets[2];  // buckets for gain updates
        LinkedList free;        // ll of free Cells
        int maxgain[2]; // max gain of any Cell in each block
        int balance[2];
        int passes;     // number of passes

public:
        Parthgraph(void) {}  // constructor
        void getgraph(string filename); // loads hgraph filename from disk
        void part(void);               // performs partitioning
        void initgains();          // initializes gains and buckets
        void swapall(void);
        void getprefix(int& prenum);
        void reinit(int prenum);
        Cell* gethighest(void);
        void fixgain(LinkedList& first, int operation, int single = 0, int sameside = 0);
        int cutset(void);
        void print(char* str);
        // debug functions
        void printCells(); // printout Cell info
        void printBucket(); // printout top buckets
        void printNodes();
        long currTime();
};


#endif // HGRAPH_H_INCLUDED
