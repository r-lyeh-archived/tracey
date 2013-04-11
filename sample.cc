// memtracer is callstack based. no more dirty new/delete macro tricks.
// memtracer is a static library. no more library integration.

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

/* Posible output:

/prj/memtracer> test.exe
<moon9/memtracer/memtracer.cpp> says: Beginning of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)
1) Leak 4 bytes [0048F200] backtrace 1/2 (50%)
        main (d:\prj\moon9\src\moon9\memtracer\memtracer.sample.cpp, line 11)
        __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 536)
        mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 377)
        BaseThreadInitThunk
        RtlInitializeExceptionChain
        RtlInitializeExceptionChain
2) Leak 1600 bytes [004AC9B0] backtrace 2/2 (100%)
        make_leaks (d:\prj\moon9\src\moon9\memtracer\memtracer.sample.cpp, line 6)
        main (d:\prj\moon9\src\moon9\memtracer\memtracer.sample.cpp, line 14)
        __tmainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 536)
        mainCRTStartup (f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c, line 377)
        BaseThreadInitThunk
        RtlInitializeExceptionChain
        RtlInitializeExceptionChain
<moon9/memtracer/memtracer.cpp> says: End of report. Error, 2 leaks found; 1604 bytes wasted ('lame' score)

[
{"backtrace": "1/2", "bytes": 4, "address": "0048F200", "callstack": [
    "main (d:\\prj\\moon9\\src\\moon9\\memtracer\\memtracer.sample.cpp, line 11)",
    "__tmainCRTStartup (f:\\dd\\vctools\\crt_bld\\self_x86\\crt\\src\\crtexe.c, line 536)",
    "mainCRTStartup (f:\\dd\\vctools\\crt_bld\\self_x86\\crt\\src\\crtexe.c, line 377)",
    "BaseThreadInitThunk",
    "RtlInitializeExceptionChain",
    "RtlInitializeExceptionChain",
""]},
{"backtrace": "2/2", "bytes": 1600, "address": "004AC9B0", "callstack": [
    "make_leaks (d:\\prj\\moon9\\src\\moon9\\memtracer\\memtracer.sample.cpp, line 6)",
    "main (d:\\prj\\moon9\\src\\moon9\\memtracer\\memtracer.sample.cpp, line 14)",
    "__tmainCRTStartup (f:\\dd\\vctools\\crt_bld\\self_x86\\crt\\src\\crtexe.c, line 536)",
    "mainCRTStartup (f:\\dd\\vctools\\crt_bld\\self_x86\\crt\\src\\crtexe.c, line 377)",
    "BaseThreadInitThunk",
    "RtlInitializeExceptionChain",
    "RtlInitializeExceptionChain",
""]},
["end of report", "2 leaks found", "1604 bytes wasted", "score: lame"]
]

*/

