#pragma once

namespace tracey
{
    // API
    void watch( const void *ptr, size_t size );
    void forget( const void *ptr );

    // Control
    void enable( bool on = true );
    void disable();
    bool is_enabled();
    void invalidate();

    // Report
    void report();

    // Versioning
    const char *version();
}
