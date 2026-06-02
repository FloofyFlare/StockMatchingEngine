## Design notes and Lessons Learned
symbol.hpp
- Using string_view to avoid heap allocation
- Data Structure for translating string stock symbols into Symbol ID's automatically and saving them for later use

order.hpp
- CPUs don't fetch individual bytes from RAM, they fetch 64-byte chunks called cache lines
"struct alignas(64) Order"
- Used static_assert to enforce cache line bondaries in correct places
- orders use price in an int form to avoid FPU operations every operations is one assembly instruction
- orders are designed to not create unnessecary cache misses
- Prev is rare and for cancels in the list so its in its own cache line
- We need to pad data structures so that two threads do not 'fight' over variables being shared on a single cache line.

order_book.hpp
- Currently correct but needs to be upgraded for cache locality
- Map should be enhanced with a sorted flat_hash_map for the first N for locality
- unordered map should become a flat_hash_map

Learning Notes:
Hot paths and 
Cold paths require different levels of code optimization in temporary solutions 