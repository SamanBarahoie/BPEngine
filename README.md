
# 🧠 C++ Tokenizer with Python Binding

This project implements a custom **Byte Pair Encoding (BPE) tokenizer** in C++, and exposes it to Python using **pybind11**. It's designed for performance, educational clarity, and easy integration with machine learning workflows.

---

## 📦 Features

- 🔤 Byte Pair Encoding (BPE) tokenizer
- ⚡ Fast C++ implementation
- 🔗 Python binding via pybind11
- 📚 JSON-based vocabulary/merges I/O (via `nlohmann/json`)
- 🧪 Easy to integrate with Jupyter, NumPy, and PyTorch

---

## 🛠️ Build Instructions

### 🔧 Requirements

- [CMake](https://cmake.org/) ≥ 3.14
- [Ninja](https://ninja-build.org/)
- C++17 compiler (e.g. g++, clang++)
- Python ≥ 3.6 (Python 3.10 or below recommended for compatibility)
- Optional: `pybind11`, `nlohmann/json` (included in `extern/`)

---

### 📁 Folder Structure

```
tokenizer_project/
├── CMakeLists.txt
├── src/
│   ├── Tokenizer.cpp
│   ├── Tokenizer.hpp
│   └── bindings.cpp
├── extern/
│   ├── pybind11/          # from https://github.com/pybind/pybind11
│   └── nlohmann/json.hpp  # from https://github.com/nlohmann/json
└── build/                 # (created after build)
```

---

### 🚀 Building the Project

```bash
# 1. Clone this repo and cd into it
cd tokenizer_project

# 2. Create the build directory
mkdir build && cd build

# 3. Configure with CMake and Ninja
cmake .. -G Ninja

# 4. Build the project
ninja
```

After building, a shared object will be created, e.g. `pytokenizer.*.pyd`, which can be imported in Python:

```python
import pytokenizer

tokenizer = pytokenizer.Tokenizer()
ids = tokenizer.encode_to_ids("hello world")
print(ids)
```

---

## 🧠 Example Usage (Python)

```python
from pytokenizer import Tokenizer

tokenizer = Tokenizer()
tokenizer.train(["this is a test", "tokenize this sentence"], vocab_size=100)

ids = tokenizer.encode_to_ids("tokenize this")
print("Encoded IDs:", ids)

tokenizer.save_vocab("vocab.json")
tokenizer.save_merges("merges.txt")
```

---

## 📜 License

This project is licensed under the MIT License.

---

## 🙌 Credits

- [pybind11](https://github.com/pybind/pybind11)
- [nlohmann/json](https://github.com/nlohmann/json)
```

---
