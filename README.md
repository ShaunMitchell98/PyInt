# PyInt

PyInt is a Python Interpreter based on the Clox interpreter by Bob Nystrom from https://craftinginterpreters.com/

![REPL mode](https://github.com/ShaunMitchell98/PyInt/blob/master/Images/REPL.png)

By default, when you run the executable it starts a REPL, where you can run code quickly. By passing arguements, you can get additional output such as the executed bytecode and the state of the stack.

![REPL with Executed Bytecode](https://github.com/ShaunMitchell98/PyInt/blob/master/Images/REPL%20with%20Execution%20Output.png)

The output doesn't have to be to the terminal, output can be redirected to a file by replacing t with f and adding the file path as the next input, for example: 

![Execution Output to File](https://github.com/ShaunMitchell98/PyInt/blob/master/Images/REPL%20with%20Execution%20Output%20to%20File(1).png)
![](https://github.com/ShaunMitchell98/PyInt/blob/master/Images/REPL%20with%20Execution%20Output%20to%20File(2).png)

As well as writing commands in the terminal, you can run a file using the arguement -fi and passing the filepath:

![File Input](https://github.com/ShaunMitchell98/PyInt/blob/master/Images/Text%20File%20Input(1).png)
![](https://github.com/ShaunMitchell98/PyInt/blob/master/Images/Text%20File%20Input(2).png)

## Building on macOS/Linux

PyInt can now be built on macOS and Linux using CMake:

1. Make sure you have CMake installed:
   ```
   # macOS
   brew install cmake
   
   # Linux
   sudo apt-get install cmake
   ```

2. Run the build script:
   ```
   ./build.sh
   ```

3. Run the interpreter:
   ```
   ./build/PyInt.Console/pyint
   ```

**Note:** For a full build with all features working on macOS/Linux, the Windows-specific functions in the source code need to be replaced with cross-platform alternatives. The compatibility layer in the `compat` directory provides macros to help with this.
