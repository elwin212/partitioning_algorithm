#include "dlinkedlist.h"
// check if a node is in list before remove it
//#define Check

// overload the ++ operator in LinkedList class 
void LinkedList::operator++(){    
        if (current->next->number != -1) {
            current = current->next;
         }
        else {
            current = NULL;
         }
}

// reset current node to the head
void LinkedList::reset(void){
        if (length != 0) {
            curcount = 1;
            current = head->next;
        }
        else {
            current = NULL;
         }
}


// Constructor and initialize variables
LinkedList::LinkedList(void){
        head = &headdummy;
        tail = &taildummy;
        head->next = tail;
        tail->prev = head;
        head->prev = NULL;
        tail->next = NULL;
        current = head;
        length = 0;
        curcount = 0;
}


// add a node to head of list
void LinkedList::addhead(node* nnode) {
        length++;
        nnode->next = head->next;
        nnode->prev = head;
        head->next = nnode;
        nnode->next->prev = nnode;
}

// add a node to tail of list
void LinkedList::addtail(node* nnode){
        length++;
        nnode->next = tail;
        nnode->prev = tail->prev;
        tail->prev = nnode;
        nnode->prev->next = nnode;
}


// remove the node at head
node* LinkedList::removehead(void){
    if (length)
    {
        length--;
        node* nnode;
        nnode = head->next;
        head->next = nnode->next;
        nnode->next->prev = head;
        nnode->next = NULL;
        nnode->prev = NULL;
        return nnode;
    }
    else return NULL;
}


// remove the node at tail
node* LinkedList::removetail(void){
    if (length)
    {
        length--;
        node* nnode;
        nnode = tail->prev;
        tail->prev = nnode->prev;
        nnode->prev->next = tail;
        nnode->prev = NULL;
        nnode->next = NULL;
        return nnode;
    }
    else return NULL;
}


// get the pointer to the node index from the head
node* LinkedList::get(int index){
    if ((index <= length) && length) // make sure index and length is valid
    {
        int count;
        node* temp;

        if (((int)(length / 2)) > index)  // count in from shortest side
        {
            temp = head;
            for (count = 0; count < index; count++)
                temp = temp->next;
        }
        else
        {
            temp = tail;
            for (count = length + 1; count > index; count--)
                temp = temp->prev;
        }
        return temp;
    }
    else return NULL;
}


// search nnode in the linkedlist
int LinkedList::search(node* nnode){
    if (length)
    {
        int index = 0;
        node* temp = head->next;
        while (temp->next)
        {
            index++;
            if (temp == nnode)
                return index;
            temp = temp->next;
        }
    }
    return 0;
}


// remove nnode from the linkedlist
void LinkedList::removenode(node* nnode){
#ifdef Check
    if (!search(nnode))  // node not in list
    {
        cout << "node" << nnode->number << " is not in list!!" << endl;
        exit(1);
    }
#endif
    if (nnode != NULL)
    {
        nnode->prev->next = nnode->next;
        nnode->next->prev = nnode->prev;
        length--;
    }
}
