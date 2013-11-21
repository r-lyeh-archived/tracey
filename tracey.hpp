#pragma once
#include <stdio.h>
#include <string>

namespace tracey {

    void fail( const char *message );
    void badalloc();

    // API
    void *watch( const void *ptr, size_t size );
    void *forget( const void *ptr );

    // Control
    //void enable( bool on = true );
    //void disable();
    //bool is_enabled();
    std::string report();
    void view( const std::string &report );
    void restart();
    bool nop();
    bool install_c_hooks();
    //void mark(int context);
    //void sweep(int context);

    // Query
    std::string summary();
    // size_t usage();
    // size_t leaks();
    // const char *leak( size_t );

    // Unchecked memory
    void *realloc( void *ptr, size_t resize );
    void *malloc( size_t size );
    void *calloc( size_t num, size_t size );
    void *free( void *ptr );

    void *amalloc( size_t size, size_t alignment );

    // Info
    std::string version();
    std::string url();
    std::string settings();
    // std::string about();
    // std::string license();
    // std::string features();
    // std::string todo();
    // std::string references();
}
