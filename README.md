# Globber

**Globber** is a lightweight Windows command-line utility that expands wildcard arguments (like `*.py`) in commands that don't support globbing natively — such as `git`, `mv`, or `custom scripts`.

## 🚀 What It Does

With **Globber**, you can run:

```cmd
globber git mv *.py src/
```

and it transforms that into:

```cmd
git mv a.py b.py c.py src/
```

Because Windows CMD does NOT expand wildcards in arguments. So if you run:

```cmd
git mv *.py src/
```

you get an error like:

> fatal: bad source, source=*.py, destination=src/*.py


## ✅ Features

- Expands `*` and `?` in any argument position
- Keeps non-wildcard args untouched
- Works via standard `system()` call
- Supports filenames with spaces
- Tiny executable: ideal for scripting or Git hooks

## 🛠️ Usage

```cmd
globber <command> [args with wildcards]
```

### Examples:

```cmd
globber echo *.txt

globber git add src/*.c include/*.h

globber custom_script.exe "data set*/raw*.csv"
```

## 💻 Build Instructions

Using Tiny C Compiler:

```cmd
tcc -o globber.exe globber.c
```

Link to Tiny C Compiler: https://bellard.org/tcc/

Or MSVC:

```cmd
cl globber.c /Fe:globber.exe
```

## 📁 File Structure

- `globber.c` — the main source file
- `globber.exe` — compiled binary
- `README.md` — this documentation

## 📦 License

MIT — free to use, hack, and share.

## ✨ Inspired By

Unix shells that just do this right.

---

Want icon ideas, batch wrappers, or a `.man` help page? Let me know!



Globber is a lightweight Windows command-line utility that expands wildcard arguments (like *.py) in commands that don't support globbing natively — such as git, mv, or custom scripts.

🚀 What It Does

Windows CMD does not expand wildcards in arguments. So if you run:

git mv *.py src/

you get an error like:

fatal: bad source, source=.py, destination=src/.py

With Globber, you can instead run:

globber git mv *.py src/

and it transforms that into:

git mv a.py b.py c.py src/

✅ Features

Expands * and ? in any argument position

Keeps non-wildcard args untouched

Works via standard system() call

Supports filenames with spaces

Tiny executable: ideal for scripting or Git hooks

🛠️ Usage

globber <command> [args with wildcards]

Examples:

globber echo *.txt

globber git add src/*.c include/*.h

globber custom_script.exe "data set*/raw*.csv"

💻 Build Instructions

Using Tiny C Compiler:

tcc -o globber.exe globber.c

Or MSVC:

cl globber.c /Fe:globber.exe

📁 File Structure

globber.c — the main source file

globber.exe — compiled binary

README.md — this documentation

📦 License

MIT — free to use, hack, and share.

✨ Inspired By

Unix shells that just do this right.

