#include<stdio.h>
#include<stdbool.h>
#include<math.h>

#define TOTAL_MEM 4096 //4MB 

void assignProcessSize();
void computeResult();
int allocateProcessToMemory();
int N, remMem;
int allocated[],p[];
int blockSize[];
int blocks=0;
int oldBase=0;
bool onceFilled=false;
int  totalHole[];
int pCount=0;
int pRoundCount=0;

int main(int argc, char const *argv[])
{

	if(argc != 2)
	{
		printf("Incorrect number of arguments. 2 needed\n");
		return -1;
	}

	N = atoi(argv[1]);
	if(N<=0)
	{
		printf("Minimum required not met. N>=1\n");
		
		return -2;
	}

	//initialize the total memory size as the remaining memory
	remMem = TOTAL_MEM;
	//function call to the best fit solution as described in problem text
	bestFit();
	//printBlocks();
	//compute the results for analysis
	computeResult();
	return 0;
}
//this function computes the result for analysis based on the average hole size and 
void computeResult()
{
	int holeSum=0;
	int holeArrSize=20;//(int)(sizeof(totalHole)/sizeof(totalHole[0]));
	for(int i=0;i<holeArrSize;i++)
	{
		holeSum+=totalHole[i];
	}
	int avgHole=holeSum/holeArrSize;
	int holeVarNum=0;
	for(int i=0;i<holeArrSize;i++)
	{
		holeVarNum+=(abs(totalHole[i]-avgHole)^2)+abs(remMem-avgHole)^2;
		//printf("%d\n",holeVarNum);
	}
	int holeVar=holeVarNum/holeArrSize;
	//printf("var %d\n",holeVar);
	int holeStdDev=sqrt(holeVar);
	//display computed result
	printf("average = %d \n Std dev = %d \n Comparative val = %d\n",avgHole,holeStdDev, (avgHole+ 4*holeStdDev));
}

//this function is to get the total size of holes after every execution of N processes
void getHole(int k)
{
	totalHole[k]=0;
	for(int i=0; i<blocks;i++)
	{
		totalHole[k] += (blockSize[i]-allocated[i]);
	}
	totalHole[k]+=remMem;
}

//this function runs the algorithm as described in the question
//assign random size to processes
//allocate processes to memory
//get the hole count for every N process assignment round
int bestFit()
{
	while (pCount <= (2^20))
	{
		assignProcessSize();
		allocateProcessToMemory();
		getHole(pRoundCount);
	}
	return 0;
}

//this function prints the content of the block and how much of the memory is allocated to a process
void printBlocks()
{
	for (int i=0;i<blocks;i++)
	{
		printf("block [%d]: %d\t allocated: %d\n",i,blockSize[i],allocated[i]);
	}
	printf("remaining memory is %d\n",remMem);
}

//this function assigns random number between 10 and 100 to N processes
void assignProcessSize()
{
	for(int i=0;i<N;i++)
	{
		int min = 10;
		int max = 100;
	       	p[i] = min + rand() % (max + 1 - min);
		printf("%d ",p[i]);
	}
	//printf("\n");
}

//this function frees the oldest N processes
void freeBlocks()
{
	
	//printf("upperbund %d\nblocks %d \n old base %d",upperBound,blocks,oldBase);
	for (int i=0;i< N;i++)
	{
		//printf("i freed block %d\n",i);
		//remMem+=blockSize[i];
		if(oldBase+1>=blocks)
			oldBase=0;
		remMem+=allocated[oldBase];
		allocated[oldBase]=0;
		oldBase+=1;
		
	}
	//printf("my old base = %d\n",oldBase);
}

//this function allocates N processes to memory segments using best fit algorithm

int allocateProcessToMemory()
{
	for(int i=0;i<N;i++)
	{
		//pcount keeps track of the processes being fit in memory. this is used to check against 2^20
		pCount += 1;
		int bestBlock = -1;
		if((remMem >= p[i])&&(!onceFilled)) 
		//this branch is taken during the first set of allocations
		{
			//assign the size of the process as the block size and subsequently, the allocated size
			blockSize[blocks]=p[i];
			allocated[blocks]=p[i];
			//printf("block %d\tallo %d \t %d\n",blockSize[blocks],allocated[blocks],blocks);
			//reduce the remaining memory space
			remMem-=p[i];
			blocks+=1;
		}
		else
		{
			//printf("djjd %d",blocks);
			onceFilled=true;
			blockSize[blocks]=remMem;
			allocated[blocks]=0;
			remMem=0;
			//break;

			//printf("no more memory to allocate\n");
			if (bestBlock<0)
			{
				freeBlocks();
				//printf("modebe");
			}
			//iterate through all blocks to find the smallest sufficient block
			for(int k=0; k<blocks;k++)
			{
				if((blockSize[k] >= p[i])&&(allocated[k]<=0))
				{
					if((bestBlock == -1) || (blockSize[k]<blockSize[bestBlock]))
						bestBlock=k;
					
				}
			}
			//if found
			if(bestBlock != -1)
			{
				allocated[bestBlock]=p[i]; //allocate the process to the found block
				//remMem -= p[i];				
				//then add the remaining memory due to fragmentation as another block
				blockSize[blocks]=blockSize[bestBlock]-allocated[bestBlock];
				blockSize[bestBlock]=p[i];
				blocks++;
				
			}
			
		}
	}
	pRoundCount+=1;
	return 0;
}


