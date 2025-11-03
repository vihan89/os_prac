# OS Practicals - Code Only (No Comments)

This directory contains **comment-free versions** of all OS lab assignments.

## 📁 Directory Structure

```
no_comments/
├── ass1/  - Shell Scripts (4 files)
├── ass2/  - Process Management (4 files)
├── ass3/  - CPU Scheduling (3 files)
├── ass4/  - Synchronization (4 files)
├── ass5/  - Banker's Algorithm (2 files)
├── ass6/  - Page Replacement (2 files)
├── ass7/  - IPC - Message Queue & Shared Memory (4 files)
└── ass8/  - Disk Scheduling (2 files)
```

## ⚠️ Important Notes

1. **Code is IDENTICAL** to original versions - only comments removed
2. **Functionality unchanged** - all logic preserved word-for-word
3. **Use for practice** - try understanding code without comments
4. **Refer to original** versions (ass1/, ass2/, etc.) for detailed explanations

## 🔄 How These Were Generated

These files were automatically generated using `remove_comments.py`:
- Removes all C++ comments (`//` and `/* */`)
- Removes all bash comments (`#` lines, except shebang)
- Preserves all code structure, blank lines, and formatting
- No manual modifications made

## 📚 Usage

### Shell Scripts (Assignment 1)
```bash
chmod +x no_comments/ass1/*.sh
./no_comments/ass1/1_1_addressbook.sh
```

### C++ Programs (Assignments 2-8)
```bash
g++ no_comments/ass2/2_1.cpp -o 2_1
./2_1

g++ no_comments/ass4/4_1.cpp -o 4_1 -lpthread
./4_1
```

## 💡 Why Use Comment-Free Versions?

1. **Exam Practice** - Understand code logic without hints
2. **Code Reading** - Improve ability to read raw code
3. **Smaller Files** - Easier to print/reference during exams
4. **Focus on Logic** - No distractions from documentation

## 📖 Learning Approach

1. **Try comment-free first** - Test your understanding
2. **If stuck**, refer to original commented versions
3. **Compare both** - See what comments explain
4. **Practice explaining** - Try adding your own comments

## 🎯 Compilation Quick Reference

| Assignment | Command |
|------------|---------|
| ass1 | `chmod +x *.sh` |
| ass2-ass3 | `g++ file.cpp -o output` |
| ass4-ass5 | `g++ file.cpp -o output -lpthread` |
| ass6 | `g++ file.cpp -o output` |
| ass7 | `g++ file.cpp -o output -lpthread -lrt` |
| ass8 | `g++ file.cpp -o output` |

---

**Original commented versions**: See `ass1/`, `ass2/`, ..., `ass8/` folders
**Main README**: See `../README.md` for full documentation
