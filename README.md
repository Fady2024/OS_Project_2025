# 🖥️ FOS - FCIS Operating System

<div align="center">

![Architecture](https://img.shields.io/badge/Architecture-x86__32-FF6B6B?style=for-the-badge&logo=intel&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%20%2F%20Assembly-4ECDC4?style=for-the-badge&logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Bochs%20Emulator-45B7D1?style=for-the-badge&logo=vmware&logoColor=white)
![Type](https://img.shields.io/badge/Type-Educational-96CEB4?style=for-the-badge&logo=bookstack&logoColor=white)
![License](https://img.shields.io/badge/License-GPL--3.0-FFEAA7?style=for-the-badge&logo=gnu&logoColor=black)

<br/>

### 🎓 A Comprehensive Educational Operating System Kernel

**Developed for Teaching OS Concepts at FCIS - Ain Shams University**

*Based on MIT Exokernel Architecture with Extensive Modifications*

<br/>

[![GitHub stars](https://img.shields.io/github/stars/Fady2024/OS_Project_2025?style=social)](https://github.com/Fady2024/OS_Project_2025)
[![GitHub forks](https://img.shields.io/github/forks/Fady2024/OS_Project_2025?style=social)](https://github.com/Fady2024/OS_Project_2025)
[![GitHub issues](https://img.shields.io/github/issues/Fady2024/OS_Project_2025)](https://github.com/Fady2024/OS_Project_2025/issues)
[![GitHub license](https://img.shields.io/github/license/Fady2024/OS_Project_2025)](https://github.com/Fady2024/OS_Project_2025/blob/main/LICENSE)

---

[🚀 Quick Start](#-quick-start) •
[📖 Documentation](#-full-documentation) •
[👥 Team](#-development-team) •
[🧪 Testing](#-testing-framework)

</div>

---

## 📋 Table of Contents

<details>
<summary><b>📚 Click to expand full table of contents</b></summary>

1. [🌟 Introduction](#-introduction)
   - [What is FOS?](#what-is-fos)
   - [Educational Goals](#-educational-goals)
   - [Why FOS?](#-why-fos)
2. [✨ Features](#-features)
   - [Core Features](#-core-kernel-features)
   - [Memory Features](#-memory-management-features)
   - [Process Features](#-process-management-features)
3. [🏗️ System Architecture](#️-system-architecture)
   - [Memory Layout](#-memory-layout)
   - [Key Constants](#-key-memory-constants)
4. [📁 Project Structure](#-project-structure)
5. [💾 Memory Management](#-memory-management)
   - [Physical Memory](#-physical-memory-management)
   - [Virtual Memory](#-virtual-memory-management)
   - [Kernel Heap](#-kernel-heap-kheap)
   - [User Heap](#-user-heap)
6. [🔄 Process Management](#-process-management)
   - [Process Structure](#-environment-structure)
   - [Process States](#-process-states)
   - [Process Lifecycle](#-process-lifecycle)
7. [⏰ CPU Scheduling](#-cpu-scheduling)
   - [Round Robin](#1️⃣-round-robin-rr)
   - [MLFQ](#2️⃣-multi-level-feedback-queue-mlfq)
   - [BSD Scheduler](#3️⃣-bsd-scheduler)
   - [Priority RR](#4️⃣-priority-round-robin-prirr)
8. [📄 Page Replacement](#-page-replacement-algorithms)
9. [📞 System Calls](#-system-calls)
10. [🚀 Quick Start](#-quick-start)
11. [💻 Commands Reference](#-commands-reference)
12. [🧪 Testing Framework](#-testing-framework)
13. [🐛 Debugging](#-debugging)
14. [👨‍🏫 Course Instructor](#-course-instructor)
15. [👥 Development Team](#-development-team)
16. [📜 License](#-license)
17. [🙏 Acknowledgments](#-acknowledgments)

</details>

---

## 🌟 Introduction

### What is FOS?

**FOS (FCIS Operating System)** is a state-of-the-art educational operating system kernel that bridges the gap between theoretical OS concepts and practical implementation. Born from the legendary **MIT Exokernel** project, FOS has evolved into a comprehensive learning platform that empowers students with hands-on experience in systems programming.

> 💡 **Fun Fact:** FOS contains over **15,000+ lines of C code** and **100+ user programs** for testing and learning!

### 🎯 Educational Goals

<table>
<tr>
<td align="center" width="25%">

### 🧠
### Memory Management
Learn virtual memory, paging, heap allocation, and page replacement strategies

</td>
<td align="center" width="25%">

### ⚡
### Process Scheduling
Master multiple scheduling algorithms with real-time process management

</td>
<td align="center" width="25%">

### 🔒
### System Protection
Understand kernel/user mode separation, trap handling, and interrupts

</td>
<td align="center" width="25%">

### 💻
### Systems Programming
Get hands-on with low-level C and x86 Assembly programming

</td>
</tr>
</table>

### 🏆 Why FOS?

| Advantage | Description |
|:----------|:------------|
| 🎓 **Educational Focus** | Designed specifically for learning, not just running |
| 📚 **Well Documented** | Extensive comments and documentation throughout |
| 🔧 **Modular Design** | Easy to understand and modify individual components |
| 🧪 **Comprehensive Tests** | 50+ test cases covering all major features |
| 🌍 **Real-World Concepts** | Implements actual OS algorithms used in production systems |

---

## ✨ Features

### 🔥 Core Kernel Features

<details>
<summary><b>💾 Memory Management</b></summary>

| Feature | Description | Status |
|:--------|:------------|:------:|
| Virtual Memory | Full paging support with 4KB pages | ✅ |
| Two-Level Page Tables | Efficient address translation | ✅ |
| Memory Protection | Kernel/User space separation | ✅ |
| Frame Allocator | Physical frame management | ✅ |
| Page Table Management | Dynamic creation and manipulation | ✅ |

</details>

<details>
<summary><b>⚡ Process Management</b></summary>

| Feature | Description | Status |
|:--------|:------------|:------:|
| Multi-tasking | Pre-emptive multitasking | ✅ |
| Context Switching | Fast assembly-based switching | ✅ |
| Process Isolation | Separate address spaces | ✅ |
| ELF Loading | Load executable programs | ✅ |
| Process Priority | Priority-based scheduling | ✅ |

</details>

<details>
<summary><b>📄 Page Replacement</b></summary>

| Algorithm | Type | Status |
|:----------|:-----|:------:|
| FIFO | Simple | ✅ |
| Clock | Second Chance | ✅ |
| Modified Clock | Dirty Bit Aware | ✅ |
| LRU (Time) | Timestamp Based | ✅ |
| LRU (Lists) | Active/Second Lists | ✅ |
| N-Chance | Configurable | ✅ |
| Optimal | For Testing | ✅ |

</details>

<details>
<summary><b>⏰ CPU Scheduling</b></summary>

| Algorithm | Description | Status |
|:----------|:------------|:------:|
| Round Robin | Equal time quantum | ✅ |
| MLFQ | Multi-level feedback | ✅ |
| BSD | FreeBSD-based | ✅ |
| Priority RR | Priority with RR | ✅ |

</details>

<details>
<summary><b>🔗 Inter-Process Communication</b></summary>

| Feature | Description | Status |
|:--------|:------------|:------:|
| Shared Memory | Share memory regions | ✅ |
| Kernel Semaphores | Synchronization | ✅ |
| User Semaphores | User-level sync | ✅ |
| Sleep/Wakeup | Process blocking | ✅ |

</details>

### 📊 Feature Statistics

| Category | Count |
|:---------|:------|
| 📄 Scheduling Algorithms | 4 |
| 🔄 Page Replacement Algorithms | 7 |
| 💾 Memory Allocation Strategies | 6 |
| 📞 System Calls | 30+ |
| 👤 User Programs | 100+ |
| 🧪 Test Cases | 50+ |
| 💻 Kernel Commands | 40+ |

---

## 🏗️ System Architecture

### 🗺️ Memory Layout

The FOS kernel uses a well-defined memory layout that separates kernel and user space:

| Region | Start Address | End Address | Permissions | Description |
|:-------|:--------------|:------------|:------------|:------------|
| **Invalid Memory** | `0xFFFFF000` | `0xFFFFFFFF` | `--/--` | Guard page at top |
| **Kernel Heap** | `0xF6000000` | `0xFFFFF000` | `RW/--` | Dynamic kernel memory |
| **Remapped Physical** | `0xF0000000` | `0xF6000000` | `RW/--` | Physical memory access |
| **Kernel Base** | `0xF0000000` | - | - | Kernel/User boundary |
| **Virtual Page Table** | `0xEFC00000` | `0xF0000000` | `RW/--` | Kernel page table access |
| **Scheduler Stack** | Below VPT | - | `RW/--` | Per-CPU kernel stacks |
| **User Limit** | `0xEF800000` | - | - | Top of user memory |
| **User Page Table** | `0xEF400000` | `0xEF800000` | `R-/R-` | User-readable page table |
| **Read-Only ENVs** | `0xEEC00000` | `0xEF000000` | `R-/R-` | Process info |
| **User Exception Stack** | `0xEEBFF000` | `0xEEC00000` | `RW/RW` | Exception handling |
| **User Stack** | Below USTACKTOP | - | `RW/RW` | Normal stack (grows up) |
| **User Heap** | `0x80000000` | `0xA0000000` | `RW/RW` | Dynamic user memory |
| **Program Code** | `0x00800000` | - | `R-/R-` | .text and .data |

### 🔑 Key Memory Constants

```c
// Kernel Memory
#define KERNEL_BASE         0xF0000000   // Kernel virtual address start
#define KERNEL_HEAP_START   0xF6000000   // Kernel heap begins here
#define KERNEL_HEAP_MAX     0xFFFFF000   // Kernel heap maximum
#define KERNEL_STACK_SIZE   (8*PAGE_SIZE) // 32 KB per kernel stack

// User Memory
#define USER_HEAP_START     0x80000000   // User heap begins here
#define USER_HEAP_MAX       0xA0000000   // User heap maximum (512 MB heap)
#define USER_TOP            0xEEC00000   // Top of user-accessible memory
#define USTACKTOP           0xEEBFE000   // Top of user stack

// Common
#define PAGE_SIZE           4096         // 4 KB pages
#define PTSIZE              (1024*PAGE_SIZE) // 4 MB per page table
```

---

## 📁 Project Structure

<details>
<summary><b>🚀 boot/ - Boot Loader</b></summary>

The boot loader is the first code to execute when the system starts.

| File | Description |
|:-----|:------------|
| `boot.S` | Assembly boot sector (512 bytes), enables A20, switches to protected mode |
| `main.c` | Loads kernel ELF from disk, validates headers, jumps to kernel |
| `sign.pl` | Perl script to add boot sector signature |
| `Makefrag` | Build configuration for boot loader |

</details>

<details>
<summary><b>🧠 kern/ - Kernel Source Code</b></summary>

The heart of the operating system.

| Directory | Files | Description |
|:----------|:------|:------------|
| `kern/` | `init.c`, `entry.S` | Kernel entry and initialization |
| `kern/cmd/` | `commands.c`, `command_prompt.c` | 40+ kernel commands |
| `kern/cons/` | `console.c` | VGA text mode console driver |
| `kern/cpu/` | `sched.c`, `context_switch.S`, `kclock.c` | CPU and scheduling |
| `kern/mem/` | `memory_manager.c`, `kheap.c`, `working_set_manager.c` | Memory management |
| `kern/proc/` | `user_environment.c`, `user_programs.c` | Process management |
| `kern/trap/` | `trap.c`, `fault_handler.c`, `syscall.c` | Trap and interrupt handling |
| `kern/disk/` | `pagefile_manager.c` | Page file operations |
| `kern/tests/` | Various test files | Kernel test suite |

</details>

<details>
<summary><b>📚 inc/ - Header Files</b></summary>

Shared definitions used by kernel and user programs.

| File | Description |
|:-----|:------------|
| `memlayout.h` | Memory layout constants and definitions |
| `mmu.h` | MMU and paging definitions |
| `environment_definitions.h` | Process (Env) structure |
| `trap.h` | Trap frame definitions |
| `queue.h` | List/Queue macros |
| `syscall.h` | System call numbers |
| `types.h` | Basic type definitions |
| `dynamic_allocator.h` | Heap allocator interface |

</details>

<details>
<summary><b>📖 lib/ - User Library</b></summary>

Runtime library for user programs.

| File | Description |
|:-----|:------------|
| `libmain.c` | User program entry point |
| `syscall.c` | System call wrappers |
| `uheap.c` | User heap (malloc/free) |
| `dynamic_allocator.c` | Block-based allocator |
| `string.c` | String manipulation |
| `printf.c` | Formatted output |

</details>

<details>
<summary><b>👤 user/ - User Programs (100+)</b></summary>

Sample user programs for testing and learning.

| Category | Examples |
|:---------|:---------|
| Basic | `fos_helloWorld.c`, `fos_factorial.c`, `fos_fibonacci.c` |
| Sorting | `quicksort_*.c`, `mergesort_*.c` |
| Memory Tests | `tst_malloc_*.c`, `tst_free_*.c`, `heap_program.c` |
| Page Replacement | `tst_page_replacement_*.c` |
| Shared Memory | `tst_sharing_*.c` |
| Semaphores | `tst_semaphore_*.c`, `tst_ksemaphore_*.c` |
| Scheduling | `priRR_*.c` |

</details>

---

## 💾 Memory Management

### 🔷 Physical Memory Management

FOS uses a frame allocator to manage physical memory pages.

```c
// Allocate a physical frame
int allocate_frame(struct FrameInfo **ptr_frame_info);

// Free a physical frame
void free_frame(struct FrameInfo *ptr_frame_info);

// Decrement reference count
void decrement_references(struct FrameInfo* ptr_frame_info);
```

<details>
<summary><b>📊 Frame Info Structure</b></summary>

```c
struct FrameInfo {
    Page_LIST_entry_t prev_next_info;  // Free list links
    uint16 references;                  // Reference count (for sharing)
    struct Env *proc;                   // Owning process
    unsigned char isBuffered;           // In modified buffer?
};
```

</details>

### 🔷 Virtual Memory Management

FOS implements two-level paging with 4KB pages.

```c
// Get page table for a virtual address
int get_page_table(uint32 *ptr_page_directory, 
                   const uint32 virtual_address, 
                   uint32 **ptr_page_table);

// Create a new page table
void create_page_table(uint32 *ptr_directory, 
                       const uint32 virtual_address);

// Map a frame to a virtual address
int map_frame(uint32 *ptr_page_directory, 
              struct FrameInfo *ptr_frame_info, 
              uint32 virtual_address, int perm);

// Unmap a virtual address
void unmap_frame(uint32 *ptr_page_directory, 
                 uint32 virtual_address);

// Invalidate TLB entry
void tlb_invalidate(uint32 *ptr_page_directory, void *virtual_address);
```

### 🔷 Kernel Heap (KHEAP)

Dynamic memory allocation within the kernel.

```c
// Allocate kernel memory
void* kmalloc(unsigned int size);

// Free kernel memory
void kfree(void* virtual_address);

// Reallocate kernel memory
void* krealloc(void* virtual_address, uint32 new_size);

// Address conversion
unsigned int kheap_virtual_address(unsigned int physical_address);
unsigned int kheap_physical_address(unsigned int virtual_address);
```

**Allocation Strategies:**

| Strategy | Command | Description | Best For |
|:---------|:--------|:------------|:---------|
| 🎯 First Fit | `khfirstfit` | First block that fits | General purpose |
| 🔍 Best Fit | `khbestfit` | Smallest fitting block | Minimize fragmentation |
| ➡️ Next Fit | `khnextfit` | Continue from last | Faster allocation |
| 📏 Worst Fit | `khworstfit` | Largest block | Large allocations |
| ⚙️ Custom Fit | `khcustomfit` | Optimized hybrid | Best performance |

### 🔷 User Heap

Each process has its own heap for dynamic memory.

```c
// User-level allocator (in lib/)
void* malloc(uint32 size);
void free(void* virtual_address);
void* realloc(void* virtual_address, uint32 new_size);

// System calls for heap management
void* sys_sbrk(int increment);
void allocate_user_mem(uint32 va, uint32 size);
void free_user_mem(uint32 va, uint32 size);
```

---

## 🔄 Process Management

### 📦 Environment Structure

Each process is represented by an `Env` structure:

<details>
<summary><b>View Complete Env Structure</b></summary>

```c
struct Env {
    // ═══════════════════════════════════════
    // MAIN INFORMATION
    // ═══════════════════════════════════════
    struct Trapframe *env_tf;        // Saved registers during trap
    struct Context *context;          // Context switch registers
    LIST_ENTRY(Env) prev_next_info;  // Queue links
    int32 env_id;                    // Unique process identifier
    int32 env_parent_id;             // Parent process ID
    unsigned env_status;             // Current state
    int priority;                    // Scheduling priority
    char prog_name[PROGNAMELEN];     // Program name (64 chars)
    void* channel;                   // Sleep channel (blocking)
    
    // ═══════════════════════════════════════
    // ADDRESS SPACE
    // ═══════════════════════════════════════
    uint32 *env_page_directory;      // Page directory (virtual)
    uint32 env_cr3;                  // Page directory (physical)
    uint32 initNumStackPages;        // Initial stack pages
    char* kstack;                    // Kernel stack bottom
    
    // Page file management
    uint32* disk_env_pgdir;
    unsigned int disk_env_pgdir_PA;
    uint32* disk_env_tabledir;
    unsigned int disk_env_tabledir_PA;
    
    // ═══════════════════════════════════════
    // WORKING SET
    // ═══════════════════════════════════════
    unsigned int page_WS_max_size;   // Maximum working set size
    struct WS_List page_WS_list;     // Working set elements
    struct WorkingSetElement* page_last_WS_element;
    
    // LRU Lists
    struct WS_List ActiveList;       // Hot pages
    struct WS_List SecondList;       // Cold pages
    int ActiveListSize;              // Max active list size
    int SecondListSize;              // Max second list size
    
    // ═══════════════════════════════════════
    // STATISTICS
    // ═══════════════════════════════════════
    uint32 pageFaultsCounter;
    uint32 tableFaultsCounter;
    uint32 freeingFullWSCounter;
    uint32 freeingScarceMemCounter;
    uint32 nModifiedPages;
    uint32 nNotModifiedPages;
    uint32 env_runs;                 // Times this env has run
    uint32 nPageIn, nPageOut, nNewPageAdded;
    uint32 nClocks;
};
```

</details>

### 🔄 Process States

| State | Value | Description | Next States |
|:------|:-----:|:------------|:------------|
| 🆓 `ENV_FREE` | 0 | Available in free list | NEW |
| 🆕 `ENV_NEW` | 4 | Newly created | READY |
| ✅ `ENV_READY` | 1 | Ready to run | RUNNING |
| ▶️ `ENV_RUNNING` | 2 | Currently executing | READY, BLOCKED, EXIT |
| ⏸️ `ENV_BLOCKED` | 3 | Waiting for resource | READY |
| 🚪 `ENV_EXIT` | 5 | Terminated normally | FREE |
| ☠️ `ENV_KILLED` | 6 | Killed externally | FREE |

### 🔄 Process Lifecycle

```
┌─────────────────────────────────────────────────────────────┐
│                    PROCESS LIFECYCLE                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   FREE ──create──► NEW ──schedule──► READY ◄───────┐       │
│     ▲                                   │           │       │
│     │                              dispatch         │       │
│     │                                   ▼           │       │
│     │                               RUNNING ────────┤       │
│     │                                   │           │       │
│   free                        ┌─────────┼─────────┐ │       │
│     │                         │         │         │ │       │
│     │                         ▼         ▼         ▼ │       │
│     └──────────────────── BLOCKED    EXIT    preempt        │
│                               │                             │
│                           wakeup ──► READY                  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 📋 Process Functions

```c
// Create a new process
struct Env* env_create(char* user_program_name, 
                       unsigned int page_WS_size,
                       unsigned int LRU_second_list_size,
                       unsigned int percent_WS_pages_to_remove);

// Free a process and its resources
void env_free(struct Env *e);

// Exit current process
void env_exit(void);

// Get current running process
struct Env* get_cpu_proc(void);

// Convert env ID to env pointer
int envid2env(int32 envid, struct Env **env_store, bool checkperm);
```

---

## ⏰ CPU Scheduling

FOS implements **four different scheduling algorithms**, each with unique characteristics.

### 1️⃣ Round Robin (RR)

The simplest fair scheduling algorithm.

| Property | Value |
|:---------|:------|
| **Fairness** | ⭐⭐⭐⭐⭐ Equal time for all |
| **Complexity** | ⭐ Very simple |
| **Responsiveness** | ⭐⭐⭐ Depends on quantum |
| **Overhead** | ⭐⭐ Context switches |

**Command:** `schedRR <quantum>`

**How it works:**
1. All processes are in a single FIFO queue
2. Each process runs for exactly `quantum` time units
3. After quantum expires, process moves to end of queue
4. Next process in queue starts executing

---

### 2️⃣ Multi-Level Feedback Queue (MLFQ)

Adaptive scheduling that learns process behavior.

| Property | Value |
|:---------|:------|
| **Fairness** | ⭐⭐⭐ Based on behavior |
| **Complexity** | ⭐⭐⭐⭐ Multiple queues |
| **Responsiveness** | ⭐⭐⭐⭐⭐ Interactive friendly |
| **Overhead** | ⭐⭐⭐ Queue management |

**Command:** `schedMLFQ <num_levels> <q1> <q2> ...`

**How it works:**
1. Multiple priority queues with different time quantums
2. New processes start at highest priority (shortest quantum)
3. If process uses full quantum → move to lower priority
4. If process blocks for I/O → stay at current or move up
5. Periodic priority boost to prevent starvation

---

### 3️⃣ BSD Scheduler

Based on the FreeBSD scheduler with dynamic priority calculation.

| Property | Value |
|:---------|:------|
| **Fairness** | ⭐⭐⭐⭐ Dynamic adjustment |
| **Complexity** | ⭐⭐⭐⭐ Priority calculation |
| **Responsiveness** | ⭐⭐⭐⭐ Good for mixed workloads |
| **Overhead** | ⭐⭐⭐ Recalculation |

**Command:** `schedBSD <num_levels> <quantum>`

**Priority Formula:**
```
priority = PRI_BASE + (recent_cpu / 4) + (nice × 2)
```

---

### 4️⃣ Priority Round Robin (PRIRR)

Combines priority scheduling with round-robin within each level.

| Property | Value |
|:---------|:------|
| **Fairness** | ⭐⭐⭐ Within priority levels |
| **Complexity** | ⭐⭐⭐ Multiple queues + RR |
| **Responsiveness** | ⭐⭐⭐⭐ High priority = fast |
| **Overhead** | ⭐⭐⭐ Anti-starvation |

**Command:** `schedPRIRR <num_priorities> <quantum> <starvation_threshold>`

**Priority Levels:**
| Level | Name | Value |
|:------|:-----|:-----:|
| 5 | HIGH | Highest priority |
| 4 | ABOVENORMAL | Above average |
| 3 | NORMAL | Default |
| 2 | BELOWNORMAL | Below average |
| 1 | LOW | Lowest priority |

**Anti-Starvation:**
After `starvation_threshold` ticks without running, low-priority processes get a priority boost.

---

## 📄 Page Replacement Algorithms

FOS implements **7 page replacement algorithms** for virtual memory management.

### 📊 Algorithm Comparison

| Algorithm | Command | Complexity | Accuracy | Best For |
|:----------|:--------|:-----------|:---------|:---------|
| 🔢 **FIFO** | `fifo` | O(1) | Low | Simple systems |
| ⏰ **Clock** | `clock` | O(n) | Medium | General purpose |
| ⏰ **Modified Clock** | `modclock` | O(n) | Good | Write-heavy |
| 📈 **LRU (Time)** | `lru 1` | O(n) | High | Accuracy needed |
| 📈 **LRU (Lists)** | `lru 2` | O(1) | Good | Fast approximation |
| 🔄 **N-Chance** | `nthclk <n>` | O(n) | Configurable | Flexible |
| ⭐ **Optimal** | `optimal` | O(n×m) | Perfect | Testing only |

<details>
<summary><b>🔢 FIFO - First In First Out</b></summary>

The simplest replacement algorithm.

- **Idea:** Replace the oldest page in memory
- **Pros:** Very simple to implement
- **Cons:** Doesn't consider page usage (Belady's anomaly possible)

</details>

<details>
<summary><b>⏰ Clock (Second Chance)</b></summary>

An approximation of LRU using a reference bit.

- **Idea:** Give pages a "second chance" if recently used
- **Implementation:** Circular buffer with clock hand
- **Reference bit:** Set to 1 when page accessed, cleared by clock hand

</details>

<details>
<summary><b>⏰ Modified Clock</b></summary>

Clock algorithm that also considers the dirty bit.

- **Idea:** Prefer replacing clean pages (no write-back needed)
- **Priority:** (R=0, M=0) > (R=0, M=1) > (R=1, M=0) > (R=1, M=1)

</details>

<details>
<summary><b>📈 LRU - Least Recently Used</b></summary>

Replace the page that hasn't been used for the longest time.

**Two implementations:**
1. **Time-based:** Each page has a timestamp of last access
2. **Lists-based:** Active list (hot) and Second list (cold)

</details>

<details>
<summary><b>⭐ Optimal (Belady's)</b></summary>

The theoretically optimal algorithm - replace the page that won't be used for the longest time.

- **Uses:** Future knowledge (reference string)
- **Purpose:** Benchmarking other algorithms
- **Reality:** Cannot be implemented in real systems

</details>

---

## 📞 System Calls

FOS provides **30+ system calls** for user programs.

<details>
<summary><b>🔄 Process Management</b></summary>

```c
int32 sys_getenvid(void);              // Get current process ID
void sys_exit(void);                    // Terminate process
int sys_create_env(char* prog_name, 
                   unsigned int page_WS_size,
                   unsigned int lru_second_size,
                   unsigned int percent_WS_remove);
void sys_run_env(int32 envid);          // Start process execution
void sys_destroy_env(int envid);        // Destroy a process
void sys_yield(void);                   // Give up CPU
```

</details>

<details>
<summary><b>💾 Memory Management</b></summary>

```c
int sys_allocate_page(void *va, int perm);
int sys_map_frame(int32 srcenvid, void *srcva,
                  int32 dstenvid, void *dstva, int perm);
int sys_unmap_frame(int32 envid, void *va);
void* sys_sbrk(int increment);
uint32 sys_get_brk();
void sys_free_user_mem(uint32 va, uint32 size);
uint32 sys_calculate_free_frames();
uint32 sys_calculate_modified_frames();
```

</details>

<details>
<summary><b>🔗 Shared Memory</b></summary>

```c
int sys_createSharedObject(char* shareName, uint32 size, 
                           uint8 isWritable, void** returned_shared_address);
int sys_getSharedObject(int32 ownerEnvID, char* shareName, 
                        void** returned_shared_address);
int sys_freeSharedObject(int32 sharedObjectID, void* startVA);
```

</details>

<details>
<summary><b>🔒 Semaphores</b></summary>

```c
// Kernel semaphores
int sys_createSemaphore(char* name, int value);
void sys_waitSemaphore(int id);
void sys_signalSemaphore(int id);

// User semaphores
void sys_acquire_lock(struct uspinlock* lock);
void sys_release_lock(struct uspinlock* lock);
```

</details>

<details>
<summary><b>🖥️ Console I/O</b></summary>

```c
void sys_cputs(const char *s, uint32 len);
int sys_cgetc(void);
void sys_cputc(int c);
```

</details>

---

## 🚀 Quick Start

### 📋 Prerequisites

| Tool | Version | Purpose |
|:-----|:--------|:--------|
| `i386-elf-gcc` | Latest | Cross-compiler for x86-32 |
| `i386-elf-as` | Latest | Assembler |
| `i386-elf-ld` | Latest | Linker |
| `i386-elf-objcopy` | Latest | Binary manipulation |
| `bochs` | 2.6+ | x86 Emulator |
| `make` | GNU Make | Build system |
| `perl` | 5.x | Build scripts |

### 🔨 Build Commands

```bash
# Build the entire project
make all

# Clean build artifacts
make clean

# Full clean (including logs)
make realclean

# Build and run in Bochs (no GUI)
make bochs
```

### ▶️ Running FOS

```bash
# Start Bochs with GUI
bochs

# Start Bochs without GUI (console mode)
bochs 'display_library: nogui'

# Windows users
bochscon.bat
```

### 🎮 First Steps

1. **Boot FOS** - Watch the initialization messages
2. **Type `help`** - See all available commands
3. **Run Hello World:** `run fos_helloWorld 10`
4. **Check memory:** `meminfo`
5. **Run more programs:** `run fos_factorial 15`
6. **Kill all processes:** `killall`

---

## 💻 Commands Reference

### 🔄 Process Management

| Command | Arguments | Description |
|:--------|:----------|:------------|
| `run` | `<program> <ws_size>` | Run a program with working set size |
| `kill` | `<env_id>` | Kill a specific process |
| `killall` | - | Kill all running processes |
| `runall` | - | Run all loaded programs |
| `printall` | - | Print info about all processes |

### 💾 Memory Management

| Command | Arguments | Description |
|:--------|:----------|:------------|
| `meminfo` | - | Display memory statistics |
| `allocuserpage` | `<va>` | Allocate a user page |
| `remove_table` | `<va>` | Remove a page table |
| `readmem_k` | `<va>` | Read kernel memory |
| `writemem_k` | `<va> <val>` | Write to kernel memory |

### 📄 Page Replacement

| Command | Description |
|:--------|:------------|
| `fifo` | Set FIFO replacement |
| `clock` | Set Clock (second chance) |
| `modclock` | Set Modified Clock |
| `lru 1` | Set LRU with timestamps |
| `lru 2` | Set LRU with lists |
| `nthclk <n>` | Set N-Chance Clock |
| `optimal` | Set Optimal (for testing) |
| `pagerep` | Print current algorithm |

### ⏰ Scheduler

| Command | Arguments | Description |
|:--------|:----------|:------------|
| `schedRR` | `<quantum>` | Set Round Robin |
| `schedMLFQ` | `<levels> <q1> <q2>...` | Set MLFQ |
| `schedBSD` | `<levels> <quantum>` | Set BSD |
| `schedPRIRR` | `<pri> <quantum> <thresh>` | Set Priority RR |
| `schedmethod` | - | Print current scheduler |
| `setpriority` | `<env_id> <priority>` | Set process priority |

### 💾 Heap Strategies

| Command | Description |
|:--------|:------------|
| `khfirstfit` | Kernel heap: First Fit |
| `khbestfit` | Kernel heap: Best Fit |
| `khnextfit` | Kernel heap: Next Fit |
| `khworstfit` | Kernel heap: Worst Fit |
| `khcustomfit` | Kernel heap: Custom Fit |
| `uhfirstfit` | User heap: First Fit |
| `uhbestfit` | User heap: Best Fit |
| `uhcustomfit` | User heap: Custom Fit |

---

## 🧪 Testing Framework

FOS includes a comprehensive testing framework with **50+ test cases**.

### 📊 Test Categories

| Category | Command Prefix | Description |
|:---------|:---------------|:------------|
| Dynamic Allocator | `tst dynalloc` | Block allocation and freeing |
| Kernel Heap | `tst kheap` | kmalloc, kfree, krealloc |
| Page Replacement | `run tpr*`, `run tclock*` | All replacement algorithms |
| User Heap | `run tm*`, `run tf*` | malloc and free tests |
| Custom Fit | `run tcf*` | Custom allocation tests |
| Shared Memory | `run tshr*` | Shared memory operations |
| Semaphores | `run tst_ksem*` | Synchronization tests |
| Scheduler | `tst priorityRR` | Priority scheduling |
| Environment Free | `run tef*` | Process cleanup tests |

### 🔄 Running Tests

```bash
# Dynamic allocator tests
FOS> tst dynalloc init
FOS> tst dynalloc alloc
FOS> tst dynalloc free
FOS> tst dynalloc realloc

# Kernel heap tests
FOS> tst kheap CF kmalloc blk
FOS> tst kheap CF kfree both

# User program tests
FOS> run tm1 3000
FOS> run tm2 3000
FOS> run tshr1 3000

# Page replacement tests
FOS> clock
FOS> run tclock1 11
FOS> run tclock2 11

# Scheduler tests
FOS> schedPRIRR 10 40 1000
FOS> tst priorityRR 0
```

---

## 🐛 Debugging

### 🔧 GDB Debugging

```bash
# Start Bochs in debug mode
bochs -f .bochsrc-debug

# In another terminal, connect with GDB
gdb obj/kern/kernel
(gdb) target remote localhost:1234
(gdb) break FOS_initialize
(gdb) continue
(gdb) info registers
(gdb) x/10x $esp
```

### 🔍 Kernel Debug Functions

```c
// Print debug information
void debug_printinfo(void);

// Print stack backtrace
void debug_backtrace(void);

// Kernel panic - halt system
void _panic(const char *file, int line, const char *fmt, ...);

// Panic and exit all environments
void _panic_all(const char *file, int line, const char *fmt, ...);

// Warning - continue execution
void _warn(const char *file, int line, const char *fmt, ...);
```

### 📝 Debug Macros

```c
// Assert condition
assert(condition);

// Panic with message
panic("Something went wrong: %d", error_code);

// Warning message
warn("Unexpected value: %x", value);
```

---

## 👨‍🏫 Course Instructor

<div align="center">

<table>
<tr>
<td align="center">

### 🎓 Dr. Ahmed Salah

**Course Instructor**

Faculty of Computers and Information Science  
Ain Shams University

*Operating Systems Course*

</td>
</tr>
</table>

</div>

---

## 👥 Development Team

<div align="center">

### 🚀 Meet Our Amazing Team

<table>
<tr>
<td align="center" width="200">
<a href="https://github.com/fady2024">
<img src="https://github.com/fady2024.png" width="100" height="100" style="border-radius:50%"/><br/>
<b>Fady</b><br/>
<sub>@fady2024</sub>
</a>
<br/>
<a href="https://github.com/fady2024">
<img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white"/>
</a>
</td>
<td align="center" width="200">
<a href="https://github.com/FadyRafat0">
<img src="https://github.com/FadyRafat0.png" width="100" height="100" style="border-radius:50%"/><br/>
<b>Fady Rafat</b><br/>
<sub>@FadyRafat0</sub>
</a>
<br/>
<a href="https://github.com/FadyRafat0">
<img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white"/>
</a>
</td>
<td align="center" width="200">
<a href="https://github.com/kaldesh">
<img src="https://github.com/kaldesh.png" width="100" height="100" style="border-radius:50%"/><br/>
<b>Kaldesh</b><br/>
<sub>@kaldesh</sub>
</a>
<br/>
<a href="https://github.com/kaldesh">
<img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white"/>
</a>
</td>
</tr>
<tr>
<td align="center" width="200">
<a href="https://github.com/0xZamel">
<img src="https://github.com/0xZamel.png" width="100" height="100" style="border-radius:50%"/><br/>
<b>Omar Zamel</b><br/>
<sub>@0xZamel</sub>
</a>
<br/>
<a href="https://github.com/0xZamel">
<img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white"/>
</a>
</td>
<td align="center" width="200">
<a href="https://github.com/ammar-adel222">
<img src="https://github.com/ammar-adel222.png" width="100" height="100" style="border-radius:50%"/><br/>
<b>Ammar Adel</b><br/>
<sub>@ammar-adel222</sub>
</a>
<br/>
<a href="https://github.com/ammar-adel222">
<img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white"/>
</a>
</td>
<td align="center" width="200">
<a href="https://github.com/Fady278">
<img src="https://github.com/Fady278.png" width="100" height="100" style="border-radius:50%"/><br/>
<b>Fady</b><br/>
<sub>@Fady278</sub>
</a>
<br/>
<a href="https://github.com/Fady278">
<img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white"/>
</a>
</td>
</tr>
</table>

</div>

---

## 📜 License

This project is licensed under the **GNU General Public License v3.0** - see the [LICENSE](LICENSE) file for details.

### 📋 License Summary

| Permissions | Conditions | Limitations |
|:------------|:-----------|:------------|
| ✅ Commercial use | 📋 License and copyright notice | ❌ Liability |
| ✅ Modification | 📋 State changes | ❌ Warranty |
| ✅ Distribution | 📋 Disclose source | |
| ✅ Patent use | 📋 Same license | |
| ✅ Private use | | |

### 🏛️ Original Components

FOS is derived from multiple open-source projects:

| Component | Source | Original License |
|:----------|:-------|:-----------------|
| Core Kernel | MIT Exokernel | MIT License |
| Console Driver | NetBSD pccons | BSD License |
| Clock/Scheduler | Exotec, Inc. | Exotec License |
| Printf | UC Berkeley | BSD License |
| Process Management | xv6 (MIT) | MIT License |

---

## 🙏 Acknowledgments

<div align="center">

| Contributor | Contribution |
|:------------|:-------------|
| **Dr. Ahmed Salah** | Course instructor and guidance |
| **MIT PDOS Group** | Exokernel foundation |
| **UC Berkeley** | Console and printf code |
| **xv6 Developers** | Inspiration and reference |

---

### 🎓 Developed at

**Faculty of Computers and Information Science**  
**Ain Shams University**  
Cairo, Egypt

---

### 📧 Contact

**Fady Gerges**  
📧 Email: fadygerges2023@gmail.com  
🔗 GitHub: [@Fady2024](https://github.com/Fady2024)  
📂 Repository: [OS_Project_2025](https://github.com/Fady2024/OS_Project_2025)

---

![Made with Love](https://img.shields.io/badge/Made%20with-❤️-red?style=for-the-badge)
![For Education](https://img.shields.io/badge/For-Education-blue?style=for-the-badge)
![Team](https://img.shields.io/badge/Team-6%20Members-green?style=for-the-badge)

---

### ⭐ Star this repo if you found it helpful!

[![GitHub stars](https://img.shields.io/github/stars/Fady2024/OS_Project_2025?style=social)](https://github.com/Fady2024/OS_Project_2025)

---

*Copyright © 2025 Fady Gerges and contributors*  
*Last Updated: January 2026*

</div>
