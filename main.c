#include<stdio.h>
#include<stdbool.h>
#include<math.h>

#define TOTAL_MEM 4096 //4MB 

void assignProcessSize();
void assignProcessSize();
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
	remMem = TOTAL_MEM;
	
	bestFit();
	//printBlocks();
	computeResult();
	return 0;
}
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
	printf("var %d\n",holeVar);
	int holeStdDev=sqrt(holeVar);
	printf("average = %d \n Std dev = %d \n Comparative val = %d\n",avgHole,holeStdDev, (avgHole+ 4*holeStdDev));
}

void getHole(int k)
{
	totalHole[k]=0;
	for(int i=0; i<blocks;i++)
	{
		totalHole[k] += (blockSize[i]-allocated[i]);
	}
	totalHole[k]+=remMem;
}
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

void printBlocks()
{
	for (int i=0;i<blocks;i++)
	{
		printf("block [%d]: %d\t allocated: %d\n",i,blockSize[i],allocated[i]);
	}
	printf("remaining memory is %d\n",remMem);
}

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
int allocateProcessToMemory()
{
	for(int i=0;i<N;i++)
	{
		pCount += 1;
		int bestBlock = -1;
		if((remMem >= p[i])&&(!onceFilled))
		{
			blockSize[blocks]=p[i];
			allocated[blocks]=p[i];
			printf("block %d\tallo %d \t %d\n",blockSize[blocks],allocated[blocks],blocks);
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
			for(int k=0; k<blocks;k++)
			{
				if((blockSize[k] >= p[i])&&(allocated[k]<=0))
				{
					if((bestBlock == -1) || (blockSize[k]<blockSize[bestBlock]))
						bestBlock=k;
					
				}
			}
			if(bestBlock != -1)
			{
				allocated[bestBlock]=p[i];
				//remMem -= p[i];				
				
				blockSize[blocks]=blockSize[bestBlock]-allocated[bestBlock];
				blockSize[bestBlock]=p[i];
				blocks++;
				
			}
			
		}
	}
	pRoundCount+=1;
	return 0;
}


