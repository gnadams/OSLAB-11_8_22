// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:___Gregory Adams__ Date:_11/23/2022

//filesys.c
//Based on a program by Michael Black, 2007
//Revised 11.3.2020 O'Neil

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int i, j, size, noSecs, startPos;

	//open the floppy image
	FILE* floppy;
	floppy=fopen("floppya.img","r+");
	if (floppy==0)
	{
		printf("floppya.img not found\n");
		return 0;
	}

	//load the disk map from sector 256
	char map[512];
	fseek(floppy,512*256,SEEK_SET);
	for(i=0; i<512; i++)
		map[i]=fgetc(floppy);

	//load the directory from sector 257
	char dir[512];
	fseek(floppy,512*257,SEEK_SET);
	for (i=0; i<512; i++)
		dir[i]=fgetc(floppy);

    //print disk map
	printf("Disk usage map:\n");
	printf("      0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
	printf("     --------------------------------\n");
	for (i=0; i<16; i++) {
		switch(i) {
			case 15: printf("0xF_ "); break;
			case 14: printf("0xE_ "); break;
			case 13: printf("0xD_ "); break;
			case 12: printf("0xC_ "); break;
			case 11: printf("0xB_ "); break;
			case 10: printf("0xA_ "); break;
			default: printf("0x%d_ ", i); break;
		}
		for (j=0; j<16; j++) {
			if (map[16*i+j]==-1) printf(" X"); else printf(" .");
		}
		printf("\n");
	}

    // print directory
	printf("\nDisk directory:\n");
	printf("Name    Type Start Length\n");
    for (i=0; i<512; i=i+16) {
		if (dir[i]==0) break;
		for (j=0; j<8; j++) {
			if (dir[i+j]==0) printf(" "); else printf("%c",dir[i+j]);
		}
		if ((dir[i+8]=='t') || (dir[i+8]=='T')) printf("text"); else printf("exec");
		printf(" %5d %6d bytes\n", dir[i+9], 512*dir[i+10]);
	}
	if (argv[1] == '\0')
	{
		printf("NULL argument, terminating program \n");
		return -1;
	}

// L function
    
    if(strcmp(argv[1], "L")  == 0)
    {
        
        unsigned int bytesUsed = 0;
        printf("\nOption L: List Files \n \n");
        for (i =0; i <512; i=i+16)
        {
            if(dir[i] != 0)
            {
                for (j = 0; j < 8; j++)
                {
                    if (dir[i+j] == 0)
                    {
                        dir[i+j] == '\0'; // empty
                    }
                    else
                    {
                        printf("%c", dir[i+j]);
                    }

                }
                
                bytesUsed = bytesUsed + dir[i+10]*512; 
                
                printf(".%c  %d bytes: ", dir[i+8], 512*dir[i+10]);printf("\n");
            }
        }
        printf("Bytes currently used %u ", bytesUsed);printf("\n");
        printf("Remaing Bytes left: %u ", 261632-bytesUsed);printf("\n");
    }


// P function
else if(strcmp(argv[1],"P")==0)
{
    int bool = 0;
    char fileName[8];
      for (i=0; i<512; i=i+16) {
        if (dir[i]==0) break; // may have to take this line out 
        for (j=0; j<8; j++) 
        {
            if (dir[i+j]==0) fileName[j] ='\0'; else fileName[j]=dir[i+j];
        }
          if(strcmp(argv[2], fileName)==0) 
            {
                if(dir[i+8] != 't')
                {
                    if(dir[i+8] !='x')
                    {
                    printf("File not found \n");
                    return -1;
                    }
                    else 
                    {
                        printf("This file cannot be printed \n");
                        return -1;
                    }
                }
                else
                {
                    // printing part
                    char buffer[12288]; // max size for text file
                    fseek(floppy,512*dir[i+9],SEEK_SET); // i+9 is where it will start
                    for(i=0; i<12288; i++) {
                    buffer[i]=fgetc(floppy); // fill buffer up
                    }
                    int k =0;
                    while(buffer[k]) 
                    {
                        printf("%c", buffer[k]);
                        k++;
                    }
                    bool = 1;
                
                }

            }
            
    }
    if (bool == 0)
    {
        printf("file does not exist. \n");
    }
}

/// M function


else if(strcmp(argv[1],"M")==0) // argv[2] is the file name
{
    printf("entered the M function \n");
    int boolFlag = 0;
    char contents[512]; // max size of 1 sector
    int startPoint; // will point to the start of the file
    char fileName[9]; // use this variable to store the file name taken in by user
    char dirSearch[9]; // use this variable to search through directory to compare with fileName above^
    dirSearch[8] = '\0'; // probably don't need this line
    
   
    for (i = 0; i < 8;++i)
    {
        fileName[i] = argv[2][i];
    }

    int strLength = strlen(argv[2]); 
    if (strLength <1)
    {
        printf("error: must give a file a name \n");
        return -1;
    }
    if (strLength < 8)
    {
        for (i = 0; i < 8 - strLength; ++i )
        { 

            fileName[7-i] = 0; 
        } 
    }
    //fileName[9] = '\0'; //need null terminator ? maybe? we will be safe
    fileName[8] = 't';

    for (i=0; i<512; i=i+16) 
    {
        for (j=0; j<8; j++) {
            if (dir[i+j]==0) dirSearch[j] ='\0'; else dirSearch[j]=dir[i+j];
        }
    }
    if(strcmp(dirSearch, fileName)==0) // BAD (if we find 2 file with same name)
    {
            printf("duplicate or invalid file name \n"); // put back inside the for loop ^
            return -1; // end program maybe use break?
    }
    else 
    {
        
        for(i=0; i<512; i=i+16) 
        {
            if(dir[i]==0) // look for free space in directory
                {
                    //printf("we have found a free space in the directory! \n");
                    for(j=0; j<8; j++) 
                    {
                        dir[i+j] = fileName[j];
                    }
                    fileName[8] = 't';
                    for(int j=0; j<512; j++)
                    {
                        if(map[j]==0)
                        {
                            //printf("free space on map has been found ! \n");
                            
                            startPoint = j;
                            map[j] = 255; // FF
                            boolFlag = 1;  //trigger bool 
                            break;
                        }
                    }
                    if (boolFlag)
                    {
                        printf("Enter the text for your file now: \n");
                        fgets(contents, sizeof(contents), stdin);
                        dir[i+9] = startPoint;
                        dir[i+10] = 1;
                        dir[i+8] = 't';
                        fseek(floppy,512*startPoint,SEEK_SET);
                        for(i=0; i<512; i++) 
                        {
                        fputc(contents[i],floppy);
                        }
                    }
                    
                }

            if (boolFlag)
                {
                    break;
                } 
            }
            
            
    }

    if (boolFlag == 0)
    {
        printf("No disks space :(( \n");
    }
}


// Delete function
else if(strcmp(argv[1],"D")==0)
{
int strLength = strlen(argv[2]); 
    if (strLength <1)
    {
        printf("error: must give a file a name \n");
        return -1;
    }


// 1 set first character of file name to 0
// 2 go through map and set map[i] to 0 based on sector numbers ->(i+10)

    char fileName[8]; // make 8 or 9? 
    int sectorStart;
    int sectorLength;
    int found = 0;
    for (i=0; i<512; i=i+16) 
    {
       // if (dir[i]==0) break;
        for (j=0; j<8; j++) 
        {
            if (dir[i+j]==0) fileName[j] ='\0'; else fileName[j]=dir[i+j];
        }
          if(strcmp(argv[2], fileName)==0) 
            {
                sectorStart = dir[i+9];
                sectorLength = dir[i+10];
                
                for (int j = 0; j < sectorLength; ++j)
                {
                    map[sectorStart+j] = 0x00;
                    dir[i] = 0;
                    found = 1;
                    //printf("we should be breaking out of the function now \n");
                }
                break;
            }
            else
            {
                found = 0;
            }

    }
    if (found ==1)
    {
    printf("file deleted \n");
    }
    else
    {
    printf("error: file not found \n");
    }
}
else // IF someone used a command other than D, L, M, or P
{
    printf("Error: you did not enter a valid command \n");
    printf("valid arguments are: D filename , L , M filename, and P filename \n");
}



	//write the map and directory back to the floppy image
    fseek(floppy,512*256,SEEK_SET);
    for (i=0; i<512; i++) fputc(map[i],floppy);

    fseek(floppy,512*257,SEEK_SET);
    for (i=0; i<512; i++) fputc(dir[i],floppy);

	fclose(floppy);
    
}
