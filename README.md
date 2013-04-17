tracey
======

- Lightweight and simple memory C++11 leak finder with no dependencies
- Easy to integrate. No source modification is required.
- Clean. Macroless and callstack based. No new/delete macro hacks.
- Lazy. Should work out of the box. Plenty configuration options are provided anyways.
- Tiny. One header and one source file.
- Cross-platform. OS dependencies only. No third party dependencies.
- MIT licensed.

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

Sample
------
<pre>
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
</pre>

Possible output
---------------
<pre>
D:\prj\tracey&gt;cl sample.cc tracey.cpp /MDd /Zi && rem g++ sample.cc tracey.cpp -g -rdynamic -std=c++0x
D:\prj\tracey&gt;sample
&lt;tracey/tracey.cpp&gt; says: Beginning of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
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
&lt;tracey/tracey.cpp&gt; says: End of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
D:\prj\tracey&gt;
</pre>
