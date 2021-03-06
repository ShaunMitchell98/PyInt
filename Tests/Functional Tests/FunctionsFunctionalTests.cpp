#include "CppUnitTest.h"
extern "C" {
#include "../../PyInt/src/RunFile.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(FunctionsFunctionalTests)
	{
	public:
		TEST_METHOD(FunctionReturningConstant) {
			char* input = "def Empty():\n\treturn 5\nprint(Empty())";
		    char* output = RunInterpreter(input);
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(FunctionTakingIntegerConstant_PrintsConstant) {
			char* input = "def PrintInteger(integer):\n\tprint(integer)\n\nPrintInteger(35)";
			char* output = RunInterpreter(input);
			Assert::AreEqual("35", output);
			free(output);
		}

		TEST_METHOD(FunctionTakingMultipleInputs_ReturnsSum) {
			char* input = "def Sum(int1, int2):\n\treturn int1 + int2\nprint(Sum(5, 6))";
			char* output = RunInterpreter(input);
			Assert::AreEqual("11", output);
			free(output);
		}

		TEST_METHOD(NestedFunction_WhenCalledWithinOuterFunction_Works) {
			char* input = "def outerFunction(text):\n\tdef innerFunction():\n\t\treturn text + 2\n\treturn innerFunction()\nprint(outerFunction(27))";
			char* output = RunInterpreter(input);
			Assert::AreEqual("29", output);
			free(output);
		}

		TEST_METHOD(Function_WithLocalVariables_ReturnsTheirSum) {
			char* input = "def myFunc():\n\ta = 5\n\tb=6\n\t return a + b\nprint(myFunc())";
			char* output = RunInterpreter(input);
			Assert::AreEqual("11", output);
			free(output);
		}

		TEST_METHOD(Function_WithReassignedLocalVariables_UsesLatestValues) {
			char* input = "Function_WithReassignedLocalVariables_UsesLatestValues";
			char* output = RunInterpreterFromFile(input);
			Assert::AreEqual("101", output);
			free(output);
		}

		TEST_METHOD(Function_ReturnedFromOuterVariable_CanBeCalledWithClosedOverVariables) {
			char* input = "Function_ReturnedFromOuterVariable_CanBeCalledWithClosedOverVariables";
			char* output = RunInterpreterFromFile(input);
			Assert::AreEqual("7", output);
			free(output);
		}
	};
}