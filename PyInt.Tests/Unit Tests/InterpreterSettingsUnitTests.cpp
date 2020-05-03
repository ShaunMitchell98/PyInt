#include "CppUnitTest.h"
extern "C" {
#include "../../PyInt/src/Virtual Machine/Settings/Settings.h"
}

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			std::wstring ToString(Location value)
			{
				switch (value) {
				case LOCATION_FILE: return L"OutputFile";
				case LOCATION_TERMINAL: return L"OutputTerminal";
				case LOCATION_STRING: return L"OutputString";
				}

				return std::to_wstring(static_cast<int>(value));
			}
		}
	}
}



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntUnitTests
{
	
	TEST_CLASS(InterpreterSettingsUnitTests)
	{
	public:


		TEST_METHOD(Initialisation_GivenNoArguements_ReturnsDefaultSettings) {
			int argc = 1;
			const char* argv[1] = { "PyInt" };
			Settings settings;
			InitialiseSettings(&settings, argc, argv);
			
			Assert::AreEqual(LOCATION_TERMINAL, settings.output.location);
			Assert::IsFalse(settings.bytecode.enabled);
			Assert::IsFalse(settings.execution.enabled);
		}

		TEST_METHOD(Initialisation_GivenFileOutputSettings_ReturnsFileOutputSettings) {
			int argc = 3;
			char* outputFilePath = "Output.txt";
			const char* argv[3] = { "PyInt", "-fo", outputFilePath };
			Settings settings;
			InitialiseSettings(&settings, argc, argv);


			Assert::AreEqual(LOCATION_FILE, settings.output.location);
			Assert::AreEqual(outputFilePath, settings.output.filePath);
		}

		TEST_METHOD(Initialisation_GivenBytecodeArgument_SetsBytecodeSettings) {
			int argc = 3;
			char* outputFilePath = "Output.txt";
			const char* argv[3] = { "PyInt", "-fb", outputFilePath };
			Settings settings;
			InitialiseSettings(&settings, argc, argv);

			Assert::IsTrue(settings.bytecode.enabled);
			Assert::AreEqual(LOCATION_FILE, settings.bytecode.location);
			Assert::AreEqual(outputFilePath, settings.bytecode.filePath);
		}

		TEST_METHOD(Initialisation_GivenDisassembleExecutionArgument_SetsDisassembleExecution) {
			int argc = 3;
			char* outputFilePath = "Output.txt";
			const char* argv[3] = { "PyInt", "-fe", outputFilePath };
			Settings settings;
			InitialiseSettings(&settings, argc, argv);

			Assert::IsTrue(settings.execution.enabled);
			Assert::AreEqual(LOCATION_FILE, settings.execution.location);
			Assert::AreEqual(outputFilePath, settings.execution.filePath);
		}
	};
}