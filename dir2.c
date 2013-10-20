/* Alex Kort, 4-9-06, Program 3, dir.c */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include "dir.h"

int numBytes;
int numBytesFree;
int numFiles;
int numDirs;
int status;
int i; //iterator

//these are for flag setting when parsing commands
int isA, isPerm = PERM_ALL, isB, isC = 1, isD, isO, isSort = SORT_ORDER_N;
int isQ, isS, isT = TIME_A, isW, is4 = 1, isNeg;

int main(int argc, char *argv[])
{
	parse(argc, argv);
	if (isB)
	{
		bare();
		exit(0);
	}
	if (isW)
	{
		printcwd();
		wide();
		exit(0);
	}
	if (isD)
	{
		printcwd();
		widesort();
		exit(0);
	}
	if (isA)
	{
		printcwd();
		printdir2();
		exit(0);
	}
	//printcwd();
	//attributes();
	//bare();
	printcwd();
	printdir2();
}

void parse(int argc, char **argv)
{
	char opt = ' ';
	while (opt != -1)
	{
		opt = getopt(argc, argv, "a:bcdro:qst:w4^");
		if (opt == '?')
		{
			printf("Unknown option '%c' encountered\n", optopt);
		}
		if (opt == '^')
		{
			isNeg = 1;
			continue;
		}
		if (isNeg == 1)
		{
			if (opt == 'a')
			{
				if (optarg != NULL)
				{
					printf("Parameter format not correct - \"a%s\".\n", optarg);
					exit(0);
				}
				isA = 1;
			}		
			if (opt == 'b')
			{
				isB = 0;
			}
			if (opt == 'c')
			{
				isC = 0;
			}
			if (opt == 'd')
			{
				isD = 0;
			}
			if (opt == 'q')
			{
				isQ = 0;
			}
			if (opt == 's')
			{
				isS = 0;
			}
			if (opt == 't')
			{
				isT = TIME_A;
			}		
			if (opt == 'w')
			{
				isW = 0;
			}
			if (opt == '4')
			{
				is4 = 0;
			}
		}
		else 
		{
			if (opt == 'a')
			{
				if (optarg == NULL)
				{
					isPerm = PERM_ALL;
				}
				else if (strcmp(optarg, "d") == 0)
				{
					isPerm = DIRS_ONLY;
				}
				else if (strcmp(optarg, "r") == 0)
				{
					isPerm = RD_ONLY;
				}
				else if (strcmp(optarg, "^d") == 0)
				{
					isPerm = RD_ONLY;
				}
				else if (strcmp(optarg, "^r") == 0)
				{
					isPerm = DIRS_ONLY;
				}
				else if (strcmp(optarg, "^d^r") == 0)
				{
					isPerm = PERM_NONE;
				}
				else if (strcmp(optarg, "dr") == 0)
				{
					isPerm = DIRS_RD;
				}
				else
				{
					printf("Parameter format not correct - \"%s\".\n", optarg);
					exit(0);	
				}
				isA = 1;
			}	
			if (opt == 'b')
			{
				isB = 1;
			}
			if (opt == 'c')
			{
				isC = 1;
			}
			if (opt == 'd')
			{
				isD = 1;
			}
			if (opt == 'q')
			{
				isQ = 1;
			}
			if (opt == 's')
			{
				isS = 1;
			}
			if (opt == 't')
			{
				if (optarg == NULL)
				{
					isT = PERM_ALL;
				}
				else if (strcmp(optarg, "a") == 0)
				{
					isT = TIME_A;
				}
				else if (strcmp(optarg, "w") == 0)
				{
					isT = TIME_W;
				}
				else if (strcmp(optarg, "c") == 0)
				{
					isT = TIME_C;
				}
				else
				{
					printf("Parameter format not correct - \"%s\".\n", optarg);
					exit(0);	
				}
			}
			if (opt == 'w')
			{
				isW = 1;
			}
			if (opt == '4')
			{
				is4 = 1;
			}
		}
		
	}
}

//directory reading, change to print out files
void printdir()
{
     struct dirent *dp;
     struct stat   statbuf;
     struct passwd *pwd;
     struct group  *grp;
     struct tm     *tm;
     char          datestring[256];
     
     DIR *dir;
     if ((dir = opendir(".")) == NULL) 
     {
        	perror("couldn't open '.'");
                return;
     }
     /* Loop through directory entries */
     while ((dp = readdir(dir)) != NULL) 
     {
     	  status = lstat(dp->d_name, &statbuf);
     	  //status = fstatat(0, dp->d_name, &statbuf, AT_SYMLNK_NOFOLLOW);

     	  //printf("symlink: %d\n", S_IFLNK);
     	  //printf("lstat ret: %d\n", status);
         /* Get entry's information. */
         if (status == -1)
         continue;

          /* Print out type, permissions, and number of links. */
          //printf("%10.10s", (statbuf.st_mode));
          //printf("%4d", statbuf.st_nlink);

          /* Print out owners name if it is found using getpwuid(). */
          //if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
          //   printf(" %-8.8s", pwd->pw_name);
          //else
          //   printf(" %-8d", statbuf.st_uid);
          
          /* Get localized date string. */
          tm = localtime(&statbuf.st_mtime);
          strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
          printf(" %s", datestring);
          /* Print out group name if it's found using getgrgid(). */
          //if ((grp = getgrgid(statbuf.st_gid)) != NULL)
          //   printf(" %-8.8s", grp->gr_name);
          //else
          //   printf(" %-8d", statbuf.st_gid);
          if (S_ISDIR(statbuf.st_mode) && ((isPerm == DIRS_RD) || (isPerm == DIRS_ONLY)))
	  {
          	printf("    <DIR>          ");
          	//if (isQ)
          	//{
          		/* Print out owners name if it is found using getpwuid(). */
          	//	if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
          	//	   printf(" %s  ", pwd->pw_name);
          	//}	
          	printf("%s\n", dp->d_name);
          	numDirs++;
          	numBytesFree += (intmax_t)statbuf.st_size;
          	
          }
          else if (S_ISREG(statbuf.st_mode) && ((isPerm == DIRS_RD) || (isPerm == RD_ONLY)))
          {
          	printf("         ");
          	numFiles++;
          	numBytes += (intmax_t)statbuf.st_size;
          	/* Print size of file. */
          	printf(" %8jd %s\n", (intmax_t)statbuf.st_size, dp->d_name);
          }
          else if (S_IFLNK == status)
          {
          	printf("    <SYM>        %s\n", dp->d_name);
          }

      }
        printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
        if (numDirs > 0)
        	printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);
        
	int numBytes = 0;
	int numBytesFree = 0;
	int numFiles = 0;
	int numDirs = 0;
        
}

void printdir2()
{
	//int for longest string length, for indenting
	//(next file name begins 3 spaces after longest name)
	int longest = 0;
	//iterator for the bufarray
	int i = 0;
	int j;
	int k;
	int l;
	int m;
	DIR *dirp;
	struct stat   statbuf;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
        struct passwd *pwd;
	struct group  *grp;
	struct tm     *tm;
	char          datestring[256];
        
        if ((dirp = opendir(".")) == NULL) 
        {
        	perror("couldn't open '.'");
                return;
        }
        
        //put files into array and sort their names
        while ((dp = readdir(dirp)) != NULL)
	{
		bufarray[i] = dp;
		i++;
	        if (i >= sizeof(bufarray))
	        {	        		        	
	        	realloc(bufarray, sizeof(bufarray)*2);
	        }
	}
	
	//bubble sort
	for (j = 0; j< i; j++) 
	{
  		for (k = 0; k < i - 1 - j; k++)
    			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) < 0) 
    			{  
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
	}
	l = 0;
	while (l < i)
	{
		errno = 0;
		char* string = bufarray[l]->d_name;
	        status = lstat(string, &statbuf);
	        if (status == -1)
	        	continue;
	        
	          //get the correct time as specified in the command line	
	          if (isT == TIME_W)
	          {
	          	tm = localtime(&statbuf.st_mtime);
	          }
	          else if (isT == TIME_C)
	          {
	          	tm = localtime(&statbuf.st_ctime);
	          }
	          else if (isT == TIME_A)
	          {
	          	tm = localtime(&statbuf.st_atime);
	          }
	          
	          /* Print out group name if it's found using getgrgid(). */
	          //if ((grp = getgrgid(statbuf.st_gid)) != NULL)
	          //   printf(" %-8.8s", grp->gr_name);
	          //else
	          //   printf(" %-8d", statbuf.st_gid);
	          char* mode = (char*) malloc(20);
   		  sprintf(mode, "%o", statbuf.st_mode);
	          
	          //print out the files according to attributes from command line and file types
	          if (S_ISDIR(statbuf.st_mode) && ((isPerm == DIRS_RD) || (isPerm == DIRS_ONLY) || (isPerm == PERM_ALL))) 
		  {
		        //default year is 4 digits, if it was unset then print only last 2
		        if (is4)
		        {
		        	strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
		        }
		        else
		        {
		        	strftime(datestring, sizeof(datestring), "%m/%d/%y  %I:%M %p", tm);
		        }
		        //print the date
		        printf(" %s", datestring);
	          	printf("    <DIR>           ");
	          	if (isQ)
	          	{
	          		/* Print out owners name if it is found using getpwuid(). */
	          		if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
	          		   printf(" %s  ", pwd->pw_name);
	          	}	
	          	printf("%s\n", string);
	          	numDirs++;
	          	numBytesFree += (intmax_t)statbuf.st_size;
	          	
	          }
	          else if (S_ISREG(statbuf.st_mode) &&  (((isPerm == RD_ONLY || isPerm == DIRS_RD) && ((mode[strlen(mode) - 1] == '4') && (mode[strlen(mode) - 2] == '4') && (mode[strlen(mode) - 3] == '4')))) || (isPerm == PERM_ALL))
	          {
	         	//default year is 4 digits, if it was unset then print only last 2
		        if (is4)
		        {
		        	strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
		        }
		        else
		        {
		        	strftime(datestring, sizeof(datestring), "%m/%d/%y  %I:%M %p", tm);
		        }
		        //print the date
		        printf(" %s", datestring);
	          	printf("         ");
	          	numFiles++;
	          	numBytes += (intmax_t)statbuf.st_size;
	          	/* Print size of file. */
	          	if (isC)
	          	{
	          		char* fileSize = separator((intmax_t)statbuf.st_size);
	          		printf(" %9js ", fileSize);
	          	}
	          	else
	          	{
	          		printf(" %9jd ", (intmax_t)statbuf.st_size);
	          	}
	          	if (isQ)
	          	{
	          		/* Print out owners name if it is found using getpwuid(). */
	          		if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
	          		   printf(" %s  ", pwd->pw_name);
	          	}	
	          	printf("%s\n", string);
	          }
	          else if (S_ISLNK(statbuf.st_mode) && ((isPerm == PERM_ALL) || (isPerm == PERM_NONE)))
	          {
	          	//default year is 4 digits, if it was unset then print only last 2
		        if (is4)
		        {
		        	strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
		        }
		        else
		        {
		        	strftime(datestring, sizeof(datestring), "%m/%d/%y  %I:%M %p", tm);
		        }
		        //print the date
		        printf(" %s", datestring);
	          	printf("    <SYM>           ");
	          	if (isQ)
	          	{
	          		/* Print out owners name if it is found using getpwuid(). */
	          		if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
	          		   printf(" %s  ", pwd->pw_name);
	          	}	
	          	printf("%s\n", bufarray[l]->d_name);
	          }
	          l++;

        }
        if (isC)
        {
        	char* bytes = separator(numBytes);
        	char* bytesFree = separator(numBytesFree);
        	printf("%15d File(s)%15s bytes\n", numFiles, bytes);
        	if (numDirs > 0)
        		printf("%15d Dir(s)%15s  bytes free\n", numDirs, bytesFree);
        }
        else
        {
                printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
        	if (numDirs > 0)
        		printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);
        }
        
	int numBytes = 0;
	int numBytesFree = 0;
	int numFiles = 0;
	int numDirs = 0;
}

void attributes()
{
	//int for longest string length, for indenting
	//(next file name begins 3 spaces after longest name)
	//int longest = 0;
	//iterator for the bufarray
	//int i = 0;
	//int j;
	//int k;
	int l;
	int m;
	DIR *dirp;
	struct stat   statbuf;
	//struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
	struct dirent **bufarray = alphabetsort(dirp);
        //struct dirent *dp;
        struct passwd *pwd;
	struct group  *grp;
	struct tm     *tm;
	char          datestring[256];
 /*       
        if ((dirp = opendir(".")) == NULL) 
        {
        	perror("couldn't open '.'");
                return;
        }
        
        //put files into array and sort their names
        while ((dp = readdir(dirp)) != NULL)
	{
		bufarray[i] = dp;
		i++;
	        if (i >= sizeof(bufarray))
	        {	        		        	
	        	realloc(bufarray, sizeof(bufarray)*2);
	        }
	}
	
	//bubble sort
	for (j = 0; j< i; j++) 
	{
  		for (k = 0; k < i - 1 - j; k++)
    			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) < 0) 
    			{  
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
	}
*/	
	l = 0;
	while (l < i)
	{
		errno = 0;
		char* string = bufarray[l]->d_name;
	        status = lstat(string, &statbuf);
	        if (status == -1)
	        	continue;
	        
	          /* Print out group name if it's found using getgrgid(). */
	          //if ((grp = getgrgid(statbuf.st_gid)) != NULL)
	          //   printf(" %-8.8s", grp->gr_name);
	          //else
	          //   printf(" %-8d", statbuf.st_gid);
	          char* mode = (char*) malloc(20);
   		  sprintf(mode, "%o", statbuf.st_mode);
	          //if ((statbuf.st_mode == S_IRUSR || S_IRGRP || S_IROTH) && 
	          //	(statbuf.st_mode != S_IWUSR || S_IWGRP || S_IWOTH) &&
	          //	(statbuf.st_mode != S_IXUSR || S_IXGRP || S_IXOTH))
	          printf("mode: %s\n", mode);
	          if ((mode[strlen(mode) - 1] == '4') && (mode[strlen(mode) - 2] == '4') && (mode[strlen(mode) - 3] == '4'))
		  {
		  	tm = localtime(&statbuf.st_mtime);
	          	strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
	          	printf(" %s", datestring);
	         	printf("         ");
	          	numFiles++;
	          	numBytes += (intmax_t)statbuf.st_size;
	          	/* Print size of file. */
	          	printf(" %9jd %s\n", (intmax_t)statbuf.st_size, string);
	          	
	          }
	          l++;

        }
        printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
        if (numDirs > 0)
        	printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);
        
	int numBytes = 0;
	int numBytesFree = 0;
	int numFiles = 0;
	int numDirs = 0;
}

//look up a word in the directory
/*
int main(int argc, char *argv[])
{
	int i;
        for (i = 1; i < argc; i++)
        	lookup(argv[i]);
        return (0);
}
*/
     
//print the current working directory
int printcwd()
{
	char *cwd;
        if ((cwd = getcwd(NULL, 64)) == NULL) 
        {
             perror("pwd");
             exit(2);
        }
        (void)printf("  Directory of %s\n", cwd);
        printf("\n");
        free(cwd); /* free memory allocated by getcwd() */
        return(0);
}
/*
//display files with specified attributes
void attributes(int a, int d)
{
        struct dirent *dp;
        struct stat   statbuf;
        struct passwd *pwd;
        struct group  *grp;
        struct tm     *tm;
        char          datestring[256];
	     
        DIR *dir;
	//only print read only files
	if (a == 1)
	{
	     if ((dir = opendir(".")) == NULL) 
	     {
	        	perror("couldn't open '.'");
	                return;
	     }
	     // Loop through directory entries
	     while ((dp = readdir(dir)) != NULL) 
	     {
	     	 status = lstat(dp->d_name, &statbuf);
	         if (status == -1)
	         continue;
	          
	          tm = localtime(&statbuf.st_mtime);
	          strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
	          printf(" %s", datestring);

	          if (S_IRUSR == statbuf.st_mode || S_IRGRP == statbuf.st_mode || S_IROTH == statbuf.st_mode)
		  {
	         	printf("         ");
	          	numFiles++;
	          	numBytes += (intmax_t)statbuf.st_size;
	          	// Print size of file. 
	          	printf(" %8jd %s\n", (intmax_t)statbuf.st_size, dp->d_name);
	          	
	          }
	
	      }
	        printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
	        if (numDirs > 0)
	        	printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);	
	}
}
*/

//display with no heading or summary
void bare()
{
	int j = 0;
	DIR* dirp;
	struct dirent** bufarray = alphabetsort(dirp);
	while (j < i)
	{
		printf("%s\n", bufarray[j]->d_name);
		j++;
	}
	printf("\n");
	//(void) closedir(dirp);
	/*
        struct dirent *dp;

        if ((dirp = opendir(".")) == NULL) 
        {
        	perror("couldn't open '.'");
                return;
        }

	do 
	{
		errno = 0;
	        if ((dp = readdir(dirp)) != NULL) 
	        {
	        	//this needs changing to distinguish files and directories
	        	//after printing theese values reinitialize them to zero
	       		numFiles++;
	       		numDirs++;
	        	printf("%s\n", dp->d_name);
	        }
        }
        while (dp != NULL);
        printf("\n");

        if (errno != 0)
        	perror("error reading directory");

       (void) closedir(dirp);
        return;
        */
}

//put commas after every 3 digits, for thousands
char* separator(int input)
{
	int mult = 3;
	int check = mult;
	int inc = 1;
	char* string = (char*) malloc(20);
   	sprintf(string, "%d", input);
   	int start = strlen(string);
   	int i;
 	if (start > 3)
 	{
 		for (i = start; i > 0; i--)
	   	{
	   		check--;
	   		string[i] = string[i-1];
	   		if (check == 0)
	   		{
	   			string[i-1] = ',';
	   			start += 2;
	   			i = start;
	   			mult += 3;
	   			check = mult + inc;
	   			inc++;
	   		}
	   			
	   	}
	   	for (i = 0; i < 19; i++)
	   	{
	   		string[i] = string[i+1];
	   	}
   	}
   	return string; 
}

//wide format with files list sorted by column
void widesort()
{

}

//list by files in sorted order
void sort()
{
}

//get owner of the file
void owner()
{
}

//recurse through subdirectories and print out contents of each
void subdirs()
{
}

//print out time based on command
void timefield()
{
	
}

//make columns shorter
void wide()
{
	//int for longest string length, for indenting
	//(next file name begins 3 spaces after longest name)
	int longest = 0;
	//iterator for the bufarray
	//int i = 0;
	//int j;
	//int k;
	int l;
	int m;
	DIR *dirp;
	struct stat   statbuf;
	struct dirent** bufarray = alphabetsort(dirp);
	//struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        //struct dirent *dp;
 /*       
        if ((dirp = opendir(".")) == NULL) 
        {
        	perror("couldn't open '.'");
                return;
        }
        
        //put files into array and sort their names
        while ((dp = readdir(dirp)) != NULL)
	{
		bufarray[i] = dp;
		i++;
	        if (i >= sizeof(bufarray))
	        {	        		        	
	        	realloc(bufarray, sizeof(bufarray)*2);
	        }
	}
	
	//bubble sort
	for (j = 0; j< i; j++) 
	{
  		for (k = 0; k < i - 1 - j; k++)
    			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) < 0) 
    			{  
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
	}
	*/
	//iterate through the array and get the length of the longest file name
	l = 0;
	while (l < i)
	{
		if (strlen(bufarray[l]->d_name) > longest)
		{
			longest = strlen(bufarray[l]->d_name);
		}
		l++;
	}
	
	l = 0;
	while (l < i)
	{
		errno = 0;
	        status = lstat(bufarray[l]->d_name, &statbuf);
	        if (status == -1)
	        continue;
	        
	        if (S_ISDIR(statbuf.st_mode))
		{
		        printf("[%s]", bufarray[l]->d_name);
		        for (m = 0; m < longest - 2 - strlen(bufarray[l]->d_name); m++)
		        {
		        	printf(" ");
		        }
		        printf("   ");
		        numDirs++;
		        numBytesFree += (intmax_t)statbuf.st_size;
		          	
		}
		else
		{
		        //this needs changing to distinguish files and directories
		        //after printing theese values reinitialize them to zero
		       	numFiles++;
		       	numBytes += (intmax_t)statbuf.st_size;
		        printf("%s", bufarray[l]->d_name);
		        for (m = 0; m < longest - strlen(bufarray[l]->d_name); m++)
		        {
		        	printf(" ");
		        }
		        printf("   ");
		}
		l++;	        	
	}
	printf("\n%15d File(s)%15d bytes\n", numFiles, numBytes);
	printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);	
	
	if (errno != 0)
        	perror("error reading directory");

        //(void) closedir(dirp);
        i = 0;
        return;	
}

//display four digit years
void four()
{
}

//sorts all files from the current directory and puts them into an array 
struct dirent** alphabetsort(DIR* dirp)
{

	//iterator for the bufarray
	i = 0;
	int j;
	int k;
	//int l;
	//int m;
	//DIR *dirp;
	//struct stat   statbuf;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
	
        if ((dirp = opendir(".")) == NULL) 
        {
        	perror("couldn't open '.'");
                return;
        }
        
        //put files into array and sort their names
        while ((dp = readdir(dirp)) != NULL)
	{
		//printf("HEY1\n");
		bufarray[i] = dp;
		i++;
	        if (i >= sizeof(bufarray))
	        {	        		        	
	        	realloc(bufarray, sizeof(bufarray)*2);
	        }
	}
	
	//bubble sort
	for (j = 0; j< i; j++) 
	{
  		for (k = 0; k < i - 1 - j; k++)
    			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) < 0) 
    			{  	
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
	}
	//(void) closedir(dirp);
	return bufarray;
}



