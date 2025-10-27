
# 🔠 KMP String Search in C

This program implements the **Knuth–Morris–Pratt (KMP)** string matching algorithm in pure C.
It finds a smaller string (**pattern**) inside a larger one (**text**) efficiently —
without rechecking characters after mismatches.
The result is a fast, linear-time search (O(n)), compared to the naive quadratic approach (O(n×m)).

##  Motivation — The Genius Behind KMP

In the **naive string search**, every mismatch forces the algorithm to restart the comparison from the next text position, even if part of the pattern was already matched. This repeated rescanning leads to wasted work and a time complexity of O(n×m).

The **genius of KMP** lies in realizing that when a mismatch occurs, the pattern itself often contains the clue to where the next comparison should begin.
If a **prefix** of the pattern matches a **suffix** of the portion already matched, KMP aligns them directly—allowing the search to “grow from within” the pattern rather than restart from scratch.

This self-overlap reuse, encoded in the **LPS array**, enables KMP to skip redundant checks and ensures the text index never moves backward, achieving true **linear-time matching**.




## 1\. Prefix vs. Suffix (The Key)

  * **Prefix:** Starts at the beginning of a word (e.g., in **`ABABA`**, `ABA` is a prefix).
  * **Suffix:** Ends at the end of a word (e.g., in **`ABABA`**, `ABA` is a suffix).

### Why it Matters: The Overlap

If you're matching a pattern and get stuck (a mismatch), KMP checks if the part you *did* match has a tail-end (**suffix**) that looks exactly like the pattern's head-end (**prefix**). If it does, you don't need to start over\! You can just **slide the pattern** so the matching prefix covers the matching suffix.

-----

## 2\. The LPS Array (The Cheat Sheet) 📝

The `get_lps` function builds the **LPS (Longest Proper Prefix which is also a Suffix) array** for your pattern. This array is the brain of KMP.

| Pattern | `A` | `B` | `A` | `B` | `A` |
| :---: | :-: | :-: | :-: | :-: | :-: |
| **LPS** | 0 | 0 | 1 | 2 | 3 |

  * **What it means:** For the part `ABABA`, the LPS value is $\mathbf{3}$. This means the prefix `ABA` (length 3) is also a suffix `ABA`.
  * **The Power:** When a mismatch happens, the LPS value tells the algorithm exactly how many characters to skip forward in the pattern.

-----

## 3\. The Search (The Magic Jump) ✨

The `main` loop compares text (`word[i]`) and pattern (`sub[j]`).

  * **Match:** Both `i` (text pointer) and `j` (pattern pointer) move forward. Simple.

  * **Mismatch (The Smart Part):**

    ```c
    if (j > 0)
        j = lps[j - 1]; // JUMP! Use the LPS array.
    else
        i++; // No match at the start, just slide the text forward.
    ```

      * If you matched some characters (`j > 0`), you **don't** reset the text pointer `i`.
      * Instead, you reset the pattern pointer `j` to the value stored in the LPS array. This is the **jump**: you shift the pattern instantly to the next possible starting point without moving backward in the main text.

**Result:** The text pointer `i` always moves forward. Since you never look backward, KMP is incredibly fast—it runs in $\mathbf{O(n)}$ time\!


---

## 🧩 High-Level Code Overview

### 🔹 Block 1 — `get_lps()`: Build the LPS Table

**Purpose:** Prepare how the pattern can “reuse itself” after mismatches.

**Variables:**

* `word`: the pattern (substring to find)
* `lps[]`: stores, for each position, the length of the longest prefix also appearing as a suffix
* `i`: length of the current prefix match
* `j`: current position being processed

**Logic:**
Compare pattern characters one by one.
If they match → record the prefix length in `lps`.
If not → fall back to a smaller prefix using previous `lps` values.
This builds the guide for future jumps.

---

### 🔹 Block 2 — `main()`: Search the Pattern in Text

**Variables:**

* `word`: main text
* `sub`: pattern to search
* `lps[]`: precomputed table from `get_lps()`
* `i`: index in text
* `j`: index in pattern
* `idx`: position where pattern starts if found
* `found`: flag if pattern exists

**Logic:**
Compare text and pattern in parallel.
If characters match → advance both.
If mismatch → jump `j` back using `lps` (no rechecking old text).
If `j` reaches pattern length → pattern found at `idx = i - j`.


