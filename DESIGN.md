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
- Orders are chained together in a doubly linked list

matching_engine.hpp/cpp
- FOK must be validated **before** any matching begins. Walk the ask side and accumulate available quantity. If insufficient, cancel without modifying the book.
- Matches only while incoming->price_ticks >= best_ask->price_ticks

Learning Notes:
- Hot paths and Cold paths require different levels of code optimization in temporary solutions 
- Remember that Vector does have an overhead if you can avoid using it you should
- You can use likely and unlikely to git the compiler hits. By giving the compiler these hints, you can optimize the actual machine code layout. This minimizes expensive cache misses and drastically improves CPU instruction cache efficiency and pipelining.
