## C++20 Coding Guidelines

> **Standard:** C++20 (with modern best practices)

---

### 1. Naming Conventions

| Category          | Style              | Example            |
|-------------------|--------------------|--------------------|
|  Static           | s_VariableName     | s_Count            |
|  Static function  | sf_FunctionName    | sf_ColorToVector3  |
|  Constant         | CONSTANT_VARIABLE  | MAX_BUFFER_SIZE    |
|  constexpr        | ce_VARIABLENAME    | ce_SCREENHEIGHT    |
|  Function         | FunctionName       | CalculateSum()     |
|  Class            | ClassName          | ParticleSystem     |
|  Struct           | t_StructName       | t_ExportedData     |
|  Private Member   | m_VariableName     | m_PlayerHealth     |
|  Normal Variable  | variable_name      | current_index      |
|  Enum Values      | VALUE1, VALUE2...  | RED, GREEN, BLUE   |
|  Enums            | e_EnumName         | e_TypeOfBlock      |
|  Boolean Variable | b_VariableName     | b_IsActive         |

---



### 2. Prevention Rules (Avoid These)

1. **Avoid C-Style Arrays**
   - Prefer **`std::array`** or other STL containers for safety and bounds checking.

2. **Avoid Raw `new` / `delete`**
   - Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) and RAII.

3. **Avoid Raw Pointers (`*`, `&`) for Ownership**
   - Replace with smart pointers or references as appropriate.

4. **Limit `auto` in Declarations**
   - Use explicit types for clarity; reserve `auto` for succinct loops.

5. **Avoid Implicit Conversions**
   - Mark single-argument constructors `explicit`.

6. **Avoid `NULL` / `0` Literals for Pointers**
   - Always use `nullptr` instead.

7. **Avoid Unchecked Access**
   - Prefer `.at()` over `operator[]` when bounds checking is required.

8. **Avoid C-Style Casts**
   - Use C++ casts (`static_cast`, `const_cast`, etc.) for explicit intent.

9. **Avoid `goto` and `friend`**
   - Use structured control flow and encapsulation instead.

10. **Avoid Deep Inheritance & Macro Overuse**
    - Favor composition and `constexpr`/`inline` over macros.

---

### 3. Preferable Rules (Do These)

1. **Use `std::array` and STL Containers**
   ```cpp
   std::array<int, 10> arr{};
   std::sort(arr.begin(), arr.end());
   ```

2. **Use `emplace_back()` / `emplace()`**
   ```cpp
   // In-place construction
   users.emplace_back("Alice", 30);
   ```

3. **Use `reserve()` for Vectors**
   ```cpp
   v.reserve(1000);
   ```

4. **Minimize Copies & Moves**
   - Pass by `const T&` or `T&&`, and use `std::move` when transferring ownership.

5. **Use `nullptr` for Pointer Initialization**
   ```cpp
   MyClass* ptr = nullptr;
   ```

6. **Use `explicit` for Constructors**
   ```cpp
   explicit ClassName(int value);
   ```

7. **Use C++14 Features**
   - Generic lambdas:
     ```cpp
     auto fn = [](auto x, auto y) { return x + y; };
     ```
   - `std::make_unique<T>()` for smart pointer creation:
     ```cpp
     auto ptr = std::make_unique<MyClass>(args);
     ```
   - **Use lambda expressions instead of raw function pointers**:
     ```cpp
     std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(oddNumbers),
                  [](int v) { return v % 2 != 0; });
     ```
   - **Use `remove_if` with a lambda to erase unwanted elements**:
     ```cpp
     // Remove all even numbers in-place
     numbers.erase(
       std::remove_if(numbers.begin(), numbers.end(),
                      [](int v) { return v % 2 == 0; }),
       numbers.end());
     ```

8. **Prefer `constexpr` Functions**
   ```cpp
   constexpr int square(int x) { return x * x; }
   ```

9. **Prefer `size_t` for sizes, counts, and indexing**  
   - Use `size_t` when working with container sizes (`.size()`), array indices, memory-related measurements, or pointer arithmetic, since it is guaranteed to be able to represent the size of any object.  
   - Avoid using signed types for these tasks to prevent negative values and signed/unsigned mismatches.  
   ```cpp
   std::vector<int> v = {1, 2, 3, 4};
   for (size_t i = 0; i < v.size(); ++i) {
       std::cout << v[i] << " ";
   }

   size_t length = buffer_length;  // representing a size
   ```
