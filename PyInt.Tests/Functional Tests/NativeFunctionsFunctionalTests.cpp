#include "CppUnitTest.h"
extern "C" {
#include "../../PyInt/src/RunFile.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(NativeFunctionsFunctionalTests)
	{
	public:
		TEST_METHOD(FunctionReturningConstant) {
			char* input = "def Empty():\n\treturn 5\nprint(Empty())";
			char* output = RunInterpreter(input);
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(CallsClockNativeFunction_DoesNotReturnError) {
			char* input = "def fib(n):\n\tif (n < 2):\n\t\treturn n\n\treturn fib(n -2) + fib(n-1)\nstart = clock()\nfib(4)\nprint(clock() - start)";
			char* output = RunInterpreter(input);
			Assert::IsFalse(memcmp("", output, 1) == 0);
			free(output);
		}
	};
}