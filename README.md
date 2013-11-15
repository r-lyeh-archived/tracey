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
- Hooks for `malloc()`/`free()` are only available on Windows.
- Slower runtime than regular builds. There is room for improvement though.

Changelog
---------
0.2-a
  - Tracey behaves better now in many aspects.
  - Support for /MT and /MTd on Windows.
  - iOS support.
  - A more compatible symbol demangling on MacOs/iOS.
  - Faster report generation.
  - New smaller hierarchical tree reports that provide also more information than before.
  - Bugfixed symbol retrieval when other memory managers are present.
  - Improved symbol retrieval on windows.
  - Experimental webserver (to be improved).
  - Experimental win32 CRT hooks (to be improved).
  - New compiler tweaks.
  - Many new options and settings (unwinding stack size, truncation, etc...)
  - Bugfixed recursive deadlocks on tinythread.
  - A bunch of other minor bugfixes and tweaks.
  - Tracey throws exceptions only if they are enabled on your compiler now.
  - API upgraded.

Configuration (optional)
------------------------
Feel free to tweak these defines in `tracey.cpp`. Or define them in your compiler directives.

```c++
  - All allocations converge to this symbol.
#   define kTraceyAlloc                 std::malloc
  - All deallocations converge to this symbol.
#   define kTraceyFree                  std::free
  - All reallocations converge to this symbol.
#   define kTraceyRealloc               std::realloc
#   define kTraceyMemset                std::memset
  - All warning and reports converge to this symbol.
#   define kTraceyPrintf                std::printf
#   define kTraceyDie                   $windows( FatalExit ) $welse( std::exit )
  - All out-of-memory runtime asserts converge to this symbol.
#   define kTraceyAssert                assert
  - All out-of-memory runtime exceptions converge to this sysmbol.
#   define kTraceyBadAlloc              std::bad_alloc

  - Tracey uses this value to simulate increased memory requirements (should be >= 1.0)
#   define kTraceyAllocsOverhead        1.0
#   define kTraceyStacktraceMaxTraces   128
#   define kTraceyStacktraceSkipBegin   0 // $windows(4) $welse(0)
#   define kTraceyStacktraceSkipEnd     0 // $windows(4) $welse(0)
#   define kTraceyCharLinefeed          "\n"
#   define kTraceyCharTab               "\t"
When enabled, Tracey warns about deallocations on pointers that have been not allocated by Tracey (wild pointers)
#   define kTraceyReportWildPointers    0
#   define kTraceyMemsetAllocations     1
  - When enabled, Tracey shows a report automatically on application exit.
#   define kTraceyReportOnExit          1
#   define kTraceyHookCRT               0
#   define kTraceyWebserver             1
#   define kTraceyWebserverPort         2001
  - When enabled, Tracey starts before application is running.
#   define kTraceyEnabled               1
  - When enabled, Tracey implements all new([])/delete([]) operators.
  - When disabled, user must provide de/allocation operators thru runtime API (see below).
#   define kTraceyDefineCppMemOperators 0
```

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
// tracey is a callstack based static library that requires no source modification.

int main() {
    int *make_leak = new int;
    int *make_leaks = new int [400];
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
