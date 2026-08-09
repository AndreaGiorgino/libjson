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
#include <iostream>
#include <sstream>

#include "libjson/extra/decode.hxx"

using namespace libjson;

auto main(int, char**) -> int {
    std::stringstream ss {R"(
    {
        "name": "John Smith",
        "age": 25,
        "city": {
            "name": "London"
        }
    }
    )"};

    auto root {libjson::decode(ss)};
    std::cout << root["name"].as<std::string>() << " from "
              << root["city"]["name"].as<std::string>() << " is "
              << root["age"].as<int>() << " years old." << std::endl;

    return 0;
}
```

> Look at [example](example) for more

## Missing features

- Number parsing:
    - Handle long numbers
    - Handle exponent
    - Handle positive exponent
- Encoding:
    - Encode json
    - Encode with indentation
    - Encode string
- ostream overloading
