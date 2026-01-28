# alcove

Mask local IPs with custom domains for seamless local development.

## Building

To build the project, you will need to install [CMake](https://cmake.org) and [vcpkg](https://github.com/microsoft/vcpkg).

### Presets
* `debug-macos-intel`
* `debug-macos-silicon`
* `release-macos-intel`
* `release-macos-silicon`

### Commands

```sh
cmake --preset debug-macos-intel
cmake --build --preset debug-macos-intel
```
