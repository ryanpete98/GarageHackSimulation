
// De Bruijn pattern generator
// Jeffrey Crowell

#ifndef DE_BRUIJN_H_
#define DE_BRUIJN_H_

#include <stdlib.h>
#include <string.h>
char* peda_charset;
void db(int t, int p, int n, int maxlen, int k, int* a, char* sequence,
        char* charset);
char* de_bruijn(char* charset, int n, int maxlen, int* a, char* sequence);
/*
char* cyclic_pattern(int size, int start, char* charset);
char* cyclic_pattern_long();
int cyclic_pattern_offset(unsigned long long value);
*/

#endif
