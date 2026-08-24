#include <stdint.h>
#include <iostream>
#include "Nextion.h"
#include "unity.h"

class TestValues {

  public:
  int16_t valueOne() {
    return (int16_t) 7000; // RPM overflow case
  }

  int16_t valueTwo() {
    return (int16_t) -53; // negative temperature case
  }

  int16_t valueThree() {
    return (int16_t) 402; // max oil temp case
  }

  int16_t valueFour() {
    return (int16_t) 0; // boundary case
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
      (char*)&keyOne, []() -> int16_t { return test.valueOne(); },
    },
    {
      (char*)&keyTwo, []() -> int16_t { return test.valueTwo(); },
    }
  };

  NextionVariable variablesTwo[] = {
    {
      (char*)&keyThree, []() -> int16_t { return test.valueThree(); },
    },
    {
      (char*)&keyFour, []() -> int16_t { return test.valueFour(); },
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
      std::cout << variable.value() << ", ";

      // use three 0xff to deliminate input
      std::cout << 0xff << ", ";
      std::cout << 0xff << ", ";
      std::cout << 0xff;

      std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
  }

  // values above uint8_t range must survive intact (this is the regression
  // the int16_t widening fixes: 7000 used to arrive as 88, -53 as 203)
  TEST_ASSERT_EQUAL_INT16(7000, state.pages[0].variables[0].value());
  TEST_ASSERT_EQUAL_INT16(-53,  state.pages[0].variables[1].value());
  TEST_ASSERT_EQUAL_INT16(402,  state.pages[1].variables[0].value());
  TEST_ASSERT_EQUAL_INT16(0,    state.pages[1].variables[1].value());
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
