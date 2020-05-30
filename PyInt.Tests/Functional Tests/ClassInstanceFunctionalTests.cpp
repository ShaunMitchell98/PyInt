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
			char* output = RunInterpreter("class MyClass:\n\tpass\ninstance = MyClass()\ninstance.value = 5\nprint(instance.value)");
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(CanUpdateProperty) {
			char* output = RunInterpreter("class MyClass:\n\tpass\ninstance = MyClass()\ninstance.value = 5\n\ninstance.value = 'Hello World'\nprint(instance.value)");
			Assert::AreEqual("Hello World", output);
			free(output);
		}

		TEST_METHOD(CanCallMethod) {
			char* output = RunInterpreter("class MyClass:\n\tdef numberPrinter(self):\n\t\treturn 6 + 7\ninstance = MyClass()\nprint(instance.numberPrinter())");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanCallMethodFromAsProperty) {
			char* output = RunInterpreter("class MyClass:\n\tdef numberPrinter(self):\n\t\treturn 6+7\ninstance = MyClass()\nmethod = instance.numberPrinter\nprint(method())");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanAccessPropertyFromInstance) {
			char* output = RunInterpreter("class MyClass:\n\tproperty = 6\n\tproperty2 = 5\ninstance = MyClass()\nprint(instance.property + instance.property2)");
			Assert::AreEqual("11", output);
			free(output);
		}

		TEST_METHOD(CanCallInitialiser) {
			char* output = RunInterpreter("class MyClass:\n\tdef __init__(self, property1, property2):\n\t\tself.property1 = property1\n\t\tself.property2 = property2\ninstance = MyClass(8,9)\nprint(instance.property1 + instance.property2)");
			Assert::AreEqual("17", output);
			free(output);
		}

		TEST_METHOD(CanAccessPropertyFromMethod) {
			char* output = RunInterpreter("class MyClass:\n\tdef __init__(self, property):\n\t\tself.property = property\n\tdef addConstant(self, constant):\n\t\treturn self.property + constant\ninstance = MyClass(7)\nprint(instance.addConstant(9))");
			Assert::AreEqual("16", output);
			free(output);
		}

		TEST_METHOD(ClassCanCallSuperClassMethods) {
			char* output = RunInterpreter("class superclass:\n\tdef myFunc(self):\n\t\treturn 6 + 7\nclass subclass(superclass):\n\tdef mySubFunc(self):\n\t\treturn superclass.myFunc()\ninstance = subclass()\nprint(instance.mySubFunc())");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(ClassCanCallSuperClassMethodAndOwnMethod) {
			char* output = RunInterpreter("class superclass:\n\tdef superFunc(self):\n\t\treturn 6\nclass subclass(superclass):\n\tdef subFunc(self):\n\t\treturn 7\n\tdef myFunc(self):\n\t\treturn self.subFunc() + superclass.superFunc()\ninstance = subclass()\nprint(instance.myFunc())");
			Assert::AreEqual("13", output);
			free(output);
		}
	};
}