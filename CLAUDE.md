# PyInt Developer Guide

## Build & Test Commands
- Open `PyInt.sln` in Visual Studio to build the solution
- Run tests: Open Test Explorer in Visual Studio (Test > Test Explorer)
- Run single test: Select individual test in Test Explorer and click "Run"
- Run PyInt: Build and run PyInt.Console project
- Run a Python file: `PyInt.Console.exe -fi [filepath]`
- Run REPL with bytecode output: `PyInt.Console.exe -e t`
- Output to file: `PyInt.Console.exe -e f [filepath]`

## Code Style Guidelines
- **Naming**: 
  - Functions: PascalCase (InitFunction)
  - Variables: camelCase (localVar)
  - Types/Structs: PascalCase (TypeName)
  - Constants/Macros: UPPER_CASE (ALLOC_MEM)

- **Organization**:
  - Use .h/.c file pairs with matching names
  - Group related functionality in subdirectories
  - Header guards: `#ifndef PyInt_Module_h`
  - Implementation pattern: [Component]Functions.c

- **Memory Management**:
  - Use ALLOCATE/FREE/GROW_ARRAY macros
  - Always pass GarbageCollector to memory functions

- **Error Handling**:
  - Use SyntaxError for compilation issues
  - Use RuntimeError for VM execution issues
  - Include clear context in error messages