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
- TCP: connection-oriented, reliable, ordered, 3-way handshake
- UDP: connectionless, best-effort, lower latency
- Trading: UDP multicast for market dtaa (feed handlers) -- one-to-many, speed over reliability
- Trading: TCP for order flow - must guarantee delivery & ordering
- Reliable UDP: application-level retransmit/sequencing on top of UDP (e.g. Aeron)
- Unicast vs multicast - when each is appropriate

# IPC - Same Server
- Unix domain sockets - most flexible
- Shared memory (mmap, shmget) - fastest, need synchronization
- Pipes / named pipes (FIFO)
- Trade-offs: Latency vs complexity vs safety

# IPC - Different Servers
- TCP/UDP sockets, HTTP/REST, gPRC(HTTP/2 + protobuf)
- Messgae brokers: Kafka, RabbitMQ - async decoupling
- Serialization: JSON vs Protobuf vs Avro

# Data Encoding
- Text formats: JSON, XML, CSV
- Binary formats: protobuf, Avro, MessagePack
- Schema evolution & backward/forward compatibility
- Character encoding: UTF-8 vs UTF-16, Base64 for binary-over-text

## System Design
# SOLID Principles
- S - Single Responsibility: one reason to change
- O - Open/Closed: extend without modifying
- L - Liskov Substitution: subtypes must be substitutable
- I - Interface Segregation: no forced unused dependencies
- D - Dependency Inversion: depend on abstractions

# OMS/EMS (optional)
- OMS vs EMS - what each owns (order lifecycle vs execution/routing)
- Order lifecycle: new -> validated -> routed -> partial fill -> filled/cancelled
- Key OMS features to mention: state management, pre-trade checks, compliance, position tracking
- Key EMS features to mention: algo execution, market connectivity, real-time market data

## Role-Specific (BAM Exec Tech)
# Execution Algorithms
- Experience building execution algos (TWAP, VWAP, POV, MOO, MOC)
- Millisecond-awareness: where does latency matter most in the order path

# Low-latency Data
- Techniques: object pooling, off-heap memory, avoiding GC pauses
- Lock-free structures in parctice (Disrupotr, CAS loops, Atomic classes)
- Measuring & profiling latency (JMH, Flame graphs, percentile distributions)

# Distributed Systems
- Designing for fault tolerance: failover, idempotency, exactly-once sematics
- State management across nodes - event sourcing, replication, consensus

# AI awareness
- Can name tools they use (Copilot, Claude, ChatGPT)
- basic awareness e.g. latest Claude/OpenAI model
- what is a Claude skill

# Communication & Pace
- Can explain technical trade-offs clearly to non-technical stakeholders
- Scenario: it is Friday 5PM, a production issue is starting to surface - what do you do?

## Coding Test
# Counting set
- set sematics and equals/hascode constract
- identifies the addAll() double counting bug
- Explains why composition is preferable in inheritance in this scenario

# Testability
- Identifies testability issus such as HTTP calls, randomness etc
- Refactors towards DI, extracting calls to HTTP client and Random()
- Cleanly refactors code