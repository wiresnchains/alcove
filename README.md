# alcove

Mask local IPs with custom domains for seamless local development.

## Building

To build the project, you will need to install [CMake](https://cmake.org) and [vcpkg](https://github.com/microsoft/vcpkg).

### Presets
* `debug-osx-intel`
* `release-osx-intel`
* `debug-osx-silicon`
* `release-osx-silicon`

### Commands

```sh
cmake --preset debug-osx-intel
cmake --build --preset debug-osx-intel
```
