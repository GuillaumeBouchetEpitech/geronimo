
# Geronimo

## Table of Contents
- [Geronimo](#geronimo)
  - [Table of Contents](#table-of-contents)
- [Dependencies](#dependencies)
  - [Dependency: Emscripten 3.1.26 (for web-wasm build)](#dependency-emscripten-3126-for-web-wasm-build)
  - [Dependency: SDL2 (for native build)](#dependency-sdl2-for-native-build)
  - [Dependency: bullet3, glm, tinyobjloader, stb](#dependency-bullet3-glm-tinyobjloader-stb)
- [How to Build (Quick)](#how-to-build-quick)
  - [Build Everything (will skip web-wasm if emscripten is absent)](#build-everything-will-skip-web-wasm-if-emscripten-is-absent)
- [How to Build (Detailed)](#how-to-build-detailed)
  - [Build native C++ libraries](#build-native-c-libraries)
  - [Build webassembly (wasm) C++ libraries](#build-webassembly-wasm-c-libraries)
- [Thanks for watching!](#thanks-for-watching)

# Dependencies

## Dependency: Emscripten 3.1.26 (for web-wasm build)
```bash
git clone https://github.com/emscripten-core/emsdk.git

cd emsdk

./emsdk install 3.1.26
./emsdk activate --embedded 3.1.26

. ./emsdk_env.sh

em++ --clear-cache
```

## Dependency: SDL2 (for native build)
```
libsdl2-dev
```

## Dependency: bullet3, glm, tinyobjloader, stb
```bash
chmod u+x ./sh_install_thirdparties.sh
./sh_install_thirdparties.sh
```

# How to Build (Quick)

## Build Everything (will skip web-wasm if emscripten is absent)

```bash
chmod u+x ./sh_everything.sh
./sh_everything.sh
# will tell if a dependency is missing
# will skip the web-wasm build if emscripten is not detected
```

# How to Build (Detailed)

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
