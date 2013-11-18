tracey
======
Tracey is a C++ static library that reports memory leaks and provides precise stacktraces with no false positives.

Features
--------
- Lightweight and simple memory C++ leak finder with no dependencies
- Easy to integrate. No source modification is required.
- Clean. Macroless and callstack based. No new/delete macro hacks.
- Lazy. Should work out of the box. Plenty configuration options are provided anyways.
- Tiny. One header and one source file.
- Cross-platform. Builds on Windows/Linux/MacosX. Compiles on g++/clang/msvc.
- OS dependencies only. No third party dependencies.
- MIT licensed.

Cons
----
- No hooks for `malloc()`/`free()`. Tracey supports `new`/`delete` C++ memory operators only (atm).
- Slower runtime than regular builds. There is enough room for improvement though.

The rest of the file is outdated
--------------------------------
Will fix it asap. Enjoy :)

Implementation directives (optional)
------------------------------------
- `kTraceyAlloc(size)` defaults to `std::malloc(size)` if not overriden.
  - All allocations merge to this symbol.
- `kTraceyFree(ptr)` defaults to `std::free(ptr)` if not overriden.
  - All deallocations merge to this symbol.
- `kTraceyPrint(str)` defaults to `fprintf(stderr,"%s",str)` if not overriden.
  - All warning and reports merge to this symbol.
- `kTraceyAssert(expr)` defaults to `assert(expr)` if not overriden.
  - All out-of-memory runtime asserts merge to this symbol.
- `kTraceyBadAlloc()` defaults to `std::bad_alloc()` if not overriden.
  - All out-of-memory runtime exceptions merge to this sysmbol.

Configuration directives (optional)
-----------------------------------
- `kTraceyAllocMultiplier` defaults to `1.0` if not overriden (should be >= 1.0).
  - Tracey uses this value to increase memory requirements, and to simulate and to debug worse memory conditions.
- `kTraceyReportWildPointers` defaults to `1` if not overriden (should be 0 or 1).
  - When enabled, Tracey warns about deallocations on pointers that have been not allocated by Tracey (wild pointers).
- `kTraceyReportNullPointers` defaults to `0` if not overriden (should be 0 or 1).
  - When enabled, Tracey warns about deallocations on null pointers.
- `kTraceyEnabledOnStart` defaults to `1` if not overriden (should be 0 or 1).
  - When enabled, Tracey starts before application is running.
- `kTraceyReportOnExit` defaults to `1` if not overriden (should be 0 or 1).
  - When enabled, Tracey shows a report automatically on application exit.
- `kTraceyDefineMemoryOperators` defaults to `1` if not overriden (should be 0 or 1).
  - When enabled, Tracey implements all new([])/delete([]) operators.
  - When disabled, user must provide de/allocation operators thru runtime API (see below).

Runtime API (optional)
----------------------

- `tracey::watch()` tells Tracey to watch a memory address.
- `tracey::forget()` tells Tracey to forget about a memory address.
- `tracey::enable()` tells Tracey to enable/disable whole watching system.
- `tracey::disable()` tells Tracey to disable whole watching system.
- `tracey::is_enabled()` returns Tracey's working status.
- `tracey::invalidate()` tells Tracey to forget whole execution.
- `tracey::report()` tells Tracey to show current report.
- `tracey::version()` tells Tracey to return current version.

special notes
-------------
- g++ users: both `-std=c++0x` and `-lpthread` may be required when compiling `tracey.cpp`

Sample
------
```c++
// tracey is callstack based. no dirty new/delete macro tricks.
// tracey is a static library. requires no source modification. just link it.

void *make_leaks()
{
    return new int [400];
}

int main( int argc, const char **argv )
{
    int *make_leak = new int();
    make_leaks();

    return 0;
}
```

Possible outputs (msvc/g++/clang)
---------------------------------
```
D:\prj\tracey>cl sample.cc tracey.cpp /MDd /Zi
D:\prj\tracey>sample
<tracey/tracey.cpp> says: Beginning of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
1) Leak 4 bytes [0033F1E8] backtrace 1/2 (50%)
        main (d:\prj\moons\tracey\sample.cc, line 11)
        __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 536)
        mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 377)
        BaseThreadInitThunk
        RtlInitializeExceptionChain
        RtlInitializeExceptionChain
2) Leak 1600 bytes [0035D8E8] backtrace 2/2 (100%)
        make_leaks (d:\prj\moons\tracey\sample.cc, line 6)
        main (d:\prj\moons\tracey\sample.cc, line 14)
        __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 536)
        mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 377)
        BaseThreadInitThunk
        RtlInitializeExceptionChain
        RtlInitializeExceptionChain
<tracey/tracey.cpp> says: End of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
D:\prj\tracey>
```

```
[live@bridge-live tracey]$ g++ sample.cc tracey.cpp -g -lpthread -std=c++0x
tracey.cpp:105:6: warning: #warning "<tracey/tracey.cpp> says: do not forget -g -lpthread compiler settings!" [-Wcpp]
[live@bridge-live tracey]$ ./a.out
<tracey/tracey.cpp> says: Beginning of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
1) Leak 4 bytes [0x1fa1010] backtrace 1/2 (50%)
    /home/live/tracey/sample.cc:11
    /usr/lib/libc.so.6(__libc_start_main+0xf5) [0x7ff22aed3a15]
    ./a.out() [0x407549]
2) Leak 1600 bytes [0x1fa11e0] backtrace 2/2 (100%)
    /home/live/tracey/sample.cc:7
    /home/live/tracey/sample.cc:14
    /usr/lib/libc.so.6(__libc_start_main+0xf5) [0x7ff22aed3a15]
    ./a.out() [0x407549]
<tracey/tracey.cpp> says: End of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
```

```
$ clang++ sample.cc tracey.cpp -g -o sample -std=c++11 -stdlib=libc++
tracey.cpp:105:6: warning: "<tracey/tracey.cpp> says: do not forget -g compiler settings!" [-W#warnings]
#    warning "<tracey/tracey.cpp> says: do not forget -g -rdynamic compiler settings!"
     ^
1 warning generated.
$ ./sample
<tracey/tracey.cpp> says: Beginning of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
1) Leak 4 bytes [0x10d800810] backtrace 1/2 (50%)
    4   sample                              0x000000010d70616e operator new(unsigned long) + 190
    5   sample                              0x000000010d703ad8 main + 40
    6   sample                              0x000000010d703a84 start + 52
    7   ???                                 0x0000000000000001 0x0 + 1
2) Leak 1600 bytes [0x7fa15b000000] backtrace 2/2 (100%)
    4   sample                              0x000000010d70623e operator new[](unsigned long) + 190
    5   sample                              0x000000010d703aa3 make_leaks() + 19
    6   sample                              0x000000010d703aea main + 58
    7   sample                              0x000000010d703a84 start + 52
    8   ???                                 0x0000000000000001 0x0 + 1
<tracey/tracey.cpp> says: End of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
```
