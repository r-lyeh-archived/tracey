/*
 * Simple and lightweight memory leak detector
 * Copyright (c) 2011,2012,2013 Mario 'rlyeh' Rodriguez

 * Callstack code is based on code by Magnus Norddahl (ClanLib)

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * Features:
 * - tiny
 * - clean
 * - callstack based
 * - lazy. should work with no source modification
 * - macroless API. no new/delete macros
 * - embeddable. just include this source file in your project

 * To do:
 * - support for malloc/realloc/free

 * References:
 * - http://www.codeproject.com/KB/cpp/allocator.aspx
 * - http://drdobbs.com/cpp/184403759

 * - rlyeh ~~ listening to Long Distance Calling / Metulsky Curse Revisited
 */

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <cctype>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// headers

#ifdef _WIN32
#   include <Windows.h>
#   if defined(DEBUG) || defined(_DEBUG)
#       include <crtdbg.h>
#   endif
#   include <dbghelp.h>
#   pragma comment(lib, "dbghelp.lib")
#else
#   include <unistd.h>
#   include <sys/time.h>
#   include <sys/types.h>
//  --
#   if defined(HAVE_SYS_SYSCTL_H) && \
        !defined(_SC_NPROCESSORS_ONLN) && !defined(_SC_NPROC_ONLN)
#       include <sys/param.h>
#       include <sys/sysctl.h>
#   endif
//  --
#   ifndef __APPLE__
#       include <execinfo.h>
#   endif
//  --
#   include <cxxabi.h>
#endif

// messages

#ifdef _MSC_VER
#    pragma message( "<tracey/tracey.cpp> says: do not forget /Zi, /Z7 or /C7 compiler settings! /Oy- also helps" )
#endif

#ifdef __GNUC__
#    warning "<tracey/tracey.cpp> says: do not forget -g -rdynamic compiler settings!"
#endif

//    flag  | directives            | working?
// ---------+-----------------------+-------------
//    /MT   | _MT                   | no (crt)
//    /MTd  | _MT && _DEBUG         | no (crt)
//    /MD   | _MT && _DLL           | yes
//    /MDd  | _MT && _DLL && _DEBUG | yes
#if defined( _WIN32 ) && defined( _MSC_VER )
#   if defined( _MT ) && !defined( _DLL )
#       if !defined( _DEBUG )
#           error Incompatible C Run-Time libraries compiler flag detected ( /MT ) : Use /MD instead
#       else
#           error Incompatible C Run-Time libraries compiler flag detected ( /MTd ) : Use /MDd instead
#       endif
#   endif
#endif

// OS utils

#ifdef _WIN32
#   define $windows(...)   __VA_ARGS__
#   define $welse(...)
#   define $defined
#else
#   define $windows(...)
#   define $welse(...)     __VA_ARGS__
#endif

#ifdef __APPLE__
#   define $apple(...)     __VA_ARGS__
#   define $aelse(...)
#   define $defined
#else
#   define $apple(...)
#   define $aelse(...)     __VA_ARGS__
#endif

#ifdef __linux__
#   define $linux(...)     __VA_ARGS__
#   define $lelse(...)
#   define $defined
#else
#   define $linux(...)
#   define $lelse(...)     __VA_ARGS__
#endif

#ifndef $defined
#   define $undefined(...) __VA_ARGS__
#else
#   define $undefined(...)
#   undef  $defined
#endif

// Compiler utils

#ifdef __GNUC__
#   define $gnuc(...)      __VA_ARGS__
#   define $gelse(...)
#   define $defined
#else
#   define $gnuc(...)
#   define $gelse(...)     __VA_ARGS__
#endif

#ifdef _MSC_VER
#   define $msvc(...)      __VA_ARGS__
#   define $melse(...)
#   define $defined
#else
#   define $msvc(...)
#   define $melse(...)     __VA_ARGS__
#endif

#ifndef $defined
#   define $other(...)     __VA_ARGS__
#else
#   define $other(...)
#   undef  $defined
#endif

#if defined(NDEBUG) || defined(_NDEBUG)
#   define $release(...) __VA_ARGS__
#   define $debug(...)
#else
#   define $debug(...) __VA_ARGS__
#   define $release(...)
#endif

namespace tracey
{
    class string : public std::string
    {
        public:

        // basic constructors

        string() : std::string()
        {}

        template<size_t N>
        string( const char (&cstr)[N] ) : std::string( cstr )
        {}

        // constructor sugar

        template< typename T >
        string( const T &t ) : std::string()
        {
            operator=(t);
        }

        // extended constructors; safe formatting

        private:
        template<unsigned N>
        void safefmt( const std::string &fmt, std::string *t ) {
            for( std::string::const_iterator it = fmt.begin(), end = fmt.end(); it != end; ++it ) {
                unsigned index(*it);
                if( index <= N ) t[0] += t[index];
                else t[0] += *it;
            }
            assign( t[0] );
        }
        public:

        template< typename T1 >
        string( const std::string &fmt, const T1 &t1 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1) };
            safefmt<1>( fmt, t );
        }

        template< typename T1, typename T2 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1), string(t2) };
            safefmt<2>( fmt, t );
        }

        template< typename T1, typename T2, typename T3 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3) };
            safefmt<3>( fmt, t );
        }

        template< typename T1, typename T2, typename T3, typename T4 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4) };
            safefmt<4>( fmt, t );
        }

        template< typename T1, typename T2, typename T3, typename T4, typename T5 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4), string(t5) };
            safefmt<5>( fmt, t );
        }

        template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4), string(t5), string(t6) };
            safefmt<6>( fmt, t );
        }

        template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7 ) : std::string()
        {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4), string(t5), string(t6), string(t7) };
            safefmt<7>( fmt, t );
        }

        // chaining operators

        template< typename T >
        string &operator +=( const T &t )
        {
            this->assign( this->str() + string(t) );
            return *this;
        }

        string &operator +=( std::ostream& ( *pf )(std::ostream&))
        {
            return *pf == static_cast<std::ostream& ( * )(std::ostream&)>( std::endl ) ? (*this) += "\n", *this : *this;
        }

        // assignment sugars

        template< typename T >
        string &operator=( const T &t )
        {
            std::stringstream ss;
            ss.precision( std::numeric_limits< long double >::digits10 + 1 );
            if( ss << /* std::boolalpha << */ t )
                this->assign( ss.str() );
            return *this;
        }

        string &operator=( const char *t )
        {
            this->assign( t ? t : std::string() );
            return *this;
        }

        std::string str() const
        {
            return *this;
        }

        size_t count( const std::string &substr ) const
        {
            size_t n = 0;
            std::string::size_type pos = 0;
            while( (pos = this->find( substr, pos )) != std::string::npos ) {
                n++;
                pos += substr.size();
            }
            return n;
        }

        std::deque< string > tokenize( const std::string &chars ) const
        {
            std::string map( 256, '\0' );

            for( std::string::const_iterator it = chars.begin(), end = chars.end(); it != end; ++it )
                map[ *it ] = '\1';

            std::deque< string > tokens;

            tokens.push_back( string() );

            for( int i = 0, end = this->size(); i < end; ++i )
            {
                unsigned char c = at(i);

                std::string &str = tokens.back();

                if( !map.at(c) )
                    str.push_back( c );
                else
                if( str.size() )
                    tokens.push_back( string() );
            }

            while( tokens.size() && !tokens.back().size() )
                tokens.pop_back();

            return tokens;
        }

    };

    class strings : public std::deque< string >
    {
        public:

            strings()
            {}

            template <typename contained>
            strings( const std::deque<contained> &c )
            {
                operator=( c );
            }

            template <typename contained>
            strings &operator =( const std::deque<contained> &c )
            {
                this->resize( c.size() );

                std::copy( c.begin(), c.end(), this->begin() );

                return *this;
            }

            template <typename contained>
            strings( const std::vector<contained> &c )
            {
                operator=( c );
            }

            template <typename contained>
            strings &operator =( const std::vector<contained> &c )
            {
                this->resize( c.size() );

                std::copy( c.begin(), c.end(), this->begin() );

                return *this;
            }

            strings( const int &argc, const char **&argv )
            {
                for( int i = 0; i < argc; ++i )
                    this->push_back( argv[i] );
            }

            strings( const int &argc, char **&argv )
            {
                for( int i = 0; i < argc; ++i )
                    this->push_back( argv[i] );
            }

            template< typename T, const size_t N >
            strings( const T (&args)[N] )
            {
                this->resize( N );
                for( int n = 0; n < N; ++n )
                    (*this)[ n ] = args[ n ];
            }

            template< typename T > strings( const T &t0, const T &t1 )
            { this->resize(2); (*this)[0] = t0; (*this)[1] = t1; }
            template< typename T > strings( const T &t0, const T &t1, const T &t2 )
            { this->resize(3); (*this)[0] = t0; (*this)[1] = t1; (*this)[2] = t2; }
            template< typename T > strings( const T &t0, const T &t1, const T &t2, const T &t3 )
            { this->resize(3); (*this)[0] = t0; (*this)[1] = t1; (*this)[2] = t2; (*this)[3] = t3; }
            template< typename T > strings( const T &t0, const T &t1, const T &t2, const T &t3, const T &t4 )
            { this->resize(3); (*this)[0] = t0; (*this)[1] = t1; (*this)[2] = t2; (*this)[3] = t3; (*this)[4] = t4; }

            const string &at( const int &pos ) const
            {
                //extended behaviour
                int size = this->size();
                if( !size ) //throw std::out_of_range("strings::at() empty");
                { static std::map< const strings *, string > map; return ( ( map[ this ] = map[ this ] ) = string() ); }
                return *( this->begin() + ( pos >= 0 ? pos % size : size - 1 + ((pos+1) % size) ) );
            }

            string &at( const int &pos )
            {
                //extended behaviour
                int size = this->size();
                if( !size ) //throw std::out_of_range("strings::at() empty");
                { static std::map< const strings *, string > map; return ( ( map[ this ] = map[ this ] ) = string() ); }
                return *( this->begin() + ( pos >= 0 ? pos % size : size - 1 + ((pos+1) % size) ) );
            }

            operator std::deque<std::string>() const //faster way to do this? reinterpret_cast?
            {
                std::deque<std::string> out;
                out.resize( this->size() );
                std::copy( this->begin(), this->end(), out.begin() );
                return out;
            }

            operator std::vector<std::string>() const //faster way to do this? reinterpret_cast?
            {
                std::vector<std::string> out;
                out.resize( this->size() );
                std::copy( this->begin(), this->end(), out.begin() );
                return out;
            }

            // @todo
            // subset( 0, EOF )
            // subset( N, EOF )
            // subset( 0, -1 ) -> 0, EOF - 1
            // subset( -2, -1 ) -> EOF-2, EOF-1

            std::string str( const char *format1 = "\1\n" ) const
            {
                if( this->size() == 1 )
                    return *this->begin();

                std::string out;

                for( const_iterator it = this->begin(); it != this->end(); ++it )
                    out += string( format1, (*it) );

                return out;
            }
    };

    // Block/unblock local resource to grant current thread exclusive access
    // Block/unblock a shared/global resource to grant current thread exclusive access

    class mutex
    {
        public:

        mutex() : locked_by( nobody() )
        {}

        void lock() {
            if( locked_by != me() ) {
                self.lock();
                locked_by = me();
            }
        }

        void unlock() {
            if( locked_by == me() ) {
                locked_by = nobody();
                self.unlock();
            }
        }

        bool try_lock() {
            if( locked_by != me() ) {
                if( !self.try_lock() )
                    return false;
                locked_by = me();
            }
            return true;
        }

        bool is_locked() const {
            return locked_by != nobody();
        }

        bool is_locked_by_me() const {
            return locked_by == me();
        }

        private:

        std::thread::id locked_by;

$linux(
        std::recursive_mutex self;
)
$lelse(
        std::mutex self;
)

        std::thread::id nobody() const { // return empty/invalid thread
            return std::thread::id();
        }

        std::thread::id me() const { // return current thread id
            return std::this_thread::get_id();
        }

        // Private copy-constructor, copy-assignment

        mutex( const mutex & );
        mutex& operator=( const mutex & );
    };

    std::string demangle( const std::string &name ) {
        $windows({
        char demangled[1024];
        return (UnDecorateSymbolName(name.c_str(), demangled, sizeof( demangled ), UNDNAME_COMPLETE)) ? demangled : name;
        })
        $gnuc({
        char demangled[1024];
        size_t sz = sizeof(demangled);
        int status;
        abi::__cxa_demangle(name.c_str(), demangled, &sz, &status);
        return !status ? demangled : name;
        })
        $linux({
        FILE *fp = popen( (std::string("echo -n \"") + name + std::string("\" | c++filt" )).c_str(), "r" );
        if (!fp) { return name; }
        char demangled[1024];
        char *line_p = fgets(demangled, sizeof(demangled), fp);
        pclose(fp);
        return line_p;
        })
        $undefined(
        return name;
        )
    }

    class callstack
    {
        public:

            // save
            callstack();

            // print
            std::string str( const char *format12 = "#\1 \2\n", size_t skip_initial = 0 );

        private:

            enum { max_frames = 32 };
            void *frames[max_frames];
            size_t num_frames;
    };

    namespace
    {
        size_t capture_stack_trace(int frames_to_skip, int max_frames, void **out_frames, unsigned int *out_hash = 0)
        {
$windows(
                if (max_frames > 32)
                        max_frames = 32;

                unsigned short capturedFrames = 0;

                // RtlCaptureStackBackTrace is only available on Windows XP or newer versions of Windows
                typedef WORD(NTAPI FuncRtlCaptureStackBackTrace)(DWORD, DWORD, PVOID *, PDWORD);

                static struct raii
                {
                    raii() : module(0), ptrRtlCaptureStackBackTrace(0)
                    {
                        module = LoadLibraryA("kernel32.dll");
                        if( module )
                            ptrRtlCaptureStackBackTrace = (FuncRtlCaptureStackBackTrace *)GetProcAddress(module, "RtlCaptureStackBackTrace");
                    }
                    ~raii() { if(module) FreeLibrary(module); }

                    HMODULE module;
                    FuncRtlCaptureStackBackTrace *ptrRtlCaptureStackBackTrace;
                } module;

                if (module.ptrRtlCaptureStackBackTrace)
                        capturedFrames = module.ptrRtlCaptureStackBackTrace(frames_to_skip+1, max_frames, out_frames, (DWORD *) out_hash);

                if (capturedFrames == 0 && out_hash)
                        *out_hash = 0;

                return capturedFrames;
)
$gnuc(
                // Ensure the output is cleared
                memset(out_frames, 0, (sizeof(void *)) * max_frames);

                if (out_hash)
                        *out_hash = 0;

                return (backtrace(out_frames, max_frames));
)
$undefined(
                return 0;
)
        }

        tracey::strings get_stack_trace(void **frames, int num_frames)
        {
$windows(
            // this mutex is used to prevent race conditions.
            // however, it is constructed with heap based plus placement-new just to meet next features:
            // a) ready to use before program begins.
            // our memtracer uses callstack() and mutex is ready to use before the very first new() is made.
            // b) ready to use after program ends.
            // our memtracer uses callstack() when making the final report after the whole program has finished.
            // c) allocations free: memory is taken from heap, and constructed thru placement new
            // we will avoid recursive deadlocks that would happen in a new()->memtracer->callstack->new()[...] scenario.
            // d) leaks free: zero global allocations are made.
            // we don't polute memmanager/memtracer reports with false positives.
            static std::mutex *mutex = 0;
            if( !mutex )
            {
                static char placement[ sizeof(std::mutex) ];
                mutex = (std::mutex *)placement; // no leak and no memory traced :P
                new (mutex) std::mutex();        // memtraced recursion safe; we don't track placement-news
            }

            mutex->lock();

            BOOL result = SymInitialize(GetCurrentProcess(), NULL, TRUE);
            if (!result)
                return mutex->unlock(), tracey::strings();

            tracey::strings backtrace_text;
            for( int i = 0; i < num_frames; i++ )
            {
                char buffer[sizeof(IMAGEHLP_SYMBOL64) + 128];
                IMAGEHLP_SYMBOL64 *symbol64 = reinterpret_cast<IMAGEHLP_SYMBOL64*>(buffer);
                memset(symbol64, 0, sizeof(IMAGEHLP_SYMBOL64) + 128);
                symbol64->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
                symbol64->MaxNameLength = 128;

                DWORD64 displacement = 0;
                BOOL result = SymGetSymFromAddr64(GetCurrentProcess(), (DWORD64) frames[i], &displacement, symbol64);
                if (result)
                {
                    IMAGEHLP_LINE64 line64;
                    DWORD displacement = 0;
                    memset(&line64, 0, sizeof(IMAGEHLP_LINE64));
                    line64.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                    result = SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64) frames[i], &displacement, &line64);
                    if (result)
                    {
                        std::string line = tracey::string(symbol64->Name) + " (" + line64.FileName + ", line " + tracey::string(line64.LineNumber) + ")";
                        backtrace_text.push_back( line );
                    }
                    else
                    {
                        backtrace_text.push_back(symbol64->Name);
                    }
                }
            }

            SymCleanup(GetCurrentProcess());
            return mutex->unlock(), backtrace_text;
)
$gnuc(
            char **strings;
            strings = backtrace_symbols(frames, num_frames);
            if (!strings)
            {
                return tracey::strings();
            }

            tracey::strings backtrace_text;

            for( int cnt = 0; cnt < num_frames; cnt++ )
            {
                // Decode the strings
                char *ptr = strings[cnt];
                char *filename = ptr;
                const char *function = "";

                // Find function name
                while(*ptr)
                {
                    if (*ptr=='(')  // Found function name
                    {
                        *(ptr++) = 0;
                        function = ptr;
                        break;
                    }
                    ptr++;
                }

                // Find offset
                if (function[0])        // Only if function was found
                {
                    while(*ptr)
                    {
                        if (*ptr=='+')  // Found function offset
                        {
                            *(ptr++) = 0;
                            break;
                        }
                        if (*ptr==')')  // Not found function offset, but found, end of function
                        {
                            *(ptr++) = 0;
                            break;
                        }
                        ptr++;
                    }
                }

                int status;
                char *new_function = abi::__cxa_demangle(function, 0, 0, &status);
                if (new_function)       // Was correctly decoded
                {
                    function = new_function;
                }

                backtrace_text.push_back( tracey::string(function) + " (" + tracey::string(filename) + ")" );

                if (new_function)
                {
                    free(new_function);
                }
            }

            free (strings);
            return backtrace_text;
)
$undefined(
            return tracey::strings();
)
        }
    }

    namespace
    {
        bool is_callstack_available = false;
    }

    callstack::callstack() // save
    {
        // When calling callstack().str() from a secondary thread will retrieve empty stack until a callstack is retrieved from main thread.
        // This happens on my machine (Win7, 64bit, msvc 2011) under some circumstances. So I've added '100 tries' instead of 'try once'.
        // - rlyeh // @Todo: examine & fix issue

        static size_t tries = 0;
        if( (!is_callstack_available) && tries < 100 )
        {
            ++tries;

            num_frames = tracey::capture_stack_trace( 0, max_frames, frames );
            for( int i = num_frames; i < max_frames; ++i ) frames[ i ] = 0;

            tracey::strings stack_trace = tracey::get_stack_trace( frames, num_frames );
            tracey::string out;

            for( size_t i = 0; i < stack_trace.size(); i++ )
                out += stack_trace[i] + '|';

            is_callstack_available = ( out.count("callstack") > 0 );
        }

        if( !is_callstack_available )
            return;

        num_frames = tracey::capture_stack_trace( 1, max_frames, frames );
        for( int i = num_frames; i < max_frames; ++i ) frames[ i ] = 0;
    }

    std::string callstack::str( const char *format12, size_t skip_initial )
    {
        tracey::string out;

        if( !is_callstack_available )
            return out;

        tracey::strings stack_trace = tracey::get_stack_trace( frames, num_frames );

        for( size_t i = skip_initial; i < stack_trace.size(); i++ )
            out += tracey::string( format12, (int)i, stack_trace[i] );

        return out;
    }
}

// } #include "tracey.inl"

#ifndef kTraceyAllocMultiplier
/*  Used to increase memory requirements, and to simulate and to debug worse memory conditions */
/*  Should be equal or bigger than 1.0                                                         */
#   define kTraceyAllocMultiplier 1.0
#endif

#ifndef kTraceyAlloc
/*  Behaviour is undefined at end of program if kTraceyAlloc() implementation uses any global C++ symbol */
#   define kTraceyAlloc(size)      std::malloc(size)
#endif

#ifndef kTraceyFree
/*  Behaviour is undefined at end of program if kTraceyFree() implementation uses any global C++ symbol */
#   define kTraceyFree(ptr)        std::free(ptr)
#endif

#ifndef kTraceyPrint
/*  Behaviour is undefined at end of program if kTraceyPrint() implementation uses any C++ global symbol, like std::cout */
#   define kTraceyPrint(str)       fprintf( stderr, "%s", tracey::string( str ).c_str() )
#endif

#ifndef kTraceyAssert
/*  Behaviour is undefined at end of program if kTraceyAssert() implementation uses any C++ global symbol */
#   define kTraceyAssert(expr)     assert(expr)
#endif

#ifndef kTraceyBadAlloc
#   define kTraceyBadAlloc()       std::bad_alloc()
#endif

#ifndef kTraceyReportWildPointers
#   define kTraceyReportWildPointers 1
#endif

#ifndef kTraceyReportNullPointers
#   define kTraceyReportNullPointers 0
#endif

#ifndef kTraceyDefineMemoryOperators
#   define kTraceyDefineMemoryOperators 1
#endif

#ifndef kTraceyReportOnExit
#   define kTraceyReportOnExit 1
#endif

#ifndef kTraceyEnabledOnStart
#   define kTraceyEnabledOnStart 1
#endif

namespace tracey
{
    namespace
    {
        void show_report( const std::string &header, const std::string &body )
        {
            kTraceyPrint( header );
            kTraceyPrint( body );
        }

        void show_report( const std::string &header, const std::string &body, const std::string &footer )
        {
            kTraceyPrint( header );
            kTraceyPrint( body );
            kTraceyPrint( footer );
        }

        volatile size_t timestamp_id = 0;

        size_t create_id()
        {
            static size_t id = 0;
            return ++id;
        }

        struct leak
        {
            size_t size, id;

            leak( const size_t _size = 0 ) : size( _size ), id( create_id() )
            {}
        };

        typedef std::pair< leak *, tracey::callstack * > leak_full;

        volatile bool enabled = kTraceyEnabledOnStart;

        void *trace( void *ptr, const size_t &size )
        {
            //* Implementation details:
            //* - mutex is used to discard any coming recursive allocation call
            //* - mutex is constructed thru std::malloc+placement-new to avoid infinite recursion
            //* - is_busy is used to avoid recursive trace. Every `new` called inside trace will be ignore.

            static volatile bool initialized = false;
            static volatile bool is_busy = false;
            static tracey::mutex *mutex = 0;

            if( !ptr )
                return ptr;

            if( !mutex )
            {
                //mutex = (tracey::mutex *) std::malloc( sizeof(tracey::mutex) ); //kTraceyAlloc( sizeof(tracey::mutex) );
                static char placement[ sizeof(tracey::mutex) ]; mutex = (tracey::mutex *)placement;
                new (mutex) tracey::mutex();    // recursion safe; we don't track placement-news
                initialized = true;
            }

            if( !initialized )
                return ptr;

            if( !enabled )
                return ptr;

            mutex->lock();
            // If `trace` was called inside trace it will be ignore because of is_busy
            if( is_busy ) {
				// If the mutex is acquired and is_busy is true it means this_thread is holding
				// the mutex, so it should not be release.
                return ptr;
            }

            is_busy = true;

            {
                static
                class container : public std::map< const void *, leak_full, std::less< const void * > > //, tracey::malloc_allocator< std::pair< const void *, leak * > > >
                {
                    public:

                    container()
                    {}

                    ~container()
                    {
                        enabled = false;

#                       if kTraceyReportOnExit
                            _report();
#                       endif

                        _clear();
                    }

                    void _clear()
                    {
                        for( iterator it = this->begin(), end = this->end(); it != end; ++it )
                        {
                            if( it->second.first )
                                delete it->second.first, it->second.first = 0;

                            if( it->second.second )
                                delete it->second.second, it->second.second = 0;
                        }

                        this->clear();
                    }

                    void _report() const
                    {
#                       ifdef _WIN32
                            AllocConsole();
#                       endif

                        // this should happen at the very end of a program (even *after* static memory unallocation)
                        // @todo: avoid using any global object like std::cout/cerr (because some implementations like "cl /MT" will crash)

                        // count number of leaks, filter them and sort them.

                        struct tuple
                        {
                            const void *addr;
                            leak *lk;
                            tracey::callstack *cs;
                        };

                        typedef std::map< size_t, tuple > map;
                        map sort_by_id;

                        size_t n_leak = 0, wasted = 0;
                        for( const_iterator it = this->begin(), end = this->end(); it != end; ++it )
                        {
                            const void *the_address = it->first;
                            leak *the_leak = it->second.first;
                            tracey::callstack *the_callstack = it->second.second;

                            if( the_leak && the_callstack /* && it->second->size != ~0 */ )
                            {
                                if( the_leak->id >= timestamp_id )
                                {
                                    ++n_leak;
                                    wasted += the_leak->size;

                                    tuple t = { the_address, the_leak, the_callstack };

                                    ( sort_by_id[ the_leak->id ] = sort_by_id[ the_leak->id ] ) = t;
                                }
                            }
                        }

                        // show score

                        double leaks_pct = this->size() ? n_leak * 100.0 / this->size() : 0.0;
                        std::string score = "perfect!";
                        if( leaks_pct >  0.00 ) score = "good";
                        if( leaks_pct >  1.25 ) score = "excellent";
                        if( leaks_pct >  2.50 ) score = "poor";
                        if( leaks_pct >  5.00 ) score = "mediocre";
                        if( leaks_pct > 10.00 ) score = "lame";

                        tracey::string header, body, footer;

                        header = tracey::string( "<tracey/tracey.cpp> says: Beginning of report. \1, \2 leaks found; \3 bytes wasted ('\4' score)\r\n", !n_leak ? "Ok" : "Error", n_leak, wasted, score );

                        kTraceyPrint( header );

                        // inspect leaks (may take a while)

                        size_t ibegin = 0, iend = sort_by_id.size(), percent = ~0, current = 0;
                        for( map::const_iterator it = sort_by_id.begin(), end = sort_by_id.end(); it != end; ++it )
                        {
                            const void *the_address = it->second.addr;
                            leak *the_leak = it->second.lk;
                            tracey::callstack *the_callstack = it->second.cs;

                            current = (size_t)( ++ibegin * 100.0 / iend );

                            /* @todo: move this to an user-defined callback { */

                            tracey::string  line( "\1) Leak \2 bytes [\3] backtrace \4/\5 (\6%)\r\n", ibegin, the_leak->size, the_address, ibegin, iend, percent = current );
                            tracey::strings lines = tracey::string( the_callstack->str("\2\n", 2) ).tokenize("\n");

                            for( size_t i = 0; i < lines.size(); ++i )
                                line += tracey::string( "\t\1\r\n", lines[i] );

                            kTraceyPrint( line );

                            /* }: @todo */

                            body += line;
                        }

                        footer = tracey::string( "<tracey/tracey.cpp> says: End of report. \1, \2 leaks found; \3 bytes wasted ('\4' score)\r\n", !n_leak ? "Ok" : "Error", n_leak, wasted, score );

                        kTraceyPrint( footer );

                        // body report (linux & macosx will display windows line endings right)

                        sort_by_id.clear();

                        // show_report( header, body, footer );
                    }
                } map;

                container::iterator it = map.find( ptr );
                bool found = ( map.find( ptr ) != map.end() );

                if( size == ~0 )
                {
                    if( found )
                    {
                        //map[ ptr ]->~leak();
                        //kTraceyFree( map[ ptr ] );

                        if( map[ ptr ].first )
                            delete map[ ptr ].first, map[ ptr ].first = 0;

                        if( map[ ptr ].second )
                            delete map[ ptr ].second, map[ ptr ].second = 0;
                    }
                    else
                    {
                        // 1st) wild/null pointer deallocation found; warn user

#                       if kTraceyReportWildPointers
                            if( ptr )
                                show_report( tracey::string( "<tracey/tracey.cpp> says: Error, wild pointer deallocation.\r\n" ), tracey::callstack().str("\t\1\r\n", 4) );
#                       endif
#                       if kTraceyReportNullPointers
                            // unreachable code
                            if( !ptr )
                                show_report( tracey::string( "<tracey/tracey.cpp> says: Error, null pointer deallocation.\r\n" ), tracey::callstack().str("\t\1\r\n", 4) );
#                       endif

                        // 2nd) normalize ptr for further deallocation (deallocating null pointers is ok)
                        ptr = 0;
                    }
                }
                else
                if( size == (~0) - 1 )
                {
                    map._report();
                }
                else
                if( size == (~0) - 2 )
                {
                    map._clear();
                    timestamp_id = create_id();
                }
                else
                {
                    if( found /* && map[ ptr ] */ )
                    {
                        // 1st) double pointer allocation (why?); warn user
                        // kTraceyFree( map[ ptr ] );
                    }

                    // create a leak and (re)insert it into map

                    map[ ptr ] = map[ ptr ];
                    map[ ptr ] = std::make_pair< leak *, tracey::callstack * >( new leak( size ), new tracey::callstack() );
                }

                is_busy=false;
                mutex->unlock();
            }

            return ptr;
        }
    }

    namespace tracey
    {
        void watch( const void *ptr, size_t size )
        {
            trace( (void *)ptr, size );
        }
        void forget( const void *ptr )
        {
            trace( (void *)ptr, ~0 );
        }
        void enable( bool on )
        {
            enabled = on;
        }
        void disable()
        {
            enabled = false;
        }
        bool is_enabled()
        {
            return enabled;
        }
        void invalidate()
        {
            char unused;
            trace( &unused, (~0) - 2 );
        }
        void report()
        {
            char unused;
            trace( &unused, (~0) - 1 );
        }
        const char *version()
        {
            return "0.1-a";
        }
    }
}

#if kTraceyDefineMemoryOperators

//* Custom memory operators

void *operator new( size_t size, const std::nothrow_t &t ) throw()
{
    void *ptr = tracey::trace( kTraceyAlloc( size*kTraceyAllocMultiplier ), size );

    kTraceyAssert(ptr);
    return ptr;
}

void *operator new[]( size_t size, const std::nothrow_t &t ) throw()
{
    void *ptr = tracey::trace( kTraceyAlloc( size*kTraceyAllocMultiplier ), size );

    kTraceyAssert(ptr);
    return ptr;
}

void operator delete( void *ptr, const std::nothrow_t &t ) throw()
{
    if( ptr ) kTraceyFree( tracey::trace( ptr, ~0 ) );
}

void operator delete[]( void *ptr, const std::nothrow_t &t ) throw()
{
    if( ptr ) kTraceyFree( tracey::trace( ptr, ~0 ) );
}

//* Custom memory operators (w/ exceptions)

void *operator new( size_t size ) //throw(std::bad_alloc)
{
    void *ptr = kTraceyAlloc( size*kTraceyAllocMultiplier );

    if( !ptr )
        throw kTraceyBadAlloc();

    return tracey::trace( ptr, size );
}

void *operator new[]( size_t size ) //throw(std::bad_alloc)
{
    void *ptr = kTraceyAlloc( size*kTraceyAllocMultiplier );

    if( !ptr )
        throw kTraceyBadAlloc();

    return tracey::trace( ptr, size );
}

void operator delete( void *ptr ) throw()
{
    if( ptr ) kTraceyFree( tracey::trace( ptr, ~0 ) );
}

void operator delete[]( void *ptr ) throw()
{
    if( ptr ) kTraceyFree( tracey::trace( ptr, ~0 ) );
}

#endif

#undef kTraceyAllocMultiplier
#undef kTraceyAlloc
#undef kTraceyFree
#undef kTraceyPrint
#undef kTraceyAssert
#undef kTraceyBadAlloc

#undef kTraceyDefineMemoryOperators
#undef kTraceyReportWildPointers
#undef kTraceyReportNullPointers
#undef kTraceyReportOnExit
#undef kTraceyEnabledOnStart

#undef $debug
#undef $release
#undef $other
#undef $melse
#undef $msvc
#undef $gelse
#undef $gnuc

#undef $undefined
#undef $lelse
#undef $linux
#undef $aelse
#undef $apple
#undef $welse
#undef $windows
