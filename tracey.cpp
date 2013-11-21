/*
 * Tracey, a simple and lightweight memory leak detector
 * Copyright (c) 2011,2012,2013 Mario 'rlyeh' Rodriguez
 *
 * Callstack code is based on Magnus Norddahl's work (ClanLib).
 * Tiny webserver code is based on Ivan Tikhonov's work.

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
 * - embeddable. just link it against your project
 * - C hooks (optional) (win32 only)
 * - C++ hooks (optional)
 * - web server (optional)

 * To do:
 * - support for C hooks on non-win archs:
 *   - http://stackoverflow.com/questions/262439/create-a-wrapper-function-for-malloc-and-free-in-c
 *   - http://src.chromium.org/svn/trunk/src/tools/memory_watcher/memory_hook.cc
 *   - void __f () { } // Do something.
 *     void f () __attribute__ ((weak, alias ("__f")));
 *   - or http://man7.org/linux/man-pages/man3/malloc_hook.3.html
 *   - as seen in https://github.com/emeryberger/Heap-Layers/blob/master/wrappers/gnuwrapper.cpp
 * - memory manager checks
 *   - Read before allocating memory
 *   - Write before allocating memory
 *   - Read before beginning of allocation
 *   - Write before beginning of allocation
 *   - Read after end of allocation
 *   - Write after end of allocation
 *   - Read after deallocation
 *   - Write after deallocation
 *   - Failure to deallocate memory
 *   - Deallocating memory twice
 *   - Deallocating nonallocated memory
 *   - Zero-size memory allocation
 * - optional pools for microallocations
 * - reports per:
 *   - symbol: namespace/class/function
 *   - author: blame, candidates (caller and callee for highest apparitions founds on blame(symbol(ns/cl/fn)))
 *   - location: filename, directory
 *   - origin: module/thread
 *   - wasted size, % total, hits
 * - ignores per project/session:
 *   - symbol: namespace/class/function
 *   - author: blame, candidates
 *   - location: filename, directory
 *   - origin: module/thread
 * - versioning support (svn/git...)
 * - cloud account linking support (github/gitorius/etc):
 *   - auto commit/pull symbol maps
 *   - auto commit/pull settings
 *   - auto commit reports
 * - auto updates on ui: load webserver templates from official github repo

 * References:
 * - http://www.codeproject.com/KB/cpp/allocator.aspx
 * - http://drdobbs.com/cpp/184403759

 * - rlyeh ~~ listening to Long Distance Calling / Metulsky Curse Revisited
 */

// A few tweaks before loading STL on MSVC

#ifdef _SECURE_SCL
#undef _SECURE_SCL
#endif
#define _SECURE_SCL 0
#ifdef _HAS_ITERATOR_DEBUGGING
#undef _HAS_ITERATOR_DEBUGGING
#endif
#define _HAS_ITERATOR_DEBUGGING 0

// Include C, then C++

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <new>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1700)
//  if C++11 is supported include std::mutex and std::thread...
#   include <mutex>
#   include <thread>
#else
//  ...else legacy C++
#   include "tinythread.h" // header not found? copy tinythread++ sources to tracey's folder!
    namespace std { using namespace tthread; }
#endif

// System headers

#if defined(_WIN32) || defined(_WIN64)
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <Windows.h>
#   // unwinding
#   if defined(DEBUG) || defined(_DEBUG)
#       include <CrtDbg.h>
#   endif
#   include <DbgHelp.h>
#   pragma comment(lib, "dbghelp.lib")
#   if defined(__GNUC__)
#       undef __GNUC__                         // switch MingW users here from $gnuc() to $windows()
#   endif
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
#   include <execinfo.h>
//  --
#   include <cxxabi.h>
#endif


// Our API

#include "tracey.hpp"

#if 1
#   // Disable optimizations. Nothing gets inlined. You get pleasant stacktraces to work with.
#   // This is the default setting.
#   ifdef _MSC_VER
#       pragma optimize( "gsy", off )       // disable optimizations on msvc
#   else
#       pragma OPTIMIZE OFF                 // disable optimizations on gcc 4.4+
#   endif
#else
#   // Enable optimizations. Tracey performs better. However, functions get inlined (specially new/delete operators).
#   // This behaviour is disabled by default, since you get weird stacktraces.
#   ifdef _MSC_VER
#       pragma optimize( "gsy", on )        // enable optimizations on msvc
#   else
#       pragma GCC optimize                 // enable optimizations on gcc 4.4+
#       pragma optimize                     // enable optimizations on a few other compilers, hopefully
#   endif
#endif

// OS utils. Here is where the fun starts... good luck

#define $no(...)
#define $yes(...)     __VA_ARGS__

#if defined(_WIN32) || defined(_WIN64)
#   define $windows   $yes
#   define $welse     $no
#else
#   define $windows   $no
#   define $welse     $yes
#endif

#ifdef __APPLE__
#   define $apple     $yes
#   define $aelse     $no
#else
#   define $apple     $no
#   define $aelse     $yes
#endif

#ifdef __linux__
#   define $linux     $yes
#   define $lelse     $no
#else
#   define $linux     $no
#   define $lelse     $yes
#endif

// Compiler utils

#if defined(NDEBUG) || defined(_NDEBUG)
#   define $release   $yes
#   define $debug     $no
#else
#   define $release   $no
#   define $debug     $yes
#endif

#ifdef __GNUC__
#   define $gnuc      $yes
#   define $gelse     $no
#else
#   define $gnuc      $no
#   define $gelse     $yes
#endif

#ifdef _MSC_VER
#   define $msvc      $yes
#   define $melse     $no
#else
#   define $msvc      $no
#   define $melse     $yes
#endif

#ifdef __clang__
#   define $clang     $yes
#   define $celse     $no
#else
#   define $clang     $no
#   define $celse     $yes
#endif

#define $on(v)        (0 v(+1))  // usage: #if $on($msvc)
#define $is           $on        // usage: #if $is($debug)
#define $has(...)     $clang(__has_feature(__VA_ARGS__)) $celse(__VA_ARGS__) // usage: #if $has(cxx_exceptions)

// create a $warning(...) macro

#if $on($msvc)
#   define $warning(msg) __pragma( message( msg ) )
#elif defined(__GNUC__) && defined(GCC_VERSION) && GCC_VERSION >= 40400
#   define $warning$message$impl(msg) _Pragma(#msg)
#   define $warning(msg) $warning$message$impl( message( msg ) )
#else
#   define $warning(msg)
#endif

// try to detect if exceptions are enabled...

#if (defined(_HAS_EXCEPTIONS) && (_HAS_EXCEPTIONS > 0)) || \
    (defined(_STLP_USE_EXCEPTIONS) && (_STLP_USE_EXCEPTIONS > 0)) || \
    (defined(HAVE_EXCEPTIONS)) || \
    (defined(__EXCEPTIONS)) || \
    ($has(cxx_exceptions)) /*(__has_feature(cxx_exceptions))*/
#   define $throw     $yes
#   define $telse     $no
#else
#   define $throw     $no
#   define $telse     $yes
#endif

// Reminders for retrieving symbols

#  if $on($msvc)
    $warning( "<tracey/tracey.cpp> says: do not forget /Zi, /Z7 or /C7 compiler settings! /Oy- also helps!" )
#elif $on($clang)
    $warning( "<tracey/tracey.cpp> says: do not forget -g compiler setting!" )
#elif $on($gnuc)
    $warning( "<tracey/tracey.cpp> says: do not forget -g -lpthread compiler settings!" )
#endif

// Constants and settings; feel free to tweak these.

#ifndef kTraceyRealloc
/*  Behaviour is undefined at end of program if kTraceyRealloc() implementation relies on C++ runtime */
#   define kTraceyRealloc               std::realloc
#endif

#ifndef kTraceyMemset
/*  Behaviour is undefined at end of program if kTraceyMemset() implementation relies on C++ runtime */
#   define kTraceyMemset                std::memset
#endif

#ifndef kTraceyPrintf
/*  Behaviour is undefined at end of program if kTraceyPrintf() implementation relies on C++ runtime */
#   define kTraceyPrintf                std::printf
#endif

#ifndef kTraceyDie
/*  Behaviour is undefined at end of program if kTraceyDie() implementation relies on C++ runtime */
#   define kTraceyDie                   $windows( FatalExit ) $welse( std::exit )
#endif

#ifndef kTraceyFile
/*  Behaviour is undefined at end of program if kTraceyFile() implementation relies on C++ runtime */
#   define kTraceyFile                  std::FILE
#endif

#ifndef kTraceyfOpen
/*  Behaviour is undefined at end of program if kTraceyfOpen() implementation relies on C++ runtime */
#   define kTraceyfOpen                 std::fopen
#endif

#ifndef kTraceyfPrintf
/*  Behaviour is undefined at end of program if kTraceyfPrintf() implementation relies on C++ runtime */
#   define kTraceyfPrintf               std::fprintf
#endif

#ifndef kTraceyfClose
/*  Behaviour is undefined at end of program if kTraceyfClose() implementation relies on C++ runtime */
#   define kTraceyfClose                std::fclose
#endif

#ifndef kTraceyAssert
/*  Behaviour is undefined at end of program if kTraceyAssert() implementation relies on C++ runtime */
#   define kTraceyAssert                assert
#endif

#ifndef kTraceyAllocsOverhead
/*  Used to simulate bigger memory requirements  */
/*  Should be equal or bigger than 1.0           */
#   define kTraceyAllocsOverhead        1.0
#endif

#ifndef kTraceyMaxStacktraces
#   define kTraceyMaxStacktraces        128
#endif

#ifndef kTraceyStacktraceSkipBegin
#   define kTraceyStacktraceSkipBegin   0 // $windows(4) $welse(0)
#endif

#ifndef kTraceyStacktraceSkipEnd
#   define kTraceyStacktraceSkipEnd     0 // $windows(4) $welse(0)
#endif

#ifndef kTraceyCharLinefeed
#   define kTraceyCharLinefeed          "\n"
#endif

#ifndef kTraceyCharTab
#   define kTraceyCharTab               "\t"
#endif

#ifndef kTraceyReportWildPointers
#   define kTraceyReportWildPointers    0
#endif

#ifndef kTraceyDefineMemoryOperators
#   define kTraceyDefineMemoryOperators 1
#endif

#ifndef kTraceyMemsetAllocations
#   define kTraceyMemsetAllocations     1
#endif

#ifndef kTraceyReportOnExit
#   define kTraceyReportOnExit          1
#endif

#ifndef kTraceyWebserver
#   define kTraceyWebserver             1
#endif

#ifndef kTraceyWebserverPort
#   define kTraceyWebserverPort         2001
#endif

#ifndef kTraceyHookLegacyCRT
#   define kTraceyHookLegacyCRT         0
#endif

#ifndef kTraceyEnabled
#   define kTraceyEnabled               1
#endif

// checks: todo
// if /MD or /MDd and kTraceyReportWildPointers warn user "not a good idea"
#if kTraceyHookLegacyCRT
    $warning( "<tracey/tracey.cpp> says: kTraceyHookLegacyCRT option ignored. CRT hooking not supported on this platform.")
#   undef  kTraceyHookLegacyCRT
#   define kTraceyHookLegacyCRT 0
#endif

// Implementation

namespace tracey
{
    static void webmain( void * );
    static void keymain( void * );

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
        string( const T &t ) : std::string() {
            operator=(t);
        }

        // extended constructors; safe formatting

        private:
        template<unsigned N>
        std::string &safefmt( const std::string &fmt, std::string (&t)[N] ) {
            for( std::string::const_iterator it = fmt.begin(), end = fmt.end(); it != end; ++it ) {
                unsigned index(*it);
                if( index <= N ) t[0] += t[index];
                else t[0] += *it;
            }
            return t[0];
        }
        public:

        template< typename T1 >
        string( const std::string &fmt, const T1 &t1 ) : std::string() {
            std::string t[] = { std::string(), string(t1) };
            assign( safefmt( fmt, t ) );
        }

        template< typename T1, typename T2 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2 ) : std::string() {
            std::string t[] = { std::string(), string(t1), string(t2) };
            assign( safefmt( fmt, t ) );
        }

        template< typename T1, typename T2, typename T3 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3 ) : std::string() {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3) };
            assign( safefmt( fmt, t ) );
        }

        template< typename T1, typename T2, typename T3, typename T4 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4 ) : std::string() {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4) };
            assign( safefmt( fmt, t ) );
        }

        template< typename T1, typename T2, typename T3, typename T4, typename T5 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5 ) : std::string() {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4), string(t5) };
            assign( safefmt( fmt, t ) );
        }

        template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6 ) : std::string() {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4), string(t5), string(t6) };
            assign( safefmt( fmt, t ) );
        }

        template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
        string( const std::string &fmt, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7 ) : std::string() {
            std::string t[] = { std::string(), string(t1), string(t2), string(t3), string(t4), string(t5), string(t6), string(t7) };
            assign( safefmt( fmt, t ) );
        }

        // chaining operators

        template< typename T >
        string &operator +=( const T &t ) {
            append( string(t) );
            return *this;
        }

        template< typename T >
        string &operator <<( const T &t ) {
            append( string(t) );
            return *this;
        }

        string &operator <<( std::ostream &( *pf )(std::ostream &) ) {
            return *pf == static_cast<std::ostream& ( * )(std::ostream&)>( std::endl ) ? (*this) += "\n", *this : *this;
        }

        // assignment sugars

        template< typename T >
        string &operator=( const T &t ) {
            std::stringstream ss;
            ss.precision( std::numeric_limits< long double >::digits10 + 1 );
            if( ss << t )
                this->assign( ss.str() );
            return *this;
        }

        string &operator=( const char *t ) {
            assign( t ? t : "" );
            return *this;
        }

        std::string str() const {
            return *this;
        }

        string replace( const std::string &target, const std::string &replacement ) const {
            size_t found = 0;
            std::string s = *this;

            while( ( found = s.find( target, found ) ) != string::npos ) {
                s.replace( found, target.length(), replacement );
                found += replacement.length();
            }

            return s;
        }
    };

    class strings : public std::deque< string >
    {
        public:

        strings( unsigned size = 0 ) : std::deque< string >( size )
        {}

        template <typename contained>
        strings( const std::deque<contained> &c ) : std::deque< string >() {
            operator=( c );
        }

        template <typename contained>
        strings &operator =( const std::deque<contained> &c ) {
            this->resize( c.size() );
            std::copy( c.begin(), c.end(), this->begin() );
            return *this;
        }

        std::string str( const char *format1 = "\1\n" ) const {
            if( this->size() == 1 )
                return *this->begin();

            std::string out;

            for( const_iterator it = this->begin(); it != this->end(); ++it )
                out += string( format1, (*it) );

            return out;
        }

        std::string flat() const {
            return str( "\1" );
        }
    };

    //#include <map>
    //#include <iostream>

    // tree class
    // [] is read-only
    // () is read-writeable (so is <<)

    template< typename K, typename V = int >
    class tree : public std::map< K, tree<K,V> > {

            typedef typename std::map< K, tree<K,V> > map;

            template< typename T >
            T zero() {
                return std::pair<T,T>().first;
            }

            template<typename T>
            T &invalid() const {
                static T t;
                return t = T(), t;
            }

            V value;
            tree *parent;

        public:

            tree() : map(), value(zero<V>()) {
                parent = this;
            }

            tree( const tree &t ) : map(), value(zero<V>()) {
                parent = this;
                operator=(t);
            }

            // tree clone

            tree &operator=( const tree &t ) {
                if( this != &t ) {
                    this->clear();
                    get() = zero<V>();
                    operator+=(t);
                }
                return *this;
            }

            // tree merge

            tree &operator+=( const tree &t ) {
                if( this != &t ) {
                    for( typename tree::const_iterator it = t.begin(), end = t.end(); it != end; ++it ) {
                        this->map::insert( *it );
                    }
                    get() = t.get();
                }
                return *this;
            }

            // tree search ; const safe find: no insertions on new searches

            const tree &at( const K &t ) const {
                typename map::const_iterator find = this->find( t );
                return find != this->end() ? find->second : invalid<tree>();
            }

            // tree insertion

            tree &insert( const K &t ) {
                map &children = *this;
                ( children[t] = children[t] ).parent = this;
                return children[t];
            }
            tree &erase( const K &t ) {
                typename map::iterator find = this->find(t);
                if( find != this->end() ) this->map::erase(t);
                return *this;
            }

            // recursive values

            V &get() {
                return value;
            }
            const V &get() const {
                return value;
            }

            template<typename other>
            tree &set( const other &t ) {
                get() = t;
                return *this;
            }
            template<typename other>
            tree &setup( const other &t ) {
                if( !is_root() ) {
                    up().set(t).setup(t);
                }
                return *this;
            }
            template<typename other>
            tree &setdown( const other &t ) {
                for( typename tree::iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    it->second.set(t).setdown( t );
                }
                return *this;
            }

            V getdown() const {
                V value = get();
                for( typename tree::const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    value += it->second.getdown();
                }
                return value;
            }

            // sugars

            tree &clone( const tree &t ) {
                return operator=( t );
            }
            tree &assign( const tree &t ) {
                return operator=( t );
            }

            template<typename other>
            tree &operator=( const other &t ) {
                return set(t);
            }
            template<typename other>
            tree &operator+=( const other &t ) {
                return get() += t, *this;
            }

            tree &merge( const tree &t ) {
                return operator +=( t );
            }
            const tree &operator[]( const K &t ) const {
                return at( t );
            }
            tree &operator()( const K &t ) {
                return insert( t );
            }

            bool has( const K &t ) const { // @todo: subhas
                return this->find(t) != this->end();
            }

            bool operator!() const {
                return this == &invalid<tree>();
            }

            const map &children() const {
                return *this;
            }
            map &children() {
                return *this;
            }

            tree &up() {
                return *parent;
            }
            const tree &up() const {
                return *parent;
            }

            bool is_root() const {
                return parent == this;
            }

            // tools
#if 0
            template<typename ostream>
            void print( ostream &cout = std::cout, unsigned depth = 0 ) const {
                std::string tabs( depth, kTraceyCharTab[0] );
                for( typename tree::const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    cout << tabs << "[" << this->size() << "] " << it->first << " (" << it->second.get() << ")\n"; // << std::endl;;
                    it->second.print( cout, depth + 1 );
                }
            }

            template<typename ostream, typename U>
            void print( const std::map< K, U > &tmap, ostream &cout = std::cout, unsigned depth = 0 ) const {
                std::string tabs( depth, kTraceyCharTab[0] );
                for( typename tree::const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    cout << tabs << "[" << this->size() << "] " << tmap.find( it->first )->second << " (" << it->second.get() << ")\n"; // << std::endl;;
                    it->second.print( tmap, cout, depth + 1 );
                }
            }
#else
            template<typename U>
            void print( const std::map< K, U > &tmap, kTraceyFile *cout, unsigned depth = 0 ) const {
                std::string tabs( depth, kTraceyCharTab[0] );
                for( typename tree::const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    kTraceyfPrintf( cout, "%s", (tracey::string() << tabs << "[" << this->size() << "] " << tmap.find( it->first )->second << " (" << it->second.get() << ")\n" ).c_str() ); // << std::endl;;
                    it->second.print( tmap, cout, depth + 1 );
                }
            }
#endif

            template<typename U>
            tree<U> rekey( const std::map< K, U > &map ) const {
                tree<U> utree;
                for( typename tree::const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    typename std::map< K, U >::const_iterator find = map.find( it->first );
                    assert( find != map.end() );
                    utree( find->second ) += it->second.rekey( map );
                    utree( find->second ).get() = it->second.get();
                }
                return utree;
            }
            template<typename U>
            tree<U> rekey( const std::map< U, K > &map ) const {
                // this could be faster
                tree<U> utree;
                for( typename std::map< U, K >::const_iterator it = map.begin(), end = map.end(); it != end; ++it ) {
                    typename tree::const_iterator find = this->find( it->second );
                    if( find == this->end() ) continue;
                    utree( it->first ) += find->second.rekey( map );
                    utree( it->first ).get() = find->second.get();
                }
                return utree;
            }

            tree collapse() const {
                tree t;
                if( this->size() == 1 ) {
                    return this->begin()->second.collapse();
                } else {
                    for( typename tree::const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                        t( it->first ) += it->second.collapse();
                        t( it->first ).get() = it->second.get();
                    }
                }
                return t;
            }

            V refresh() {
                V value = this->empty() ? get() : zero<V>();
                for( typename tree::iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    value += it->second.refresh();
                }
                return get() = value;
            }
    };

    namespace {
        template<typename T>
        std::ostream &operator <<( std::ostream &os, const tree<T> &t ) {
            return t.print(os), os;
        }
    }

    struct branch {
        size_t hits;
        size_t size;
        double total;
        branch() : hits(0), size(0), total(0) {
        }
        branch( const branch &other ) {
            operator=( other );
        }
        branch &operator =( const branch &other ) {
            if( this != &other ) {
                hits = other.hits;
                size = other.size;
                total = other.total;
            }
            return *this;
        }
        branch &operator +=( const branch &other ) {
            this->size += (other.hits ? other.hits : 1) * other.size;
            this->total += other.total;
            return *this;
        }
    };

    namespace {
        std::ostream &operator <<( std::ostream &os, const branch &t ) {
            std::string human;
            size_t size = t.size;
            /**/ if( size >= 1024 * 1024 * 1024 ) human = tracey::string("\1 GB", size / (1024 * 1024 * 1024));
            else if( size >=        1024 * 1024 ) human = tracey::string("\1 MB", size / (       1024 * 1024));
            else if( size >=          10 * 1024 ) human = tracey::string("\1 KB", size / (              1024));
            else                                  human = tracey::string("\1 bytes", size );
            /**/ if( t.hits > 1 ) return os << human << " * " << (t.hits) << " .. " << int(t.total) << "%";
            else                  return os << human                      << " .. " << int(t.total) << "%";
        }
    }

    std::string demangle( const std::string &mangled ) {
    $linux({
        $no( /* c++filt way */
        FILE *fp = popen( (std::string("echo -n \"") + mangled + std::string("\" | c++filt" )).c_str(), "r" );
        if (!fp) { return mangled; }
        char demangled[1024];
        char *line_p = fgets(demangled, sizeof(demangled), fp);
        pclose(fp);
        return demangled;
        )
        $yes( /* addr2line way. wip & quick proof-of-concept. clean up required. */
        tracey::string binary = mangled.substr( 0, mangled.find_first_of('(') );
        tracey::string address = mangled.substr( mangled.find_last_of('[') + 1 );
        address.pop_back();
        tracey::string cmd( "addr2line -e \1 \2", binary, address );
        FILE *fp = popen( cmd.c_str(), "r" );
        if (!fp) { return mangled; }
        char demangled[1024];
        char *line_p = fgets(demangled, sizeof(demangled), fp);
        pclose(fp);
        tracey::string demangled_(demangled);
        if( demangled_.size() ) demangled_.pop_back(); //remove \n
        return demangled_.size() && demangled_.at(0) == '?' ? mangled : demangled_;
        )
    })
    $apple({
        std::stringstream ss;
        if( !(ss << mangled) )
            return mangled;
        std::string number, filename, address, funcname, plus, offset;
        if( !(ss >> number >> filename >> address >> funcname >> plus >> offset) )
            return mangled;
        int status = 0;
        char *demangled = abi::__cxa_demangle(funcname.c_str(), NULL, NULL, &status);
        tracey::string out( mangled );
        if( status == 0 && demangled )
            out = out.replace( funcname, demangled );
        if( demangled ) free( demangled );
        return out;
    })
    $windows({
        char demangled[1024];
        return (UnDecorateSymbolName(mangled.c_str(), demangled, sizeof( demangled ), UNDNAME_COMPLETE)) ? std::string(demangled) : mangled;
    })
    $gnuc({
        std::string out;
        int status = 0;
        char *demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);
        out = ( status == 0 && demangled ? std::string(demangled) : mangled );
        if( demangled ) free( demangled );
        return out;
    })
        return mangled;
    }

    struct callstack {
        enum { max_frames = kTraceyMaxStacktraces };
        std::vector<void *> frames;

        callstack( bool autosave = false ) {
            if( autosave ) save();
        }

        size_t space() const {
            return sizeof(frames) + sizeof(void *) * frames.size();
        }

        void save( unsigned frames_to_skip = 0 ) {

            if( frames_to_skip > max_frames )
                return;

            frames.clear();
            frames.resize( max_frames, (void *)0 );
            void **out_frames = &frames[0]; // .data();

            $windows({
                unsigned short capturedFrames = 0;

                // RtlCaptureStackBackTrace is only available on Windows XP or newer versions of Windows
                typedef WORD(NTAPI FuncRtlCaptureStackBackTrace)(DWORD, DWORD, PVOID *, PDWORD);

                static struct raii
                {
                    raii() : module(0), ptrRtlCaptureStackBackTrace(0)
                    {
                        module = LoadLibraryA("kernel32.dll");
                        if( !module )
                            tracey::fail( "<tracey/tracey.cpp> says: error! cant load kernel32.dll" );

                        ptrRtlCaptureStackBackTrace = (FuncRtlCaptureStackBackTrace *)GetProcAddress(module, "RtlCaptureStackBackTrace");
                        if( !ptrRtlCaptureStackBackTrace )
                            tracey::fail( "<tracey/tracey.cpp> says: error! cant find RtlCaptureStackBackTrace() process address" );
                    }
                    ~raii() { if(module) FreeLibrary(module); }

                    HMODULE module;
                    FuncRtlCaptureStackBackTrace *ptrRtlCaptureStackBackTrace;
                } module;

                if( module.ptrRtlCaptureStackBackTrace )
                    capturedFrames = module.ptrRtlCaptureStackBackTrace(frames_to_skip+1, max_frames, out_frames, (DWORD *) 0);

                frames.resize( capturedFrames );
                std::vector<void *>(frames).swap(frames);
                return;
            })
            $gnuc({
                // Ensure the output is cleared
                kTraceyMemset(out_frames, 0, (sizeof(void *)) * max_frames);

                frames.resize( backtrace(out_frames, max_frames) );
                std::vector<void *>(frames).swap(frames);
                return;
            })
        }

        tracey::strings unwind( unsigned from = 0, unsigned to = ~0 ) const
        {
            if( to == ~0 )
                to = this->frames.size();

            if( from > to || from > this->frames.size() || to > this->frames.size() )
                return tracey::strings();

            const size_t num_frames = to - from;
            tracey::strings backtraces( num_frames );

            void * const * frames = &this->frames[ from ];
            const std::string invalid = "????";

            $windows({
                SymSetOptions(SYMOPT_UNDNAME);

                $no(
                    // polite version. this is how things should be done.
                    HANDLE process = GetCurrentProcess();
                    if( SymInitialize( process, NULL, TRUE ) )
                )
                $yes(
                    // this is what we have to do because other memory managers are not polite enough. fuck them off
                    static HANDLE process = GetCurrentProcess();
                    static int init = SymInitialize( process, NULL, TRUE );
                    if( !init )
                        tracey::fail( "<tracey/tracey.cpp> says: cannot initialize Dbghelp.lib" );
                )
                {
                    enum { MAXSYMBOLNAME = 512 - sizeof(IMAGEHLP_SYMBOL64) };
                    char symbol64_buf     [ 512 ];
                    char symbol64_bufblank[ 512 ] = {0};
                    IMAGEHLP_SYMBOL64 *symbol64       = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbol64_buf);
                    IMAGEHLP_SYMBOL64 *symbol64_blank = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbol64_bufblank);
                    symbol64_blank->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
                    symbol64_blank->MaxNameLength = (MAXSYMBOLNAME-1) / 2; //wchar?

                    IMAGEHLP_LINE64 line64, line64_blank = {0};
                    line64_blank.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

                    for( unsigned i = 0; i < num_frames; i++ ) {
                        *symbol64 = *symbol64_blank;
                        DWORD64 displacement64 = 0;

                        if( SymGetSymFromAddr64( process, (DWORD64) frames[i], &displacement64, symbol64 ) ) {
                            line64 = line64_blank;
                            DWORD displacement = 0;
                            if( SymGetLineFromAddr64( process, (DWORD64) frames[i], &displacement, &line64 ) ) {
                                backtraces[i] = tracey::string( "\1 (\2, line \3)", symbol64->Name, line64.FileName, line64.LineNumber );
                            } else {
                                backtraces[i] = symbol64->Name;
                            }
                        } else  backtraces[i] = invalid;
                    }

                    $no(
                        // fuck the others. cleanup commented.
                        SymCleanup(process);
                    )
                }
                DWORD error = GetLastError();

                return backtraces;
            })
            $gnuc({
                char **strings = backtrace_symbols(frames, num_frames);

                // Decode the strings
                if( strings ) {
                    for( unsigned i = 0; i < num_frames; i++ ) {
                        backtraces[i] = ( strings[i] ? demangle(strings[i]) : invalid );
                    }
                    free( strings );
                }

                return backtraces;
            })

            return backtraces;
        }

        tracey::strings str( const char *format12 = "#\1 \2\n", size_t skip_begin = 0 ) {
            tracey::strings stacktrace = unwind( skip_begin );

            for( size_t i = 0, end = stacktrace.size(); i < end; i++ )
                stacktrace[i] = tracey::string( format12, i + 1, stacktrace[i] );

            return stacktrace;
        }
    };

    template<typename T>
    std::string lookup( T *ptr ) {
        tracey::callstack cs;
        cs.frames.push_back( (void *)ptr );
        tracey::strings stacktrace = cs.unwind();
        return stacktrace.size() ? stacktrace[0] : std::string("????");
    }
}

namespace tracey
{
    namespace
    {
        volatile size_t timestamp_id = 0;
        struct stats_t {
            size_t usage, usage_peak, num_leaks, leak_peak, overhead;
            stats_t() : usage(0), usage_peak(0), num_leaks(0), leak_peak(0), overhead(0) {}
            std::string str() const {
                return tracey::string("highest peak: \1 Kb total, \2 Kb greatest peak // \3 allocs in use: \4 Kb + overhead: \5 Kb = total: \6 Kb",
                                    stats.usage_peak/1024, stats.leak_peak/1024, stats.num_leaks, stats.usage/1024, stats.overhead/1024, stats.usage/1024 + stats.overhead/1024 );
            }
        } stats;

        size_t create_id() {
            static size_t id = 0;
            return ++id;
        }

        bool view_report( const std::string &html ) {
            $windows( return std::system( tracey::string("start \1", html).c_str() ), true );
            $apple( return std::system( tracey::string("open \1", html).c_str() ), true );
            $linux( return std::system( tracey::string("xdg-open \1", html).c_str() ), true );
            return false;
        }

        std::string get_temp_pathfile() {
            $windows(
                std::string path = std::string( std::getenv("TEMP") ? std::getenv("TEMP") : "." ) + std::tmpnam(0);
                while( path.size() && path.at( path.size() - 1 ) == '.' ) path.resize( path.size() - 1 );
                return path;
            )
            $welse(
                return std::string() + std::tmpnam(0);
            )
        }

        struct leak {
            size_t id, size;
            const void *addr;
            tracey::callstack cs;

            leak() : size(0), id(0), addr(0)
            {}

            void wipe() {
                id = create_id();
                cs = tracey::callstack();
                size = 0;
                addr = 0;
            }

            ~leak() {
                wipe();
            }
        };

        typedef std::vector< const leak * > leaks;

        struct custom_mutex;
        custom_mutex *mutex = 0;

        class container : public std::map< const void *, leak, std::less< const void * > > //, tracey::malloc_allocator< std::pair< const void *, leak * > > >
        {
            public:

            container() {
                static tracey::string dummy( 1.0 ); // forces container to be deallocated after trace
            }

            ~container() {
                mutex = 0;

                // Warning for /MT && kTraceyReportOnExit=1
                // A crash is likely to happen at this point on some configurations depending on the C/C++ runtime implementation.
                // Just because we are going to use the runtime C/C++ library (in _report()) *after* it has been probably deallocated.
                // This only happens when a static runtime library is linked (see flag /MT) and kTraceyReportOnExit is defined to 1.
                // In order to fix this either use another runtime library (like /MTd, /MD or /MDd), or define kTraceyReportOnExit to 0.
                // For now, I've made a workaround by enforcing runtime to be deallocated after us (check dummy string on constructor).
                // So it should work in all cases. Just cross your fingers. Kthx.

                if(kTraceyReportOnExit) {
                    view_report( _report() );
                }

                // We should clear the memory involved in all allocations but after all, we are a memory tracker and we should be the very
                // last thing to deallocate in any program. So we don't do it and we save some time for developing when exiting the app.
                // _clear();

                // Silent exit. Not a requirement in all architectures but who knows. See /MT issue kTraceyPrint() warning above .
                kTraceyDie( __LINE__ );
            }

            void _clear() {

                for( iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    it->second.wipe();
                }

                this->clear();
            }

            leaks collect_leaks( size_t *wasted ) const {
                leaks list;
                *wasted = 0;
                for( const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    const tracey::leak &L = it->second;
                    if( L.addr && L.size && L.id >= timestamp_id ) {
                        *wasted += L.size;
                        list.push_back( &L );
                    }
                }
                return list;
            }

            std::string _report() const {

                std::string logfile = get_temp_pathfile() + "xxx-tracey.html";

                kTraceyPrintf( "%s", tracey::string( "<tracey/tracey.cpp> says: summary: \1" kTraceyCharLinefeed, stats.str() ).c_str() );
                kTraceyPrintf( "%s", tracey::string( "<tracey/tracey.cpp> says: creating report: \1" kTraceyCharLinefeed, logfile).c_str() );
                kTraceyFile *fp = kTraceyfOpen( logfile.c_str(), "wb" );

                // this code often runs at the very end of a program cycle (even when static memory has been deallocated)
                // so, avoid using global C++ objects like std::cout/cerr as much as possible; otherwise crashes may happen

                $windows(
                    AllocConsole();
                )

                // Find leaks
                kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: filtering leaks..." kTraceyCharLinefeed).c_str() );
                size_t wasted, n_leak;
                leaks filtered = collect_leaks( &wasted );
                n_leak = filtered.size();
                kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: found \1 leaks wasting \2 bytes" kTraceyCharLinefeed, n_leak, wasted).c_str() );

                // Calc score
                double leaks_pct = this->size() ? n_leak * 100.0 / this->size() : 0.0;
                std::string score = "perfect!";
                if( leaks_pct >  0.00 ) score = "excellent";
                if( leaks_pct >  1.25 ) score = "good";
                if( leaks_pct >  2.50 ) score = "poor";
                if( leaks_pct >  5.00 ) score = "mediocre";
                if (leaks_pct > 10.00 ) score = "lame";

                // Header
                kTraceyfPrintf( fp, "%s", tracey::string( "<html><body><xmp>" ).c_str() );
                kTraceyfPrintf( fp, "%s", tracey::string( "<tracey/tracey.cpp> says: generated with \1 (\2)" kTraceyCharLinefeed, tracey::version(), tracey::url() ).c_str() );
                kTraceyfPrintf( fp, "%s", tracey::string( "<tracey/tracey.cpp> says: best viewed on foldable text editor (like SublimeText2) with tabs=2sp and no word-wrap" kTraceyCharLinefeed ).c_str() );
                kTraceyfPrintf( fp, "%s", tracey::string( "<tracey/tracey.cpp> says: \1, \2 leaks found; \3 bytes wasted ('\4' score)" kTraceyCharLinefeed, !n_leak ? "ok" : "error", n_leak, wasted, score ).c_str() );
                kTraceyfPrintf( fp, "%s", tracey::string( "<tracey/tracey.cpp> says: summary: \1" kTraceyCharLinefeed, stats.str() ).c_str() );
                kTraceyfPrintf( fp, "%s", tracey::string( "<tracey/tracey.cpp> says: report filename: \1" kTraceyCharLinefeed, logfile).c_str() );

                // Body
                // Get all frame addresses involved in all leaks
                // Also, create a tree of frames; so we will take decisions from above by examining node weights (@todo)
                kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: creating trees of frames..."  kTraceyCharLinefeed).c_str() );
                std::set< void * > set;
                tracey::tree<void *, branch> tree;
                tree( (void *)((~0)-0) ); // bottom-top branch
                tree( (void *)((~0)-1) ); // top-bottom branch
                for( leaks::const_iterator it = filtered.begin(), end = filtered.end(); it != end; ++it ) {
                    const leak &L = **it;
                    const tracey::callstack &callstack = L.cs;
                    tracey::tree<void *, branch> *_tree = &tree((void *)((~0)-0));
                    tracey::tree<void *, branch> *_tree_inv = &tree((void *)((~0)-1));
                    if( callstack.frames.size() )
                    for( unsigned i = 0, start = kTraceyStacktraceSkipBegin, end = callstack.frames.size() - 1 - kTraceyStacktraceSkipEnd; start+i <= end; ++i ) {

                        (*_tree)( callstack.frames[start + i] ).get().size = L.size;
                        (*_tree)( callstack.frames[start + i] ).get().hits ++;
                        (*_tree)( callstack.frames[start + i] ).get().total = 100.0 * L.size / wasted *
                        ( (*_tree)( callstack.frames[start + i] ).get().hits );
                        _tree = &(*_tree)( callstack.frames[start + i] );
                        set.insert( callstack.frames[start + i] );

                        (*_tree_inv)( callstack.frames[end - i] ).get().size = L.size;
                        (*_tree_inv)( callstack.frames[end - i] ).get().hits ++;
                        (*_tree_inv)( callstack.frames[end - i] ).get().total = 100.0 * L.size / wasted *
                        ( (*_tree_inv)( callstack.frames[end - i] ).get().hits );
                        _tree_inv = &(*_tree_inv)( callstack.frames[end - i] );
                        set.insert( callstack.frames[end - i] );
                    }
                }

                // Some apps are low on memory in here, so we free memory as soon as possible
                filtered = leaks();

                if( !set.size() ) {
                    if( n_leak ) {
                        kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: error! failed to resolve symbols." $msvc(" Are PDB files available?") kTraceyCharLinefeed).c_str() );
                    }
                } else {
                    kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: resolving \1 unique frames..." kTraceyCharLinefeed, set.size()).c_str() );
                    // convert set of unique frames into array of frames
                    tracey::callstack cs;
                    std::vector< void * > &frames = cs.frames;
                    frames.reserve( set.size() );
                    for( std::set< void * >::iterator it = set.begin(), end = set.end(); it != end; ++it ) {
                        frames.push_back( *it );
                    }
                    tracey::strings symbols = cs.unwind();
                    std::map< void *, std::string > translate;
                    {
                        if( frames.size() != symbols.size() ) {
                            kTraceyfPrintf( fp, "%s", tracey::string("<tracey/tracey.cpp> says: error! cannot resolve all frames (\1 vs \2)!" kTraceyCharLinefeed, frames.size(), symbols.size() ).c_str() );
                            for( unsigned i = 0, end = frames.size(); i < end; ++i ) {
                                translate[ frames[i] ] = tracey::string("\1", frames[i]);
                            }
                        } else {
                            for( unsigned i = 0, end = frames.size(); i < end; ++i ) {
                                translate[ frames[i] ] = symbols[ i ];
                            }
                        }
                        // Create a tree report, if possible
                        kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: converting tree of frames into tree of symbols..." kTraceyCharLinefeed).c_str() );
#if 1
                        translate[ (void *)((~0)-1) ] = "leak beginnings";
                        translate[ (void *)((~0)-0) ] = "leak endings";
                        kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: flattering tree of symbols..." kTraceyCharLinefeed).c_str() );
                        tree.refresh();
						//tree.collapse().print(translate, fp);
                        tree.print(translate, fp);
#else
                        kTraceyfPrintf( fp, "%s", std::string(64, '-') << "bottom-top tree (leakers)" << std::string(64, '-') << std::en ).c_str()dl;
                        tree.rekey(translate).print(log);
                        kTraceyfPrintf( fp, "%s", std::string(64, '-') << "top-bottom tree (traces)" << std::string(64, '-') << std::en ).c_str()dl;
                        tree_inv.rekey(translate).print(log);
#endif
                    }
                }

                // Footer
                kTraceyfPrintf( fp, "%s", tracey::string( "</xmp></body></html>" ).c_str() );
                kTraceyfClose( fp );

                return logfile;
            }
        };

        void *tracer( void *ptr, size_t &size );

        bool init() {
            static bool once = false; if(! once ) { once = true;
                kTraceyPrintf( "%s", tracey::settings().c_str() );
                if( kTraceyWebserver ) {
                    volatile bool sitdown = false;
                    // std::thread( tracey::webmain, &sitdown ).detach();
                    // old tinythread versions do not support detach, so workaround following:
                    new std::thread( tracey::webmain, (void *)&sitdown );
                    while( !sitdown ) {
                    $windows( Sleep( 1000 ) );
                    $welse( sleep( 1 ) );
                    }
                }
                if( 1 ) {
                    volatile bool sitdown = false;
                    new std::thread( tracey::keymain, (void *)&sitdown );
                    while( !sitdown ) {
                    $windows( Sleep( 1000 ) );
                    $welse( sleep( 1 ) );
                    }
                }
                // Construct internals of tracer (static initializers)
                size_t dummy = 0;
                tracer( 0, dummy );
            }
            return true;
        }

        struct dummy_mutex {
            volatile bool recursive;
            dummy_mutex() : recursive(false) {
            }
            bool try_lock() {
                if( recursive ) {
                    return false;
                }
                recursive = true;
                return true;
            }
            void unlock() { recursive = false; }
        };

        struct complex_mutex {
            std::recursive_mutex first;
            volatile bool recursive;
            complex_mutex() : recursive(false) {
            }

            bool try_lock() {
                // block allocations coming from different threads
                first.lock();

                // discard any recursive allocations
                if( recursive ) {
                    first.unlock();
                    return false;
                }
                recursive = true;
                return true;
            }

            void unlock() {
                recursive = false;
                first.unlock();
            }
        };

#if kTraceyHookLegacyCRT
        struct custom_mutex : dummy_mutex {
#else
        struct custom_mutex : complex_mutex {
#endif
            container map;

            custom_mutex() {
                mutex = (this);
            }

            ~custom_mutex() { mutex = 0; }
        };

        bool is_recursive() {
            if( !mutex )
                return false;

            return mutex->recursive;
        }

        void *tracer( void *ptr, size_t &size )
        {
            if( !kTraceyEnabled )
                return size = 0, ptr;

            { static custom_mutex m; }

            if( !ptr )
                return size = 0, ptr;

            // threads will lock here till the slot is free.
            // threads will return on recursive locks.
            if( !mutex )
                return size = 0, ptr;

            if( !mutex->try_lock() )
                return size = 0, ptr;

#if         kTraceyHookLegacyCRT
            // do nothing
#else
            static const bool init = tracey::init();
#endif

            container &map = mutex->map;
            // ready
            container::iterator it = map.find( ptr );
            bool found = ( it != map.end() );

            if( size == ~0 )
            {
                if( found )
                {
                    leak &L = it->second;
                    stats.overhead -= L.cs.space();
                    stats.usage -= L.size;
                    stats.num_leaks--;
                    L.wipe();
                }
                else
                {
                    // 1st) wild pointer deallocation found; warn user
                    if( kTraceyReportWildPointers )
                        kTraceyPrintf( "%s", (tracey::string( "<tracey/tracey.cpp> says: Error, wild pointer deallocation." kTraceyCharLinefeed ) +
                            tracey::callstack( true ).str( kTraceyCharTab "\1) \2" kTraceyCharLinefeed, kTraceyStacktraceSkipBegin).flat() ).c_str() );

                    // 2nd) normalize ptr for further deallocation (deallocating null pointers is ok)
                    ptr = 0;
                }
            }
            else
            if( size == (~0) - 1 )
            {
                int code = *((int*)ptr);
                //ptr = 0;
                if( code == 1 ) {
                    stats = stats_t();
                    map._clear(), timestamp_id = create_id();
                }

                if( code == 2 ) { *((stats_t*)ptr) = stats; };
                if( code == 3 ) (void)0;
                if( code == 4 ) (void)0;
            }
            else
            if( size == (~0) - 2 )
            {
                static char placement[ sizeof(std::string) ];
                static std::string *log = new ((std::string *)placement) std::string();
                *log = map._report();
                ptr = (void *)log;
            }
            else
            if( size == (~0) - 3 )
            {
                std::string &log = *((std::string *)(ptr));
                view_report( log );
            }
            else
            if( size == (~0) - 4 )
            {
                static char placement[ sizeof(std::string) ];
                static std::string *log = new ((std::string *)placement) std::string();
                *log = stats.str();
                ptr = (void *)log;
            }
            else
            {
                if( found ) {
                    // kTraceyAssert( !map[ptr].first );
                    // kTraceyAssert( !map[ptr].second );
                }

                // create a leak and (re)insert it into map
                tracey::leak &leak = (map[ptr] = map[ptr]);
                leak.wipe();
                leak.addr = ptr;
                leak.size = size;
                leak.cs.save();
                stats.num_leaks++;
                stats.usage += size;
                stats.overhead += leak.cs.space();
                if( leak.size   > stats.leak_peak  ) stats.leak_peak = leak.size;
                if( stats.usage > stats.usage_peak ) stats.usage_peak = stats.usage;
            }

            mutex->unlock();

            return ptr;
        }
    };
}

namespace tracey
{
    void *watch( const void *ptr, size_t size ) {
        tracer( (void *)ptr, size );
        return (void *)ptr;
    }
    void *forget( const void *ptr ) {
        size_t forget = (~0);
        tracer( (void *)ptr, forget );
        return (void *)ptr;
    }
    void restart() {
        size_t opcode = 1, special_fn = (~0) - 1;
        tracer( &opcode, special_fn );
    }
    std::string report() {
        size_t special_fn = (~0) - 2;
        return *((std::string *)tracer( &report, special_fn ));
    }
    void view( const std::string &report ) {
        std::string copy = report;
        size_t special_fn = (~0) - 3;
        tracer( &copy, special_fn );
    }
    std::string summary() {
        size_t special_fn = (~0) - 4;
        return *((std::string *)tracer( &report, special_fn ));
    }
    void fail( const char *message ) {
        kTraceyPrintf( "%s\n", message );
        kTraceyAssert( !"<tracey/tracey.cpp> says: fail() requested" );
        $throw(
            throw std::runtime_error( message );
        )
        $telse(
            kTraceyDie(__LINE__);
        )
    }
    void badalloc() {
        $throw(
            throw std::bad_alloc();
        )
        tracey::fail( "<tracey/tracey.cpp> says: error! out of memory" );
    }
    std::string version() {
        return "tracey-0.21.b";  /* format is major.minor.(a)lpha/(b)eta/(r)elease/(c)andidate */
    }
    std::string url() {
        return "https://github.com/r-lyeh/tracey";
    }
    static void runtime_checks() {
        // if md mdd + wild pointers
        //
    }
    static std::string settings( const std::string &prefix ) {
        if( tracey::lookup(url) == "????" ) {
            tracey::fail( "failed to decode lookups. Is debug information available?" $msvc(" Are .PDB files available?") );
        }
        tracey::string out;
        out += tracey::string( "\1\2 ready" kTraceyCharLinefeed, prefix, tracey::version() );
        out += tracey::string( "\1using \2 as realloc" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyRealloc) );
        out += tracey::string( "\1using \2 as memset" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyMemset) );
        out += tracey::string( "\1using \2 as printf" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyPrintf) );
        out += tracey::string( "\1using \2 as exit" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyDie) );
        out += tracey::string( "\1using \2 as fopen" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyfOpen) );
        out += tracey::string( "\1using \2 as fclose" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyfClose) );
        out += tracey::string( "\1using \2 as fprintf" kTraceyCharLinefeed, prefix, tracey::lookup(kTraceyfPrintf) );
        out += tracey::string( "\1with C++ exceptions=\2" kTraceyCharLinefeed, prefix, $throw("enabled") $telse("disabled") );
        out += tracey::string( "\1with kTraceyAllocsOverhead=x\2" kTraceyCharLinefeed, prefix, kTraceyAllocsOverhead );
        out += tracey::string( "\1with kTraceyMaxStacktraces=\2 range[\3..\4]" kTraceyCharLinefeed, prefix, kTraceyMaxStacktraces, kTraceyStacktraceSkipBegin, kTraceyStacktraceSkipEnd );
        // kTraceyCharLinefeed
        // kTraceyCharTab
        out += tracey::string( "\1with kTraceyReportWildPointers=\2" kTraceyCharLinefeed, prefix, kTraceyReportWildPointers ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyDefineMemoryOperators=\2" kTraceyCharLinefeed, prefix, kTraceyDefineMemoryOperators ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyMemsetAllocations=\2" kTraceyCharLinefeed, prefix, kTraceyMemsetAllocations ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyStacktraceSkipBegin=\2" kTraceyCharLinefeed, prefix, kTraceyStacktraceSkipBegin );
        out += tracey::string( "\1with kTraceyStacktraceSkipEnd=\2" kTraceyCharLinefeed, prefix, kTraceyStacktraceSkipEnd );
        out += tracey::string( "\1with kTraceyReportOnExit=\2" kTraceyCharLinefeed, prefix, kTraceyReportOnExit ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyWebserver=\2" kTraceyCharLinefeed, prefix, kTraceyWebserver ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyWebserverPort=\2" kTraceyCharLinefeed, prefix, kTraceyWebserverPort );
        out += tracey::string( "\1with kTraceyHookLegacyCRT=\2" kTraceyCharLinefeed, prefix, kTraceyHookLegacyCRT );
        out += tracey::string( "\1with kTraceyEnabled=\2" kTraceyCharLinefeed, prefix, kTraceyEnabled );
        return out;
    }
    std::string settings() {
        return tracey::settings("<tracey/tracey.cpp> says: ");
    }
    /*
    // legal
    namespace tracey { const char *const license() { return ::license; } }
    namespace tracey { const char *const features() { return ::features; } }
    namespace tracey { const char *const todo() { return ::todo; } }
    namespace tracey { const char *const references() { return ::references; } }
    namespace tracey { const char *const version() { return "tracey-v0.2.a"; } }
    namespace tracey { const char *const url() { return "https://github.com/r-lyeh/tracey"; } }
    */

    // Extra

    bool nop() {
        tracey::free( tracey::forget( tracey::watch( tracey::malloc( 1 ), 1 ) ) );
        return true;
    }
    bool install_c_hooks() {
        return false;
    }

    void *realloc( void *ptr, size_t resize ) {
        static const bool init = install_c_hooks();

        ptr = kTraceyRealloc( ptr, (size_t)(kTraceyAllocsOverhead * resize) );

        if( !ptr && resize )
            tracey::badalloc();

        return ptr;
    }
    void *malloc( size_t size ) {
        void *ptr = tracey::realloc( 0, size );
        return kTraceyMemsetAllocations ? kTraceyMemset( ptr, 0, size ) : ptr;
    }
    void *calloc( size_t num, size_t size ) {
        return tracey::malloc( num * size );
    }
    void *free( void *ptr ) {
        return tracey::realloc( ptr, 0 );
    }

    void *amalloc( size_t size, size_t alignment ) {
        std::vector<void *> invalids( 1, tracey::malloc(size) );
        if( alignment ) {
#          define is_aligned(POINTER, BYTE_COUNT) \
                (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)
            while( !is_aligned(invalids.back(),alignment) )
                invalids.push_back( tracey::malloc( size ) );
            if( invalids.size() > 1 ) {
                std::swap( invalids.front(), invalids.back() );
                for( unsigned i = 1; i < invalids.size(); ++i ) {
                    tracey::free( invalids[i] );
                }
            }
        }
        return invalids[0];
    }
}

#if kTraceyDefineMemoryOperators

//* Custom memory operators (with no exceptions)

void *operator new( size_t size, const std::nothrow_t &t ) throw() {
    return tracey::watch( tracey::malloc( size ), size );
}

void *operator new[]( size_t size, const std::nothrow_t &t ) throw() {
    return tracey::watch( tracey::malloc( size ), size );
}

void operator delete( void *ptr, const std::nothrow_t &t ) throw() {
    tracey::free( tracey::forget( ptr ) );
}

void operator delete[]( void *ptr, const std::nothrow_t &t ) throw() {
    tracey::free( tracey::forget( ptr ) );
}

//* Custom memory operators (with exceptions)

void *operator new( size_t size ) throw(std::bad_alloc) {
    return tracey::watch( tracey::malloc( size ), size );
}

void *operator new[]( size_t size ) throw(std::bad_alloc) {
    return tracey::watch( tracey::malloc( size ), size );
}

void operator delete( void *ptr ) throw() {
    tracey::free( tracey::forget( ptr ) );
}

void operator delete[]( void *ptr ) throw() {
    tracey::free( tracey::forget( ptr ) );
}

#endif

#if kTraceyWebserver

#if $on($windows)

#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <windows.h>

#   pragma comment(lib,"ws2_32.lib")

#   define INIT()                    do { static WSADATA wsa_data; static const int init = WSAStartup( MAKEWORD(2, 2), &wsa_data ); } while(0)
#   define SOCKET(A,B,C)             ::socket((A),(B),(C))
#   define ACCEPT(A,B,C)             ::accept((A),(B),(C))
#   define CONNECT(A,B,C)            ::connect((A),(B),(C))
#   define CLOSE(A)                  ::closesocket((A))
#   define RECV(A,B,C,D)             ::recv((A), (char *)(B), (C), (D))
#   define READ(A,B,C)               ::recv((A), (char *)(B), (C), (0))
#   define SELECT(A,B,C,D,E)         ::select((A),(B),(C),(D),(E))
#   define SEND(A,B,C,D)             ::send((A), (const char *)(B), (int)(C), (D))
#   define WRITE(A,B,C)              ::send((A), (const char *)(B), (int)(C), (0))
#   define GETSOCKOPT(A,B,C,D,E)     ::getsockopt((A),(B),(C),(char *)(D), (int*)(E))
#   define SETSOCKOPT(A,B,C,D,E)     ::setsockopt((A),(B),(C),(char *)(D), (int )(E))

#   define BIND(A,B,C)               ::bind((A),(B),(C))
#   define LISTEN(A,B)               ::listen((A),(B))
#   define SHUTDOWN(A)               ::shutdown((A),2)
#   define SHUTDOWN_R(A)             ::shutdown((A),0)
#   define SHUTDOWN_W(A)             ::shutdown((A),1)

    namespace
    {
        // fill missing api

        enum
        {
            F_GETFL = 0,
            F_SETFL = 1,

            O_NONBLOCK = 128 // dummy
        };

        int fcntl( int &sockfd, int mode, int value )
        {
            if( mode == F_GETFL ) // get socket status flags
                return 0; // original return current sockfd flags

            if( mode == F_SETFL ) // set socket status flags
            {
                u_long iMode = ( value & O_NONBLOCK ? 0 : 1 );

                bool result = ( ioctlsocket( sockfd, FIONBIO, &iMode ) == NO_ERROR );

                return 0;
            }

            return 0;
        }
    }

#else

#   include <fcntl.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netdb.h>
#   include <unistd.h>    //close

#   include <arpa/inet.h> //inet_addr

#   define INIT()                    do {} while(0)
#   define SOCKET(A,B,C)             ::socket((A),(B),(C))
#   define ACCEPT(A,B,C)             ::accept((A),(B),(C))
#   define CONNECT(A,B,C)            ::connect((A),(B),(C))
#   define CLOSE(A)                  ::close((A))
#   define READ(A,B,C)               ::read((A),(B),(C))
#   define RECV(A,B,C,D)             ::recv((A), (void *)(B), (C), (D))
#   define SELECT(A,B,C,D,E)         ::select((A),(B),(C),(D),(E))
#   define SEND(A,B,C,D)             ::send((A), (const int8 *)(B), (C), (D))
#   define WRITE(A,B,C)              ::write((A),(B),(C))
#   define GETSOCKOPT(A,B,C,D,E)     ::getsockopt((int)(A),(int)(B),(int)(C),(      void *)(D),(socklen_t *)(E))
#   define SETSOCKOPT(A,B,C,D,E)     ::setsockopt((int)(A),(int)(B),(int)(C),(const void *)(D),(int)(E))

#   define BIND(A,B,C)               ::bind((A),(B),(C))
#   define LISTEN(A,B)               ::listen((A),(B))
#   define SHUTDOWN(A)               ::shutdown((A),SHUT_RDWR)
#   define SHUTDOWN_R(A)             ::shutdown((A),SHUT_RD)
#   define SHUTDOWN_W(A)             ::shutdown((A),SHUT_WR)

#endif

namespace tracey {

static std::string p( const std::string &str ) { return tracey::string("<p>\1</p>", str); }
static std::string h1( const std::string &str ) { return tracey::string("<h1>\1</h1>", str); }
static std::string pre( const std::string &str ) { return tracey::string("<xmp>\1</xmp>", str); }
static std::string html( const std::string &str ) { return tracey::string("<html>\1</html>", str); }
static std::string style( const std::string &str ) { return tracey::string("<style>\1</style>", str); }
static std::string body( const std::string &str ) { return tracey::string("<body>\1</body>", str); }
static std::string a( const std::string &text, const std::string &href = "#" ) { return tracey::string("<a href=\"\1\">\2</a>", href, text); }
static std::string ul( const std::string &str ) { return tracey::string("<ul>\1</ul>", str ); }
static std::string li( const std::string &str ) { return tracey::string("<li>\1</li>", str ); }

#define $quote(...) #__VA_ARGS__
static tracey::string get_html_template() {
return $quote(
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>{TITLE}</title>
</head>
<body>
    <div id="header">
        <h2>{TITLE}</h2>
    </div>
    <div id="content">
        <p>{SUMMARY}</p>
        <p>{REPORT}</p>
        <p>{SETTINGS}</p>
    </div>
</body>
</html>
);
}

static int req(int socket, const std::string &input, const std::string &url ) {
    std::string answer;
    /**/ if( url == "/report" ) { answer += p("Tracey generating report"); tracey::view( tracey::report() ); }
    else                        {}

    tracey::string response = get_html_template().
        replace("{TITLE}", "tracey webserver").
        replace("{SETTINGS}", pre( tracey::settings("") ) ).
        replace("{REPORT}", a("generate leak report (may take a while)", "report")).
        replace("{SUMMARY}",  tracey::summary() );

    tracey::string headers( "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html;charset=UTF-8\r\n"
        "Content-Length: \1\r\n"
        "\r\n", response.size() );
    WRITE( socket, headers.c_str(), headers.size() );
    WRITE( socket, response.c_str(), response.size() );
    return 0;
}

static void webmain( void *arg ) {
    volatile bool &sitdown = *(volatile bool *)arg;

    INIT();

    int s = SOCKET(PF_INET, SOCK_STREAM, IPPROTO_IP);

    {
        struct sockaddr_in l = {AF_INET,htons(kTraceyWebserverPort),{INADDR_ANY}};
        $welse(
            int r = 1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&r,sizeof(r));
        )
        BIND(s,(struct sockaddr *)&l,sizeof(l));
        LISTEN(s,5);
    }

    sitdown = true;

    for(;;) {
        int c = ACCEPT(s,0,0), o = 0, h[2], hi = 0;
        char b[1024];
        while(hi<2&&o<1024) {
            int n = READ(c,b+o,sizeof(b)-o);
            if(n<=0) { break; }
            else {
                int i = o;
                o+=n;
                for(;i<n&&hi<2;i++) {
                    if(b[i] == '/' || (hi==1&&b[i] == ' ')) { h[hi++] = i; }
                }
            }
        }
        if(hi == 2) {
            b[ o ] = '\0';

            char org = b[h[1]];
            b[h[1]] = '\0';
            std::string url( &b[4] ); //skip "GET "
            b[h[1]] = org;

            req(c,b,url);
            SHUTDOWN(c);
            CLOSE(c);
        }
    }
}
} // tracey::

#else

namespace tracey {
static void webmain( void *arg ) {}
} // tracey::

#endif

namespace tracey {
    #pragma comment(lib, "user32.lib")
    static void keymain( void *arg ) {
        volatile bool &sitdown = *(volatile bool *)arg;
        sitdown = true;

        $windows(
            for(;;) {
                if( GetAsyncKeyState(VK_NUMLOCK) ) {
                    tracey::view( tracey::report() );
                }
                Sleep( 1000/60 );
            }
        )
    }
}

// platform related, externals here.

#ifdef INCLUDE_GOOGLEINL_AT_END
#undef INCLUDE_GOOGLEINL_AT_END
#include "google.hpp"
#include "google.inl"
#ifdef _MSC_VER
/* #pragma init_seg( ".CRT$XCA" ) */
#endif
static const bool lazy_init = tracey::install_c_hooks();
#endif


#undef kTraceyAllocsOverhead
#undef kTraceyAlloc
#undef kTraceyFree
#undef kTraceyRealloc
#undef kTraceyPrintf
#undef kTraceyMemset
#undef kTraceyDie
#undef kTraceyAssert
#undef kTraceyFile
#undef kTraceyfOpen
#undef kTraceyfPrintf
#undef kTraceyfClose

#undef kTraceyMaxStacktraces
#undef kTraceyStacktraceSkipBegin
#undef kTraceyStacktraceSkipEnd
#undef kTraceyCharLinefeed
#undef kTraceyCharTab
#undef kTraceyMemsetAllocations
#undef kTraceyDefineMemoryOperators
#undef kTraceyReportWildPointers
#undef kTraceyReportOnExit
#undef kTraceyWebserver
#undef kTraceyWebserverPort

#undef $warning
#undef $has
#undef $on
#undef $is

#undef $telse
#undef $throw
#undef $celse
#undef $clang
#undef $melse
#undef $msvc
#undef $gelse
#undef $gnuc
#undef $release
#undef $debug

#undef $lelse
#undef $linux
#undef $aelse
#undef $apple
#undef $welse
#undef $windows

#undef $yes
#undef $no
