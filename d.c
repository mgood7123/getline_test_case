#include<stdio.h>
#include<string.h>
#include "getline.h"
#define ps(x)      printf("%s = %s\n", #x, x);

struct lines{
    char stack_lines1[100];
    char stack_lines2[100];
 };

int main(int argc,char *argv[])
{
    struct lines st[20];
    FILE *file1,*file2;
    char * line1;
    int line_count1=0,flag2[10],flag1[10],line_count2=0;
    int i=0,j=0,k=0,n=0,m=0,o=0;
    if (argc < 2) {
    	puts("usage: diff file1 file2");
    	return -1;
    }
    file1 = fopen(argv[1],"r");
    size_t l;
    while (getline(&line1, &l, file1) != -1) {
    	ps(line1);
    }
    free(line1);
    fclose(file1);
    return 0;
}

