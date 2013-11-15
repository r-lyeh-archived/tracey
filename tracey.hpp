#pragma once
#include <stdio.h>
#include <string>

namespace tracey {

    struct stats_t {
        size_t usage, peak, leaks;
        stats_t() : usage(0), peak(0), leaks(0) {
        }
    };

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
    void nop();
    void install_c_hooks();
    //void mark(int context);
    //void sweep(int context);

    // Query
    stats_t summary();
    // size_t usage();
    // size_t leaks();
    // const char *leak( size_t );

    // Info
    std::string version();
    std::string url();
    std::string settings();
    // std::string about();
    // std::string license();
    // std::string features();
    // std::string todo();
    // std::string references();

//    static const bool lazy_initializer = ( tracey::nop(), false );
}
