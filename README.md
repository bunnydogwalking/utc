# utc

"utc" is a helper program for converting utc-like numbers in arguments or
input to human-readable timestamps

## Usage

```shell
zzz@lop:~$ utc --help
Convert utc-like numbers in arguments or input to human-readable timestamps

Usage: utc [--tz timezone] [--] [text...]

  --tz timezone    Timezone to use for presenting converted timestamps.
                   Defaults to America/New_York

Arguments given on the command line will be treated as text to be
processed. Otherwise, text will be read from the standard input.
Numbers that appears to be a potential UTC timestamp with second,
millisecond, microsecond, or nanosecond precision will be converted
to a human-readable format using the specified timezone.
```

## Example

```shell
zzz@lop:~$ utc --tz America/Chicago -- Timestamp 1650825477123 was a couple of years ago.
Timestamp 20220424 13:37:57.123 CDT was a couple of years ago.
```

## Building

Building utc requires the [cctz](https://github.com/google/cctz) library.
Here are CMake and conan instructions for building it:

```shell
pip3 install conan
mkdir build && cd build
conan install .. --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build .
```

## License
[MIT](https://choosealicense.com/licenses/mit/)
