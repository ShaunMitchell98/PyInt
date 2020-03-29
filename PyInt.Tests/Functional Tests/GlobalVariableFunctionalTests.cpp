#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(GlobalVariableFunctionalTests)
	{
	public:
		TEST_METHOD(GlobalVariable_InitialisedAsInteger_StoresInteger) {
			char* output = RunInterpreter("a = 5 \n print a");
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(GlobalVariable_InitialisedAsStringWithSingleQuotes_StoresString) {
			char* output = RunInterpreter("a = 'Hello world'\n print a");
			Assert::AreEqual("Hello world", output);
			free(output);
		}

		TEST_METHOD(GlobalVariable_InitialisedAsStringWithDoubleQuotes_StoresString) {
			char* output = RunInterpreter("a = \"Hello world\"\n print a");
			Assert::AreEqual("Hello world", output);
			free(output);
		}

		TEST_METHOD(GlobalVariable_InitialisedAsTrueBool_StoresBool) {
			char* output = RunInterpreter("a = true\n print a");
			Assert::AreEqual("true", output);
			free(output);
		}

		TEST_METHOD(GlobalVariable_InitialisedAsFalseBool_StoresBool) {
			char* output = RunInterpreter("a = false\n print a");
			Assert::AreEqual("false", output);
			free(output);
		}

	};
}