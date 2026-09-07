# libjson

JSON interface library in Cpp23

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

Look at [example](example) for an example usage

## Quick reference

### Capacity

Method | Brief
:----- | :-----
`libjson::json::empty` | Checks wether the container is empty (more in definition)
`libjson::json::size`  | Returns the number of elements (more in definition)

### Lookup

Method | Brief
:----- | :-----
`libjson::json::as` (template)                | Get the stored value as the target type or throws
`libjson::json::at`                           | Get a const reference to the element in the container or throws
`libjson::json::contains`                     | Check whether the container contains an element with the specified key (more in definition)
`libjson::json::has_value`                    | Check wether a value is being stored
`libjson::json::holds_alternative` (template) | Check wether the a value of the target type is being stored

### Modifiers

Method | Brief
:----- | :-----
`libjson::json::operator[]` | Get a reference to the element in the container (more in definition)
`libjson::json::push_back`  | Add data to the stored array (more in definition)
`libjson::json::insert`     | Add data to the stored object
`libjson::json::clear`      | Clear the stored value

### Decoding/Encoding

Method | Brief
:----- | :----
`libjson::json::decode`      | Decode a stream/string
`libjson::json::encode`      | Encode the stored value to a string
`libjson::json::operator <<` | Encode the stored value to a stream (more in definition)

## Missing features

- Number parsing:
    - Handle long numbers
    - Handle exponent
    - Handle positive exponent
- String parsing:
    - Handle double unicode sequence
