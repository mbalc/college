#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Przyklad pracy ze stringami i parametrami wywolania.
// Program wypisuje w kolejnych liniach swoje parametry wywolania.

int main(int argc, char **argv) {
    int i;
    for(i = 0; i < argc; i++) {
        char *paramString = (char*) malloc((strlen(argv[i]) + 1) * sizeof(char));
	strcpy(paramString, argv[i]);
	printf("%s\n", paramString);
	free(paramString);
    }
    return 0;
}

