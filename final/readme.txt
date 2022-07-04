源代码中
class.h中储存一些用到的类的定义
scanner.h为词法分析器的实现
parser.h为语法分析器的实现
semantic_analyzer.h为语义分析器的实现
code_generator.h为目标代码生成器的实现
compiler.cpp为需要编译的代码，为完整的测试过程。

demo.txt为测试的代码文件
tokens.txt为生成的tokens串序列
mips.asm为最终生成的目标代码文件

测试方法：可以直接运行可执行程序，也可以编译compiler.cpp并运行生成的可执行文件
即可得到对应的mips.asm文件，接下来将对应的mips.asm放到Mars中运行测试即可。
（Mars.jar也在附件中可以使用，在菜单栏Run中先进行Assemble接着GO运行即可查看结果）。