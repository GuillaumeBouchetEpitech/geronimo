
# Geronimo

## Table of Contents
- [Geronimo](#geronimo)
  - [Table of Contents](#table-of-contents)
- [Online Demo Link](#online-demo-link)
  - [Diagrams](#diagrams)
- [Dependencies](#dependencies)
  - [Dependency: Emscripten 3.1.44 (for web-wasm build)](#dependency-emscripten-3144-for-web-wasm-build)
  - [Dependency: SDL2 (for native build)](#dependency-sdl2-for-native-build)
  - [Dependency: bullet3, glm, tinyobjloader, stb](#dependency-bullet3-glm-tinyobjloader-stb)
- [How to Build (Quick)](#how-to-build-quick)
  - [Build Everything (will skip web-wasm if emscripten is absent)](#build-everything-will-skip-web-wasm-if-emscripten-is-absent)
- [How to Build (Detailed)](#how-to-build-detailed)
- [Project(s) using it](#projects-using-it)
- [Thanks for watching!](#thanks-for-watching)


# Online Demo Link

**`/!\ important /!\`**

https://guillaumebouchetepitech.github.io/geronimo/samples/test-bed/dist/index.html

**`/!\ important /!\`**

## Diagrams

```mermaid

mindmap
  root((gero))
    ((system))
      containers
        dynamic
          double<br>linked<br>list
          heap array
        static
          heap<br>grid<br>array
        weak ref<br>heap array<br>pool
      utils
        math
        string
        file
        parser
      multi threading
      logger
    ((graphics))
      camera
      sld2
      obj loader
      primitive<br>geometries<br>builder
      OpenGL ES3
        context
        image
        textures
          color
          depth
          data
        shader
          program
          builder
        geometry
          instance
          builder
        vertex<br>buffer<br>object
        vertex<br>buffer<br>array
        frame<br>buffer
        render<br>buffer
        resource<br>manager
    ((physics))
      bullet<br>physics
        physic<br>world
        physic<br>shape
        physic<br>body
          instance
          manager
        physic<br>vehicle
          instance
          manager
        physic<br>query
          convex<br>shape
          ray<br>cast
          convex<br>cast
        physic<br>constraints
          hinge
          6dof
    ((audio))
      OpenAL<br>context
      ogg loaded

```


```mermaid

%%{
  init: {
    'theme': 'base',
    'themeVariables': {
      'primaryColor': '#242424',
      'primaryTextColor': '#DDD',
      'primaryBorderColor': '#000',
      'lineColor': '#A0A0A0',
      'secondaryColor': '#454545',
      'tertiaryColor': '#353535'
    }
  }
}%%

  erDiagram

    SYSTEM ||--o{ GRAPHIC : dependency
    SYSTEM ||--o{ AUDIO : dependency
    SYSTEM ||--o{ PHYSICS : dependency

    SYSTEM ||--|| GLM : dependency
    GRAPHIC ||--|| SDL2 : dependency
    GRAPHIC ||--|| TINY_OBJ_LOADER : dependency
    GRAPHIC ||--|| STB_IMAGE : dependency
    AUDIO ||--|| OPENAL : dependency
    AUDIO ||--|| STB_VORBIS : dependency
    PHYSICS ||--|| BULLET_PHYSICS : dependency

```


# Dependencies

## Dependency: Emscripten 3.1.44 (for web-wasm build)
```bash
git clone https://github.com/emscripten-core/emsdk.git

cd emsdk

./emsdk install 3.1.44
./emsdk activate --embedded 3.1.44

. ./emsdk_env.sh

em++ --clear-cache
```

## Dependency: SDL2 (for native build)
```
libsdl2-dev
```

## Dependency: bullet3, glm, tinyobjloader, stb

The dependencies will be downloaded and built with the `Build Everything` method below

# How to Build (Quick)

## Build Everything (will skip web-wasm if emscripten is absent)

```bash
chmod u+x ./sh_everything.sh
./sh_everything.sh
# will tell if a dependency is missing
# will skip the web-wasm build if emscripten is not detected
```

# How to Build (Detailed)

[Specific Doc](./detailed-build.md)

# Project(s) using it

- Self Learning 3d Cars [Github Link](https://github.com/GuillaumeBouchetEpitech/self-learning-3d-cars)
- Basic Genetic Algorithm [Github Link](https://github.com/GuillaumeBouchetEpitech/basic-genetic-algorithm)

# Thanks for watching!
