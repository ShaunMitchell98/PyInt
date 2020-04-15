#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(LogicFunctionalTests)
	{
	public:
		TEST_METHOD(IntegerGreaterThan)
		{
			char* output1 = RunInterpreter("print(7 > 6)");
			char* output2 = RunInterpreter("print(6 > 7)");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			free(output1);
			free(output2);
		}

		TEST_METHOD(IntegerLessThan) {
			char* output1 = RunInterpreter("print(6 < 7)");
			char* output2 = RunInterpreter("print(7 < 6)");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			free(output1);
			free(output2);
		}

		TEST_METHOD(IntegerGreaterThanOrEqualTo) {
			char* output1 = RunInterpreter("print(7 >= 7)");
			char* output2 = RunInterpreter("print(7 >= 5)");
			char* output3 = RunInterpreter("print(5 >= 7)");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("true", output2);
			Assert::AreEqual("false", output3);
			free(output1);
			free(output2);
			free(output3);
		}

		TEST_METHOD(IntegerLessThanOrEqualTo) {
			char* output1 = RunInterpreter("print(7 <= 7)");
			char* output2 = RunInterpreter("print(7 <= 5)");
			char* output3 = RunInterpreter("print(5 <= 7)");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			Assert::AreEqual("true", output3);
			free(output1);
			free(output2);
			free(output3);
		}

		TEST_METHOD(IntegerEqual) {
			char* output1 = RunInterpreter("print(5 == 5)");
			char* output2 = RunInterpreter("print(5 == 4)");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			free(output1);
			free(output2);
		}

		TEST_METHOD(IntegerNotEqual) {
			char* output1 = RunInterpreter("print(5 != 4)");
			char* output2 = RunInterpreter("print(5 != 5)");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			free(output1);
			free(output2);
		}

		TEST_METHOD(AndOperation_LeftOperandFalseRightOperandFalse_ReturnsFalse) {
			char* output = RunInterpreter("print((5 == 4) and (2 == 3))");
			Assert::AreEqual("false", output);
			free(output);
		}

		TEST_METHOD(AndOperation_LeftOperandTrueRightOperandFalse_ReturnsFalse) {
			char* output = RunInterpreter("print((5 == 5) and (5 == 4))");
			Assert::AreEqual("false", output);
			free(output);
		}

		TEST_METHOD(AndOperation_LeftOperandFalseRightOperandTrue_ReturnsFalse) {
			char* output = RunInterpreter("print((5 == 4) and (5 == 5))");
			Assert::AreEqual("false", output);
			free(output);
		}

		TEST_METHOD(AndOperation_LeftOperandTrueRightOperandTrue_ReturnsTrue) {
			char* output = RunInterpreter("print((5 == 5) and (3 == 3))");
			Assert::AreEqual("true", output);
			free(output);
		}


		TEST_METHOD(OrOperation_LeftOperandFalseRightOperandFalse_ReturnsFalse) {
			char* output = RunInterpreter("print((5 == 4) or (2 == 3))");
			Assert::AreEqual("false", output);
			free(output);
		}

		TEST_METHOD(OrOperation_LeftOperandTrueRightOperandFalse_ReturnsTrue) {
			char* output = RunInterpreter("print((5 == 5) or (5 == 4))");
			Assert::AreEqual("true", output);
			free(output);
		}

		TEST_METHOD(OrOperation_LeftOperandFalseRightOperandTrue_ReturnsTrue) {
			char* output = RunInterpreter("print((5 == 4) or (5 == 5))");
			Assert::AreEqual("true", output);
			free(output);
		}

		TEST_METHOD(OrOperation_LeftOperandTrueRightOperandTrue_ReturnsTrue) {
			char* output = RunInterpreter("print((5 == 5) or (3 == 3))");
			Assert::AreEqual("true", output);
			free(output);
		}

		TEST_METHOD(NotOperation_OperandTrue_ReturnsFalse) {
			char* output = RunInterpreter("print(not (5 == 5))");
			Assert::AreEqual("false", output);
			free(output);
		}

		TEST_METHOD(NotOperation_OperandFalse_ReturnsTrue) {
			char* output = RunInterpreter("print(not (5 == 4))");
			Assert::AreEqual("true", output);
			free(output);
		}
	};
}