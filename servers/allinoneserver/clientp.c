#include "vivek.h"

int main(){
	int c;
	while((c =getchar()) != EOF){
		if(isupper(c))
			c=tolower(c);
		if(putchar(c) == EOF)
			printf("System error\n");
		if(c == '\n')
			fflush(stdout);
	}
}