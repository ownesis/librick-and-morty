# Rick-And-Morty API C library 
**librick-and-morty** is a **C** Library for the API [rickandmortyapi](https://rickandmortyapi.com).

- [Download](#Download)
- [Build](#Build)
- [Install](#Install)
- [Headers files](#Headers-files)
- [Library object](#Library-object)
- [Compiling yout project](#Compiling-your-project)
- [Documentation](#Documentation)

# Download
```sh
git clone https://github.com/ownesis/librick-and-morty.git
cd librick-and-morty
```

# Build
```sh
mkdir build && cd build
cmake ..
make
```

# Install
```sh
sudo make install
```

# Headers files
All headers files of this library is inside `/usr/include/rick-and-morty`

# Library-object
All object files of this library is inside `/usr/local/lib/`.

# Compiling your project
```sh
gcc -o example example.c -lrick-and-morty -lcurl -ljson-c
```

# Documentation
You can found the HTML documentation of the library [here](Documentation/html), and man page [here](Documentation/man).
