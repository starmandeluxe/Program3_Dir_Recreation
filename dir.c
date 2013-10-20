/*! \file
*
* Alex Kort, 4-9-06, Program 3, dir.c
*
* NOTE: All of this program's options will run without
* exceptions in my home directory
*/
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
#include <ftw.h>
#include <sys/statvfs.h>
#include "dir.h"

///
///a list of dirs to open
///
char** dirs;
///
///pathname of the home directory
///
char* homeDir;
///
///pathname of a temporary directory
///
char* prevdir;
///
///integer for the number of bytes in the directory
///
int numBytes;
///
///integer for the number of free bytes in the directory
///
int numBytesFree;
///
///integer for the number of files in the directory
///
int numFiles;
///
///integer for the number of directories in the directory
///
int numDirs;
///
///integer for the number of total bytes in all directories
///
long totalBytes;
///
///integer for the number of total bytes free in all directories
///
long totalBytesFree;
///
///integer for the number of total files in all directories
///
int totalFiles;
///
///integer for the number of total dirs in all directories
///
int alltotalDirs;
///
///integer for the longest pathname
///
int longest;
///
///integer for the status of the stat struct
///
int status;
///
///flag for detecting if a file was found
///
int fileFound;
///
///integer for the number of total dirs to open
///
int totalDirs;
///
///flag for the first time the current path is initialized
///
int firstTime = 1;
///
///general all purpose iterator
///
int i;

///
/// These variables are for flag setting when parsing commands
///
int isA, isPerm = PERM_ALL, isB, isC = 1, isD, isO;
int isSort = SORT_N;
int isDefault, isT, isTimeField = TIME_A;
int isQ, isS, isW, is4 = 1, isReverse, isNeg;
int skipA, skipW, skipC, skipD, skipS, skipT;

/// 
/// Main function that calls the parsing function to get the command 
/// line options, then executes the options
///
int main(int argc, char *argv[])
{
	parse(argc, argv);
	if (isDefault)
	{
		printdir();
		exit(0);
	}
	if (isS)
	{
		subdirs();
		exit(0);
	}
	if (isB)
	{
		bare();
		exit(0);
	}
	if (isW)
	{
		wide();
		exit(0);
	}
	if (isO)
	{
		printdir();
		exit(0);
	}
	if (isA)
	{
		printdir();
		exit(0);
	}
	if (totalDirs > 0)
	{
		printdir();
		exit(0);
	}
}

///
/// Parsing function that stores what to print and how to print it
///
/**
 * @param argc The number of arguments passed from the command line
 * @param argv The list of strings from the command line
 */
void parse(int argc, char **argv)
{
	struct stat statbuf;
	char opt = ' ';
	int x = 1;
	int y;
	int z = 0;
	dirs = (char**) malloc(2);
	if (argc <= 1)
	{
		dirs[0] = ".";
		totalDirs = 1;
		isDefault = 1;
		return;
	}
	while (x < argc)
	{
		opt = argv[x][0];
		if (opt == '-')
			break;
		if (x >= sizeof(dirs))
		{
			realloc(dirs, sizeof(dirs)*2);
		}
		dirs[z] = (char*)malloc(strlen(argv[x]) + 1);
		strcpy(dirs[z], argv[x]);
		lstat(dirs[z], &statbuf);
		if (!S_ISDIR(statbuf.st_mode))
		{
			printfile(dirs[z], statbuf);
			exit(0);
		}
		totalDirs++;
		x++;
		z++;
	}
	if (totalDirs == 0)
	{
		totalDirs = 1;
		dirs[0] = ".";
	}
	///
	/// After getting directories, get each option and set constants
	///
	while (x < argc)
	{
		opt = argv[x][0];
		if (opt == '-')
		{
			for (y = 1; y < strlen(argv[x]); y++)
			{
				opt = argv[x][y];
				if (isA)
				{
					switch(opt)
					{
					  case '^':
					  	isNeg = 1;
					  	break;
					  case 'd':
					  	skipD = 1;
					  	if (isNeg)
					  	{
					  		isNeg = 0;
					  		isPerm = NOT_DIRS;
							break;
					  	}
					  	else
					  	{
					  		isPerm = DIRS_ONLY;
							break;
					  	}
					  case 'r':
					  	if (isNeg)
					  	{
					  		isNeg = 0;
					  		isPerm = NOT_RD_ONLY;
							break;
					  	}
					  	else
					  	{
					  		isPerm = RD_ONLY;
							break;
					  	}
					}
				}
				if (isO)
				{
					
					switch(opt)
					{
					  case '^':
					  	isReverse = 1;
					  	break;
					  case 'n':
					  	isSort = SORT_N;
						break;
					  case 's':
					  	skipS = 1;
					  	isSort = SORT_S;
						break;
					  case 'e':
					  	isSort = SORT_E;
						break;
					  case 'd':
					  	skipD = 1;
					  	isSort = SORT_D;
						break;
					  case 'g':
					  	isSort = SORT_G;
						break;
					}
				}
				if (isT && !skipT)
				{
					switch(opt)
					{
					  case 'c':
					  	skipC = 1;
					  	isTimeField = TIME_C;
						break;
					  case 'a':
					  	skipA = 1;
					  	isTimeField = TIME_A;
						break;
					  case 'w':
					  	skipW = 1;
					  	isTimeField = TIME_W;
						break;
					}
					skipT = 1;	
				}
				switch(opt)
				{
				  case 'a':
				  	if (!skipA)
				  	  isA = 1;
				  	else
				  	  skipA = 0;
				  	break;
				  case 'o':
				  	isO = 1;
				  	break;
				  case 't':
				  	isT = 1;
				  	break;
				  case 'b':
				  	isB = 1;
				  	break;
				  case 'c':
				  	if (!skipC)
				  	{
					  	if (isNeg)
					  	{
					  	  isNeg = 0;
					  	  isC = 0;
					  	}
					  	else
					  	  isC = 1;
					}
					else
					  skipC = 0;
				  	break;
				  case 'd':
				  	if (!skipD)
				  	  isD = 1;
				  	else
				  	  skipD = 0;
				  	break;
				  case 'q':
				  	if (isNeg)
				  	{
				  	  isNeg = 0;
				  	  isQ = 0;
				  	}
				  	else
				  	  isQ = 1;
				  	break;
				  case 's':
				  	if (!skipS)
				  	  isS = 1;
				  	else
				  	  skipS = 0;
				  	break;
				  case 'w':
				  	if (!skipW)
				  	  isW = 1;
				  	else
				  	  skipW = 0;
				  	break;
				  case '4':
				  	if (isNeg)
				  	{
				  	  is4 = 0;
				  	  isNeg = 0;
				  	}
				  	else
				  	  is4 = 1;
				  	break;
				  case '^':
				  	isNeg = 1;
				  	break;
				  //default:
				  //	printf("Invalid switch - \"%c\".\n", opt);
				  //	exit(0);
				  //	break;			  					
				}
			}
		}
		x++;		
	}
	
}

void printfile(char* string, struct stat statbuf)
{
	printcwd(".");
	char* bytes;
        char* bytesFree;
        char* fileSize;
	struct tm     *tm;
	tm = localtime(&statbuf.st_atime);
	char          datestring[256];
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
		          		fileSize = separator((intmax_t)statbuf.st_size);
		          		printf(" %9js ", fileSize);
		          	}
		          	else
		          	{
		          		printf(" %9jd ", (intmax_t)statbuf.st_size);
		          	}
		          	printf("%s\n", string);
			if (isC)
		        {
		        	bytes = separator(numBytes);
		        	bytesFree = separator(numBytesFree);
		        	printf("%15d File(s)%15s bytes\n", numFiles, bytes);
		        	printf("%15d Dir(s)%15s  bytes free\n", numDirs, bytesFree);
		        }
		        else
		        {
		                printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
		        	printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);
		        }
		        printf("\n");
}

///
/// Function that does all the printing work
/// as well as formats according to the command line options
///
void printdir()
{	
     	int x = 0;
	int l;
	int m;
	int readonly;
	char* bytes;
        char* bytesFree;
        char* fileSize;
        char* mode;
        char* string = (char*)malloc(1);
	DIR *dirp;
	struct stat statbuf;
	struct dirent **bufarray; 
	while (x < totalDirs)
	{
		///
		/// Determine the sorting method
		///
		printcwd(dirs[x]);
		if (isSort == 8)
		{
			bufarray = alphabetsort(dirp, dirs[x]);
		}
		else if (isO == 1 && isSort == 9)
		{
			bufarray = sizesort(dirp, dirs[x]);
		}
		else if (isO == 1 && isSort == 10)
		{
			bufarray = extsort(dirp, dirs[x]);
		}
		else if (isO == 1 && isSort == 11)
		{
			bufarray = timesort(dirp, dirs[x]);
		}
		else if (isO == 1 && isSort == 12)
		{
			bufarray = dirsort(dirp, dirs[x]);
		}
	        struct passwd *pwd;
		struct group  *grp;
		struct tm     *tm;
		char          datestring[256];

		if (!((isSort == 9 || isSort == 10)&& isReverse))
		{
		l = 0;
		while (l < i)
		{
			errno = 0;
			string = bufarray[l]->d_name;
		        status = lstat(string, &statbuf);
		        if (status == -1)
		        {
		        	l++;
		        	continue;
		        }
		          ///
		          ///get the correct time as specified in the command line
		          ///	
		          if (isTimeField == TIME_W)
		          {
		          	tm = localtime(&statbuf.st_mtime);
		          }
		          else if (isTimeField == TIME_C)
		          {
		          	tm = localtime(&statbuf.st_ctime);
		          }
		          else if (isTimeField == TIME_A)
		          {
		          	tm = localtime(&statbuf.st_atime);
		          }
		          
		          mode = (char*) malloc(20);
	   		  sprintf(mode, "%o", statbuf.st_mode);
	   		  readonly = ((mode[strlen(mode) - 1] == '4') && (mode[strlen(mode) - 2] == '4') && (mode[strlen(mode) - 3] == '4'));
		          //symbolic links
			  if (S_ISLNK(statbuf.st_mode) && ((isPerm == PERM_ALL) || (isPerm == PERM_NONE) || (isPerm == NOT_DIRS) || (isPerm == NOT_RD_ONLY)))
		          {
		          	fileFound = 1;
		          	numFiles++;
		          	numBytes += (intmax_t)statbuf.st_size;
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
		          //directory only 
		          //print out the files according to attributes from command line and file types
		          else if (S_ISDIR(statbuf.st_mode) && ((isPerm == DIRS_RD) || (isPerm == DIRS_ONLY) || (isPerm == PERM_ALL) || (isPerm == NOT_RD_ONLY))) 
			  {
			  	fileFound = 1;
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
		          //read only files
		          else if (S_ISREG(statbuf.st_mode) &&  (((isPerm == RD_ONLY || isPerm == DIRS_RD || isPerm == NOT_DIRS) && readonly)) || (isPerm == PERM_ALL))
		          {
		          	fileFound = 1;
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
		          		fileSize = separator((intmax_t)statbuf.st_size);
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
		          else if (S_ISREG(statbuf.st_mode) && (((isPerm == PERM_NONE) && !readonly) || (isPerm == PERM_ALL) || ((isPerm == NOT_RD_ONLY) && !readonly) || (isPerm == NOT_DIRS)))
		          {
		          	fileFound = 1;
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
		          		fileSize = separator((intmax_t)statbuf.st_size);
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
		          l++;
	
	        }
	        }
		else if (isReverse && (isSort == 9 || isSort == 10))
		{
		l = i-1;
		while (l > 0)
		{
			errno = 0;
			string = bufarray[l]->d_name;
		        status = lstat(string, &statbuf);
		        
		          //get the correct time as specified in the command line	
		          if (isTimeField == TIME_W)
		          {
		          	tm = localtime(&statbuf.st_mtime);
		          }
		          else if (isTimeField == TIME_C)
		          {
		          	tm = localtime(&statbuf.st_ctime);
		          }
		          else if (isTimeField == TIME_A)
		          {
		          	tm = localtime(&statbuf.st_atime);
		          }
		          
		          mode = (char*) malloc(20);
	   		  sprintf(mode, "%o", statbuf.st_mode);
	   		  readonly = ((mode[strlen(mode) - 1] == '4') && (mode[strlen(mode) - 2] == '4') && (mode[strlen(mode) - 3] == '4'));
	   		  ///
		          ///symbolic links
		          ///
			  if (S_ISLNK(statbuf.st_mode) && ((isPerm == PERM_ALL) || (isPerm == PERM_NONE) || (isPerm == NOT_DIRS) || (isPerm == NOT_RD_ONLY)))
		          {
		          	numFiles++;
		          	numBytes += (intmax_t)statbuf.st_size;
		          	fileFound = 1;
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
		          ///
		          ///directories only 
		          ///print out the files according to attributes from command line and file types
		          else if (S_ISDIR(statbuf.st_mode) && ((isPerm == DIRS_RD) || (isPerm == DIRS_ONLY) || (isPerm == PERM_ALL) || (isPerm == NOT_RD_ONLY))) 
			  {
			  	fileFound = 1;
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
		          ///
		          ///read only files
		          ///
		          else if (S_ISREG(statbuf.st_mode) &&  (((isPerm == RD_ONLY || isPerm == DIRS_RD || isPerm == NOT_DIRS) && readonly)) || (isPerm == PERM_ALL))
		          {
		          	fileFound = 1;
		          	///
		         	///default year is 4 digits, if it was unset then print only last 2
		         	///
			        if (is4)
			        {
			        	strftime(datestring, sizeof(datestring), "%m/%d/%Y  %I:%M %p", tm);
			        }
			        else
			        {
			        	strftime(datestring, sizeof(datestring), "%m/%d/%y  %I:%M %p", tm);
			        }
			        ///
			        ///print the date
			        ///
			        printf(" %s", datestring);
		          	printf("         ");
		          	numFiles++;
		          	numBytes += (intmax_t)statbuf.st_size;
		          	/* Print size of file. */
		          	if (isC)
		          	{
		          		fileSize = separator((intmax_t)statbuf.st_size);
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
		          else if (S_ISREG(statbuf.st_mode) && (((isPerm == PERM_NONE) && !readonly) || (isPerm == PERM_ALL) || ((isPerm == NOT_RD_ONLY) && !readonly) || (isPerm == NOT_DIRS)))
		          {
		          	fileFound = 1;
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
		          		fileSize = separator((intmax_t)statbuf.st_size);
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
		          l--;
	
	        }
	        }
	        if (fileFound == 0)
	        {
	        	printf("File Not Found\n\n");
	        }
	        else
	        {
		        if (isC)
		        {
		        	bytes = separator(numBytes);
		        	bytesFree = separator(numBytesFree);
		        	printf("%15d File(s)%15s bytes\n", numFiles, bytes);
		        	printf("%15d Dir(s)%15s  bytes free\n", numDirs, bytesFree);
		        }
		        else
		        {
		                printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
		        	printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);
		        }
		        printf("\n");
		}
	        
		numBytes = 0;
		numBytesFree = 0;
		numFiles = 0;
		numDirs = 0;
		x++;
	}
}
///     
///prints the current working directory
///
/**
 * @param path The path to get and print out
 */
int printcwd(char* path)
{
	char *cwd;
        if ((cwd = getcwd(NULL, 64)) == NULL) 
        {
             perror("pwd");
             exit(2);
        }
        if (firstTime)
        {
        	homeDir = cwd;
        	firstTime = 0;
        }
        chdir(path);
        cwd = getcwd(NULL, 64);
        (void)printf("  Directory of %s\n", cwd);
        printf("\n");
        chdir(homeDir);
        free(cwd); /* free memory allocated by getcwd() */
        return(0);
}
///
///display with no heading or summary
///
void bare()
{
	int x = 0;
	int j = 0;
	DIR* dirp;
	while (x < totalDirs)
	{
		struct dirent** bufarray = alphabetsort(dirp, dirs[x]);
		while (j < i)
		{
			printf("%s\n", bufarray[j]->d_name);
			j++;
		}
		printf("\n");
		x++;
	}
}
///
///puts commas after every 3 digits, for thousands
///
/**
 * @param input The int to convert into a comma string
 * @return A string of the converted integer
 */
char* separator(int input)
{
	int mult = 3;
	int check = mult;
	int inc = 1;
	int i;
	char* string = (char*) malloc(20);
   	sprintf(string, "%d", input);
   	int start = strlen(string);
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
///
///recurse through subdirectories and print out contents of each
///
void subdirs()
{
	char* bytes;
	char* bytesFree;
	prevdir = getcwd(NULL, 64);
	if (isW && isB)
	{
	}
	else if (isW)
	{
		printcwd(".");
		nftw(".", widevisit, 10, 0);
		printf("\n      Total Files Listed:");
		
		if (isC)
		{
		        	bytes = separator(totalBytes);
		        	bytesFree = separator(totalBytesFree);
		        	printf("\n%15d File(s)%15s bytes\n", totalFiles, bytes);
		        		printf("%15d Dir(s)%15s  bytes free\n", alltotalDirs, bytesFree);
		}
		else
		{
		                printf("\n%15d File(s)%15d bytes\n", totalFiles, totalBytes);
		        		printf("%15d Dir(s)%15d  bytes free\n", alltotalDirs, totalBytesFree);
		}
		printf("\n");			
	}
	else if (isB)
	{
		nftw(".", visitbare, 10, 0);
	}
	else	
	{
		printcwd(".");
		if (nftw(".", visit, 10, FTW_CHDIR ) != 0)
			perror("nftw");
		printf("\n      Total Files Listed:");
		
		if (isC)
		{
		        	bytes = separator(totalBytes);
		        	bytesFree = separator(totalBytesFree);
		        	printf("\n%15d File(s)%15s bytes\n", totalFiles, bytes);
		        		printf("%15d Dir(s)%15s  bytes free\n", alltotalDirs, bytesFree);
		}
		else
		{
		                printf("\n%15d File(s)%15d bytes\n", totalFiles, totalBytes);
		        		printf("%15d Dir(s)%15d  bytes free\n", alltotalDirs, totalBytesFree);
		}
		printf("\n");
		strcpy(prevdir, homeDir);		
	}
}
///
/// Visit with wide display function for recursive function
///
/**
* @param path The pathname supplied
* @param statbuf the stat struct holding file info
* @param flags optional flag int
* @param ftw pointer to ftw struct
*/
int widevisit(const char* path,  const struct stat* statbuf, int flags, struct FTW* ftw)
{
	char* bytes;
	char* bytesFree;
	char* cwd;
	int x = 0;
	//int for longest string length, for indenting
	//(next file name begins 3 spaces after longest name)
	int line = 0;
	int linemax = 80;
	int words = 0;
	int wordMax;
	int firstTime;
	int l;
	int m;
	DIR *dirp;
	printcwd(".");

		//struct dirent** bufarray = alphabetsort(dirp, dirs[x]);
		//iterate through the array and get the length of the longest file name
		l = 0;
			if ((cwd = getcwd(NULL, 64)) == NULL) 
			{
			     perror("pwd");
			     exit(2);
			}
			if (firstTime)
			{
				homeDir = cwd;
				firstTime = 0;
			}
			if (strcmp(prevdir, cwd) != 0)
			{
				if (isC)
			        {
			        	bytes = separator(numBytes);
			        	bytesFree = separator(numBytesFree);
			        	printf("%15d File(s)%15s bytes\n", numFiles, bytes);
			        
			        	printf("\n");
			        }
			        else
			        {
			                printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
			        	
			        	printf("\n");
			        }
				(void)printf("  Directory of %s\n", cwd);
				printf("\n");
				strcpy(prevdir, cwd);
				numBytes = 0;
				numBytesFree = 0;
				numFiles = 0;
				numDirs = 0;
			}
			free(cwd); /* free memory allocated by getcwd() */
		
				longest = strlen(path);
		l = 0;
			errno = 0;
		       
		        
		        if (S_ISDIR(statbuf->st_mode))
			{
				line += 2 + strlen(path);
				if (line > linemax || words >= wordMax)
				{
					printf("\n[%s]", path);
					
					linemax = line - (2 + strlen(path));
					if (firstTime)
					{
						wordMax = words;
						firstTime = 0;
					}
					words = 1;
					line = 0;
				}
				else
				{
			        	printf("[%s]", path);
			        	words++;
			        }
			        if (strlen(path) == longest-1)
			        {
			        	printf("  ");
			        }
			        else
			        {
				        for (m = 0; m < (longest - 2) - strlen(path); m++)
				        {
				        	line += 1;
				        	printf(" ");
				        }
				        	printf("   ");
				}
			        numDirs++;
			        numBytesFree += (intmax_t)statbuf->st_size;
			          	
			}
			else
			{
			       	numFiles++;
			       	numBytes += (intmax_t)statbuf->st_size;
			        line += strlen(path);
				if (line > linemax || words >= wordMax)
				{
					printf("\n%s", path);
					if (firstTime)
					{
						wordMax = words;
						firstTime = 0;
					}
					words = 1;
					line = 0;
				}
				else
				{
			        	printf("%s", path);
			        	words++;
			        }
			        if (strlen(path) == longest)
			        {
			        	printf("   ");
			        }
			        else
			        {
				        for (m = 0; m < longest - strlen(path); m++)
				        {
				        	line += 1;
				        	if (line > linemax)
				        	{
				        		line = 0;
				        		break;
				        	}
				        	printf(" ");
				        }
				        	printf("   ");
			        }
			}
			l++;	        	
		
		if (errno != 0)
	        	perror("error reading directory");
	
	        i = 0;
        return 0;	
}
///
/// Visit function for bare recursive function
///
int visitbare(const char* path,  const struct stat* statbuf, int flags, struct FTW* ftw)
{
	printf("%s\n", path);
	return 0;
}
///
/// Visit function for recursive function
///
/**
* @param string The pathname supplied
* @param statbuf the stat struct holding file info
* @param flags optional flag int
* @param ftw pointer to ftw struct
*/
int visit(const char* string,  const struct stat* statbuf, int flags, struct FTW* ftw)
{
	int l;
	int m;
	int j;
	int k;
	int readonly;
	char* bytes;
        char* bytesFree;
        char* fileSize;
        char* mode;
        char *cwd;
	DIR *dirp;
	struct dirent **bufarray; 
	
			if ((cwd = getcwd(NULL, 64)) == NULL) 
			{
			     perror("pwd");
			     exit(2);
			}
			if (firstTime)
			{
				homeDir = cwd;
				firstTime = 0;
			}
			if (strcmp(prevdir, cwd) != 0 && strcmp(cwd, homeDir) != 0)
			{
				if (isC)
			        {
			        	bytes = separator(numBytes);
			        	bytesFree = separator(numBytesFree);
			        	printf("%15d File(s)%15s bytes\n", numFiles, bytes);
			        	printf("\n");
			        }
			        else
			        {
			                printf("%15d File(s)%15d bytes\n", numFiles, numBytes);
			        	printf("\n");
			        }
				(void)printf("  Directory of %s\n", cwd);
				printf("\n");
				strcpy(prevdir, cwd);
				numBytes = 0;
				numBytesFree = 0;
				numFiles = 0;
				numDirs = 0;
			}
				free(cwd); /* free memory allocated by getcwd() */
	        
	        struct dirent *dp;
	        struct passwd *pwd;
		struct group  *grp;
		struct tm     *tm;
		char          datestring[256];
	        
	        
		l = 0;
		        
		          //get the correct time as specified in the command line	
		          if (isTimeField == TIME_W)
		          {
		          	tm = localtime(&statbuf->st_mtime);
		          }
		          else if (isTimeField == TIME_C)
		          {
		          	tm = localtime(&statbuf->st_ctime);
		          }
		          else if (isTimeField == TIME_A)
		          {
		          	tm = localtime(&statbuf->st_atime);
		          }
		          
		          mode = (char*) malloc(20);
	   		  sprintf(mode, "%o", statbuf->st_mode);
	   		  readonly = ((mode[strlen(mode) - 1] == '4') && (mode[strlen(mode) - 2] == '4') && (mode[strlen(mode) - 3] == '4'));
		          //symbolic links
			  if (S_ISLNK(statbuf->st_mode) && ((isPerm == PERM_ALL) || (isPerm == PERM_NONE) || (isPerm == NOT_DIRS) || (isPerm == NOT_RD_ONLY)))
		          {
		          	fileFound = 1;
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
		          		if ((pwd = getpwuid(statbuf->st_uid)) != NULL)
		          		   printf(" %s  ", pwd->pw_name);
		          	}	
		          	printf("%s\n", bufarray[l]->d_name);
		          }
		          //directory only 
		          //print out the files according to attributes from command line and file types
		          else if (S_ISDIR(statbuf->st_mode) && ((isPerm == DIRS_RD) || (isPerm == DIRS_ONLY) || (isPerm == PERM_ALL) || (isPerm == NOT_RD_ONLY))) 
			  {
			  	fileFound = 1;
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
		          		if ((pwd = getpwuid(statbuf->st_uid)) != NULL)
		          		   printf(" %s  ", pwd->pw_name);
		          	}	
		          	printf("%s\n", string);
		          	numDirs++;
		          	alltotalDirs++;
		          	totalBytesFree += (intmax_t)statbuf->st_size;
		          	numBytesFree += (intmax_t)statbuf->st_size;
		          	
		          }
		          //read only files
		          else if (S_ISREG(statbuf->st_mode) &&  (((isPerm == RD_ONLY || isPerm == DIRS_RD || isPerm == NOT_DIRS) && readonly)) || (isPerm == PERM_ALL))
		          {
		          	fileFound = 1;
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
		          	totalFiles++;
		          	numBytes += (intmax_t)statbuf->st_size;
		          	totalBytes += (intmax_t)statbuf->st_size;
		          	/* Print size of file. */
		          	if (isC)
		          	{
		          		fileSize = separator(statbuf->st_size);
		          		printf(" %9js ", fileSize);
		          	}
		          	else
		          	{
		          		printf(" %9jd ", statbuf->st_size);
		          	}
		          	if (isQ)
		          	{
		          		/* Print out owners name if it is found using getpwuid(). */
		          		if ((pwd = getpwuid(statbuf->st_uid)) != NULL)
		          		   printf(" %s  ", pwd->pw_name);
		          	}	
		          	printf("%s\n", string);
		          }
		          else if (S_ISREG(statbuf->st_mode) && (((isPerm == PERM_NONE) && !readonly) || (isPerm == PERM_ALL) || ((isPerm == NOT_RD_ONLY) && !readonly) || (isPerm == NOT_DIRS)))
		          {
		          	fileFound = 1;
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
		          	totalFiles++;
		          	numBytes += (intmax_t)statbuf->st_size;
		          	totalBytes += (intmax_t)statbuf->st_size;
		          	/* Print size of file. */
		          	if (isC)
		          	{
		          		fileSize = separator(statbuf->st_size);
		          		printf(" %9js ", fileSize);
		          	}
		          	else
		          	{
		          		printf(" %9jd ", statbuf->st_size);
		          	}
		          	if (isQ)
		          	{
		          		/* Print out owners name if it is found using getpwuid(). */
		          		if ((pwd = getpwuid(statbuf->st_uid)) != NULL)
		          		   printf(" %s  ", pwd->pw_name);
		          	}	
		          	printf("%s\n", string);
		          }
		          l++;
	
	        if (fileFound == 0)
	        {
	        	printf("File Not Found\n\n");
	        }
		return 0;
}
///
///Print function sideways to a max width of 80
///
void wide()
{
	char* bytes;
	char* bytesFree;
	int x = 0;
	//int for longest string length, for indenting
	//(next file name begins 3 spaces after longest name)
	int longest = 0;
	int line = 0;
	int linemax = 80;
	int words = 0;
	int wordMax;
	int firstTime;
	int l;
	int m;
	DIR *dirp;
	struct stat   statbuf;
	printcwd(".");
	while (x < totalDirs)
	{
		struct dirent** bufarray = alphabetsort(dirp, dirs[x]);
		//iterate through the array and get the length of the longest file name
		l = 0;
		while (l < i)
		{
			if (strlen(bufarray[l]->d_name) > longest)
			{
				longest = strlen(bufarray[l]->d_name);
			}
			wordMax++;
			l++;
		}
		
		l = 0;
		while (l < i)
		{
			errno = 0;
		        status = lstat(bufarray[l]->d_name, &statbuf);
		        if (status == -1)
		        {
		        	l++;
		        	continue;
		        }
		        
		        if (S_ISDIR(statbuf.st_mode))
			{
				line += 2 + strlen(bufarray[l]->d_name);
				if (line > linemax || words >= wordMax)
				{
					printf("\n[%s]", bufarray[l]->d_name);
					
					linemax = line - (2 + strlen(bufarray[l]->d_name));
					if (firstTime)
					{
						wordMax = words;
						firstTime = 0;
					}
					words = 1;
					line = 0;
				}
				else
				{
			        	printf("[%s]", bufarray[l]->d_name);
			        	words++;
			        }
			        if (strlen(bufarray[l]->d_name) == longest-1)
			        {
			        	printf("  ");
			        }
			        else
			        {
				        for (m = 0; m < (longest - 2) - strlen(bufarray[l]->d_name); m++)
				        {
				        	line += 1;
				        	printf(" ");
				        }
				        	printf("   ");
				}
			        numDirs++;
			        numBytesFree += (intmax_t)statbuf.st_size;
			          	
			}
			else
			{
				numFiles++;
				numBytes += (intmax_t)statbuf.st_size;
			        line += strlen(bufarray[l]->d_name);
				if (line > linemax || words >= wordMax)
				{
					printf("\n%s", bufarray[l]->d_name);
					if (firstTime)
					{
						wordMax = words;
						firstTime = 0;
					}
					words = 1;
					line = 0;
				}
				else
				{
			        	printf("%s", bufarray[l]->d_name);
			        	words++;
			        }
			        if (strlen(bufarray[l]->d_name) == longest)
			        {
			        	printf("   ");
			        }
			        else
			        {
				        for (m = 0; m < longest - strlen(bufarray[l]->d_name); m++)
				        {
				        	line += 1;
				        	if (line > linemax)
				        	{
				        		line = 0;
				        		break;
				        	}
				        	printf(" ");
				        }
				        	printf("   ");
			        }
			}
			l++;	        	
		}
		if (isC)
		{
		        	bytes = separator(numBytes);
		        	bytesFree = separator(numBytesFree);
		        	printf("\n%15d File(s)%15s bytes\n", numFiles, bytes);
		        		printf("%15d Dir(s)%15s  bytes free\n", numDirs, bytesFree);
		}
		else
		{
		                printf("\n%15d File(s)%15d bytes\n", numFiles, numBytes);
		        		printf("%15d Dir(s)%15d  bytes free\n", numDirs, numBytesFree);
		}
		printf("\n");		
		
		if (errno != 0)
	        	perror("error reading directory");
	
	        i = 0;
	        numBytes = 0;
		numBytesFree = 0;
		numFiles = 0;
		numDirs = 0;
	        x++;
        }
        return;	
}
///
///sorts all files from the current directory by alpha and puts them into an array 
///
/**
 * @param dirp The directory to open
 * @param path String of the pathname to open
 * @return An array of dirents of all the elements in the directory
 */
struct dirent** alphabetsort(DIR* dirp, char* path)
{

	//iterator for the bufarray
	i = 0;
	int j;
	int k;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
	
        if ((dirp = opendir(path)) == NULL) 
        {
        	printf("couldn't open path: ");
        	printf("%s\n", path);
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
  		{
  			if (isReverse)
  			{
	    			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) > 0) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k];
	      			   bufarray[k] = bufarray[k+1];
	      			   bufarray[k+1] = tmp;
	  			}
	  		}
	  		else
	  		{
	  		
	  			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) < 0) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k];
	      			   bufarray[k] = bufarray[k+1];
	      			   bufarray[k+1] = tmp;
	  			}
	  		}
  		}
	}
	return bufarray;
}

///
///sorts all files from the current directory by extension and puts them into an array 
///
/**
/**
 * @param dirp The directory to open
 * @param path String of the pathname to open
 * @return An array of dirents of all the elements in the directory
 */
struct dirent** extsort(DIR* dirp, char* path)
{

	//iterator for the bufarray
	i = 0;
	int j;
	int k;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
	
        if ((dirp = opendir(path)) == NULL) 
        {
        	printf("couldn't open path: ");
        	printf("%s\n", path);
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
  		{
	  			if (strcmp(getExt(bufarray[k+1]->d_name), getExt(bufarray[k]->d_name)) < 0) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k];
	      			   bufarray[k] = bufarray[k+1];
	      			   bufarray[k+1] = tmp;
	  			}
  		}
	}
	return bufarray;
}
///
///Function to get the extension of a file
///
/**
 * @param name The name of the file whose extension to return
 * @return A string of the file's extension
 */
char* getExt(char* name)
{
	int len = strlen(name);
	int isExt = 0;
	int i;
	int x = 0;
	for (i = 0; i < len; i++)
	{
		if (name[i] == '.')
		{
			isExt = 1;
			i++;
			break;
		}
	}
	char* ext = (char*)malloc(len-i+1);
	if (isExt)
	{
		while (i < len)
		{
			ext[x] = name[i];
			i++;
			x++;
		}
	}
	return ext;
}


///
///sorts all files from the current directory by size and puts them into an array 
///
///If a dir, print it out. If a file, make an array of them, sort by size by inserting it into the correct place
/**
 * @param dirp The directory to open
 * @param path String of the pathname to open
 * @return An array of dirents of all the elements in the directory
 */
struct dirent** sizesort(DIR* dirp, char* path)
{

	//iterator for the bufarray
	i = 0;
	int j;
	int k;
	int size;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
        struct stat   statbuf;
        struct stat   statbuf2;
	
        if ((dirp = opendir(path)) == NULL) 
        {
        	perror("couldn't open path");
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
  		{
  			lstat(bufarray[k+1]->d_name, &statbuf);
  			lstat(bufarray[k]->d_name, &statbuf2);

	    			if ((intmax_t)statbuf.st_size < (intmax_t)statbuf2.st_size) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k];
	      			   bufarray[k] = bufarray[k+1];
	      			   bufarray[k+1] = tmp;
	  			}
  		}
	}
	//bubble sort again to move dirs and sym links to the front
	for (j = 0; j< i; j++) 
	{
  		for (k = 0; k < i - 1 - j; k++)
  		{
  			lstat(bufarray[k+1]->d_name, &statbuf);
  			
    			if (S_ISDIR(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) 
    			{  	
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
  		}
	}
	return bufarray;
}
///
///sorts all files from the current directory by 
///directories first and puts them into an array 
///
/**
 * @param dirp The directory to open
 * @param path String of the pathname to open
 * @return An array of dirents of all the elements in the directory
 */
struct dirent** dirsort(DIR* dirp, char* path)
{
	//iterator for the bufarray
	i = 0;
	int j;
	int k;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
        struct stat   statbuf;
        struct stat   statbuf2;
	
        if ((dirp = opendir(path)) == NULL) 
        {
        	perror("couldn't open path");
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
  		{
  			if (isReverse)
  			{
	    			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) > 0) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k];
	      			   bufarray[k] = bufarray[k+1];
	      			   bufarray[k+1] = tmp;
	  			}
	  		}
	  		else
	  		{
	  		
	  			if (strcmp(bufarray[k+1]->d_name, bufarray[k]->d_name) < 0) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k];
	      			   bufarray[k] = bufarray[k+1];
	      			   bufarray[k+1] = tmp;
	  			}
	  		}
  		}
	}
	if (isReverse)
	{
		for (j = i-1; j >= 0; j--) 
		{
	  		for (k = 0; k < i - 1 - j; k++)
	  		{
	  			lstat(bufarray[k]->d_name, &statbuf);
	  			
	    			if (S_ISDIR(statbuf.st_mode)) 
	    			{  	
	    			   //compare and swap
	      			   struct dirent *tmp = bufarray[k+1];
	      			   bufarray[k+1] = bufarray[k];
	      			   bufarray[k] = tmp;
	  			}
	  		}
		}	
	}
	else
	{	
	//bubble sort again to move dirs and sym links to the front
	for (j = 0; j< i; j++) 
	{
  		for (k = 0; k < i - 1 - j; k++)
  		{
  			lstat(bufarray[k+1]->d_name, &statbuf);
  			
    			if (S_ISDIR(statbuf.st_mode)) 
    			{  	
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
  		}
	}
	}
	return bufarray;
}

///
///sorts all files from the current directory by time and puts them into an array 
///
/**
 * @param dirp The directory to open
 * @param path String of the pathname to open
 * @return An array of dirents of all the elements in the directory
 */
struct dirent** timesort(DIR* dirp, char* path)
{
	struct tm     *tm;
	struct tm     *tm2;
	//iterator for the bufarray
	i = 0;
	int j;
	int k;
	struct dirent **bufarray = (struct dirent**) malloc(sizeof(struct dirent));
        struct dirent *dp;
        struct stat   statbuf;
        struct stat   statbuf2;
        int date1;
        int date2;
	
        if ((dirp = opendir(path)) == NULL) 
        {
        	perror("couldn't open path ");
        	printf("%s", path);
                return;
        }
        
        //put files into array
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
  		{
  			lstat(bufarray[k+1]->d_name, &statbuf);
  			lstat(bufarray[k]->d_name, &statbuf2);
  			  if (isTimeField == TIME_W)
		          {
		          	tm = localtime(&statbuf.st_mtime);
		          	tm2 = localtime(&statbuf2.st_mtime);
		          }
		          else if (isTimeField == TIME_C)
		          {
		          	tm = localtime(&statbuf.st_ctime);
		          	tm2 = localtime(&statbuf2.st_ctime);
		          }
		          else
		          {
		          	tm = localtime(&statbuf.st_atime);
		          	tm2 = localtime(&statbuf2.st_atime);
		          }
		 	date1 = tm->tm_year + tm->tm_mon + tm->tm_mday + tm->tm_hour + tm->tm_min + tm->tm_sec;
		 	date2 = tm2->tm_year + tm2->tm_mon + tm2->tm_mday + tm2->tm_hour + tm2->tm_min + tm2->tm_sec;
    			if (date1 < date2) 
    			{  	
    			   //compare and swap
      			   struct dirent *tmp = bufarray[k];
      			   bufarray[k] = bufarray[k+1];
      			   bufarray[k+1] = tmp;
  			}
  		}
	}
	return bufarray;
}

///
///Function to compare date and time 
///
/**
 * @param *tm The first time to compare
 * @param *tm2 The second time to compare
 * @return 1 if greater, -1 if less, or zero if equal
 */
int comparedate(struct tm *tm, struct tm *tm2)
{
        char* datestring = (char*)malloc(256);
        char* datestring2 = (char*)malloc(256);
        char* timestring = (char*)malloc(256);
        char* timestring2 = (char*)malloc(256);
        int day1;
        int day2;
        int time1;
        int time2;
	strftime(datestring, sizeof(datestring), "%Y%m%d", tm);
	strftime(datestring2, sizeof(datestring2), "%Y%m%d", tm2);
	day1 = atoi(datestring);
	day2 = atoi(datestring2);
	
	strftime(timestring, sizeof(timestring), "%k%M", tm);
	strftime(timestring2, sizeof(timestring2), "%k%M", tm2);
	time1 = atoi(timestring);
	time2 = atoi(timestring2);
	if (day1 < day2)
	{
		return -1;
	}
	else if (day1 > day2)
	{
		return 1;
	}
	else if (day1 == day2)
	{
		if (time1 < time2)
		{
			return -1;
		}
		else if (time1 > time2)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
	
}



