#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(FunctionsFunctionalTests)
	{
	public:
		TEST_METHOD(FunctionReturningConstant) {
			char* input = "def Empty():\n\treturn 5\nprint Empty()";
		    char* output = RunInterpreter(input);
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(FunctionTakingIntegerConstant_PrintsConstant) {
			char* input = "def PrintInteger(integer):\n\tprint integer\n\nPrintInteger(35)";
			char* output = RunInterpreter(input);
			Assert::AreEqual("35", output);
			free(output);
		}

		TEST_METHOD(FunctionTakingMultipleInputs_ReturnsSum) {
			char* input = "def Sum(int1, int2):\n\treturn int1 + int2\nprint Sum(5, 6)";
			char* output = RunInterpreter(input);
			Assert::AreEqual("11", output);
			free(output);
		}
	};
}