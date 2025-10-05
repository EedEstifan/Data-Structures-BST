# Data Structures –  BST (AVL Tree)

This project implements an  Store Management System using a **self-implemented Balanced Binary Search Tree (AVL Tree)** in **C**.  
All data structures, balancing logic, and algorithms were **fully written from scratch** — no external libraries or pre-made code were used.

Developed as part of the *Data Structures* course at the University of Haifa.

---

## 📂 Project Files

| File | Description |
|------|--------------|
| `HW3_Q1_DS.c` | Implements a store using a self-implemented AVL tree with `insert`, `delete`, and `Max_Price` operations. |
| `HW3_Q2_DS.c` | Extends the store with a **“Monthly Hits”** feature — adds range selection and traversal via custom list management. |
| `HW3_Q3_DS.c` | Implements **multiple AVL sub-stores (“series”)** and a global AVL tree of series for constant-time global maximum queries. |

---

## 🧩 Core Implementations

All algorithms below were **designed and coded manually** to ensure deep understanding of data structures and complexity analysis.

### 🔸 AVL Tree Implementation
Implemented from scratch, including:
- `insertAVL()` – Recursive insertion with height updates  
- `deleteAVL()` – Handles all cases (leaf, one child, two children)  
- `BalanceAVL()` – Calculates balance factor and triggers rotations  
- `LeftRotate()` / `RightRotate()` – Manual pointer rotation logic  
- `UpdateMaxPrice()` – Maintains max item per subtree in O(1)  

> 🧩 *No STL containers or pre-built tree utilities were used.*

### 🔸 Store Operations (Q1)
- `Init()` – Initialize empty store  
- `Insert(price, name)` – Add item with next serial number  
- `Delete(number)` – Remove item by key  
- `Max_Price()` – Return the max-priced item in O(1)  

### 🔸 Monthly Hits (Q2)
- `Pick_Month_Hits(i, j)` – Build a linked list of relevant subtrees  
- `Show_Month_Hits()` – Print all names in order  
- `End_Month()` – Release all temporary data  

### 🔸 Series Management (Q3)
- `Insert(price, name, i)` / `Delete(number, i)` – Per-series operations  
- `Split_Series(i, j)` – Custom AVL **split** and **concatenate** implementation  
- `Priv_Max(i)` / `Public_Max()` – Max in series or across all series in O(1)

---

## ⚙️ Compilation and Run

```bash
# Compile each question separately
gcc HW3_Q1_DS.c -o q1
gcc HW3_Q2_DS.c -o q2
gcc HW3_Q3_DS.c -o q3

# Run
./q1
./q2
./q3
