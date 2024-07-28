#include <stdint.h>
#include <iostream>
#include "Nextion.h"
#include "unity.h"

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

void test_nextionPrintValues() {

  char keyOne[] = "test";

  char keyTwo[] = "blah";

  char keyThree[] = "foo";

  char keyFour[] = "bar";

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
  TEST_ASSERT_TRUE(true);
}

void setUp(void) {
}

void tearDown(void) {
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_nextionPrintValues);
  return UNITY_END();
}

int main(void) {
  return runUnityTests();
}
