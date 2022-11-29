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

// L function
    
    if(strcmp(argv[1], "L")  == 0)
    {
        unsigned int bytesUsed = 0;
        printf("\nOption L: List Files \n \n");
        for (i =0; i <512; i=i+16)
        {
            if (dir[i] == 0) break;
            for (j = 0; j < 8; j++)
            {
                if (dir[i+j] == 0)
                {
                    //printf("");
                    dir[i+j] == '\0'; // empty
                }
                else
                {
                    printf("%c", dir[i+j]);
                }

            }
            // end of j for loop
            bytesUsed = bytesUsed + dir[i+10]*512; // 
            printf(".%c  %d bytes: ", dir[i+8], 512*dir[i+10]);printf("\n");
            
        }
        printf("Bytes currently used %u ", bytesUsed);printf("\n");
        printf("Remaing Bytes left: %u ", 261632-bytesUsed);printf("\n");
    }


// P function
if(strcmp(argv[1],"P")==0)
{
    char fileName[8]; // make 8 or 9? 
      for (i=0; i<512; i=i+16) {
        if (dir[i]==0) break;
        for (j=0; j<8; j++) {
            if (dir[i+j]==0) fileName[j] ='\0'; else fileName[j]=dir[i+j];
        }
          if(strcmp(argv[2], fileName)==0) {
                if(dir[i+8] != 't')
            {
                if(dir[i+8] !='x')
                {
                 printf("File not found");
                }
                else { printf("File is not printable");}
            }
            else
            {
                //PRINT FILE
                char buffer[12288];
                fseek(floppy,512*dir[i+9],SEEK_SET);
                for(i=0; i<256; i++) {
                   buffer[i]=fgetc(floppy);
                }
                for(i=0; i<256; i=i+16) 
                {
                    if(buffer[i] == 0) break;
                    for (j=0; j < 16; j++)
                    if (buffer[i+j] == 0)
                    {
                        buffer[i+j] = "\0";
                    }
                    else
                    {
                        printf("%c", buffer[i+j]);
                    }
                    
                }


            }

            }
            
    }
    printf("\n end of P function \n");


}

/// M function


if(strcmp(argv[1],"M")==0) // argv[2] is the file name
{
    printf("entered the M function \n");
    char contents[512]; // max size is 1 sector 
    int startPoint; // (i+9) points to where the sector will begin at.
    char fileName[9];
    char dirSearch[9]; // char temp[9];
    dirSearch[8] = '\0';
    int boolFlag = 0;
   
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
            printf("duplicate or invalid file name \n"); // exit
            return -1; // end program maybe use break?
    }
    else 
    {
        
        for(i=0; i<512; i=i+16) 
        {
            if(dir[i]==0) // look for free space in directory
                {
                    printf("we have found a free space in the directory! \n");
                    for(j=0; j<8; j++) 
                    {
                        dir[i+j] = fileName[j];
                    }
                    fileName[8] = 't';
                    for(int j=0; j<512; j++)
                    {
                        if(map[j]==0)
                        {
                            printf("free space on map has been found ! \n");
                            // should set map location to 16 * i * j
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





	//write the map and directory back to the floppy image
    fseek(floppy,512*256,SEEK_SET);
    for (i=0; i<512; i++) fputc(map[i],floppy);

    fseek(floppy,512*257,SEEK_SET);
    for (i=0; i<512; i++) fputc(dir[i],floppy);

	fclose(floppy);
    
}