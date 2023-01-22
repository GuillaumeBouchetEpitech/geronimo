
# Geronimo

## Table of Contents
- [Geronimo](#geronimo)
  - [Table of Contents](#table-of-contents)
- [How to Build](#how-to-build)
  - [Dependencies](#dependencies)
    - [System Dependencies](#system-dependencies)
    - [Local Dependencies](#local-dependencies)
  - [Build native C++ libraries](#build-native-c-libraries)
  - [Build webassembly (wasm) C++ libraries](#build-webassembly-wasm-c-libraries)
- [Thanks for watching!](#thanks-for-watching)

# How to Build

## Dependencies

### System Dependencies
```
sdl2
```

### Local Dependencies
```bash
sh sh_install_thirdparties.sh
```

## Build native C++ libraries

```bash
cd ./thirdparties
sh sh_build.sh
# this script is interactive:
# -> select platform
#   -> "native"
# -> then mode
#   -> "release"
# -> then rebuild
#   -> "yes"
```

Files expected to be created:
```
thirdparties/lib/
└── native
    ├── lib-bullet-physics-collision.a
    ├── lib-bullet-physics-dynamics.a
    └── lib-bullet-physics-linearmath.a
```


```bash
#
sh sh_build.sh
# this script is interactive:
# -> select platform
#   -> "native"
# -> then mode
#   -> "release"
# -> then rebuild
#   -> "yes"
```

Files expected to be created:
```
lib/
└── native
    ├── lib-geronimo-audio.a
    ├── lib-geronimo-graphic.a
    ├── lib-geronimo-physics.a
    └── lib-geronimo-system.a
```

## Build webassembly (wasm) C++ libraries

```bash
cd ./thirdparties
sh sh_build.sh
# this script is interactive:
# -> select platform
#   -> "web wasm"
# -> then mode
#   -> "release"
# -> then rebuild
#   -> "yes"
```

Files expected to be created:
```
thirdparties/lib/
└── web-wasm
    ├── lib-bullet-physics-collision.bc
    ├── lib-bullet-physics-dynamics.bc
    └── lib-bullet-physics-linearmath.bc
```


```bash
#
sh sh_build.sh
# this script is interactive:
# -> select platform
#   -> "web wasm"
# -> then mode
#   -> "release"
# -> then rebuild
#   -> "yes"
```

Files expected to be created:
```
lib/
└── web-wasm
    ├── lib-geronimo-audio.bc
    ├── lib-geronimo-graphic.bc
    ├── lib-geronimo-physics.bc
    └── lib-geronimo-system.bc
```

# Thanks for watching!
