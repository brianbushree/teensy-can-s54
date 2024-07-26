#include <stdint.h>
#include <iostream>
#include "../Nextion.h"

class TestValues {

  public:
  uint8_t valueOne() {
    return (uint8_t) 'f';
  }

  uint8_t valueTwo() {
    return (uint8_t) 0x01;
  }

  uint8_t valueThree() {
    return (uint8_t) 0;
  }

  uint8_t valueFour() {
    return (uint8_t) 420;
  }
};

TestValues test = TestValues();

int main(int argc, char const *argv[]) {

  char keyOne[5] = "test"; // declare one extra char

  char keyTwo[5] = "blah"; // declare one extra char

  char keyThree[4] = "foo"; // declare one extra char

  char keyFour[4] = "bar"; // declare one extra char

  NextionVariable variables[] = {
    {
      (char*)&keyOne, [] { return test.valueOne(); },
    },
    {
      (char*)&keyTwo, [] { return test.valueTwo(); },
    }
  };

  NextionVariable variablesTwo[] = {
    {
      (char*)&keyThree, [] { return test.valueThree(); },
    },
    {
      (char*)&keyFour, [] { return test.valueThree(); },
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
      std::cout << std::hex << static_cast<int>(variable.value()) << ", ";

      // use three 0xff to deliminate input
      std::cout << 0xff << ", ";
      std::cout << 0xff << ", ";
      std::cout << 0xff;

      std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
  }
}