//This is my code
//Matt Heim
//CS366

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <utime.h>
#include <time.h>
#include <string.h>

void SupportedCommands()
{
   printf("\n");
   printf("’c’ list the current file\n");
   printf("’d’ copy the current file\n");
   printf("’r’ rename the current file\n");
   printf("’u’ delete (unlink) the current file\n");
   printf("'t' zero (length) the current file\n");
   printf("'a' append the current file to another file\n");
   printf("'l' display the last 100 bytes of the curent file\n");
   printf("'m' change the current file's access permissions\n");
   printf("'x' change the current file's access to the current time\n");
   printf("'n' go to the next file\n");
   printf("'q' quit\n");
   printf("\n");
   //fflush(stdout);
   //exit(0);
}


int main(int argc, char *argv[])
{
   SupportedCommands();
   char userCommand;
   char destinationCommand;

   //Print Test for Arguments (changed 0 to 1 to eliminate file name as first arg)
   for(int i = 1; i<argc; i++)
   {
      printf("argv[%d]: %s\n", i, argv[i]); // prints all files the user has selected.
   }
   printf("---\n"); // line skip
   
   int i = 1;
   char c;
   char ret;
   FILE *currFile; //pointer
   do 
   {
      printf("Current File: argv[%d]: %s\n", i, argv[i]);
      scanf(" %c", &userCommand);
      //char userCommand =  scanf("%c", &userCommand);
      
      if(userCommand == 'n')
      {
         printf("This should break and go to next file...\n");
         i ++;
      }
      
      if(userCommand == 'c')
      {

    	 currFile = fopen(argv[i], "rt");

    	 while((c=fgetc(currFile)) != EOF)
	 {
            printf("%c",c);
    	 }
     	 fclose(currFile);
      }

      if(userCommand == 'd')
      {
         currFile = fopen(argv[i], "r");
         if(currFile == NULL)
         {
	    printf("Press any key...\n");
	    exit(EXIT_FAILURE);
	 }

         char copy[50]; 
         printf("Enter name for copy file(ex: name.txt)\n");
         scanf(" %s", copy);

         FILE *copyFile = fopen(copy, "w");

         if(copyFile == NULL)
         {
            fclose(currFile); 
	    printf("Press any key...\n");
	    exit(EXIT_FAILURE);
	 }
         while((c=fgetc(currFile)) != EOF)
         {
            fputc(c, copyFile);
         }
         fclose(currFile);
         fclose(copyFile);
      }

      if(userCommand == 'r')
      {
         char newname[100]; 
         printf("Enter a new name for file(ex: name.txt)\n");
         scanf(" %s", newname);      

         ret = rename(argv[i], newname);
	 argv[i] = newname;

         
         if(ret == 0) 
         {
            printf("File renamed successfully\n");
         }
         else 
         {
            printf("Error: unable to rename the file\n");
         }
       
      }
      
      if(userCommand == 'u')
      {
	 int status = remove(argv[i]);
         
         if(status  == 0)
	 {
	    printf("file deleted...\n");
         }
         else
         {
            printf("Unable to delete the file\n");
	    perror("Error");
	 }
      }

      if(userCommand == 't')
      {
         currFile = fopen(argv[i],"w");
	 if (currFile != NULL) 
	 fclose(currFile);   
      }

      if(userCommand == 'a')
      {
         currFile = fopen(argv[i], "r");
	 char appendName[100]; 
	 char buffer [256];
         printf("Enter a new name for the file to append(ex: name.txt)\n");
         scanf(" %s", appendName);
         FILE *append = fopen(appendName, "a");

         if(currFile == NULL)
         {
            perror("Error opening file.");
	 }
 	 else
  	 {
	     while(fgets(buffer, sizeof(buffer), currFile))
	     {
                fprintf(append, "%s", buffer);
	     }
	 }
         fclose(currFile);
	 fclose(append);
      }
      
      if(userCommand == 'l') //display last 100 bytes
      {
         unsigned char buffer[100];
         int x, cu;
         long size;

         currFile = fopen(argv[i], "r+b");
         fseek(currFile, 0, SEEK_END); //set position to end of file 
         size = ftell(currFile); //get current position 

         fseek(currFile, size-100, SEEK_SET); // set position at begining

	 if(currFile != NULL)
	 {
	    for(x = 0; x < 100; x ++)
	    {
   	       cu = fgetc(currFile);
	       buffer[x] = cu; // store in array
	       printf("%c", cu);
	    }
	    printf("\n");
	 }
	 else
	 {
	    printf("file not found.\n");
	 }

	 fclose(currFile);
	 buffer[x] = '\0';

	 for(x = 0; x < sizeof(buffer); x++)
	 {
	    //printf("Result : %X\n", buffer[x]);
	 } 
      }

      if(userCommand == 'm') //change file's access permissions
      {
        char mode[15];
        printf("Here are the commands you can enter: 777, 700, 600\n");
        printf("Type what permission to change: \n");
        scanf("%s", mode);
       
        if(strcmp(mode, "700") == 0)
        {
          chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR);
          printf("Changed mode.\n");
        }
        else if(strcmp(mode, "777") == 0)
        {
           chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH |     S_IXOTH); 
          printf("Changed mode.\n");
        }
        else if(strcmp(mode, "600") == 0)
        {
          chmod(argv[i], S_IRUSR | S_IWUSR);
          printf("Changed mode.\n");
        }
      }

      if(userCommand == 'x')
      {
         struct utimbuf timebuf;
	 struct stat statbuf;

         if(stat(argv[i], &statbuf) < 0)
	 {
	    printf("Error getting file file stats\n");
	    return -1;
	 }

	 if((currFile = fopen(argv[i], "r+b")) < 0)
	 {
	    printf("Error opening file\n");
	    return -1;
	 }
		
	 fclose(currFile);

	 if(utime(argv[i], &timebuf) < 0)
	 {
	    printf("unable to reset time\n");
	    return -1;
	 }

	 printf("time is %s", ctime(&statbuf.st_ctime));
	    
      }

      if(argv[i] == NULL) // start over if you get to end 
      {
         i = 1;
      }
   }while(userCommand != 'q');   
}









