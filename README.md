# StockMatchingEngine
📈 Stock Matching Engine
A low‑latency, deterministic limit order book + matching engine implemented in C++.

🚀 Overview
This project implements a high‑performance, price‑time‑priority matching engine similar to those used in modern electronic exchanges.
It is designed for:

Deterministic behavior

Microsecond‑level latency

Million+ orders/sec throughput per core

Replayability and auditability

Sharded, single‑threaded architecture for predictable performance

The engine supports multiple symbols, aggressive and passive order types, and a fully in‑memory limit order book optimized for cache locality.

✨ Features
Limit, Market, IOC, FOK, Cancel, Replace order types

Price‑time priority with strict FIFO queues

Partial fills and multi‑level sweeps

Deterministic single‑threaded matching core

Sharded architecture (one thread per symbol group)

SPSC ring buffers for lock‑free event passing

Zero heap allocations on the hot path

Replay engine for correctness testing

Top‑of‑book + trade event output

🏛 Architecture
High‑Level Flow
Code
[Ingress] → [Shard Router] → [Matching Engine Shards] → [Egress / Market Data]
Components
Ingress
Parses inbound messages into internal Event structs

Assigns sequence numbers

Routes events to shards via SPSC ring buffers

Shards (One Thread Per Symbol Group)
Own independent order books

Deterministic, single‑threaded matching

Emits outbound events (trades, acks, book updates)

Egress
Consumes outbound events

Writes logs for replay

Publishes market data

Replay Engine
Refeeds logged events

Verifies deterministic output

Checks invariants (no crossed book, FIFO preserved, etc.)

📦 Data Structures
Order
cpp
struct Order {
    uint64_t order_id;
    bool     is_buy;
    int64_t  price_ticks;
    int64_t  quantity;
    uint64_t timestamp_ns;
    uint64_t sequence_number;
    Order*   next_in_level;
    Order*   prev_in_level;
};
Price Level
Fixed price

FIFO queue of orders

Aggregate volume

Order Book
Bid side: descending price

Ask side: ascending price

unordered_map<order_id → Order*> for O(1) cancels

std::map or custom tree/skiplist for price levels

⚡ Matching Logic
Buy Order Path
While best ask exists and buy_price ≥ best_ask_price

Match against head of best ask FIFO

Emit trade event

Remove or update resting order

Remove empty price levels

Insert remainder (if limit) or discard (if IOC/FOK)

Sell Order Path
Symmetric to buy.

Cancel / Replace
Cancel: remove from price level + ID map

Replace: atomic cancel + reinsert (new time priority)

🧪 Testing & Replay
Deterministic Replay
Reproduce engine output bit‑for‑bit

Validate:

FIFO ordering

No negative quantities

No crossed book

Identical outputs for identical inputs

Stress Tests
Cancel storms

Deep sweeps

Bursty microbatches

Multi‑symbol load

📊 Performance Targets
1–5M orders/sec per core sustained

P50 < 5µs, P99 < 10–15µs

Zero allocations on hot path

Cache‑aligned structures

Branch‑predictable matching loops

🧱 Build & Run
Requirements
C++20

CMake

Linux (recommended for perf tools)

Build
Code
mkdir build && cd build
cmake ..
make -j
Run Matching Engine
Code
./matching_engine --config config.json
Run Replay
Code
./replay --input logs/events.bin
📡 Market Data Output
Trade events

Order acknowledgements

Top‑of‑book updates

Optional: depth snapshots, WebSocket gateway

🗺 Roadmap
Persistent snapshots

Multi‑core scaling via more shards

Risk checks (fat‑finger, credit limits)

Pro‑rata or size‑time matching modes

Visualization dashboard

🧠 Why This Project Exists
This engine is built as a systems‑level exploration of:

Deterministic state machines

Low‑latency data structures

Cache‑aware programming

Lock‑free communication

Exchange‑style matching algorithms

It is intentionally designed to mirror real‑world exchange architecture while remaining small enough to study and extend.


A Jane‑Street‑style technical write‑up explaining design tradeoffs

Just tell me what direction you want to push next.
