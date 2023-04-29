#pragma once
#ifndef DLINKEDLIST_H_INCLUDED
#define DLINKEDLIST_H_INCLUDED

#include <fstream>

using namespace std;


class node {
public:
        int number;
        node* next, * prev;
        node(int num) { number = num; }
        node() { number = -1; }
};

class LinkedList {
protected:
        node headdummy, taildummy;  // dummy nodes for the ends of the ll        
        node* head;                // ptr to head of ll
        node* tail;                // ptr to tail of ll
        unsigned int curcount;     // keeps track of where current is pointing

public:
        node* current;             // ptr to current point of ll
        LinkedList(void);                  // constructor
        void addhead(node* nnode); // adds nnode to head of ll
        void addtail(node* nnode); // adds nnode to tail of ll
        node* removehead(void);    // removes head of ll, returns ptr to it
        node* removetail(void);    // removes tail of ll, returns ptr to it
        void removenode(node* nnode); // removes nnode from the ll
        node* get(int index);      // returns ptr to index number from head
        void reset(void);          // resets current to head
        void operator++();         // increments current
        int search(node* nnode);   // search if nnode is in list, return index
        unsigned int length;        //length of the linkedlist
};


#endif // DLINKEDLIST_H_INCLUDED
