/*
MIT License

Copyright (c) 2020 ABHISHEK DHITAL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define MAX_PAGE_FRAME 10



//function to return the index containing the largest or smallest element of an array
//if the value of argument n==0, it returns the largest
//if the value of argument n!=0, it returns the smallest
int largestOrsmallest(int a[], int s, int n) 
{

 	if (n==0)
 	{
	 int i;
	 int largest=0;
	 for(i=1; i<s; i++)
	  {
	     if(a[i]> a[largest])
	     	largest=i;
	  }

	 return largest;
 	}

 	else 
  {
     int i;
 	 int smallest=0;
 	 for(i=1;i<s;i++)
 	 {
 		if(a[i]<a[smallest])
 			smallest=i;
 	 }
 	 return smallest;
  }
 
}


//function used to shift the page frame by one place 
//used for the implementation of FIFO algorithm, and LRU
void update_pageframe(char* frame, char new, int size)
  {
       int z;
       for (z=1;z<size;z++)
       		frame[z-1]=frame[z];
       	frame[z-1]=new;
  }


//function used to order the page frame such that the most recent hit goes to the end
//and becomes the most recent used
//and the least recently used moves to the other end as the least recently used
//used in implementing the lRU and MFU algorithms

void update_hit(char* frame, char new, int size)
{
	int z;
	z=strchr(frame,new)-(frame);
	update_pageframe(&frame[z],new,size-z);

}



//implements the FIFO page replacement algorithm to the page_reference_string
//using the page frame pointed to by page_frame with size of working_set_size
int FIFO(char* page_reference_string, char* page_frame, int working_set_size)
{
	int pf=working_set_size;	//counter to count the number of page faults using this algorithm
	char temp_prs[strlen(page_reference_string)+1];
	char temp_pageframe[strlen(page_frame)+1];
	strcpy(temp_prs,page_reference_string);
	strcpy(temp_pageframe,page_frame);


	
	int index=working_set_size;  //index to point at each element in the page reference string
								// starting at the first element after the working page frame
	while(index<strlen(page_reference_string))
 {	
 	int count=0;	
 	int i;
	for(i=0;i<working_set_size;i++)
	{ //debug: in case of the confusion add here.......
	 if(temp_pageframe[i]!=temp_prs[index])
	 	count++;
	}

	if(count==working_set_size)
	{
	 update_pageframe(temp_pageframe,temp_prs[index],working_set_size);
	 pf++;
	}
	index++;
 }

	return pf;
}





//implements the LRU page replacement algorithm to the page_reference_string
//using the page frame pointed to by page_frame with size of working_set_size
int LRU(char* page_reference_string, char* page_frame, int working_set_size)
{
	int pf=working_set_size;	//counter to count the number of page faults using this algorithm
	char temp_prs[strlen(page_reference_string)+1];
	char temp_pageframe[strlen(page_frame)+1];

	strcpy(temp_prs,page_reference_string);
	strcpy(temp_pageframe,page_frame);

	
	int index=working_set_size;  //index to point at each element in the page reference string
								// starting at the first element after the working page frame


	while(index<strlen(page_reference_string))
 {	
 	int count=0;	
 	int i;
	for(i=0;i<working_set_size;i++)
	{ 
	 
	 if(temp_pageframe[i]!=temp_prs[index])
	    	count++;

	 else
	 {
	 	//printf("current page frame: %s\n",temp_pageframe);
		update_hit(temp_pageframe,temp_prs[index],working_set_size);
		//printf("wen page frame: %s\n",temp_pageframe);
		break;
	 }

	}

	if(count==working_set_size)
	{
	  update_pageframe(temp_pageframe,temp_prs[index],working_set_size);
	  pf++;	
	}	
	index++;

 }
 return pf;

} 


//implements the MFU page replacement algorithm to the page_reference_string
//using the page frame pointed to by page_frame with size of working_set_size
int MFU(char* page_reference_string, char* page_frame, int working_set_size)
{
	int pf=working_set_size;	//counter to count the number of page faults using this algorithm
	char temp_prs[strlen(page_reference_string)+1];
	char temp_pageframe[strlen(page_frame)+1];
	//temp_pageframe[strlen(page_frame)]='\0';
	strcpy(temp_prs,page_reference_string);
	strcpy(temp_pageframe,page_frame);

	
	int index=working_set_size;  //index to point at each element in the page reference string
								// starting at the first element after the working page frame

	int counter_array[10]={0}; //to keep a count of the particular page 
							  //at the index equal to the int value of the same page
					       	 //for example: the count of page '2' is kept at index 2 of  this array

	int k;
	for(k=0;k<working_set_size;k++)
	{
		char a=(char)temp_pageframe[k];
		counter_array[atoi(&a)]++;
	}


	while(index<strlen(page_reference_string))
 {	
 	int count=0;	
 	int i;
	for(i=0;i<working_set_size;i++)
	{ 
	 if(temp_pageframe[i]!=temp_prs[index])
	 	count++;

	 else
	 {
	 	char letter=(char) temp_prs[index];
	 	counter_array[letter-48]++;
	 	update_hit(temp_pageframe,temp_prs[index],working_set_size);
	 	break;
	 }

	}

	if(count==working_set_size)
	{
		int temp;
		int indexofLargest;
		char letter=(char) temp_prs[index];
		
		temp=largestOrsmallest(counter_array, sizeof(counter_array)/sizeof(counter_array[0]),0);
		counter_array[temp]=0;

		char strtemp;
		strtemp=temp+'0';
		indexofLargest=strchr(&temp_pageframe[0],strtemp)-(&temp_pageframe[0]);

		
		temp_pageframe[indexofLargest]=temp_prs[index];

		counter_array[letter-48]++;
		pf++;
	}
	 index++;		
 }
	return pf;
} 


//implements the Optimal page replacement algorithm to the page_reference_string
//using the page frame pointed to by page_frame with size of working_set_size
int Optimal(char* page_reference_string, char* page_frame, int working_set_size)
{
	int pf=working_set_size;	//counter to count the number of page faults using this algorithm
	char temp_prs[strlen(page_reference_string)+1];
	char temp_pageframe[strlen(page_frame)+1];
	//temp_pageframe[strlen(page_frame)]='\0';
	strcpy(temp_prs,page_reference_string);
	strcpy(temp_pageframe,page_frame);

	
	int index=working_set_size;  //index to point at each element in the page reference string
								// starting at the first element after the working page frame
	while(index<strlen(page_reference_string))
 {	
   // printf("working on:%c \n",temp_prs[index]);
 	int count=0;	
 	int i;
	for(i=0;i<working_set_size;i++)
	{ //debug: in case of the confusion add here.......
	 if(temp_pageframe[i]!=temp_prs[index])
	 	count++;
	}
	int test=0;
	if(count==working_set_size)
	{
	 int ref[working_set_size];
	 int k;
	 for(k=0;k<working_set_size;k++)
	 {	
		int z;
		if(strchr(&temp_prs[index],temp_pageframe[k])!=NULL)
		{	
	    	z=strchr(&temp_prs[index],temp_pageframe[k])-&(temp_prs[index]);
			ref[k]=z;

		}	
		else
		{
			test=1;
			temp_pageframe[k]=temp_prs[index];
		   // printf(".current page frame: %s\n",&temp_pageframe[0]);

			pf++;
			break;
		}
			
	 }

	 if(!test)
	 {
	   int largest_index;
	   largest_index=largestOrsmallest(ref, sizeof(ref)/sizeof(ref[0]),0);

	   *(temp_pageframe+largest_index)=temp_prs[index];

	   pf++;
	 }
	
	}
	index++;
 
 }

 return pf;
}




//function to print the results after calculating 
//the number of page faults for each of the 4 algorithms
//THis function is called once for each line in the data file
void print_results(int * a)
{
	char algo[4][8]={"FIFO","LRU","MFU","Optimal"};
	int i=0;
	for(i=0;i<4;i++)
		printf("Page faults of %s:	%d\n",algo[i],a[i]);
}




int main( int argc, char * argv[] ) 
{
  char * line = NULL;
  size_t line_length = MAX_LINE;
  char * filename;

  FILE * file;

  if( argc < 2 )
  {
    printf("Error: You must provide a datafile as an argument.\n");
    printf("Example: ./fp datafile.txt\n");
    exit( EXIT_FAILURE );
  }


  filename = ( char * ) malloc( strlen( argv[1] ) + 1 );
  line     = ( char * ) malloc( MAX_LINE);

  memset( filename, 0, strlen( argv[1] + 1 ) );
  strncpy( filename, argv[1], strlen( argv[1] ) );

  printf("Opening file %s\n", filename );
  file = fopen( filename , "r");


  if ( file ) 
  {
  	int line_count=1;
    while ( fgets( line, line_length, file ) )
    {
      char * token;
      token = strtok( line, " ");
      int working_set_size = atoi( token );
      
      if(working_set_size>MAX_PAGE_FRAME)
      {
      	printf("working set size is greater than the maximum page frames allowed.\n");
      	exit(EXIT_FAILURE);
      }

      printf("\nWorking set size: %d\n", working_set_size );
 	  char* page_frame= (char*) malloc(working_set_size+1);  //character array to store the 
 	  														//page frame
 	  page_frame[working_set_size]='\0';
 	  int c=0;
      
      char* page_reference_string=(char*) malloc(MAX_LINE+1); //to store the page reference string
      														  //from each line of the data file
      page_reference_string[MAX_LINE]='\0';
      int d=0;

      while( token != NULL )
      {         
        token = strtok( NULL, " " );
        
        	if(token!=NULL)   //get the first working set page frame
        	{
        		if(c<working_set_size)
        		{	
        			*(page_frame+c)=*token;
        			c++;
        		}

        	
        		*(page_reference_string+d)=*token; 	//construct the page reference string
        		d++;
        		//printf("Request: %d\n", atoi( token ) ); 

        	} 
       
      }

      *(page_reference_string+d)='\0';
      int page_fault[4];

      page_fault[0]=FIFO(page_reference_string,page_frame,working_set_size);
      page_fault[1]=LRU(page_reference_string,page_frame,working_set_size);
      page_fault[2]=MFU(page_reference_string,page_frame,working_set_size);
      page_fault[3]=Optimal(page_reference_string,page_frame,working_set_size);

      printf("*****************************Page faults of line #%d***************************** \n"
      	    ,line_count);
      print_results(page_fault);

      //printf("page reference string: %s\n",page_reference_string);
      printf("\n");

      line_count++;
      free(page_frame);
      free(page_reference_string);
    }

    free( line );
    fclose(file);
  }

  return 0;
}
