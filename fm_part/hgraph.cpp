#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <process.h>
#include <iostream>
#include "hgraph.h"

using namespace std;

long TT = (long)time(NULL);
extern bool MODE;
extern int initcut;
//#define debug1

//#define debug
//#define bb


// error function , exit program with code 1
void error(string s, string s2 = ""){
		std::cout << s << ' ' << s2 << '\n';
		exit(1);
}

Cell::Cell(int num, int side) {
	number = num;
	block = side;
	gainbucket = NULL;
	gain = 0;
	gainfromlock = LOCKED;
}

Net::Net(int num) {
	number = num;
	unlock[0] = unlock[1] = 0;
	lock[0] = lock[1] = 0;
}


void Parthgraph::getgraph(string filename){
	int side = 0, count, padoffset, temp;
	node* nnode;
	char type, state, line[80];
	FILE* fr;
	ifstream infile;

	infile.open(filename);
	infile.ignore(256, '\n'); // ignore the first number
	infile.ignore(256, '\n'); // ignore the second number
	infile >> numnets >> numCells >> padoffset;
	
	
	srand(TT);

	balance[0] = 0;
	balance[1] = 0;


	fr = fopen(filename.c_str(), "r");
	if (!fr) {
		error("Can not open input file, maybe there is typo in file name or the file does not exist.", filename);		
	}
	
	//fscanf(fr, "%*d %*d %d %d %d", &numnets, &numCells, &padoffset); //for scanf function %*d allow us to ignore

	//cout << numCells << " " << numnets << " " << padoffset << " " << endl;
	Cells = (Cell*) new Cell[numCells];
	if (!Cells) {
		error("memory allocation error");
	}

	minsize[0] = minsize[1] = int(ceil(numCells * RATIO));
	//cout << numCells << "  "<< minsize[0] << endl;

	int size = numCells >> 1;  //shift to right for one bit
	//cout << size << endl;
	int max[2] = { size, numCells - size };  // max size of each part
	// ***read in nodes
	for (count = 0; count < numCells; count++)
	{
		Cells[count].number = count;
		side = (rand() & 256) ? 1 : 0;
		if (balance[side] == max[side]) {
			side = other(side); // check whether both side is nearly equal
		}
		Cells[count].block = side;
		Cells[count].gainbucket = NULL;
		Cells[count].gain = 0;
		Cells[count].gainfromlock = LOCKED;
		balance[side]++;
	}
	// ***read in nets
	nets = (Net*)new Net[numnets];
	if (!nets) {
		error("memory allocation error");
	}
	count = -1;
	

	while (infile.getline(line, 80)) {
		if (sscanf(line, "%c%d %c", &type, &temp, &state) != 3) {  // skip the first 5 line (# of Cells, # of nets...
			continue;
		}

		if (type == 'p') {
			temp += padoffset;
		}

		if (state == 's') {
			count++;
			nets[count].number = 0;
			nets[count].lock[0] = 0;
			nets[count].lock[1] = 0;
			nets[count].unlock[0] = 0;
			nets[count].unlock[1] = 0;
		}
		nnode = new node(count);
		if (!nnode) error("memory allocation error");
		Cells[temp].first.addhead(nnode);
		//	  Cells[temp].first.addtail(nnode);
		nnode = new node(temp);
		if (!nnode) error("memory allocation error");
		nets[count].first.addhead(nnode);
		//	  nets[count].first.addtail(nnode);
		nets[count].unlock[Cells[temp].block]++;
		nets[count].number++;
	}
	
	pmax = 0;
	for (count = 0; count < numCells; count++)
	{
		if (Cells[count].first.length > pmax)
			pmax = Cells[count].first.length;
	}
	buckets[0] = (ll) new LinkedList[2 * pmax + 1];
	buckets[1] = (ll) new LinkedList[2 * pmax + 1];
	if (!buckets[0] || !buckets[1]) error("memory allocation error");
#ifdef bb
	printNodes();
#endif
	initcut = cutset();
	if (MODE == false) {
		printNodes();
	}
	std::cout << "TestBench: " << filename << endl;
	std::cout << "Init Cutset: " << initcut << endl;
}


long Parthgraph::currTime() {
	return TT;
}

void Parthgraph::printNodes() {
	std::cout << "\tBisection1" << "\t  " << "\tBisection2" << endl;
	for (int i = 0; i < numCells; ++i) {
		if (Cells[i].block == 0) {
			std::cout << "\t" << "\t\t" << "\tCell " << i << endl;
		}
		else {
			std::cout << "\tCell " << i << "\t" << "\t " << endl;
		}
	}

}


void Parthgraph::initgains() {

		int cnt;
		int from, to;

		// ****calc gain of unlocked Cells
		for (cnt = 0; cnt < numCells; cnt++)
		{
			if (Cells[cnt].gainfromlock == LOCKED)
			{
				Cells[cnt].first.reset();
				from = Cells[cnt].block;
				to = other(from);
				while (Cells[cnt].first.current != NULL)
				 {
					int nnum = Cells[cnt].first.current->number;
					if ((nets[nnum].unlock[from] + nets[nnum].lock[from]) == 1)
						Cells[cnt].gain++;
					if ((nets[nnum].unlock[to] + nets[nnum].lock[to]) == 0)
						Cells[cnt].gain--;
					++Cells[cnt].first;
				 }
			}
		}

		maxgain[0] = -pmax;
		maxgain[1] = -pmax;

		node* nnode;
		for (cnt = 0; cnt < numCells; cnt++)
		{
			if (Cells[cnt].gainbucket == NULL)
			{
				nnode = (node*)new node(cnt);
				if (!nnode) error("memory allocation error");
				Cells[cnt].gainbucket = nnode;
			}
			else {
				nnode = Cells[cnt].gainbucket;
			 }
				buckets[Cells[cnt].block][Cells[cnt].gain + pmax].addhead(nnode);

			if (Cells[cnt].gain > maxgain[Cells[cnt].block]) {
				maxgain[Cells[cnt].block] = Cells[cnt].gain;
			}
										
		}
		//   if(MODE == false){
		//        for(int i=0;i<numCells;++i){
		//        cout << "Cell:" <<Cells[i].number << "  Gain:" << Cells[i].gain << "  ";
		//       }
		//       cout << endl;
		//   }

	#ifdef debug1
		printCells();
		//exit(0);
	#endif
}


void Parthgraph::part(void)
{
	int prefix = 0; // holds prefix sum of gains
	int done = 0;

	passes = 0;   // num of passes

	while (!done)
	{

		passes++;
		swapall();

		getprefix(prefix);

		//   if(prefix % 2)
		//     prefix--;

		if (prefix <= 0)
			done = 1;
		reinit(prefix);
	}
}

void Parthgraph::swapall(void)
{
	int from, to;
	Cell* bestCell;
	Net* curnet;
	int done = 0;

	bestCell = gethighest();

	while (!done)
	{
#ifdef debug
		printBucket();
		for (int i = 0; i < numCells; ++i) {
			cout << Cells[i].gain << " ";
		}
		cout << endl;
		printf("best Cell=%d(%d) gain=%d\n", bestCell->number, bestCell->block,
			bestCell->gain);
		char go;
		scanf(" %c", &go);
#endif
		//      printf("Moving Cell No.%d(%d) gain=%d\n", bestCell->number, bestCell->block,bestCell->gain);
		//	  char go;
		//	  scanf(" %c", &go);
		from = bestCell->block;
		to = other(from);
		bestCell->block = to;
		bestCell->gainfromlock = bestCell->gain;
		//bestCell->gainbucket=NULL;
		bestCell->first.reset(); //bestCell point to head of list
		balance[from]--;
		balance[to]++;
		while (bestCell->first.current != NULL)
		{
			curnet = &nets[bestCell->first.current->number];

			if (curnet->lock[to] == 0)
			{
				if (curnet->unlock[to] == 0)
					fixgain(curnet->first, 1);
				else if (curnet->unlock[to] == 1)
					fixgain(curnet->first, -1, 1, to);
			}

			curnet->unlock[from]--;
			curnet->lock[to]++;

			if (curnet->lock[from] == 0)
			{
				if (curnet->unlock[from] == 0)
					fixgain(curnet->first, -1);
				else if (curnet->unlock[from] == 1)
					fixgain(curnet->first, 1, 1, from);
			}
			++bestCell->first;
			//		 for(int i=0;i<numCells;++i){
			//            cout << "Cell:" <<Cells[i].number << "  Gain:" << Cells[i].gain << "  ";
			//           }
			//           cout << endl;
		}
		bestCell = gethighest();
		if (bestCell == NULL)
			done = 1;
	}

}

void Parthgraph::fixgain(LinkedList& first, int operation, int single, int sameside)
{
	Cell* Cellptr;
	int block;

	first.reset();
	if (operation == 1)
	{
		while (first.current != NULL)
		{
			Cellptr = &Cells[first.current->number];
			if (Cellptr->gainfromlock == LOCKED)
			{
				block = Cellptr->block;
				if (!single || (block == sameside))
				{
					if (Cellptr->gain == maxgain[block])
						++maxgain[block];
					buckets[block][Cellptr->gain + pmax].removenode(Cellptr->gainbucket);
					Cellptr->gain++;
					buckets[block][Cellptr->gain + pmax].addhead(Cellptr->gainbucket);
				}
			}
			++first;
		}
	}
	else
	{
		while (first.current != NULL)
		{
			Cellptr = &Cells[first.current->number];
			if (Cellptr->gainfromlock == LOCKED)
			{
				block = Cellptr->block;
				if (!single || (block == sameside))
				{
					if ((Cellptr->gain == maxgain[block]) && (buckets[block][Cellptr->gain + pmax].length == 1))
						maxgain[block]--;
					buckets[block][Cellptr->gain + pmax].removenode(Cellptr->gainbucket);
					Cellptr->gain--;
					buckets[block][Cellptr->gain + pmax].addhead(Cellptr->gainbucket);
				}
			}
			++first;
		}
	}
}

Cell* Parthgraph::gethighest(void)
{
	Cell* tmpCell = NULL;
	node* tmpnode = NULL;
	//static int swapside=0;  //*******force toggle swap
	//cout << maxgain[0] << "  " << maxgain[1] << endl;
	if ((maxgain[0] >= maxgain[1]))
	{
		if (balance[0] >= minsize[0])
			//  if(!swapside)  //******force toggle swap
		{
			tmpnode = buckets[0][maxgain[0] + pmax].removehead();
			if (tmpnode != NULL)
			{
				tmpCell = &Cells[tmpnode->number];
				free.addtail(tmpnode);
			}
			while ((!buckets[0][maxgain[0] + pmax].length) && (maxgain[0] > -pmax))
				maxgain[0]--;
		}
	}
	if (tmpCell == NULL)
	{
		if (balance[1] >= minsize[1])
			//  if(swapside)  //*******force toggle swap
		{
			tmpnode = buckets[1][maxgain[1] + pmax].removehead();
			if (tmpnode != NULL)
			{
				tmpCell = &Cells[tmpnode->number];
				free.addtail(tmpnode);
			}
			while ((!buckets[1][maxgain[1] + pmax].length) && (maxgain[1] > -pmax))
				maxgain[1]--;
		}
		else
		{
			tmpnode = buckets[0][maxgain[0] + pmax].removehead();
			if (tmpnode != NULL)
			{
				tmpCell = &Cells[tmpnode->number];
				free.addtail(tmpnode);
			}
			while ((!buckets[0][maxgain[0] + pmax].length) && (maxgain[0] > -pmax))
				maxgain[0]--;
		}
	}
	//swapside=other(swapside);  //******force toggle swap
	return tmpCell;
}

void Parthgraph::getprefix(int& prenum)
{
	int max = -pmax, current = 0;
	int curnum = 0;

	free.reset();
	while (free.current != NULL)
	{
		current += Cells[free.current->number].gainfromlock;
		curnum++;
		if (max < current)
		{
			max = current;
			prenum = curnum;
		}
		++free;
	}
	if (max <= 0)
		prenum = 0;
}

void Parthgraph::reinit(int prenum)
{
	int cnt = 0;
	Cell* Cellptr;

	if (free.length != numCells)
	{
		for (cnt = 0; cnt < numCells; cnt++)
		{
			if (Cells[cnt].gainfromlock == LOCKED)
			{
				buckets[Cells[cnt].block][Cells[cnt].gain + pmax].removenode(Cells[cnt].gainbucket);
				Cells[cnt].gain = 0;  // reset to 0 --WD
			}
		}
	}
	free.reset();
	while (free.current != NULL)
	{
		cnt++;
		Cellptr = &Cells[free.current->number];
		if (cnt > prenum)
		{
			Cellptr->block = other(Cellptr->block);
			balance[Cellptr->block]++;
			balance[other(Cellptr->block)]--;
		}
		Cellptr->gainfromlock = LOCKED;
		Cellptr->gain = 0;
		free.removenode(Cellptr->gainbucket);
		++free;
	}

	for (cnt = 0; cnt < numnets; cnt++)
	{
		nets[cnt].lock[0] = 0;
		nets[cnt].lock[1] = 0;
		nets[cnt].unlock[0] = 0;
		nets[cnt].unlock[1] = 0;
		// ***build Cell list for each Net and initialize Net
		nets[cnt].first.reset();
		while (nets[cnt].first.current != NULL)
		{
			nets[cnt].unlock[Cells[nets[cnt].first.current->number].block]++;
			++nets[cnt].first;
		}
	}

	initgains();
}

int Parthgraph::cutset(void)   //define what does cutset mean, if two nodes connect to each other and in different bisection, cutset count
{
	int cutset = 0;
	int count;
	for (count = 0; count < numnets; count++)
	{
		//  if( ( (nets[count].unlock[0]>0) || (nets[count].lock[0]>0) ) &&
		//      ( (nets[count].unlock[1]>0) || (nets[count].lock[1]>0) ) )
		nets[count].first.reset();
		int sidezero = 0;
		int sideone = 0;
		while (nets[count].first.current != NULL)
		{
			if (Cells[nets[count].first.current->number].block == 0)
				sidezero = 1;
			else
				sideone = 1;
			++nets[count].first;
		}
		if ((sideone == 1) && (sidezero == 1))
			cutset++;
	}
	return cutset;
}

void Parthgraph::print(char* str)
{
	sprintf(str, "LeftSide:%i, RightSide:%i,Passes: %i", balance[0], balance[1], passes);
}

void Parthgraph::printCells()
{
	std::cout << "Node\tPart\tGain" << endl;
	for (int i = 0; i < numCells; i++)
	{
		std::cout << Cells[i].number << "\t" << Cells[i].block << "\t" << Cells[i].gain << endl;
	}
}

void Parthgraph::printBucket()
{
	for (int part = 0; part < 2; part++)
	{
		printf("****** Top Bucket in part %d *****\n", part);
		buckets[part][maxgain[part] + pmax].reset();
		while (buckets[part][maxgain[part] + pmax].current != NULL)
		 {
			std::cout << buckets[part][maxgain[part] + pmax].current->number << "\t";
			++buckets[part][maxgain[part] + pmax];
		 }
		std::cout << endl;
	}
}

