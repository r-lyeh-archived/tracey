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
- Slower runtime than regular builds. There is room for improvement though.

Changelog
---------
0.20-b
  - Tracey behaves better now in many aspects.
  - Support for /MT and /MTd on Windows.
  - iOS support.
  - A more compatible symbol demangling on MacOs/iOS.
  - Faster report generation.
  - New smaller hierarchical tree reports that provide also more information than before.
  - Bugfixed symbol retrieval when other memory managers are present.
  - Improved symbol retrieval on windows.
  - Experimental webserver (to be improved).
  - New compiler tweaks.
  - Many new options and settings (unwinding stack size, truncation, etc...)
  - Bugfixed recursive deadlocks on tinythread.
  - A bunch of other minor bugfixes and tweaks.
  - Tracey throws exceptions only if they are enabled on your compiler now.
  - API upgraded.
  - Many things I have forgotten.

Configuration (optional)
------------------------
Feel free to tweak these defines in `tracey.cpp`. Or define them in your compiler directives.
```c++
  - All de/allocations converge to this symbol:
#   define kTraceyRealloc                 std::realloc
  - All memset operations converge to this symbol:
#   define kTraceyMemset                std::memset
  - All warning and reports converge to this symbol:
#   define kTraceyPrintf                std::printf
  - All fatal exits converge to this symbol:
#   define kTraceyDie                   $windows( FatalExit ) $welse( std::exit )
  - All out-of-memory runtime asserts converge to this symbol.
#   define kTraceyAssert                assert
  - All out-of-memory runtime exceptions converge to this sysmbol.
#   define kTraceyBadAlloc              std::bad_alloc

  - Tracey uses this value to simulate increased memory requirements (should be >= 1.0)
#   define kTraceyAllocsOverhead        1.0
  - Tracey retrieves up to 128 traces by default. The longer the slower, though.
#   define kTraceyStacktraceMaxTraces   128
  - Tracey head position on every stacktrace.
#   define kTraceyStacktraceSkipBegin   0 // $windows(4) $welse(0)
  - Tracey tail position on every stacktrace.
#   define kTraceyStacktraceSkipEnd     0 // $windows(4) $welse(0)
  - Tracey characters when logging.
#   define kTraceyCharLinefeed          "\n"
#   define kTraceyCharTab               "\t"
  - When enabled, Tracey warns about deallocations on pointers that have been not allocated by Tracey (wild pointers)
#   define kTraceyReportWildPointers    0
#   define kTraceyMemsetAllocations     1
  - When enabled, Tracey shows a report automatically on application exit.
#   define kTraceyReportOnExit          1
  - Tracey's webserver services:
#   define kTraceyWebserver             1
#   define kTraceyWebserverPort         2001
  - If you want to disable Tracey quickly, tweak this
#   define kTraceyEnabled               1
  - When enabled, Tracey implements all new([])/delete([]) operators.
  - When disabled, user must provide de/allocation operators thru runtime API (see below).
#   define kTraceyDefineCppMemOperators 0
```

Runtime API (optional)
----------------------

- `tracey::watch(ptr,size)` tells Tracey to watch a memory address.
- `tracey::forget(ptr)` tells Tracey to forget about a memory address.
- `tracey::restart()` tells Tracey to forget whole execution.
- `tracey::report()` creates a report and returns its physical address.
- `tracey::view(addr)` views given report address.
- `tracey::badalloc()` throws a bad_alloc() exception, if possible.
- `tracey::fail(msg)` shows given error then fail.
- `tracey::nop()` does allocate/watch/forget/free some memory.
- `tracey::summary()` returns a brief summary.
- `tracey::version()` returns current version.
- `tracey::url()` returns project repository.
- `tracey::settings()` returns current settings.

special notes
-------------
- g++ users: both `-std=c++0x` and `-lpthread` may be required when compiling `tracey.cpp`

Sample
------
```c++
// tracey is callstack based. no dirty new/delete macro tricks.
// tracey is a static library. requires no source modification. just link it.

void *make_leaks() {
    return new int [400];
}

int main( int argc, const char **argv ) {
    int *make_leak = new int();
    make_leaks();

    return 0;
}
```

Possible outputs (msvc/g++/clang)
---------------------------------
```
D:\prj\tracey>rem LINUX: g++ sample.cc tracey.cpp -g -lpthread -std=c++0x
D:\prj\tracey>rem APPLE: clang++ sample.cc tracey.cpp -g -o sample -std=c++11 -stdlib=libc++
D:\prj\tracey>rem WINDOWS following
D:\prj\tracey>cl sample.cc tracey.cpp /MDd /Zi
D:\prj\tracey>sample
<tracey/tracey.cpp> says: generated with tracey-0.20.b (https://github.com/r-lyeh/tracey)
<tracey/tracey.cpp> says: best viewed on foldable text editor (like SublimeText2) with tabs=2sp and no word-wrap
<tracey/tracey.cpp> says: error, 2 leaks found; 1604 bytes wasted ('lame' score)
[2] top-bottom normal tree (useful to find leak beginnings) (1604)
    [1] RtlInitializeExceptionChain (1604)
        [1] RtlInitializeExceptionChain (1604)
            [1] BaseThreadInitThunk (1604)
                [1] mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 403) (1604)
                    [2] __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 501) (4)
                        [1] initterm (4)
                            [1] `dynamic initializer for 'static_leak'' (c:\prj\tracey\sample.cc, line 3) (4)
                                [1] operator new (c:\prj\tracey\tracey.cpp, line 1569) (4)
                                    [1] tracey::watch (c:\prj\tracey\tracey.cpp, line 1429) (4)
                                        [1] tracey::`anonymous namespace'::tracer (c:\prj\tracey\tracey.cpp, line 1413) (4)
                    [2] __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 586) (1600)
                        [1] main (c:\prj\tracey\sample.cc, line 6) (1600)
                            [1] operator new[] (c:\prj\tracey\tracey.cpp, line 1573) (1600)
                                [1] tracey::watch (c:\prj\tracey\tracey.cpp, line 1429) (1600)
                                    [1] tracey::`anonymous namespace'::tracer (c:\prj\tracey\tracey.cpp, line 1413) (1600)
[2] bottom-top normal tree (useful to find leak endings) (1604)
    [1] tracey::`anonymous namespace'::tracer (c:\prj\tracey\tracey.cpp, line 1413) (1604)
        [1] tracey::watch (c:\prj\tracey\tracey.cpp, line 1429) (1604)
            [2] operator new (c:\prj\tracey\tracey.cpp, line 1569) (4)
                [1] `dynamic initializer for 'static_leak'' (c:\prj\tracey\sample.cc, line 3) (4)
                    [1] initterm (4)
                        [1] __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 501) (4)
                            [1] mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 403) (4)
                                [1] BaseThreadInitThunk (4)
                                    [1] RtlInitializeExceptionChain (4)
                                        [1] RtlInitializeExceptionChain (4)
            [2] operator new[] (c:\prj\tracey\tracey.cpp, line 1573) (1600)
                [1] main (c:\prj\tracey\sample.cc, line 6) (1600)
                    [1] __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 586) (1600)
                        [1] mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 403) (1600)
                            [1] BaseThreadInitThunk (1600)
                                [1] RtlInitializeExceptionChain (1600)
                                    [1] RtlInitializeExceptionChain (1600)
```
