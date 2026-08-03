# libjson

JSON interface library in Cpp23

> [!IMPORTANT]
> Since this library is still in development, it may not work correctly or some
> functionalities might be missing

## Build

### Requirements

- [CMake](https://cmake.org/)

### Compilation

```bash
git clone https://github.com/giorgi.page/libjson
cd libjson

./build.sh
```

### Debug

```bash
cmake -B build -DDEBUG=ON \
    && cmake --build build --parallel $(($(nproc) - 1))
```

### Testing

```bash
cmake -B build \
    -DJSON_BUILD_TESTING=ON \
    && cmake --build build --parallel $(($(nproc) - 1)) \
    && ctest --test-dir build 
```

### Example

```bash
cmake -B build \
    -DJSON_BUILD_EXAMPLE=ON \
    && cmake --build build --parallel $(($(nproc) - 1))

./build/example/example
```


## Usage

```cpp
using namespace libjson;

auto main(int, char**) -> int {
    std::stringstream ss {R"(
    {
        "name": "John Smith",
        "age": 25,
        "city": "London"
    }
    )"

    reader r {ss};

    ...

    return 0;
}
```

> Look at [example](example) for more
