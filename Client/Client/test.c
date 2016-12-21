#include "stdio.h"

int main(){
	FILE * fp;
	fp = fopen("test.txt","w");
	close(fp);

}