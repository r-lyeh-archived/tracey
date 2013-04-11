tracey
======

- Lightweight and simple memory leak finder. Requires C++11.
- Easy to integrate. No source modification is required.
- Clean. Macroless and callstack based. No new/delete macro hacks.
- Lazy. Should work out of the box. Plenty configuration options are provided anyways.
- Tiny. One header and one source file.
- Cross-platform. No extra dependencies.
- MIT licensed.

Implementation directives (optional)
------------------------------------
- `kTraceyAlloc` defaults to `std::malloc()` if not overriden. All allocations merge to this symbol.
- `kTraceyFree` defaults to `std::free()` if not overriden. All deallocations merge to this symbol.
- `kTraceyPrint` defaults to `fprintf(stderr,...)` if not overriden. All reports merge to this symbol.
- `kTraceyAssert` defaults to `assert()` if not overriden. All exceptions merge to this symbol.

Configuration directives (optional)
-----------------------------------
- `kTraceyAllocMultiplier` defaults to `1` if not overriden (should be >= 1). Tracy uses this value to increase memory requirements, and to simulate and to debug worse memory conditions.
- `kTraceyReportWildPointers` defaults to `1` if not overriden. When enabled, Tracey Warns about deallocations on pointers that have been not allocated by Tracey (wild pointers).
- `kTraceyReportNullPointers` defaults to `0` if not overriden. When enabled, Tracey warns about deallocations on null pointers.
- `kTraceyDefineMemoryOperators` defaults to `1` if not overriden. When enabled, Tracey implements all new/new[]/delete/delete[] operators.
- `kTraceyEnabledOnStart` defaults to `1` if not overriden. When enabled, Tracey starts on app init.
- `kTraceyReportOnExit` defaults to `1` if not overriden. When enabled, Tracey shows a report automatically on app exit.

Runtime API (optional)
----------------------

- `tracey::watch()` tells Tracey to watch a memory address.
- `tracey::forget()` tells Tracey to forget about a memory address.
- `tracey::enable()` tells Tracey to enable/disable whole watching system.
- `tracey::disable()` tells Tracey to disable whole watching system.
- `tracey::is_enabled()` returns Tracey's working status.
- `tracey::invalidate()` tells Tracey to forget whole execution.
- `tracey::report()` tells Tracey to show current report.

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
D:\prj\tracey&gt;cl sample.cc tracey.cpp /MD /Zi -DkTraceyReportOnExit=1
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
