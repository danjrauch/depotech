# depotech

# Build
```
git clone https://github.com/danjrauch/depotech
cd depotech
mkdir build
cmake --build build/
./build/depotech
```

# Memchecks

## Using experimental valgrind on macOS Catalina

https://github.com/sowson/valgrind

Not currently working on Catalina. Need to wait for main valgrind master branch. 

```
valgrind ./client --leak-check=full -vvv  
```