#ifndef Nextion_H
#define Nextion_H
#include <stdint.h>

typedef struct {
  // Key is Nextion variable ID
  char* key; // null-terminated string
  uint8_t* value; // pointer to value
} NextionVariable;

typedef struct {
  NextionVariable* variables;
  uint8_t variablesLength;

  uint8_t pageId;
} NextionPage;

typedef struct {
  NextionPage* pages;
  uint8_t pagesLength;
} NextionState;

#endif
