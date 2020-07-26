# build-analysis

This repo includes some experiments for the build process, on 5 different compilers, to see how many unique processes are called, while building applications. The 5 different used compilers, include: Microsoft C++, Microsoft C#, GO, TCC, GCC MingW.

Another MSVC project is used to list and even stop any involved process, as it gets called during the build process. 

Before building the projects, make sure that your compilers are accessible from environment variables and that the process monitoring application (StopProcess) is running.

Command lines used for build:

* Microsoft Visual C++:
cl f1.cpp f2.cpp f3.cpp f4.cpp f5.cpp f6.cpp f7.cpp f8.cpp f9.cpp s1.cpp /link /out:result.exe

* GCC:
gcc -o result.exe f1.c f2.c f3.c f4.c f5.c f6.c f7.c f8.c f9.c s1.c 

* GO:
go build -o result.exe

* Microsoft Visual C#:
csc.exe -out:result.exe s1.cs f1.cs f2.cs f3.cs f4.cs f5.cs f6.cs f7.cs f8.cs f9.cs

* TCC:
tcc.exe -o result.exe f1.c f2.c f3.c f4.c f5.c f6.c f7.c f8.c f9.c s1.c


