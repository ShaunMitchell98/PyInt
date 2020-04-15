#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(IfStatementFunctionalTests)
	{
	public:
		TEST_METHOD(IfStatement_GivenTrueExpression_RunsStatementBody) {
			char* output = RunInterpreter("if (5 == 5):\n\t print(4)");
			Assert::AreEqual("4", output);
			free(output);
		}

		TEST_METHOD(IfStatement_GivenFalseExpression_DoesNotRunStatementBody) {
			char* output = RunInterpreter("if (5 == 4):\n\t print(3)");
			Assert::AreEqual("", output);
			free(output);
		}

		TEST_METHOD(IfStatementWithElse_GivenTrueExpression_RunsIfStatementBody) {
			char* output = RunInterpreter("if (5 == 5):\n\t print (4)\nelse:\n\t print(3)");
			Assert::AreEqual("4", output);
			free(output);
		}

		TEST_METHOD(IfStatementWithElse_GivenFalseExpression_RunsElseStatementBody) {
			char* output = RunInterpreter("if (5 == 4):\n\t print(4)\nelse:\n\t print(3)");
			Assert::AreEqual("3", output);
			free(output);
		}
		TEST_METHOD(IfStatementWithElifAndElse_GivenTrueIfExpression_RunsIfStatementBody) {
			char* output = RunInterpreter("if (5 == 5):\n\t print(4)\nelif (4 == 4):\n\t print(7)\nelse:\n\t print(3)");
			Assert::AreEqual("4", output);
			free(output);
		}

		TEST_METHOD(IfStatementWithElifAndElse_GivenFalseIfAndTrueElifExpression_RunsElifStatementBody) {
			char* output = RunInterpreter("if (5 == 4):\n\t print(4)\nelif (4 == 4):\n\t print(7)\nelse:\n\t print(3)");
			Assert::AreEqual("7", output);
			free(output);
		}

		TEST_METHOD(IFStatementWithElifAndElse_GivenFalseIfAndElifExpression_RunsElseStatementBody) {
			char* output = RunInterpreter("if (5 == 4):\n\t print(4)\nelif (4 == 3):\n\t print(7)\nelse:\n\t print(3)");
			Assert::AreEqual("3", output);
			free(output);
		}

		TEST_METHOD(IfStatementWithMultipleElifStatements_GivenMultipleTrueElifs_RunsFirstTrueElifBody) {
			char* output = RunInterpreter("if (5 == 4):\n\t print(4)\nelif (4 == 3):\n\t print(7)\nelif (5 > 2):\n\t print(3)\nelif (3 < 6):\n\t print(9)");
			Assert::AreEqual("3", output);
			free(output);
		}
	};
}