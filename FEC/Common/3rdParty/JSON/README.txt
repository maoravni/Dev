
-----------------------------
License:
-----------------------------
The Library and the example code is released under the Eclipse Public License
http://en.wikipedia.org/wiki/Eclipse_Public_License
https://www.eclipse.org/legal/epl-v10.html


-----------------------------
Documentation:
-----------------------------
See the following link for instructions on how to read the
documentation online or on how to download the documentation:
https://realtimelogic.com/products/json/



-----------------------------
Compiler:
-----------------------------

You can use any C compiler for compiling the code, however, the
included Makefile is configured for gcc.

If you are using an IDE, include the source code in a project as
explained below.


-----------------------------
Compiling:
-----------------------------

Decide if you want to use the more complex JParserValFact or the less
complex JDecoder. See the documentation for details regarding these
two classes.

 * if using JParserValFact, include all C files in the build, except
   JDecoder.c

 * if using JDecoder, include all C files in the build, except JVal.c
   and define the macro NO_JVAL_DEPENDENCY when compiling the
   code. See the comment at the top of the source file JEncoder.c for
   details.

The included example Makefile, which is configured for GCC, builds 3
libraries. See the Makefile for details.

NOTE: The C files in the src directory MUST be compiled by a C
compiler, not a C++ compiler. Files ending with .c must be compiled by
a C compiler and files ending with .cpp must be compiled by a C++
compiler.

-----------------------------
Examples: ./examples/
-----------------------------

StaticAllocatorEx.cpp shows how to use a static allocator with JParserValFact

m2m-led.c, solib.c: the M2M LED JSON example: See the following for details:
https://realtimelogic.com/ba/doc/en/C/reference/html/md_en_C_md_JSON.html#M2MLED

-----------------------------
Test programs (and examples) ./test/:
-----------------------------

Generic test and example: test/test1.cpp
