#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define SUCCESS 0
#define FAILURE -1


int fsize(FILE *f){
	int prev=ftell(f);
	fseek(f, 0L, SEEK_END);
	int size=ftell(f);
	fseek(f,prev,SEEK_SET); //go back to where we were
	return size;
}

int main (int argc , char **argv){
	typedef int (*func_t)(int);
	func_t func;
	void *data;
	FILE *f = NULL;
	int size = 0;

	f = fopen(argv[1],"rb");

	if (!f){
		return FAILURE;
	}
	size = fsize(f);

	//allocating memory
	data = mmap(NULL,size,PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (data == (void *) -1){
		return FAILURE;
	}

	//filling memory with nops
	memset(data,0x90,size);

	fread (data,1,size,f);
	fclose(f);

	printf("%p \n",data);
	//position where data resides in loaded stream
	func = (func_t)data+0x400;

	//running the function
	printf("%d\n",func(13));

	munmap(data,size);

	return SUCCESS;
}

