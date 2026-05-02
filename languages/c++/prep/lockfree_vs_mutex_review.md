# Lock-Free vs Mutex: SPSC Queue Study Notes

## 1. Ring Buffer Design

A ring buffer uses head and tail indices into a fixed array:

```
- x x x x x x x x x - - - -
  |               |
  head            tail
```

- **head** — where data is read (pop)
- **tail** — where data is written (push)
- **Full condition**: `(tail + 1) % N == head`
- **Empty condition**: `tail == head`

### The N-1 Capacity Gotcha

The full condition deliberately leaves one slot empty to distinguish full from empty. If you filled all N slots, `tail == head` would be indistinguishable from empty. So **actual capacity is N-1**, not N.

This shows up empirically — with `retryOnFull=false` at high producer speeds, collected items converges to exactly N-1, because the producer fills the queue once and finishes before the consumer can keep up.

Alternative approaches to get true N capacity:
- Separate `size_` counter (atomic in lock-free, complicates memory ordering)
- Unbounded indices: let head/tail grow freely, use `% N` only for array access. Full condition becomes `tail - head == N`. Most popular in production SPSC queues.

---

## 2. Exception Safety: Why pop() Takes T& Out

From Herb Sutter's *Exceptional C++* — you cannot have both exception safety and a return value in one operation:

```cpp
// UNSAFE — item is lost if copy constructor throws during return
T pop() {
    T val = data_[head_];
    ++head_;
    return val;  // if this throws, item is gone forever
}
```

The safe pattern is to write to an out parameter first, modify queue state after:

```cpp
bool pop(T& out) {
    if (tail_ == head_) return false;
    out = data_[head_];   // copy first — if this throws, head_ unchanged
    head_ = (head_ + 1) % N;  // only advance after safe copy
    return true;
}
```

This is safe because the queue is only modified after the data is safely delivered. If the copy throws, the item is still in the queue.

### Requirements on T

For `pop(T& out)` to be exception safe, `T`'s copy assignment must either not throw, or leave `out` in a valid state if it does. This is worth encoding in your concept:

```cpp
// ideally T should be nothrow copy assignable for strongest guarantee
static_assert(std::is_nothrow_copy_assignable_v<T>);
```

---

## 3. peek() + pop() Split — SPSC Safety Analysis

An alternative interface splits observation and removal:

```cpp
std::optional<T> peek() const;  // observe without removing
bool pop();                      // remove without returning
```

Safety analysis by threading model:

| Scenario | Safe? | Reason |
|---|---|---|
| Single producer, single consumer | ✅ | Consumer owns head exclusively, no race between peek and pop |
| Multi-producer | ✅ | Producers touch tail only, don't interfere with consumer's head |
| Multi-consumer | ❌ | Two consumers can both peek same item, both pop — TOCTOU race |

For multi-consumer, the correct fix is `pop(T& out)` — combining observation and removal into one locked operation.

---

## 4. Memory Ordering

### The Core Problem

CPUs and compilers reorder instructions for performance. On a single thread this is invisible. Across threads, another thread can observe writes in a different order than they were written.

### The Three Orders

**`memory_order_relaxed`** — atomicity only, no ordering constraints. Use when you own the variable exclusively and only need the read/write to be atomic.

**`memory_order_release`** — on a store: nothing written *before* this store can be reordered to appear *after* it. One-way downward barrier:

```
write A  --|
write B    |  cannot cross downward past release
write C  --|
release  ===== one-way barrier
write D  <-- CAN move above the release
```

**`memory_order_acquire`** — on a load: nothing read *after* this load can be reordered to appear *before* it. One-way upward barrier:

```
read A   <-- CAN move below the acquire
acquire  ===== one-way barrier
read B   --|
read C     |  cannot cross upward past acquire
read D   --|
```

### Release-Acquire Handshake

Release and acquire work as a **pair** across threads. When thread A does a release-store, and thread B does an acquire-load and sees that value, then B is guaranteed to see everything A wrote before the release-store:

```cpp
// Thread A (producer)
data = 42;                          // 1
tail_.store(next, release);         // 2 — "data is ready"

// Thread B (consumer)
head = tail_.load(acquire);         // 3 — "I see your store"
use(data[head]);                    // 4 — guaranteed to see data=42
```

### How Mutex Uses Acquire/Release

Mutex implements the same primitives under the hood:
- `mutex.lock()` is an **acquire** operation
- `mutex.unlock()` is a **release** operation

Everything inside a locked section is automatically sandwiched between acquire and release. Lock-free just makes you do it explicitly yourself.

### Memory Ordering in SPSC Queue

```cpp
bool push(const T& t) {
    size_t tail = tail_.load(relaxed);          // relaxed: we own tail_
    size_t next = (tail + 1) % N;
    if (next == head_.load(acquire)) {          // acquire: pair with consumer's release
        return false;
    }
    data_[tail] = t;
    tail_.store(next, release);                 // release: "data at this slot is ready"
}

bool pop(T& out) {
    size_t head = head_.load(relaxed);          // relaxed: we own head_
    if (head == tail_.load(acquire)) {          // acquire: pair with producer's release
        return false;
    }
    out = data_[head];                          // safe: acquire above guarantees visibility
    head_.store((head + 1) % N, release);       // release: "slot is free, head has moved"
}
```

The symmetry:
- Producer owns `tail_` → loads relaxed, stores release
- Consumer owns `head_` → loads relaxed, stores release
- Each reads the other's index with acquire to synchronize

### memory_order_seq_cst

The strongest ordering and the default if you specify nothing. Gives everything acquire/release gives, plus a **single total order** across all seq_cst operations across all threads. More expensive — requires a full memory fence on x86/ARM. For SPSC queues, release/acquire is sufficient and preferred.

---

## 5. Happens-Before

A formal relationship: if A **happens-before** B, all effects of A are visible to B.

Key relationships that establish happens-before:
- **Sequenced-before**: within a single thread, code order establishes this
- **Synchronizes-with**: a release-store observed by an acquire-load creates a cross-thread edge
- **Transitivity**: if A happens-before B, and B happens-before C, then A happens-before C

Transitivity is powerful — thread 3 can have visibility guarantees about thread 1's writes even if they never directly synchronized, as long as there's a chain through thread 2.

Important: happens-before is about **visibility guarantees**, not wall clock timing. It means "if B observes A's effect, B sees everything A did before that effect."

This is the same concept as Java's Memory Model happens-before — the idea is universal across languages with memory models.

---

## 6. C++ Concepts for Queue Interface

### Two-Parameter Concept

```cpp
template<typename T, typename U>
concept BoundedQ = requires(T t, const U& uin, U& uout) {
    { t.pop(uout) } -> std::same_as<bool>;
    { t.push(uin) } -> std::same_as<bool>;
    { T::impl_type } -> std::same_as<const std::string_view&>;
};
```

Notes:
- `T` is the queue type, `U` is the element type
- Dummy parameters in `requires(...)` determine what expressions are valid — be deliberate about `const U&` vs `U&&`
- Static member access like `T::impl_type` yields a `const T&` — `same_as` requires exact match including the reference
- `convertible_to` is often more practical than `same_as` for concept constraints

### Using the Concept

```cpp
template<typename QueueType>
requires BoundedQ<QueueType, int>
static void runTest(QueueType& queue) { ... }
```

The `auto` in terse concept syntax always fills the **first** parameter. For multi-parameter concepts, either reorder parameters or use the verbose `requires` clause.

### Concept Hierarchy (like Haskell typeclasses)

```cpp
template<typename T, typename U>
concept BoundedQ = requires(...) { ... };  // basic interface

template<typename T, typename U>
concept MoveableBoundedQ = BoundedQ<T, U> && requires(T t, U&& rval) {
    { t.push(std::declval<U&&>()) } -> std::same_as<bool>;
};
```

Use `std::declval` in requires expressions to conjure expressions of any reference type without dummy variables. Worth it when you need precise control over value categories.

Only build the hierarchy when genuinely needed — the standard library pattern is a good model but don't over-engineer upfront.

---

## 7. Benchmark Results and Interpretation

### Setup

- Retry vs no-retry: retry forces no drops (clean throughput comparison), no-retry shows drop behavior under pressure
- Use `steady_clock` not `system_clock` — system clock can be adjusted by NTP and produce negative elapsed times
- Use `int` or fixed-size types rather than `std::string` to avoid heap allocation noise

### What the Numbers Show

At 500k items with retry (pure throughput):
- Lock-free is ~17x faster than mutex at large counts
- At small counts (~100 items), mutex can be competitive — thread creation overhead dominates

### Why Lock-Free Wins

Every mutex push/pop acquires and releases a lock — even with no contention, this involves memory fence instructions and potential kernel involvement. Lock-free SPSC has no such overhead in the common case — producer and consumer operate on different ends of the queue with minimal synchronization.

### The Accidental Rate Limiter Effect

With `retryOnFull=false`, mutex drops **fewer** items than lock-free at small N. This seems counterintuitive but makes sense: the mutex is accidentally throttling the producer, giving the consumer time to drain. Lock-free producer runs so fast it fills and drops before the consumer gets scheduled.

This shows up as the N-1 pattern — lock-free collected exactly N-1 items because the producer filled the queue once and finished before any real interleaving occurred.

### Lock-Free Is Not Unconditionally Faster

| Scenario | Winner |
|---|---|
| High throughput, large counts | Lock-free by large margin |
| Small counts (~100 items) | Comparable or mutex wins |
| Same CPU core for both threads | Advantage shrinks |
| No-retry, small N | Mutex drops fewer (throttling effect) |

---

## 8. Correct Termination Logic

### The Bug

```cpp
// WRONG — race condition
while (true) {
    if (queue.pop(temp)) {
        ++result;
    } else if (isProducerDone.load()) {
        break;  // might exit with items still in queue
    }
}
```

Race: consumer sees empty queue AND isProducerDone=true, exits — but producer may have just set the flag with items still unread.

### The Fix

```cpp
// CORRECT — drain completely before exiting
while (true) {
    if (queue.pop(temp)) {
        ++result;
    } else if (isProducerDone.load()) {
        while (queue.pop(temp)) {  // guaranteed to terminate — no new items coming
            ++result;
        }
        break;
    }
}
```

Once `isProducerDone` is true, no new items will ever be added, so draining until empty is guaranteed to terminate and guaranteed to not miss anything.

---

## 9. Key Takeaways

- **Ring buffers waste one slot** by design — it's the simplest way to distinguish full from empty without extra state
- **pop(T& out) pattern** exists for exception safety — sequence matters, copy before modify
- **Release-acquire is a pair** — release alone or acquire alone gives no cross-thread guarantee
- **Relaxed is for variables you own** — only the other side needs acquire/release to synchronize
- **Mutex secretly uses acquire/release** — lock-free just makes it explicit
- **happens-before is about visibility**, not timing
- **Lock-free wins at scale** — at small counts the overhead difference is negligible
- **steady_clock for benchmarking** — system_clock can go backwards
- **Termination races are subtle** — always drain completely after producer signals done
