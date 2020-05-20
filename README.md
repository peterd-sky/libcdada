[![Build Status](https://travis-ci.org/msune/libcdada.svg?branch=master)](https://travis-ci.org/msune/libcdada)

**libcdada** - basic data structures in C (`libstdc++` wrapper)
---------------------------------------------------------------

Small library that offers basic data structures (`list`, `set`, `map`...) in a pure C API. Key features:

* Easy to use, portable
* No "magic" MACROs, and no need to modify your data structures (except, perhaps, for `__attribute__((packed))`)
* Stable and well-tested backend engine (`libstdc++`) for most of the data structures
* Reasonable performance - comparable to `libstdc++`)

Example
-------
```c
#include <cdada/list.h>

int x, val=0;
cdada_list_t* my_list = cdada_list_create(sizeof(int));

//Add to list {10, 11, 5, 5}
x=10;
cdada_list_push_back(my_list, &x);
x=11;
cdada_list_push_back(my_list, &x);
x=5;
cdada_list_push_back(my_list, &x);
cdada_list_push_back(my_list, &x);

//Get element in position 1
cdada_list_get(my_list, 1, &val);
assert(val == 11);

//Remove duplicates {10,11,5}
cdada_list_unique(my_list);
assert(cdada_list_size(my_list) == 3);

//First/last
cdada_list_first(my_list, &val);
assert(val == 10);

//Add {10, 11, 5, 11}
x=11;
cdada_list_push_back(my_list, &val);

//Remove all 11s
cdada_list_remove(my_list, &val);
assert(cdada_list_size(my_list) == 2);

//Traverse list
cdada_list_traverse(my_list, my_iterator_func, opaque);
```

More examples `map` and `set` and custom containers in the [examples](examples/) folder.

Documentation
-------------

Public API:

* [list.h](include/cdada/list.h): an ordered list of objects (equivalent to `std::list`)
* [map.h](include/cdada/map.h): a hashmap {key -> value}, with unique keys (equivalent to `std::map`)
* [set.h](include/cdada/set.h): a set of objects, with unique values hashmap (equivalent to `std::set`)
* [util.h](include/cdada/util.h): error codes and utility functions

`libcdada` (as `libstdc++`) is not thread-safe.

Default containers support 1-256 bytes keys (values for lists), but they will
perform better when aligned to {1,2,4,8,32,64,128,256} bytes - keys are padded to
a power of 2 bytes.

#### Custom containers

For larger keys (any length), optimal memory usage and performance take a look at `libcdada`'s
**[custom containers](doc/Custom.md)**.

Installation
------------

Requirements:

* POSIX system
* C and C++ gcc compatible compilers (gcc, icc, clang...)
* Automake
* Autoconf
* Libtool
* libstdc++ (C++98)

```
sh autogen.sh
cd build
../configure
sudo make install
```

Author
------

Marc Sune < marcdevel (at) gmail (dot) com>