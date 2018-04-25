#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//------------------- FSUID: rde16 -----------------
//------------------- Romario Estrella -------------

#define MAX 100

int main(int argc, char *argv[])
{
  int block, sets, assoc;
  int offset, index, tag;
  int hitW, missW;
  int hitT, missT;

  hitW = hitT = missW = missT = 0;

  char type[2];
  char temp[MAX];

  char line[MAX];


  if(argc != 7){
    printf("Usage: sample_proj3 <args> < <tracefile>\n");
    printf("\t-b: block size in bytes.\n");
    printf("\t-s: number of sets.\n");
    printf("\t-n: associativity of cache.\n");
    return 1;
  }

  block = atoi(argv[2]);
  sets = atoi(argv[4]);
  assoc = atoi(argv[6]);

  if(block < 0 || sets < 0 || assoc < 0){

    printf("Block size, number of sets, and associativity must be positive integers.\n");
    return 1;

  }

  else if(strcmp(argv[1], "-b") != 0 || strcmp(argv[3], "-s") != 0 || strcmp(argv[5],"-n" ) != 0){
    printf("Block size, number of sets, and associativity must be positive integers.\n");
    return 1;

  }


  printf("Block size: %d\n", block);
  printf("Number of sets: %d\n", sets);
  printf("Associativity: %d\n", assoc);

  offset = log2(block);
  index = log2(sets);
  tag = 32 - offset - index;

  printf("Number of offset bits: %d\n", offset);
  printf("Number of index bits: %d\n", index);
  printf("Number of tag bits: %d\n\n", tag);

  int i = 0;
  //int size2 = assoc + (block/4);
  //caches created and cleared
  int size2 = assoc;
  int cache2[sets][size2];
  int cache1[sets][size2];

  int x,y;
  for(x = 0; x < sets; x++)
  {
    for(y = 0; y < size2; y++)
    {
      cache1[x][y] = -1;
    }

  }

  for(x = 0; x < sets; x++)
    {
      for(y = 0; y < size2; y++)
	{
	  cache2[x][y] = -1;
	}

    }


  //mem references
  int mem1 =0;
  int mem2 =0;

  //found num
  int found= 0;

  //have to replace 
  int replace = 0;

  int check;

  int last1 = 0,last2 = 0;

  while(fgets(line, MAX, stdin) != NULL)
  {
    sscanf(line, "%s %s", type, temp);
    check = atoi(temp);

    //-------------------------cache 1 ----------------------------
    found = 0;
    replace = 0;
    int val3 = check;
    int ind1 = check/block;
    check = (check/block) % sets;
    //printf("%d\n",check);
    //block add % #sets
    if(strcmp(type, "R") == 0){
      //printf("%d\n",ind1);
      for(x=0; x < size2; x++){
	//printf("%d\n",ind1);
	if(cache1[check][x] == ind1){
	  found = 1;
	  break;
	}

	else if(cache1[check][x] == -1 && (replace == 0)){
	  last1 = x;
	  replace = 1;
	}
      }

      if(found == 0){
	missT++;
	mem1++;

	//replacing least recent block
	if(replace == 1){

	  cache1[check][last1] = ind1;
	}

	else{
	  int tempNum1;
	  for(x = (size2 - 1); x > 0; x--){
	    tempNum1 =  cache1[check][x -1];
	    cache1[check][x] = tempNum1;
	  }
	  cache1[check][0] = ind1;

	}

      }

      else
	hitT++;

    }

    else{
      replace = 0;
      found = 0;
      for(x=0; x < size2; x++){
        //printf("%d\n",ind1);
        if(cache1[check][x] == ind1){
          found = 1;
          break;
	}
	  else if(cache1[check][x] == -1 && (replace == 0)){
	    last1 = x;
	    replace = 1;
	  }
      }

      if(found == 1)
      {
	hitT++;
	mem1++;
	int tempNum1;
	for(x = (size2 - 1); x > 0; x--){
	  tempNum1 =  cache1[check][x -1];
	  cache1[check][x] = tempNum1;
	}
	cache1[check][0] = ind1;
      }

      else{
	missT++;
	mem1++;
      }
    }

      
    //--------------------------cache 2 ----------------------------
      found = 0;
      replace = 0;

      if(strcmp(type, "R") == 0){
	//printf("%d\n",ind1);
	for(x=0; x < size2; x++){
	  //printf("%d\n",ind1);
	  if(cache2[check][x] == ind1){
	    found = 1;
	    break;
	  }

	  else if(cache2[check][x] == -1 && (replace == 0)){
	    last1 = x;
	    replace = 1;
	  }
	}

	if(found == 0){
	  missW++;
	  mem2++;

	  //replacing least recent block
	  if(replace == 1){
	    cache2[check][last1] = ind1;
	  }

	  else{
	    int tempNum1;

	    for(x = (size2 - 1); x > 0; x--){
	      tempNum1 =  cache2[check][x -1];
	      cache2[check][x] = tempNum1;
	    }
	    cache2[check][0] = ind1;

	  }

	}

	else
	  hitW++;

      }

      else{
	for(x=0; x < size2; x++){
	  //printf("%d\n",ind1);
	  if(cache2[check][x] == ind1){
	    found = 1;
	    break;
	  }
	}

	if(found == 1)
	  {
	    hitW++;
	  }

	else{
	  missW++;
	  mem2++;
	}
      }

      i++;
  }


  printf("****************************************\n");
  printf("Write-through with No Write Allocate\n");
  printf("****************************************\n");
  printf("Total number of references: %d\n", i);
  printf("Hits: %d\n", hitT);
  printf("Misses: %d\n", missT);
  printf("Memory References: %d\n\n",mem1);


  printf("****************************************\n");
  printf("Write-back with Write Allocate\n");
  printf("****************************************\n");
  printf("Total number of references: %d\n", i);
  printf("Hits: %d\n", hitW);
  printf("Misses: %d\n", missW);
  printf("Memory References: %d\n",mem2);


  return 0;
}
