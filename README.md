# SAS Static Analysis Suite

## Introduction
The Static Analysis Suite (SAS) allows to easily create checkers for static analysis with clang. This is achieved creating a plugin (dynamic shared object library), loadable by the compiler. SAS is shipped with a set of custom checkers and allows easy extension with new checkers. See [Credit and History](#creditAndHistory)

## How to build and use the SAS plugin
Clone the SAS repository and then:
```
export SASBUILDDIR=SASbuild
mkdir $SASBUILDDIR;cd $SASBUILDDIR
cmake -D LLVM_DEV_DIR=<location of clang> ../SAS
make -j 4
```

Having performed these steps, you should have the library ready in
"$SASBUILDDIR/lib/libSas.so".

There are two ways of using the plugin: directly with clang and in combination with scan-build

# With Clang
You can invoke the static analyzer with your own checkers with this command:
```
clang++ <all the options to compile the unit>  -Xclang -analyze -Xclang -analyzer-output=text -Xclang -load -Xclang $SASBUILDDIR/lib/libSas.so -Xclang -analyzer-checker=MyAnalyzer1 -Xclang -analyzer-checker=MyAnalyzer2 ... -analyzer-checker=MyAnalyzerN
```

# Scan-build
To invoke scan-build prepended to make, a possible command could be:
```
scan-build -load-plugin $BUILD_DIR/lib/libSas.so -enable-checker sas \
           -disable-checker sas.example -disable-checker sas.optional \
           -o MyReportDir make
```

## Requirements
To start using SAS, you need cmake (at least version 2.6), clang, llvm and their development files. The required version for Clang is 3.5.

## Available checkers
**sas**

1. security
   1. NonFiniteMath
2. threadsafety
   1. ConstCast *
   2. ConstCastAway
   3. GlobalStatic
   4. StaticLocal
   5. MutableMember
3. performance
   1. ArgSize
4. CodeRules
   1. UsingNamespace *
   2. CatchAll
5. optional
   1. ClassDumperCT
   2. ClassDumperFT
   3. ClassDumperInherit
6. example
   1. Varname *

Checkers marked with an asterisk can be disabled using comments (see next section for details).

## How to add a new checker
To add a new checker to the library, you need to define a checker class derived from `clang::ento::Checker` and register the checker in the library.

## Details about the implementation
You can find the source code of the existing SAS checkers in the files *src/*Checker.**. Feel free to inspect the source code to get basic understanding of how checkers are implemented. Note that `VarnameChecker` and `GlobalAccInCtorChecker` are expected to have more comprehensive documentation. Please consult the [Checker Developer Manual] for more information about checker development.
Useful links:
* Checker Developer Manual: http://clang-analyzer.llvm.org/checker_dev_manual.html
* Clang API Documentation:  http://clang.llvm.org/doxygen/

## Registration in SAS plugin
Once you've created a new checker class, you need to register it in the SAS library. To do that, edit the files *src/ClangSasCheckerPluginRegister.cpp* and *CMakeLists.txt*. The comments in the files explain the steps to be taken in order to add a checker to the library.
After this step is performed, configure SAS build again with `cmake` to propagate the changes in the build files and re-build SAS.

# How to disable a checker from within the source code
A checker disabling mechanism is provided for suppressing false positives.
To disable a checker on a particular line in target source code, add a comment on a preceding line that starts with "//" (two slashes) and contains the following text:
```
sas[disable_checker : "MyChecker"]
```
(including the double quotes) where MyChecker is a full name of the checker to be disabled.

Example (source code extract):
```
// sas[disable_checker : "sas.example.Varname"]
int varLowercase; // not bug (Varname): lowercase, but disabled by comment
```

For more examples see the file *test/checkerdisabler.cpp*.

## How to allow a checker to be disable
Consult the file *src/CheckerDisabler.h* for instructions on how to disable your checker.

# <a id="creditAndHistory"></a> Credit and History
SAS originates from an effort within athe CMS collaboration at CERN, based on a first implementation by T. Hauth and D. Piparo. The credit to single developers is preserved within the single source files. The first version of SAS was created by F. Bartek, D. Piparo and T. Hauth.

# Useful Links:
o LLVM:                  http://llvm.org/
o Clang:                 http://clang.llvm.org/
o Clang Static Analyzer: http://clang-analyzer.llvm.org/
o scan-build:            http://clang-analyzer.llvm.org/scan-build.html
o CMake:                 http://www.cmake.org/
