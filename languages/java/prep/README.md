### Java interviewing question list that we use

## Java Core & JVM
# Stack vs Heap
- Stack: thread-private, primitives & local refs, fixed-size (StackOverflowError)
- Heap: shared across threads, GC-managed, stores objects
- Escape analysis - when JVM can stack-allocate objects

# GC Concepts
- Generational model: Young (Eden + Survivors) -> Old
- GC root & mark-sweep-compact-cycle
- Stop-the-world pauses -- impact & mitigation
- Collator trade-offs: G1 vs ZGC vs Shenandoah

# Pass By Value vs Reference
- Java is always pass-by-value (reference is copied, not the object)
- Reassigning param inside method does NOT affect caller
- Mutating object via copied ref DOES affect caller

# Compilation & Execution
- Compiled vs interpreted -- know the difference and give examples
- Java: Source -> javac -> bytecode -> JIT (C1/C2) to native at runtime
- AOT compilation (GraalVM native image) -- when and why
- Class loading: bootstrap -> extension -> app

## Concurrency
# Java Memory Model (JMM)
- Happens-before relationship - the rule that governs when one thread's write is visible to another
- What establishes happens-before: synchronized, volatile, thread start/join, final fields
- why code can appear to execute out of order (compiler/CPU re-ordering) without these guarantees

# Threading
- name the basic thread states (new, runnable, blocked, waiting, terminated)
- Daemon (background) vs user (foreground) threads - JVM shutdown behavior
- volatile - visibility guarantees & when it is not enough

# Locking
- `synchronized` (intrinsic monitor lock) vs `ReentrantLock`
- `ReadWriteLock` - when reads vastly outnumber writes
- Lock-free approaches: CAS, AtomicXXX classes
- Optimistic vs pessimistic locking trade-offs

# Concurrency Issues
- Race condition: unsynchronized shared mutable state
- Deadlock: what it is and how to prevent it
- Livelock: threads respond to each other without progress
- Starvation: thread never gets CPU/lock (unfair scheduling)
- Detection: jstack, thread dumps, tooling

# Thread Pools vs Dedicated Threads
- Thread pool: shared workers, good for IO-bound (many short-lived tasks, DB calls, HTTP)
- Dedicated thread: pinned to one job, good for CPU-bound / latency critical hot paths
- CPU-bound sizing: pool ~ nCores -- more threads = context-switch overhead
- IO-bound sizing: threads mostly idle waiting -- can safely overprovision

# Ring Buffers
- Fixed-size circular array with head/tail pointers
- Lock-free single-producer/single-consumer pattern
- Why fast: cache-friendly, no GC pressure, predictable latency
- Real-world: LMAX disruptor

## OS
# Data Path - App to Disk/Network
- App write -> user-space buffer -> syscall -> kernel buffer (page cache) -> device

# Kernel Bypass
- Standard path: app -> syscall -> kernel -> NIC (context switch, copying)
- Bypass: app talks directly to NIC -- skip kernel (DPDK, io_uring, Solarflare OpenOnload)
- why it matters: eliminates syscall overhead & kernel buffering for ultra-low-latency

# CPU Pinning & Affinity
- Bind thread/process to specific core - avoid migration & cache invalidation
- Trading use case: pin critical thread (e.g. order gateway) to isolated core

## Networking
# TCP vs UDP
- TCP: connection-oriented 