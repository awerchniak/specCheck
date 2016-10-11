/* Clears unnecessary files that specCheck created */

#include <stdio.h>
#include <string.h>

#define SECTNAME 30
#define SECTNUM 400

int main(void)
{
	//declare & initialize variables, arrays, & pointers
	int go,i,j,tableCount;
	int skip[SECTNUM];
	char section[SECTNUM][SECTNAME];
	char dummy[SECTNAME];
	char command[SECTNAME];
	FILE *in;
	
	//the program only works if specCheck successfully wrote sections.txt
	if(fopen("sections.txt", "r")!=NULL)
	{
		//open file, scan # of sections
		in = fopen("sections.txt", "r");
		fscanf(in, "%d", &go);
		if(go==1)
		{	
			fscanf(in, "%d", &tableCount);
	
			//fill an array with section names
			for(i=0;i<tableCount;i++)
				fscanf(in, "%s", section[i]);
		
			//get the names of table sections that weren't actually in the document
			for(i=0;fscanf(in, "%d", &skip[i])!=EOF;i++);

			//close the infile
			fclose(in);
		
			//this is because skip keeps glitching and giving a bunch of extraneous 0's. there should only
			//	be one, and if it appears it must be the first entry
			if(skip[1]!=0)
			{
				//delete each section from the tableArray
				j=0;
				for(i=0;i<tableCount;i++)
				{
					if(i!=skip[j])
					{
						strcpy(command, "exec del ");
						strcat(command,section[i]);
						strcat(command,".txt");
						system(command);
					}else j++;
				}
			}else
			{
				for(i=0;i<tableCount;i++)
				{
					strcpy(command, "exec del ");
					strcat(command,section[i]);
					strcat(command,".txt");
					system(command);
				}
			}
		
			//delete the save file that specCheck created
			system("exec del sections.txt");
		
			return(0);
		} else return(-1);
	//if sections.txt doesn't exist, throw an exception
	}else
	{
		printf("Error! ""sections.txt"" not correctly written\n");
		printf("Numerous created section files not deleted.\n");
		return(-1);
	}
}