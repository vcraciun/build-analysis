# build-analysis

This repo includes some experiments for the build process, on 5 different compilers, to see how many unique processes are called, while building applications. The 5 different used compilers, include: Microsoft C++, Microsoft C#, GO, TCC, GCC MingW.

Another MSVC project is used to list and even stop any involved process, as it gets called during the build process. 

Before building the projects, make sure that your compilers are accessible from environment variables and that the process monitoring application (StopProcess) is running.
