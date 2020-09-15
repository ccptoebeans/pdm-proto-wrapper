platform-pdm-proto-wrapper
===
> A C++ library to wrap platform-pdm output in protobuf

### Building

Assuming that you have the google protobuf SDK in your environment it is as trivial as this:
```
cmake -S . -B .build
cmake --build .build
```

### Using

#### with cmake

1. fill in the instructions ;-)

#### without cmake

1. add the `include/` directory to your compilers include directories
2. somewhere in your code do this:
```c++
#include <pdm/protobuf.h>
// call the method we yet have to write that returns you the protobuf object
```
3. Add `pdm_protobuf.lib` to your linker libraries