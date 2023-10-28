
# Changelog



## [0.0.15] - ????-??-??
### Changed
- feat: graphics, input managers (keyboard, mouse, touchscreen)
- feat: graphics, sdl2 window set size now exposed
- feat: graphics, safer shader binding
- feat: physics, 6dof constraint
- feat: physics, rigid body definition exposing more options
- feat: system, reduced iostream inclusion for faster compile time
- feat: system, 2d polygon helpers
- feat: system, kd-tree minor perf improvement
- feat: system, bit-mask templated abstraction
- feat: system, clearer messaging classes
- feat: system, performance profiler now with custom size
- chore: samples, test-bed updated



## [0.0.14] - 2023-08-??
### Changed
- feat: generic KdTree
- feat: some more math angle features
- feat: text renderer -> horizontal/vertical text align
- feat: better fps meter

## [0.0.13] - 2023-07-20
### Changed
- feat: better metrics handling (sub class nad rendering)
- feat: experimental support of physic hinge constraint
- feat: templated KdTree and spatial indexer (2d/3d)

## [0.0.12] - 2023-07-09
### Changed
- feat: audio alias grouping
- fix: basic_double_linked_list memory corruption issue
- fix: weak_ref_data_pool memory corruption issues
- feat: redesign of physics body/vehicle managers
- fix: math clamp and glm
- feat: templated 3d KdTree class

## [0.0.11] - 2023-06-24
### Changed
- feat: absolute position audio
- feat: copyable images instances
- feat: camera interface
- fix: physic queries (ray/sphere cast)
- fix: math lerp and glm
- chore: some typos
- feat: stream formatter class (what the trace logger use)
- feat: clock class

## [0.0.10] - 2023-05-20
### Changed
fix: script file permissions
chore: clang-format
fix: profiler widget
chore: better build system
chore: better build system


## [0.0.9] - 2023-05-6
### Changed
- feat: new sample -> test-bed
- chore: clang format
- chore: typo initialise -> initialize
- fix: basic_double_linked_list container is now stable
- feat: safe_weak_ref_data_pool container
- fix: weak_ref_data_pool container is now stable
- feat: better parser utilities

## [0.0.8] - 2023-02-17
### Changed
- chore: rng -> unused method
- chore: typo in tests
- feat: parsers -> better separation
- feat: threading -> increased "avoid blocking" feature support
- feat: more types support for messaging
- chore: correct invalidFloat logic
- feat: total live vehicle (physics)
- chore: typo (graphics)
- feat: texture pack/unpack + vbo types
- feat: performance profiler

## [0.0.7] - 2023-02-08
### Changed
- chore: physic now using the correct primitive types
- fix: graphic make geometries missing pragma once
- feat: physic vehicle share get wheels position/orientation
- feat: physic body share shapeDef + body orientation
- feat: messaging support quaternions
- chore: proper int types
- feat: threading that can avoid blocking (wasm friendly)
- chore: rename graphic -> graphics

## [0.0.6] - 2023-02-04
### Changed
- test: switch from "expect" to "assert"
- test: parser-utils tests
- feat: basic regex parser now rely on string_view

## [0.0.5] - 2023-01-31
### Changed
- feat: new templated function aliasing
- fix: generic container ref counting disabled

## [0.0.4] - 2023-01-29
### Changed
- chore: refactor of the container
- chore: refactor of the unit tests

## [0.0.3] - 2023-01-28
### Changed
- feat physics queries now support heap container
- feat: the gl context now support the deapth mask
- chore: small refactor of the multiphreading

## [0.0.2] - 2023-01-23
### Changed
- fix obj loader not handling optional normals

## [0.0.1] - 2023-01-22
### Changed
- initial version
