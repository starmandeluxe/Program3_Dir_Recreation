/* Alex Kort, CPE 357, Lab 1, lab1.c */
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
	/*define string, print prompt*/
	char *number;
	printf("Input a number and press the Enter or Return key: ");
	scanf("%s", number);
	/*scan in the input and find the length with the string length function*/
	int length = strlen(number);
	/*initialize the 3rd digit*/
	char third = number[2];
	/*initialize the number added to 100*/
	int add = atoi(number) + 100;
	/*print all info according to java example*/
	printf("\nThe number has %d digits\n", length);
	if (strlen(number) >= 3)
	printf("The third digit of the number is %c", third);
	printf("\nThe number times 100 is %s00", number);
	printf("\nThe number plus 100 is %d\n", add);
}
