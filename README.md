
# 🔠 KMP String Search in C

This program implements the **Knuth–Morris–Pratt (KMP)** string matching algorithm in pure C.
It finds a smaller string (**pattern**) inside a larger one (**text**) efficiently —
without rechecking characters after mismatches.
The result is a fast, linear-time search (O(n)), compared to the naive quadratic approach (O(n×m)).


Here’s a clearer and more focused rewrite — shorter, tighter, and directly emphasizing the *realization* behind KMP:

---

## Motivation and Main Idea

When a mismatch happens while comparing the pattern to the text, the naive approach restarts the comparison from the next character in the text —

it shifts the text index i back to where it started +1.



But this wastes work.



### 1. The Flaw of Naive Shifting

The naive approach is inefficient because when a mismatch occurs after a partial match of length $k$:

* It moves the **text index ($i$)** backward (resets the comparison start to $i_{start} + 1$).
* It restarts the **pattern index ($j$)** from $0$.

This means it completely throws away the knowledge gained from the previous $k$ successful comparisons. As the text states, this shift to $\mathbf{\text{text}+1}$ is merely a brute-force way of **re-checking all the suffixes** of the segment that just failed to match.

### 2. KMP's Optimization: The LPS Array

The KMP algorithm addresses this by realizing that the partial match itself holds the clue for the next possible alignment.

* **The Overlap:** The successful part of the match (e.g., `'aaaa'` in the example) has a **suffix** that might be identical to a **prefix** of the pattern.
* **The Jump:** The **LPS (Longest Proper Prefix which is also a Suffix) array** is a precomputed table that tells KMP the length of the longest such useful overlap.
* **The Result:** Instead of shifting the text backward ($i$), KMP **keeps the text index ($i$) stationary** at the mismatch point and **shifts the pattern index ($j$)** directly to the position indicated by the LPS array. This **re-aligns** the pattern instantly so its longest prefix-suffix overlap sits right before the current text character.

This strategy ensures that KMP:

1.  **Never re-examines a character in the text more than once.**
2.  **Does not miss any potential match.**
3.  Achieves optimal $\mathcal{O}(n+m)$ time complexity (where $n$ is text length and $m$ is pattern length).

**In short:** The **realization** that **moving forward in the text is literally exploring the same suffixes KMP already encodes in LPS** is the fundamental insight that makes the KMP algorithm a significant improvement over the naive method. 

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

Perfect — here’s the **same 2-block high-level explanation**, but now including **key variable meanings** so it’s complete yet still concise 👇

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


