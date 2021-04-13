Group Project for SEHH2042 Computer Programming
===============================================

Building the Project
--------------------

### Pre-requirements

- [Git][git-homepage]
- [CMake][cmake-homepage]

### Build Instruction

First, clone this repository:

```sh
$ git clone https://github.com/JasonHK-HKCC/SEHH2042-Group-Project.git
```

Next, navigate to the root of the cloned repository:

```sh
$ cd /path/to/the/cloned/repo/
```

Then configure CMake:

```sh
$ cmake -E make_directory ./build
$ cd ./build
$ cmake .. -DCMAKE_BUILD_TYPE=Build
```

Finally, build the project:

```sh
$ cmake --build . --config 
```

[git-homepage]: https://git-scm.com/ "The homepage for Git"

[cmake-homepage]: https://cmake.org/ "The homepage for CMake"
