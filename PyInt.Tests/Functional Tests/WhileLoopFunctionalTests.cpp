#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(WhileLoopFunctionalTests)
	{
	public:
		TEST_METHOD(WhileLoop_GivenFalseCondition_DoesNotExecuteStatements) {
			char* output = RunInterpreter("while (5 < 3):\n\t print(3)");
			Assert::AreEqual("", output);
			free(output);
		}

		TEST_METHOD(WhileLoop_GivenCondition_ExecutesStatementsWhileConditionTrue) {
			char* output = RunInterpreter("i = 5\n while (i > 0):\n\t print(i)\n\t i = i-1");
			Assert::AreEqual("54321", output);
			free(output);
		}
	};
}