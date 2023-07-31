#include "hgraph.h"
#include <string>
#include <windows.h>
#include <iostream>



using namespace std;

//class type global declaration

// global parameters
int SEED = 3;
double RATIO = 0.5;
bool MODE;
int initcut;



int main()
{
    //set for time counter
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime;
    QueryPerformanceFrequency(&nFreq);
    int opt;
    cout << "1 for speedtest mode, 2 for visualization mode:  " << endl;
    cin >> opt;
     if (opt == 1) {
        MODE = true;
     }
    else if (opt == 2) {
        MODE = false;
     }
    else {
        cout << "Invalid input, program exit." << endl;
        exit(0);
     }
    
    string  folderName = "testbench/";

    /*      testbench file name here       */ 
    string  fileName = "ibm03.net";
    
    string filePath = folderName + fileName;
    char name[] = "partition_result.txt";
    double time;
    //init the graph parameters
    Parthgraph hg;
    QueryPerformanceCounter(&nBeginTime);  // start timer
    hg.getgraph(filePath);
    hg.initgains();
    hg.part();
    QueryPerformanceCounter(&nEndTime);    // stop timer
    time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;

    //output file setting
    char* fname;
    fname = name;

    ofstream myresult(fname, ios::app);
    if (!myresult)
    {
        cout << "error opening file ";
        cin >> name;
        exit(0);
    }
    else {
        char string[100];
        hg.print(string);
        myresult << "Benchmark: " << fileName << "  ";
        myresult << "Seed: " << hg.currTime() << "(System Time in seconds)" << "  ";
        myresult << "Init Cutset: " << initcut << "  ";
        myresult << "Final Cutset: " << hg.cutset() << "  ";
        myresult << "Improvement percentage: " << ((double)initcut - (double)hg.cutset()) / (double)initcut * 100 << "%   ";
        myresult << "Time: " << time * 1000 << "ms" << "  ";
        myresult << string;
        myresult << '\n';
    }

    cout << "Final Cutsets: " << hg.cutset();

    return 0;
}

