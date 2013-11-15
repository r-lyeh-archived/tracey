/*
 * Tracey, a simple and lightweight memory leak detector
 * Copyright (c) 2011,2012,2013 Mario 'rlyeh' Rodriguez
 *
 * Callstack code is based on Magnus Norddahl's work (ClanLib).
 * Win32 hooks code is based on The Chromium Authors's work.
 * Tiny webserver code is based on Ivan Tikhonov's work.
 * Webserver HTML code uses jQuery, Flot and Flot templates.

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
 * - C++ hooks (optional)
 * - C hooks (optional) (windows only)
 * - web server (optional)

 * To do:
 * - support for C hooks on non-windows archs:
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

// Constants and settings; feel free to tweak these.

#ifndef kTraceyAlloc
/*  Behaviour is undefined at end of program if kTraceyAlloc() implementation relies on C++ runtime */
#   define kTraceyAlloc                 std::malloc
#endif

#ifndef kTraceyFree
/*  Behaviour is undefined at end of program if kTraceyFree() implementation relies on C++ runtime */
#   define kTraceyFree                  std::free
#endif

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

#ifndef kTraceyAssert
/*  Behaviour is undefined at end of program if kTraceyAssert() implementation relies on C++ runtime */
#   define kTraceyAssert                assert
#endif

#ifndef kTraceyBadAlloc
#   define kTraceyBadAlloc              std::bad_alloc
#endif

#ifndef kTraceyAllocsOverhead
/*  Used to simulate bigger memory requirements  */
/*  Should be equal or bigger than 1.0           */
#   define kTraceyAllocsOverhead        1.0
#endif

#ifndef kTraceyStacktraceMaxTraces
#   define kTraceyStacktraceMaxTraces   128
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

#ifndef kTraceyDefineCppMemOperators
#   define kTraceyDefineCppMemOperators 1
#endif

#ifndef kTraceyMemsetAllocations
#   define kTraceyMemsetAllocations     0
#endif

#ifndef kTraceyReportOnExit
#   define kTraceyReportOnExit          1
#endif

#ifndef kTraceyHookCRT
#   define kTraceyHookCRT               0
#endif

#ifndef kTraceyWebserver
#   define kTraceyWebserver             1
#endif

#ifndef kTraceyWebserverPort
#   define kTraceyWebserverPort         2001
#endif

#ifndef kTraceyEnabled
#   define kTraceyEnabled               1
#endif

// Optimizations and tweaks

#ifdef _MSC_VER
#   ifdef _SECURE_SCL
#      undef _SECURE_SCL
#   endif
#   define _SECURE_SCL 0
#   ifdef _HAS_ITERATOR_DEBUGGING
#       undef _HAS_ITERATOR_DEBUGGING
#   endif
#   define _HAS_ITERATOR_DEBUGGING 0
#   pragma optimize( "gty", on )        // optimization tweaks on msvc
#else
#   pragma optimize                     // optimization on a few other compilers, hopefully
#   pragma GCC optimize                 // optimization tweaks on gcc 4.4+
#endif

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

// Headers

#if defined(_WIN32) || defined(_WIN64)
#   include <Winsock2.h>
#   include <Windows.h>
#   // unwinding
#   if defined(DEBUG) || defined(_DEBUG)
#       include <CrtDbg.h>
#   endif
#   include <DbgHelp.h>
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
#   include <execinfo.h>
//  --
#   include <cxxabi.h>
#endif

#include "tracey.hpp"

// OS utils

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

#define $on(v)  (0 v(+1))  // usage: #if $on($msvc)
#define $is     $on        // usage: #if $is($debug)

#define $has(...)    $clang(__has_feature(__VA_ARGS__)) $celse(__VA_ARGS__) // usage: #if $has(cxx_exceptions)
/*
#if !$on($clang)
#   define __has_feature(...) (0)
#endif
*/

#if $on($msvc)
#   define $warning(msg) __pragma( message( msg ) )
#elif defined(__GNUC__) && defined(GCC_VERSION) && GCC_VERSION >= 40400
#   define $warning$message$impl(msg) _Pragma(#msg)
#   define $warning(msg) $warning$message$impl( message( msg ) )
#else
#   define $warning(msg)
#endif

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

// Remainders for retrieving symbols

#  if $on($msvc)
    $warning( "<tracey/tracey.cpp> says: do not forget /Zi, /Z7 or /C7 compiler settings! /Oy- also helps!" )
#elif $on($clang)
    $warning( "<tracey/tracey.cpp> says: do not forget -g compiler setting!" )
#elif $on($gnuc)
    $warning( "<tracey/tracey.cpp> says: do not forget -g -lpthread compiler settings!" )
#endif


// Checks

#if kTraceyHookCRT
#   if $on($windows)
#       undef  kTraceyAlloc
#       define kTraceyAlloc MemoryHook::Alloc
#       undef  kTraceyFree
#       define kTraceyFree  MemoryHook::Free
#       undef  kTraceyDefineCppMemOperators
#       define kTraceyDefineCppMemOperators 0
#       define INCLUDE_GOOGLE_INL  // checked at end of this file
        // MemoryHook provides a private heap for allocating
        // unwatched memory.
        typedef int int32;
        struct MemoryHook {

          HANDLE heap_;

           MemoryHook();
          ~MemoryHook();

          // MemoryObserver interface.
          static void trace(HANDLE heap, int32 id, int32 size);
          static void untrace(HANDLE heap, int32 id, int32 size);
          // Safe allocations
          static void* Alloc(size_t size);
          static void Free(void* ptr);
        };
#   else
        $warning( "<tracey/tracey.cpp> says: kTraceyHookCRT not supported on this platform (skipped)")
#       undef kTraceyHookCRT
#       define kTraceyHookCRT 0
#   endif
#endif

// Implementation

namespace tracey
{
    static void webmain( void * );

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

            V recalc() {
                V value = this->empty() ? get() : zero<V>();
                for( typename tree::iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    value += it->second.recalc();
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

    struct callstack_naked {
        enum { max_frames = kTraceyStacktraceMaxTraces };
        void *frames[max_frames];
        size_t num_frames;

        // void save();
        // std::vector<std::string> unwind() const;
        // std::string lookup( void *symbol ) const;
    };

    struct callstack : callstack_naked
    {
        // save
        callstack();

        // print
        tracey::strings str( const char *format12 = "#\1 \2\n", size_t skip_begin = 0 );
    };

    namespace
    {
        size_t capture_stack_trace(unsigned frames_to_skip, unsigned max_frames, void **out_frames)
        {
            if( max_frames > callstack::max_frames )
                max_frames = callstack::max_frames;

            if( frames_to_skip > max_frames )
                return 0;

            if( !out_frames )
                return 0;

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

            return capturedFrames;
        })
        $gnuc({
            // Ensure the output is cleared
            kTraceyMemset(out_frames, 0, (sizeof(void *)) * max_frames);

            return (backtrace(out_frames, max_frames));
        })
            return 0;
        }

        tracey::strings resolve_stack_trace( void **frames, unsigned num_frames )
        {
            const char *const invalid = "????";

        $windows({
            tracey::strings backtraces;

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
            )
            {
                enum { MAXSYMBOLNAME = 256 - sizeof(IMAGEHLP_SYMBOL64) };
                char symbol64_buf     [sizeof(IMAGEHLP_SYMBOL64) + MAXSYMBOLNAME];
                char symbol64_bufblank[sizeof(IMAGEHLP_SYMBOL64) + MAXSYMBOLNAME] = {0};
                IMAGEHLP_SYMBOL64 *symbol64       = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbol64_buf);
                IMAGEHLP_SYMBOL64 *symbol64_blank = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbol64_bufblank);
                symbol64_blank->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
                symbol64_blank->MaxNameLength = MAXSYMBOLNAME - 1;

                IMAGEHLP_LINE64 line64, line64_blank = {0};
                line64_blank.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

                for( unsigned i = 0; i < num_frames; i++ )
                {
                    backtraces.push_back( invalid );

                    *symbol64 = *symbol64_blank;
                    DWORD64 displacement64 = 0;

                    if( SymGetSymFromAddr64( process, (DWORD64) frames[i], &displacement64, symbol64 ) ) {
                        line64 = line64_blank;
                        DWORD displacement = 0;
                        if( !SymGetLineFromAddr64( process, (DWORD64) frames[i], &displacement, &line64 ) ) {
                            backtraces.back() = symbol64->Name;
                        } else {
                            backtraces.back() = tracey::string( "\1 (\2, line \3)", symbol64->Name, line64.FileName, line64.LineNumber );
                        }
                    }
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
            tracey::strings backtraces;
            char **strings = backtrace_symbols(frames, num_frames);

            // Decode the strings
            if( strings ) {
                for( unsigned i = 0; i < num_frames; i++ )
                    if( strings[i] )
                        backtraces.push_back( demangle(strings[i]) );
                    else
                        backtraces.push_back( invalid );
                free( strings );
            }

            return backtraces;
        })
            return tracey::strings();
        }
    }

    callstack::callstack() { // save
        num_frames = tracey::capture_stack_trace( 1, max_frames, frames );
        for( int i = num_frames; i < max_frames; ++i ) frames[ i ] = 0;
    }

    tracey::strings callstack::str( const char *format12, size_t skip_begin ) {
        tracey::strings stack_trace;

        stack_trace = tracey::resolve_stack_trace( &frames[ skip_begin ], num_frames - skip_begin );

        for( size_t i = 0, end = stack_trace.size(); i < end; i++ )
            stack_trace[i] = tracey::string( format12, i + 1, stack_trace[i] );

        return stack_trace;
    }

    template<typename T>
    std::string symbol( T *ptr ) {
        void *frames[] = { (void *)ptr, 0 };
        tracey::strings stacktrace = tracey::resolve_stack_trace( &frames[ 0 ], 1 );
        return stacktrace.size() ? stacktrace[0] : std::string("????");
    }
}

namespace tracey
{
    namespace
    {
        volatile size_t timestamp_id = 0;
        stats_t stats;

        size_t create_id() {
            static size_t id = 0;
            return ++id;
        }

        bool save_report( const std::string &pathfile, const std::string &header, const std::string &body, const std::string &footer ) {
            FILE *fp = std::fopen( pathfile.c_str() , "wb" );
            if( !fp )
                return false;
            std::fwrite( header.c_str(), 1, header.size(), fp );
            std::fwrite( body.c_str(), 1, body.size(), fp );
            std::fwrite( footer.c_str(), 1, footer.size(), fp );
            std::fclose( fp );
            return true;
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
                return path.resize( path.size() - 1 ), path;
            )
            $welse(
                return std::string() + std::tmpnam(0);
            )
        }

        struct leak
        {
            size_t size, id;

            leak( const size_t _size = 0 ) : size( _size ), id( create_id() )
            {}
        };

        typedef std::pair< leak *, tracey::callstack * > leak_full;

        struct tuple
        {
            const void *addr;
            leak *lk;
            tracey::callstack *cs;
        };

        typedef std::vector< tuple > tuples;

        struct custom_mutex;
        custom_mutex *mutex = 0;

        class container : public std::map< const void *, leak_full, std::less< const void * > > //, tracey::malloc_allocator< std::pair< const void *, leak * > > >
        {
            public:

            container()
            {}

            ~container() {
                mutex = 0;

                // On some Windows configurations, a crash is likely to happen at this point.
                // Just because we're using the runtime C library (in _report()) *after* it has been deallocated.
                // This only happens when the static runtime library is linked (flags /MT or /MTd) and kTraceyReportOnExit is defined to 1.
                // In order to fix this either use /MD or /MDd libraries, or define kTraceyReportOnExit to 0.
                // For now and to get your system happy, we just exit silently before any crash (see kTraceyDie() below).
                // After all, we are a memory tracker and we should be the very last thing to deallocate in any program.
                if( kTraceyReportOnExit ) {
                    view_report( _report<std::ofstream>() );
                }

                // _clear();

                // Silent exit. Not a requirement in all architectures but who knows.
                kTraceyDie( 0 );
            }

            void _clear() {

                for( iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    if( it->second.first )
                        delete it->second.first, it->second.first = 0;
                    if( it->second.second )
                        delete it->second.second, it->second.second = 0;
                }

                this->clear();
            }

            tuples collect_leaks( size_t *wasted ) const {
                tuples list;
                *wasted = 0;
                for( const_iterator it = this->begin(), end = this->end(); it != end; ++it ) {
                    const void *my_address = it->first;
                    leak *my_leak = it->second.first;
                    tracey::callstack *my_callstack = it->second.second;
                    if( my_leak && my_callstack /* && it->second->size != ~0 */ ) {
                        if( my_leak->id >= timestamp_id ) {
                            *wasted += my_leak->size;

                            tuple t = { my_address, my_leak, my_callstack };
                            list.push_back( t );
                        }
                    }
                }
                return list;
            }

            template<typename OSTREAM>
            std::string _report() const {

                std::string logfile = get_temp_pathfile() + "xxx-tracey.html";
                OSTREAM log( logfile.c_str(), std::ios::binary );
                enum { BUFSIZE = 65536 };
                char buffer[BUFSIZE];
                log.rdbuf()->pubsetbuf(buffer, BUFSIZE);

                // this code often runs at the very end of a program cycle (even when static memory has been deallocated)
                // so, avoid using global C++ objects like std::cout/cerr as much as possible; otherwise crashes may happen

                $windows(
                    AllocConsole();
                )

                // Find leaks
                kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: filtering leaks..." kTraceyCharLinefeed).c_str() );
                size_t wasted, n_leak;
                tuples filtered = collect_leaks( &wasted );
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
                log << tracey::string( "<html><body><xmp>" );
                log << tracey::string( "<tracey/tracey.cpp> says: generated with \1 (\2)" kTraceyCharLinefeed, tracey::version(), tracey::url() );
                log << tracey::string( "<tracey/tracey.cpp> says: best viewed on foldable text editor (like SublimeText2) with tabs=2sp and no word-wrap" kTraceyCharLinefeed );
                log << tracey::string( "<tracey/tracey.cpp> says: \1, \2 leaks found; \3 bytes wasted ('\4' score)" kTraceyCharLinefeed, !n_leak ? "ok" : "error", n_leak, wasted, score);

                // Body
                // Get all frame addresses involved in all leaks
                // Also, create a tree of frames; so we will take decisions from above by examining node weights (@todo)
                kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: creating trees of frames..."  kTraceyCharLinefeed).c_str() );
                std::set< void * > set;
                tracey::tree<void *> tree;
                tree( (void *)((~0)-0) ); // bottom-top branch
                tree( (void *)((~0)-1) ); // top-bottom branch
                for( tuples::const_iterator it = filtered.begin(), end = filtered.end(); it != end; ++it ) {
                    const void *my_address = it->addr;
                    leak *my_leak = it->lk;
                    tracey::callstack *my_callstack = it->cs;
                    tracey::tree<void *> *_tree = &tree((void *)((~0)-0));
                    tracey::tree<void *> *_tree_inv = &tree((void *)((~0)-1));
                    for( unsigned i = 0, start = kTraceyStacktraceSkipBegin, end = my_callstack->num_frames - 1 - kTraceyStacktraceSkipEnd; start+i <= end; ++i ) {

                        (*_tree)( my_callstack->frames[start + i] ) = my_leak->size; // id
                        _tree = &(*_tree)( my_callstack->frames[start + i] );
                        set.insert( my_callstack->frames[start + i] );

                        (*_tree_inv)( my_callstack->frames[end - i] ) = my_leak->size; // id
                        _tree_inv = &(*_tree_inv)( my_callstack->frames[end - i] );
                        set.insert( my_callstack->frames[end - i] );
                    }
                }

                // Some apps are low on memory in here, so we free memory as soon as possible
                filtered = tuples();

                kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: resolving \1 unique frames..." kTraceyCharLinefeed, set.size()).c_str() );
                if( set.size() ) {
                    // convert set of unique frames into array of frames
                    std::vector< void * > frames;
                    frames.reserve( set.size() );
                    for( std::set< void * >::iterator it = set.begin(), end = set.end(); it != end; ++it ) {
                        frames.push_back( *it );
                    }
                    tracey::strings symbols = resolve_stack_trace( &frames[0] /*frames.data()*/, frames.size() );
                    std::map< void *, std::string > translate;
                    {
                        if( frames.size() != symbols.size() ) {
                            log << tracey::string("<tracey/tracey.cpp> says: error! cannot resolve all frames (\1 vs \2)!" kTraceyCharLinefeed, frames.size(), symbols.size() );
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
                        translate[ (void *)((~0)-0) ] = "bottom-top normal tree (useful to find leak endings)";
                        translate[ (void *)((~0)-1) ] = "top-bottom normal tree (useful to find leak beginnings)";
                        kTraceyPrintf( "%s", tracey::string("<tracey/tracey.cpp> says: flattering tree of symbols..." kTraceyCharLinefeed).c_str() );
                        tree.recalc();
                        tree.print(translate, log);
#else
                        log << std::string(64, '-') << "bottom-top tree (leakers)" << std::string(64, '-') << std::endl;
                        tree.rekey(translate).print(log);
                        log << std::string(64, '-') << "top-bottom tree (traces)" << std::string(64, '-') << std::endl;
                        tree_inv.rekey(translate).print(log);
#endif
                    }
                }

                // Footer
                log << tracey::string( "</xmp></body></html>" );
                return logfile;
            }
        };

        struct custom_mutex {
            std::recursive_mutex first;
            volatile bool recursive;
            container map;

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

            custom_mutex() : recursive(false) {
                mutex = (this);
            }

            ~custom_mutex() { mutex = 0; }

            static bool preinit() {
                return true;
            }
        };

        void *tracer( void *ptr, size_t &size )
        {
            if( !kTraceyEnabled )
                return size = 0, ptr;

			{ static custom_mutex m; }

            if( !ptr )
                return size = 0, ptr;

            if( !mutex )
                return size = 0, ptr;

            if( !mutex->try_lock() )
                return size = 0, ptr;

            static bool postinit = false; if( !postinit ) { postinit = true;
                kTraceyPrintf( "%s", tracey::settings().c_str() );
                if( kTraceyWebserver ) {
                    // std::thread( tracey::webmain, 0 ).detach();
                    // old tinythread versions do not support detach, so workaround following:
                    new std::thread( tracey::webmain, (void *)0 );
                }
            }

            // threads will lock here till the slot is free.
            // threads will return on recursive locks.
            //if( !(ptr && mutex && mutex->try_lock()) )
            //   return size = 0, ptr;

            container &map = mutex->map;

            // ready
            container::iterator it = map.find( ptr );
            bool found = ( it != map.end() );

            if( size == ~0 )
            {
                if( found )
                {
                    leak_full &map_ptr = it->second;

                    //map_ptr->~leak();
                    //kTraceyFree( map_ptr );

                    if( map_ptr.first ) {
                        stats.leaks--;
                        stats.usage -= map_ptr.first->size;
                        delete map_ptr.first, map_ptr.first = 0;
                    }

                    if( map_ptr.second )
                        delete map_ptr.second, map_ptr.second = 0;
                }
                else
                {
                    // 1st) wild pointer deallocation found; warn user

                    if( kTraceyReportWildPointers )
                        kTraceyPrintf( "%s", (tracey::string( "<tracey/tracey.cpp> says: Error, wild pointer deallocation." kTraceyCharLinefeed ) +
                            tracey::callstack().str( kTraceyCharTab "\1) \2" kTraceyCharLinefeed, kTraceyStacktraceSkipBegin).flat() ).c_str() );

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
                // SIZE OPCODE
                if( found ) {
                leak_full &map_ptr = it->second;
                if( map_ptr.first )
                    size = map_ptr.first->size;
                else
                    size = 0;
                } else {
                ptr = 0;
                size = 0;
                }
            }
            else
            if( size == (~0) - 3 )
            {
                tracey::string &log = *((tracey::string *)(ptr));
                log = map._report<std::ofstream>();
            }
            else
            if( size == (~0) - 4 )
            {
                tracey::string &log = *((tracey::string *)(ptr));
                view_report( log );
            }
            else
            {
                if( found /* && map[ ptr ] */ )
                {
                    // 1st) double pointer allocation (why?); warn user
                    // kTraceyFree( map[ ptr ] );
                }

                // create a leak and (re)insert it into map

                (map[ ptr ] = map[ ptr ]) = std::make_pair< leak *, tracey::callstack * >( new leak( size ), new tracey::callstack() );
                stats.leaks++;
                stats.usage += size;
                if( stats.usage > stats.peak ) stats.peak = stats.usage;
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
    stats_t summary() {
        stats_t st;
        size_t opcode = 1, special_fn = (~0) - 2;
        tracer( &st, special_fn );
        return st;
    }
    std::string report() {
        tracey::string rep;
        size_t special_fn = (~0) - 3;
        tracer( &rep, special_fn );
        return rep;
    }
    void view( const std::string &report ) {
        tracey::string rep = report;
        size_t special_fn = (~0) - 4;
        tracer( &rep, special_fn );
    }
    void fail( const char *message ) {
        $throw(
            throw std::runtime_error( message );
        )
        $telse(
            kTraceyPrintf( "%s", message );
            kTraceyAssert( !"<tracey/tracey.cpp> says: fail() requested" );
            std::exit(-1);
        )
    }
    void badalloc() {
        $throw(
            throw kTraceyBadAlloc();
        )
        $telse(
            tracey::fail( "<tracey/tracey.cpp> says: error! out of memory" );
        )
    }
    void install_c_hooks() {
#       if $on($windows) && kTraceyHookCRT
            static char placement[ sizeof(MemoryHook) ];
            static MemoryHook* self = new ((MemoryHook *)placement) MemoryHook();
#       endif
    }
    std::string version() {
        return "tracey-0.2.a";  /* format is major.minor.(a)lpha/(b)eta/(r)elease/(c)andidate */
    }
    std::string url() {
        return "https://github.com/r-lyeh/tracey";
    }
    static std::string settings( const std::string &prefix ) {
        tracey::string out;
        if( tracey::symbol(kTraceyAlloc) == "????" )
            fail( "failed to decode symbols. Is debug information available?" $msvc(" Are .PDB files available?" ) );
        out += tracey::string( "\1\2 ready" kTraceyCharLinefeed, prefix, tracey::version() );
        out += tracey::string( "\1using \2 as malloc" kTraceyCharLinefeed, prefix, tracey::symbol(kTraceyAlloc) );
        out += tracey::string( "\1using \2 as free" kTraceyCharLinefeed, prefix, tracey::symbol(kTraceyFree) );
        out += tracey::string( "\1using \2 as realloc" kTraceyCharLinefeed, prefix, tracey::symbol(kTraceyRealloc) );
        out += tracey::string( "\1using \2 as memset" kTraceyCharLinefeed, prefix, tracey::symbol(kTraceyMemset) );
        out += tracey::string( "\1using \2 as printf" kTraceyCharLinefeed, prefix, tracey::symbol(kTraceyPrintf) );
        out += tracey::string( "\1using \2 as exit" kTraceyCharLinefeed, prefix, tracey::symbol(kTraceyDie) );
        out += tracey::string( "\1with C++ exceptions=\2" kTraceyCharLinefeed, prefix, $throw("enabled") $telse("disabled") );
        out += tracey::string( "\1with kTraceyAllocsOverhead=x\2" kTraceyCharLinefeed, prefix, kTraceyAllocsOverhead );
        out += tracey::string( "\1with kTraceyStacktraceMaxTraces=\2" kTraceyCharLinefeed, prefix, kTraceyStacktraceMaxTraces );
        // kTraceyCharLinefeed
        // kTraceyCharTab
        out += tracey::string( "\1with kTraceyReportWildPointers=\2" kTraceyCharLinefeed, prefix, kTraceyReportWildPointers ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyDefineCppMemOperators=\2" kTraceyCharLinefeed, prefix, kTraceyDefineCppMemOperators ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyMemsetAllocations=\2" kTraceyCharLinefeed, prefix, kTraceyMemsetAllocations ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyStacktraceSkipBegin=\2" kTraceyCharLinefeed, prefix, kTraceyStacktraceSkipBegin );
        out += tracey::string( "\1with kTraceyStacktraceSkipEnd=\2" kTraceyCharLinefeed, prefix, kTraceyStacktraceSkipEnd );
        out += tracey::string( "\1with kTraceyReportOnExit=\2" kTraceyCharLinefeed, prefix, kTraceyReportOnExit ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyHookCRT=\2" kTraceyCharLinefeed, prefix, kTraceyHookCRT ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyWebserver=\2" kTraceyCharLinefeed, prefix, kTraceyWebserver ? "yes" : "no" );
        out += tracey::string( "\1with kTraceyWebserverPort=\2" kTraceyCharLinefeed, prefix, kTraceyWebserverPort );
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

    void *realloc( void *ptr, size_t resize ) {
        static bool init = (tracey::install_c_hooks(), true);

        ptr = kTraceyRealloc( ptr, (size_t)(kTraceyAllocsOverhead * resize) );

        return ptr ? ptr : (tracey::badalloc(), (void *)0);
    }
    void *malloc( size_t size ) {
        void *ptr = tracey::realloc( 0, size );
        return kTraceyMemsetAllocations ? kTraceyMemset( ptr, 0, size ) : ptr;
    }
    void *calloc( size_t num, size_t size ) {
        return tracey::malloc( num * size );
    }
    void *free( void *ptr ) {
        return kTraceyFree( ptr ), (void *)0;
    }
    void nop() {
        tracey::free( tracey::forget( tracey::watch( tracey::malloc( 1 ), 1 ) ) );
    }
}

#if kTraceyDefineCppMemOperators

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

void *operator new( size_t size ) throw(kTraceyBadAlloc) {
    return tracey::watch( tracey::malloc( size ), size );
}

void *operator new[]( size_t size ) throw(kTraceyBadAlloc) {
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
static tracey::string templated() {
return $quote(
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>{TITLE}</title>
    <link href="http://www.flotcharts.org/flot/examples/examples.css" rel="stylesheet" type="text/css">
    <!--[if lte IE 8]><script language="javascript" type="text/javascript" src="http://www.flotcharts.org/flot/excanvas.min.js"></script><![endif]-->
    <script language="javascript" type="text/javascript" src="http://www.flotcharts.org/flot/jquery.js"></script>
    <script language="javascript" type="text/javascript" src="http://www.flotcharts.org/flot/jquery.flot.js"></script>
    <script type="text/javascript">
    $(function() {
        var datasets = {
            "usa": {
                label: "sample #0",
                data: [[1988, 483994], [1989, 479060], [1990, 457648], [1991, 401949], [1992, 424705], [1993, 402375], [1994, 377867], [1995, 357382], [1996, 337946], [1997, 336185], [1998, 328611], [1999, 329421], [2000, 342172], [2001, 344932], [2002, 387303], [2003, 440813], [2004, 480451], [2005, 504638], [2006, 528692]]
            },
            "russia": {
                label: "sample #1",
                data: [[1988, 218000], [1989, 203000], [1990, 171000], [1992, 42500], [1993, 37600], [1994, 36600], [1995, 21700], [1996, 19200], [1997, 21300], [1998, 13600], [1999, 14000], [2000, 19100], [2001, 21300], [2002, 23600], [2003, 25100], [2004, 26100], [2005, 31100], [2006, 34700]]
            },
            "uk": {
                label: "sample #2",
                data: [[1988, 62982], [1989, 62027], [1990, 60696], [1991, 62348], [1992, 58560], [1993, 56393], [1994, 54579], [1995, 50818], [1996, 50554], [1997, 48276], [1998, 47691], [1999, 47529], [2000, 47778], [2001, 48760], [2002, 50949], [2003, 57452], [2004, 60234], [2005, 60076], [2006, 59213]]
            },
            "germany": {
                label: "sample #3",
                data: [[1988, 55627], [1989, 55475], [1990, 58464], [1991, 55134], [1992, 52436], [1993, 47139], [1994, 43962], [1995, 43238], [1996, 42395], [1997, 40854], [1998, 40993], [1999, 41822], [2000, 41147], [2001, 40474], [2002, 40604], [2003, 40044], [2004, 38816], [2005, 38060], [2006, 36984]]
            },
            "denmark": {
                label: "sample #4",
                data: [[1988, 3813], [1989, 3719], [1990, 3722], [1991, 3789], [1992, 3720], [1993, 3730], [1994, 3636], [1995, 3598], [1996, 3610], [1997, 3655], [1998, 3695], [1999, 3673], [2000, 3553], [2001, 3774], [2002, 3728], [2003, 3618], [2004, 3638], [2005, 3467], [2006, 3770]]
            },
            "sweden": {
                label: "sample #5",
                data: [[1988, 6402], [1989, 6474], [1990, 6605], [1991, 6209], [1992, 6035], [1993, 6020], [1994, 6000], [1995, 6018], [1996, 3958], [1997, 5780], [1998, 5954], [1999, 6178], [2000, 6411], [2001, 5993], [2002, 5833], [2003, 5791], [2004, 5450], [2005, 5521], [2006, 5271]]
            },
            "norway": {
                label: "sample #6",
                data: [[1988, 4382], [1989, 4498], [1990, 4535], [1991, 4398], [1992, 4766], [1993, 4441], [1994, 4670], [1995, 4217], [1996, 4275], [1997, 4203], [1998, 4482], [1999, 4506], [2000, 4358], [2001, 4385], [2002, 5269], [2003, 5066], [2004, 5194], [2005, 4887], [2006, 4891]]
            },
            "random" : {
                label: "sample #7",
                data: [[1988,4000]]
            }
        };
        // hard-code color indices to prevent them from shifting as
        // countries are turned on/off
        var i = 0;
        $.each(datasets, function(key, val) {
            val.color = i;
            ++i;
        });
        // insert checkboxes
        var choiceContainer = $("#choices");
        $.each(datasets, function(key, val) {
            choiceContainer.append("<br/><input type='checkbox' name='" + key +
                "' checked='checked' id='id" + key + "'></input>" +
                "<label for='id" + key + "'>"
                + val.label + "</label>");
        });
        choiceContainer.find("input").click(plotAccordingToChoices);
        var year = 1989;
        function plotAccordingToChoices() {
            var data = [];
            choiceContainer.find("input:checked").each(function () {
                var key = $(this).attr("name");
                if (key && datasets[key]) {
                    data.push(datasets[key]);
                }
            });
            if (data.length > 0) {
                $.plot("#placeholder", data, {
                    yaxis: {
                        min: 0
                    },
                    xaxis: {
                        tickDecimals: 0
                    }
                });
            }
            datasets["random"].data.push( [year++, Math.floor( Math.random() * 600000 ) ] );
            //alert(JSON.stringify(datasets));
            setTimeout(plotAccordingToChoices, 1000);
        }
        plotAccordingToChoices();
        // Initiate a recurring data update
        $("button.dataUpdate").click(function () {
            data = [];
            alreadyFetched = {};
            $.plot("#placeholder", data, options);
            var iteration = 0;
            function fetchData() {
                ++iteration;
                function onDataReceived(series) {
                    // Load all the data in one pass; if we only got partial
                    // data we could merge it with what we already have.
                    data = [ series ];
                    $.plot("#placeholder", data, options);
                }
                // Normally we call the same URL - a script connected to a
                // database - but in this case we only have static example
                // files, so we need to modify the URL.
                $.ajax({
                    url: "data-eu-gdp-growth-" + iteration + ".json",
                    type: "GET",
                    dataType: "json",
                    success: onDataReceived
                });
                if (iteration < 5) {
                    setTimeout(fetchData, 1000);
                } else {
                    data = [];
                    alreadyFetched = {};
                }
            }
        });
        // Add the Flot version string to the footer
        $("#footer").prepend("Flot " + $.plot.version + " &ndash; ");
    });
    </script>
</head>
<body>
    <div id="header">
        <h2>{TITLE}</h2>
    </div>
    <div id="content">
        <p>{USAGE}</p>
        <p>{REPORT}</p>
        <div class="demo-container">
            <div id="placeholder" class="demo-placeholder" style="float:left; width:675px;"></div>
            <p id="choices" style="float:right; width:135px;"></p>
        </div>
        <p>This is a placeholder example (source: <a href="https://github.com/r-lyeh/tracey"></a>).</p>
        <p>
            <button class="dataUpdate">Do nothing</button>
        </p>
        <p>{SETTINGS}</p>
    </div>
    <!--div id="footer">
        Copyright &copy; 2007 - 2013 IOLA and Ole Laursen
    </div-->
</body>
</html>
);
}

static int req(int socket, const std::string &input, const std::string &url ) {
    std::string answer;
    /**/ if( url == "/report" )      { answer += p("Tracey generating report"); tracey::view( tracey::report() ); }
    else {}

    tracey::string response = templated().
        replace("{TITLE}","tracey webserver").
        replace("{SETTINGS}", pre( tracey::settings("") ) ).
        replace("{REPORT}", a("generate leak report (may take a while)", "report")).
        replace("{USAGE}", tracey::string("highest peak: \1 Kb // in use: \2 Kb // num leaks: \3", stats.peak/1024, stats.usage/1024, stats.leaks));

    tracey::string headers( "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html;charset=UTF-8\r\n"
        "Content-Length: \1\r\n"
        "\r\n", response.size() );
    WRITE( socket, headers.c_str(), headers.size() );
    WRITE( socket, response.c_str(), response.size() );
    return 0;
}

static void webmain( void *arg ) {
    (void *)arg;

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


#undef kTraceyAllocsOverhead
#undef kTraceyAlloc
#undef kTraceyFree
#undef kTraceyRealloc
#undef kTraceyPrintf
#undef kTraceyMemset
#undef kTraceyDie
#undef kTraceyAssert
#undef kTraceyBadAlloc

#undef kTraceyStacktraceMaxTraces
#undef kTraceyStacktraceSkipBegin
#undef kTraceyStacktraceSkipEnd
#undef kTraceyCharLinefeed
#undef kTraceyCharTab
#undef kTraceyMemsetAllocations
#undef kTraceyDefineCppMemOperators
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

// platform related, externals here.

#ifdef INCLUDE_GOOGLE_INL
#undef INCLUDE_GOOGLE_INL
#include "google.hpp"
#include "google.inl"
#endif // INCLUDE_GOOGLE_INL
