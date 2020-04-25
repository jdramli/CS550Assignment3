#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(char argc, char ** argv){
	printf("Testing the dmesg read for scatterplot\n");
	system("dmesg | tail -50 > output.txt");
	printf("The latest 50 kernel log reports are printed to output.txt\n");
	system("cat output.txt");
	return 0;

}
