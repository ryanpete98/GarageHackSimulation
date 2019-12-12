// De Bruijn pattern generator.
// Jeffrey Crowell

#include "debruijn.h"
//#include <string.h>
char* peda_charset =
    "A%sB$nC-(D;)Ea0Fb1Gc2Hd3Ie4Jf5Kg6Lh7Mi8Nj9OkPlQmRnSoTpUqVrWsXtYuZvwxyz";

// Generate a De Bruijn sequence. Code interpreted from wikipedia.
void db(int t, int p, int n, int maxlen, int k, int* a, char* sequence,
        char* charset) {
  if (strlen(sequence) == maxlen) {
    return;
  }
  if (t > n) {
    if (n % p == 0) {
      int j;
      for (j = 1; j <= p; ++j) {
        sequence[strlen(sequence)] = charset[a[j]];
        if (strlen(sequence) == maxlen) {
          return;
        }
      }
    }
  } else {
    a[t] = a[t - p];
    db(t + 1, p, n, maxlen, k, a, sequence, charset);
    int j;
    for (j = a[t - p] + 1; j < k; ++j) {
      a[t] = j;
      db(t + 1, t, n, maxlen, k, a, sequence, charset);
    }
  }
}

// Generate a De Bruijn sequence. Code interpreted from wikipedia.
// The returned string is malloced, and it is the responsibility of the caller
// to free the memory.
char* de_bruijn(char* charset, int n, int maxlen, int* a, char* sequence) {
  int k = strlen(charset);
  //int* a = malloc(sizeof(int) * k * n);
  //memset(a, 0, sizeof(int) * k * n);
  //char* sequence = malloc(sizeof(char) * maxlen + 1);
  //memset(sequence, 0, sizeof(char) * maxlen + 1);
  db(1, 1, n, maxlen, k, a, sequence, charset);
  //free(a);
  return sequence;
}

// Generate a cyclic pattern of desired size, and charset, return with starting
// offset of start.
// The returned string is malloced, and it is the responsibility of the caller
// to free the memory.
/*
char* cyclic_pattern(int size, int start, char* charset) {
  if (start >= size) {
    return (char*)NULL;
  }
  char* pattern = de_bruijn(charset, 3, size);
  if (start == 0) {
    return pattern;
  } else {
    char* returned_pattern = malloc(sizeof(char) * (size - start) + 1);
    memset(returned_pattern, 0, sizeof(char) * (size - start) + 1);
    strncpy(returned_pattern, pattern + start, size - start);
    free(pattern);
    return returned_pattern;
  }
}*/

// Generate a cyclic pattern of 0x10000 long.
// The returned string is malloced, and it is the responsibility of the caller
// to free the memory.
/*
char* cyclic_pattern_long() { return cyclic_pattern(0x10000, 0, peda_charset); }
*/

// Finds the offset of a given value in a cyclic pattern of an integer.
/*
int cyclic_pattern_offset(unsigned long long value) {
  if (value == 0) {
    return -1;
  }
  char* pattern = cyclic_pattern_long();
  unsigned long long needle_l[2];  // Hold the value as a string.
  needle_l[0] = value;
  needle_l[1] = 0;
  char* needle = (char*)&needle_l;
  char* pch = strstr(pattern, needle);
  int retval = -1;
  if (pch != NULL) {
    retval = (int)(pch - pattern - 1);
  }
  free(pattern);
  return retval;
}
*/
