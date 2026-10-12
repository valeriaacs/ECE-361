# HW02 Bonus: Tail Recursion and Stack Depth Analysis

*Environment*
* **System**: Linux (Ubuntu 22.04 LTS x86_64)
* **Compiler**: gcc (Ubuntu 11.4.0)

*Maximum Depth Results Before Crash*

| Function | Optimization | Largest $N$ without Crash | Outcome |
| :--- | :---: | :---: | :--- |
| `stats_max` (Standard) | `-O0` | ~260,000 | Segmentation Fault (Stack Overflow) |
| `stats_max_tail` | `-O0` | ~260,000 | Segmentation Fault (Stack Overflow) |
| `stats_max` (Standard) | `-O2` | ~260,000 | Segmentation Fault (Stack Overflow) |
| `stats_max_tail` | `-O2` | > 20,000,000 | **Survives** |

*Analysis*

1. **Why `stats_max_tail` survives at `-O2`**:
   Since no work remains after the recursive call, `stats_max_tail` overwrites the current stack frame instead of pushing a new one. 

2. **Why `stats_max_tail` fails at `-O0`**: 
Because even though the code is written in tail position, the optimizations are unable and without tail position optimization, the code allocates separate stack frames and runs out of stack space.
   
3. **Why `-O2` does not save `stats_max`**:
   Because `stats_max` performs a comparation (a[0] > rest) after the recursive call completes.

4. **Implications for Binary Search Trees**:
   Balance BST rarely exhaust stack space, on the other hand, unbalanced BST will crash with standard recursion