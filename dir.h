/*! \file
*
*Alex Kort, 4-9-06, Program 3, dir.h
*/
#define TIME_C 1
#define TIME_A 2
#define TIME_W 3
#define DIRS_ONLY 4
#define RD_ONLY 5
#define DIRS_RD 6
#define PERM_NONE 7
#define PERM_ALL 13
#define NOT_RD_ONLY 14
#define NOT_DIRS 15
#define SORT_N 8;
#define SORT_S 9;
#define SORT_E 10;
#define SORT_D 11;
#define SORT_G 12;

/**
* prints the specified directory with special formatting
*
*/
int printcwd(char* path);
/**
* prints all directory and file information
* based on command line options
*/
void printdir();
/**
* prints wide format, sideways columns with max width of 80
*/
void wide();
int comparedate(struct tm *tm, struct tm *tm2);
struct dirent** alphabetsort(DIR* dirp, char* path);
struct dirent** sizesort(DIR* dirp, char* path);
struct dirent** timesort(DIR* dirp, char* path);
struct dirent** dirsort(DIR* dirp, char* path);
struct dirent** extsort(DIR* dirp, char* path);
char* getExt(char* name);
void subdirs();
int visit(const char* path,  const struct stat* stat, int flags, struct FTW* ftw);
int visitbare(const char* path,  const struct stat* stat, int flags, struct FTW* ftw);
int widevisit(const char* path,  const struct stat* statbuf, int flags, struct FTW* ftw);
void attributes();
char* separator(int input);
void bare();
void printfile(char* string, struct stat statbuf);
void parse(int argc, char **argv);
