# alcove

Mask local IPs with custom domains for seamless local development.

## Building

To build the project, you will need to install [CMake](https://cmake.org) and [vcpkg](https://github.com/microsoft/vcpkg).

### Presets

- `debug-macos-intel`
- `debug-macos-silicon`
- `release-macos-intel`
- `release-macos-silicon`
- `debug-windows`
- `release-windows`
- `debug-linux`
- `release-linux`

### Commands

```sh
cmake --preset debug-macos-intel
cmake --build --preset debug-macos-intel
```

## Usage

```sh
alcove --help
sudo alcove --add mydomain.ltd,192.168.0.1
alcove --list
sudo alcove --delete 0
```

Tip: you can also use shorter options such as `-a` instead of `--add`.

Note: The reason why you need to use `sudo` on add/delete/clear commands is the fact that the program edits the hosts file on your system, which requires elevated privileges.
