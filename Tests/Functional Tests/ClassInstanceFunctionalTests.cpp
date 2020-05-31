#include "CppUnitTest.h"
extern "C" {
#include "../../PyInt/src/RunFile.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(ClassInstanceFunctionalTests)
	{
	public:
		TEST_METHOD(CanSetProperty)
		{
			char* output = RunInterpreterFromFile("CanSetProperty");
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(CanUpdateProperty) {
			char* output = RunInterpreterFromFile("CanUpdateProperty");
			Assert::AreEqual("Hello World", output);
			free(output);
		}

		TEST_METHOD(CanCallMethod) {
			char* output = RunInterpreterFromFile("CanCallMethod");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanCallMethodBoundToProperty) {
			char* output = RunInterpreterFromFile("CanCallMethodBoundToProperty");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanAccessPropertyFromInstance) {
			char* output = RunInterpreterFromFile("CanAccessPropertyFromInstance");
			Assert::AreEqual("11", output);
			free(output);
		}

		TEST_METHOD(CanCallInitialiser) {
			char* output = RunInterpreterFromFile("CanCallInitialiser");
			Assert::AreEqual("17", output);
			free(output);
		}

		TEST_METHOD(CanAccessPropertyFromMethod) {
			char* output = RunInterpreterFromFile("CanAccessPropertyFromMethod");
			Assert::AreEqual("16", output);
			free(output);
		}

		TEST_METHOD(ClassCanCallSuperClassMethods) {
			char* output = RunInterpreterFromFile("ClassCanCallSuperClassMethods");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(ClassCanCallSuperClassMethodAndOwnMethod) {
			char* output = RunInterpreterFromFile("ClassCanCallSuperClassMethodAndOwnMethod");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanCallSuperMethodFromDerivedInstance) {
			char* output = RunInterpreterFromFile("CanCallSuperMethodFromDerivedInstance");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(ClassInstanceCanAccessSuperClassPropertySetInInitialiser) {
			char* output = RunInterpreterFromFile("ClassInstanceCanAccessSuperClassPropertySetInInitialiser");
			Assert::AreEqual("7", output);
			free(output);
		}


		TEST_METHOD(ClassInstanceCanAccessSuperClassProperty) {
			char* output = RunInterpreterFromFile("ClassInstanceCanAccessSuperClassProperty");
			Assert::AreEqual("79", output);
			free(output);
		}
	};
}