/* Linear search program to facilitate tedious SPEC search process *
 * Created by Andy Werchniak on 07/08/15. *
 * Copyright (c) 2015 Andy Werchniak. All rights reserved. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SECTNAME 30
#define SECTNUM 400
#define MAXLEN 1000

/* section structure*/
//	shortName for comparison, name for printing
//	the only time there would be a difference is if the section is of order ## ## ##.##.## or greater
typedef struct{
	char name[SECTNAME];
	char shortName[SECTNAME];
	int count;
} section_t;

void automaticSeparate();
void manualSeparate();
void checkSection(char*, section_t*,FILE*,int);
void separateTable(char*, char*, char*);
void findSections(section_t*,FILE*);
int testNumber(FILE*, char*);
int linearSearch(const section_t*, const char*);
void separateSections(section_t*, char*, int*);
void findProblems(FILE*, int*, int*, int*);
char* upperCase(char*);
void clearFiles(section_t *, int*, char*, char*);

//opening menu
int main(void)
{
	char choice;
	char dig;
	
	printf("\n\nWelcome to the SPEC checker program!\n\n");
	printf("What would you like to do?\n");
	printf("(1) Manually enter section names to check specific sections\n");
	printf("(2) Input an entire specbook and let specCheck do the work!\n :  ");
	
	/* Flags for the loops to scan UI */
	int flag = 1;
	int flag2 = 1;
	
	while(flag)
	{
		scanf("%c", &choice);
		if(choice=='1' || choice=='2')
			flag = 0;
		else{
			if(choice==10)
				printf("Please enter a valid number.\n :  ");
			while(flag2)
			{
				scanf("%c", &dig);
				if(dig==10)
				{
					flag2 = 0;
					printf("Please enter a valid number.\n :  ");
	}	}	}	}
	
	if(choice=='2')
		automaticSeparate();
	else if(choice=='1')
		manualSeparate();
	else printf("Error.\n");	//bug check
	
	return(0);
}

//separates the table & sections from one file
void automaticSeparate(void)
{
	//declare & initialize variables, pointers, and arrays
	int i, stop;
	char choice;
	FILE *table, *outfile;
	
	int skipArray[SECTNUM];
	
	char fileName[SECTNAME];
	char tableName[SECTNAME];
	char bodyName[SECTNAME];
	char sectionName[SECTNAME];
	char specName[SECTNAME];
	char otherName[SECTNAME];
	
	section_t tableArray[SECTNUM];
	section_t sectionArray[SECTNUM];
	section_t discrepancies[SECTNUM];
	
	//get file to save to and make sure it doesn't exist
	stop = 0;
	while (!stop){
		printf("\nEnter a name for the spec book you are checking (one word only): ");
		scanf("%s", specName);
		strcpy(otherName,specName);
		strcat(specName, " Spec Check.txt");
		
		if(fopen(specName, "r") != NULL){
			printf("Warning: File '%s' already exists. Would you like to overwrite it?\n", specName);
			printf("Enter y or n: ");
			
			do{
				scanf("%c", &choice);
			} while (choice!='y' && choice!='n');
            
			if(choice=='y')
				stop = 1;
            
		} else stop = 1;
	}
	
	//get the file for the spec book
	printf("\nEnsure that your spec book is saved in the same directory as this program, and that its name is only one word");
    do{
	    printf("\nEnter the name of the file where your spec book is stored: ");
	    scanf("%s", fileName);
	    if(fopen(fileName, "r")==NULL)
	    	printf("Error! File does not exist.\n");
	}while(fopen(fileName, "r")==NULL);
    
    //create filenames for table and body
    strcpy(tableName, otherName);
    strcat(tableName, "table.txt");
    
    strcpy(bodyName, otherName);
    strcat(bodyName, "body.txt");
    
    //create individual files for the table and for the body
    separateTable(fileName, tableName, bodyName);
    
    //fill an array with all sections from the table of contents
	table = fopen(tableName, "r");
	findSections(tableArray,table);
    fclose(table);
	
	printf("\nSpec Check running\n");
	printf("Please wait...\n");
	
	//separate it into one file per section
	separateSections(tableArray, bodyName, skipArray);
	
	//open your outfile and start writing the issues
	outfile = fopen(specName, "w");
	fprintf(outfile, "%s SPEC Check:\n\n", otherName);
    
	//check each section in the tableArray, stop when you've reached the last section
	for(i=1;tableArray[i].count!=0;i++)
	{
		strcpy(sectionName,tableArray[i].shortName);
		checkSection(sectionName,tableArray,outfile,1);
	}
    
    fclose(outfile);
    
	printf("Your Spec Check is saved under the file %s\n\n", specName);
	
	//delete all of the individual section files to reduce clutter
	clearFiles(tableArray, skipArray, tableName, bodyName);
}

//user manually inputs individual files
void manualSeparate(void)
{
	//declare & initialize variables, pointers, and arrays
	char choice;
	int stop;
	FILE *table,*outfile;
	
	int skipArray[SECTNUM];
	
	char tableName[SECTNAME];
	char sectionName[SECTNAME];
	char specName[SECTNAME];
	char fileName[SECTNAME];
	
	section_t tableArray[SECTNUM];
	section_t sectionArray[SECTNUM];
	section_t discrepancies[SECTNUM];
	
	//get file to save to and make sure it doesn't exist
	stop=0;
	while (!stop){
		printf("\nEnter a name for the spec book you are checking (one word only): ");
		scanf("%s", specName);
		strcpy(fileName,specName);
		strcat(specName, " Spec Check.txt");
		
		if(fopen(specName, "r") != NULL){
			printf("Warning: File '%s' already exists. Would you like to overwrite it?\n", specName);
			printf("Enter y or n: ");
			
			do{
				scanf("%c", &choice);
			} while (choice!='y' && choice!='n');
            
			if(choice=='y')
				stop = 1;
            
		} else stop = 1;
	}
	
	//get the file for the spec book
	printf("\nEnsure that your index is saved in the same directory as this program, and that its name is only one word");
    do{
	    printf("\nEnter the name of the file where your index is stored: ");
	    scanf("%s", tableName);
	    if(fopen(tableName, "r")==NULL)
	    	printf("Error! File does not exist.\n");
	}while(fopen(tableName, "r")==NULL);
    
    //fill an array with all sections from the table of contents
	table = fopen(tableName, "r");
	findSections(tableArray,table);
    fclose(table);
    
	//open your outfile and start writing the issues
	outfile = fopen(specName, "w");
	fprintf(outfile, "%s SPEC Check:\n\n", fileName);
	
	printf("\nEnsure that your sections are saved in the same directory as this program, and that their names are 1 word\n");
	
	//prompt for sections and test them against the tableArray. print results to outfile
	do
	{
		printf("Enter a section number to check: ");
		scanf("%s", sectionName);
		
		//run the spec check on that section
		checkSection(sectionName, tableArray, outfile,2);
	} while(strcmp(sectionName, "quit")!=0);
    
    fclose(outfile);
	printf("Your Spec Check is saved under the file %s\n\n", specName);

}

/* checkSection */
//	checks referenced sections in a given section against the table of contents
void checkSection(char *sectionName, section_t *tableArray, FILE *outfile, int choice)
{
	int i,j,slashcount,questioncount,swncount;
	
	FILE *section;
	char fileName[SECTNAME];
	section_t sectionArray[SECTNUM];
	section_t discrepancies[SECTNUM];
	
	strcpy(fileName, sectionName);
	strcat(fileName,".txt");
	//only if the file exists
	if(fopen(fileName, "r")!=NULL)
	{
		//open the file and make an array of the sections in it
		section = fopen(fileName, "r");
	    findSections(sectionArray,section);
	    	
	    //test each section in sectionArray against tableArray
	    //if there is a discrepancy, add it to an array
		j=0;
	    for(i=1;tableArray[i].count!=0; i++)
	    {
			if(linearSearch(tableArray, sectionArray[i].shortName)==0 && sectionArray[i].count!=0)
			{
				strcpy(discrepancies[j].name,sectionArray[i].name);
				discrepancies[j].count=sectionArray[i].count;
				j++;
			}
	    }
            
    	//also check for '//','?','spec writer note'
		findProblems(section,&slashcount,&questioncount,&swncount);
    		
    	//if there are problems, note them
    	if(j!=0 || slashcount!=0 || questioncount!=0 || swncount!=0)
    	{
	    	fprintf(outfile, "Section %s:\n", sectionName);
	    		
	    	if(j!=0){
	        	fprintf(outfile, "Reference Check:\n");
	        	for(i=0; i<j ;i++){
   	    			fprintf(outfile, "\t%s appears %d time", discrepancies[i].name, discrepancies[i].count);
           		 	if(discrepancies[i].count!=1)
           		        fprintf(outfile, "s");
           		    fprintf(outfile, "\n");
	        	}
	    	}
	    		
			if(slashcount!=0){
            	fprintf(outfile, "'//' appears %d time", slashcount);
            	if(slashcount!=1)
            	    fprintf(outfile, "s");
            	fprintf(outfile, "\n");
			}
           	if(questioncount!=0){
            	fprintf(outfile, "'?' appears %d time", questioncount);
            	if(questioncount!=1)
            	    fprintf(outfile, "s");
            	fprintf(outfile, "\n");
           	}
            if(swncount!=0){
            	fprintf(outfile, "'SPEC WRITER NOTE' appears %d time", swncount);
            	if(swncount!=1)
            	    fprintf(outfile, "s");
            	fprintf(outfile, "\n");
            }
            fprintf(outfile, "\n");
	    }
	    	
		//close the file
	    fclose(section);
	} else{
		if(choice == 1)			//indicates automaticSeparate
		{
			//if there is no file for a section listed in the table of contents, make note
			if(strcmp(sectionName, "000110")!=0)
				fprintf(outfile, "Warning! Unable to find Section %s in the body.\n\n", sectionName);
		} else if(choice == 2)	//indicates manualSeparate
		{
			//if there is no file for a section listed in the table of contents, make note
			if(strcmp(sectionName, "quit")!=0)
				printf("Warning! this section does not appear in the index.\n\n");
		} else printf("Invalid input parameter for checkSection function\n");
	}
		
}

/* separateTable */
//	makes separate files for the table of contents and the body of the file
//	scans&copies until it finds "SECTION", then scans&copies until end
void separateTable(char *fileName, char *tableName, char *bodyName)
{
	int quit;
	
    FILE *spec;
    FILE *table;
    FILE *body;
    
    char specName[SECTNAME];
	char word[MAXLEN];
	char test[MAXLEN];
	
    table = fopen(tableName, "w");
    body = fopen(bodyName, "w");
	spec = fopen(fileName, "r");

	quit = 0;
	fscanf(spec, "%s", word);

	//scan until we encounter "SECTION", and the number isn't 00 01 10 (the table)
	do{
		if(strcmp(word, "SECTION")==0){
			fscanf(spec, "%s", test);
			if(testNumber(spec, test) && strcmp(test, "000110")!=0)
			{
				quit = 1;
				//make sure "SECTION ## ## ##" is copied to the body
				fprintf(body, "%s %s ", word, test);
			}
			else fprintf(table, "%s %s ", word, test);
		}else fprintf(table, "%s ", word);
	}while(fscanf(spec, "%s", word)!=EOF && !quit);
	    
	//scan the rest into the body
	while(fscanf(spec, "%s", word)!=EOF)
	    fprintf(body, "%s ", word);
    
    fclose(spec);fclose(table);fclose(body);
	
}

/* findSections: */
//	finds sections in a chunk of text and makes an array of them
//	accounts for repeats
void findSections(section_t *array, FILE *file)
{
	char word[MAXLEN];
	char condensed[MAXLEN];
	int i,already, choice;
	
	int index=1;
    
	//sets the count of every entry in the array to zero, so later on we can tell when to stop reading
	for(i=0;i<SECTNUM;i++)
	    array[i].count=0;
    
	//start reading from the beginning of the file
	fseek(file, 0, SEEK_SET);
	
	//if a section appears, test if it's appeared before
	//	if it hasn't, add this section to the array
	//	if it has, increment its count
	while(fscanf(file, "%s", word) !=EOF)
	{
		choice = testNumber(file, word);
		if(choice)
		{
			//add a short name and a long name. Print long names, but compare short names
			//bug fixing the ######.##.## vs ## ## ##.## ## dilemma
			strcpy(condensed, word);
			if(condensed[6]=='.')
				condensed[9]='\0';
			else condensed[6]='\0';
			
			already = linearSearch(array, condensed);
			
			if(already==0)
			{
				strcpy(array[index].name,word);
				strcpy(array[index].shortName,condensed);
				array[index].count=1;
				index++;
			} else array[already].count++;
		}
	}
}

/* testNumber */
//	tests if a scanned word is a section number
int testNumber(FILE *spec, char *word)
{
	int i;
	
	char second[MAXLEN];
	char third[MAXLEN];
	char fourth[MAXLEN];
	
	if((word[0]>47 && word[0]<58) && (word[1]>47 && word[1]<58) && (word[2]=='\0'))
	{
		fscanf(spec, "%s", second);
		if((second[0]>47 && second[0]<58) && (second[1]>47 && second[1]<58) && (second[2]=='\0'))
		{
	   		fscanf(spec, "%s", third);
			if((third[0]>47 && third[0]<58) && (third[1]>47 && third[1]<58) && ((third[2]>31 && third[2]<48) || third[2]=='\0' || (third[2]>57 && third[2]<65)))
			{
				strcat(word,second);
				strcat(word,third);
                
				//sometimes sections are ## ## ##.##... this accounts for this
				if(third[2]=='.')
				{
					fscanf(spec, "%c", &fourth[0]);
					if(fourth[0]>47 && fourth[0]<58)
					{
						for(i=1;((fourth[i]>47 && fourth[i]<58) || fourth[i]=='.');i++)
							fscanf(spec, "%c", &fourth[i]);
						strcat(word,fourth);
					}
				}
                
				//make sure the section ends in .##
				if(!(word[strlen(word)-1]>47 && word[strlen(word)-1]<58))
					word[strlen(word)-1]='\0';
				if((word[strlen(word)-1]>47 && word[strlen(word)-1]<58) && !(word[strlen(word)-2]>47 && word[strlen(word)-2]<58))
					word[strlen(word)-2]='\0';
				
				return(1);
			} else return(0);
		} else return(0);
    //sometimes there are typos and people enter the number as one word instead of 3
	}else if((word[0]>47 && word[0]<58) && (word[1]>47 && word[1]<58) && (word[2]>47 && word[2]<58) && (word[3]>47 && word[3]<58) && (word[4]>47 && word[4]<58) && (word[5]>47 && word[5]<58) && ((word[6]>31 && word[6]<48) || word[6]=='\0' || (word[6]>57 && word[6]<65)))
	{
		//sometimes sections are ######.##... this accounts for this
		if(word[6]=='.')
		{
			fscanf(spec, "%c", &second[0]);
			if((second[0]>47 && second[0]<58))
			{
				for(i=1;((second[i]>47 && second[i]<58) || second[i]=='.');i++)
					fscanf(spec, "%c", &second[i]);
				strcat(word,second);
			}
		}
        
		//make sure the section ends in .##
		if(!(word[strlen(word)-1]>47 && word[strlen(word)-1]<58))
			word[strlen(word)-1]='\0';
		if((word[strlen(word)-1]>47 && word[strlen(word)-1]<58) && !(word[strlen(word)-2]>47 && word[strlen(word)-2]<58))
			word[strlen(word)-2]='\0';
			
    	return(1);
	}else return(0);
}

/* linearSearch: */
//checks each element in the table[] array to see if it matches the input section name
//	if it returns 0, it means that the section does not appear. Otherwise it will return
//	the index of the corresponding entry.
//	NOTE: START THE ARRAYS IN FUNCTIONS THAT CALL THIS ONE AT INDEX 1 SO AS TO AVOID CONFUSION
int linearSearch(const section_t *table, const char *section)
{
	int i;
    
	for(i=0;i<SECTNUM;i++){
		if(strcmp(table[i].shortName,section)==0){
			return(i);
		}
	}
	
	return(0);
}

/*separateSections*/
//	first, searches for "SECTION"
//	next, checks if the word is followed by a section number
//	next, checks if that section number is the sequential one in the table of contents
//	if it's found a new section, it creates a file called ######.txt and copies it to this file
//	it will continue copying until it reaches SECTION ######, and the new number is the next section
//	after copying, it will rewind to the beginning of the file and go again, until it's reached the end
void separateSections(section_t *tableArray, char *specName, int *skipArray)
{
	int i, j, choice, cease, found;
	FILE *spec, *ofile;
	char word[MAXLEN];
	char section[SECTNAME];
		
	int index = 0;
	
	spec = fopen(specName, "r");
	for(i=1;tableArray[i].count !=0 ;i++)
	{
	    choice = 0;
		cease = 0;
		found = 0;
        
		while(fscanf(spec, "%s", word)!=EOF)
		{
			//find SECTION
		    if(strcmp(word,"SECTION")==0)
		    {
		    	fscanf(spec, "%s", word);
		    	choice = testNumber(spec,word);
				
				//find a section number
		    	if(choice)
		    	{
		    		//find the next section number. bingo
		            if(strcmp(word,tableArray[i].shortName)==0)
			        {
			        	//start copying
			            found = 1;
			            strcpy(section, word);
			    	    strcat(word,".txt");
			    	    
			    	    //BUG FIX: some sections were only printing "SECTION ######" b/c they were being reopen and overwritten
			    	    //	this ensures no files will be overwritten (only write if the file doesn't already exist)
						if(fopen(word, "r")==NULL){
			        		ofile = fopen(word, "w");
			            
			            	fprintf(ofile, "SECTION %s ", section);
			            
			            	//go until end of file or until found the next section
			            	//  bug fix: check the section after that, too (in case the next section isn't there)
			        		while(fscanf(spec, "%s", word)!=EOF && !cease)
			        		{
				     	  	  if(strcmp(word,"SECTION")!=0){
				    	    		fprintf(ofile, "%s ", word);
				    	    	}
				    	    	else{
				    	    		fscanf(spec, "%s", word);
				    	    		choice = testNumber(spec,word);
				    	    		for(j=i+1; tableArray[j].count!=0;j++)
				    	    			if(strcmp(word,tableArray[j].shortName)==0)
						            		cease = 1;
				    	   		}
			    	   		}
			    	    
			    	    	fclose(ofile);
			    	    }
			        }
                    // in case "SECTION" appears twice in a row
			    }else fseek(spec, -8, SEEK_CUR);
		    }
		}
		
		//if we're missing a section, add it to the array
		if(!found)
		    skipArray[index++]=i;
        
		//go back to the beginning of the file
	    rewind(spec);
 
	}
	printf("done\n");
	
	fclose(spec);
}

/* findProblems*/
// searches for //, ?, and 'spec writer note'
void findProblems(FILE *ifile, int *slashcount, int *questioncount, int *swncount)
{
    char character;
    char string[MAXLEN];
    
    //using pointers b/c can't return multiple values
    *slashcount = 0;
    *questioncount=0;
    *swncount=0;
    
    //start at the beginning of the file & search for "//"
    fseek(ifile, 0, SEEK_SET);
    while(fscanf(ifile, "%c", &character)!=EOF)
    {
        if(character=='/')
        {
            fscanf(ifile, "%c", &character);
            if(character=='/')
            {
                fscanf(ifile, "%c", &character);    //make sure not followed by www.
                if(character!='w')
                {
                    fscanf(ifile, "%c", &character);
                    if(character!='w')
                    {
                        fscanf(ifile, "%c", &character);
                        if(character!='w')
                        {
                            fscanf(ifile, "%c", &character);
                            if(character!='.')
                                (*slashcount)++;
    }   }   }   }   }   }
    
	//start at the beginning of the file and search for "?"
    fseek(ifile, 0, SEEK_SET);
    while(fscanf(ifile,"%c",&character)!=EOF)
    {
        if(character=='?')
            (*questioncount)++;
    }
    
    //start at the beginning of the file and search for "spec writer note"
    fseek(ifile, 0, SEEK_SET);
    while(fscanf(ifile, "%s", string)!=EOF)
    {
        if(strcmp(upperCase(string),"SPEC")==0)
        {
            fscanf(ifile, "%s", string);
            if(strcmp(upperCase(string),"WRITER")==0)
            {
                fscanf(ifile,"%s", string);
                //in case you get notes, note:, note., etc.
                string[4]='\0';
                if(strcmp(upperCase(string),"NOTE")==0)
                    (*swncount)++;
            }
        }
    }
}

/* upperCase */
//  takes an inputed string and changes all lowercase letters to uppercase. returns pointer to that string
char* upperCase(char *string)
{
    int i;
    for(i=0;string[i]!='\0';i++)
        if(string[i]>96 && string[i]<123)
            string[i]-=32;
    
    return(string);
}

/* clearFiles */
//	deletes the section files after the program has executed
//  skips the sections in the table that don't appear in the body
//	also deletes the table & body files
void clearFiles(section_t *tableArray, int *skipArray, char *tableName, char *bodyName)
{
 	int i, status;
	char str[SECTNAME];
	char command[SECTNAME];
	
	int index = 0;
	
	//	clear section files
	for(i=1; tableArray[i].count!=0;i++)
	{
		if(i!=skipArray[index])
		{
            
			strcpy(str, tableArray[i].shortName);
			strcat(str,".txt");

			status = remove(str);
			if(status!=0)
				printf("Warning! File %s not deleted.\n", str);
				
		} else index++;
	}
	
	//	clear table file
	status = remove(tableName);
	if(status!=0)
		printf("Warning! File %s not deleted.\n", tableName);

	//	clear body file
	status = remove(bodyName);
	if(status!=0)
		printf("Warning! File %s not deleted.\n", bodyName);
}