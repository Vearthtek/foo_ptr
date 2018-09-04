# foo_ptr
## Educational project
The purpose of this is to create smart pointer to be smart enough to invalidate all references to the owned object (the `destroy()` method).

## Compilation
```sh
g++ Source.cpp --std=c++14
```

## Output
```sh
a[0x249ec20] b[(nil)] c[(nil)]
```