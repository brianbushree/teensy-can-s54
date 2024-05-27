#include <stdint.h>
#include <iostream>
#include "../nextion/Nextion.h"

int main(int argc, char const *argv[]) {

  char keyOne[5] = "test"; // declare one extra char
  uint8_t valueOne = (uint8_t) 'f';

  char keyTwo[5] = "blah"; // declare one extra char
  uint8_t valueTwo = (uint8_t) 0x01;

  char keyThree[4] = "foo"; // declare one extra char
  uint8_t valueThree = (uint8_t) 0;

  char keyFour[4] = "bar"; // declare one extra char
  uint8_t valueFour = (uint8_t) 420;

  NextionVariable variables[] = {
    {
      (char*)&keyOne, &valueOne
    },
    {
      (char*)&keyTwo, &valueTwo
    }
  };

  NextionVariable variablesTwo[] = {
    {
      (char*)&keyThree, &valueThree
    },
    {
      (char*)&keyFour, &valueFour
    }
  };

  NextionPage pages[] = {
    {
      variables, (sizeof(variables)/sizeof(*variables))
    },
    {
      variablesTwo, (sizeof(variablesTwo)/sizeof(*variablesTwo))
    },
  };

  NextionState state = NextionState{
    pages,
    (sizeof(pages)/sizeof(*pages))
  };

  // print each page sequentially
  for (int page = 0; page < state.pagesLength; page++) {
    std::cout << "page " << static_cast<int>(page) << " {" << std::endl;
    for (int var = 0; var < state.pages[page].variablesLength; var++) {

      // print variable
      NextionVariable variable = state.pages[page].variables[var];
      std::cout << "\t[";
      std::cout << variable.key << ", ";
      std::cout << std::hex << static_cast<int>(*variable.value) << ", ";

      // use three 0xff to deliminate input
      std::cout << 0xff << ", ";
      std::cout << 0xff << ", ";
      std::cout << 0xff;

      std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
  }
}