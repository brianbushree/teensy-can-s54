#include <stdint.h>
#include <iostream>
#include "../nextion/Nextion.h"

int main(int argc, char const *argv[]) {

  char keyOne[5] = "test"; // declare one extra char
  uint8_t valueOne = (uint8_t) 'f';

  NextionVariable variables[1] = {
    NextionVariable{
      (char*)&keyOne, &valueOne
    }
  };
  NextionPage pages[1] = {
    NextionPage{
      variables,
      (sizeof(variables)/sizeof(*variables)),
      0
    }
  };

  NextionState state = NextionState{
    pages,
    (sizeof(pages)/sizeof(*pages))
  };

  // print each page sequentially
  for (int page = 0; page < state.pagesLength; page++) {
    std::cout << "page " << page << " {" << std::endl;
    for (int var; var < state.pages[page].variablesLength; var++) {

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