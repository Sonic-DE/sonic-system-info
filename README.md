# sonic-system-info

sonic-system-info is used to display information about your computer's hardware on the Sonic desktop.
This repository hosts the modules for it while the main shell is [System Settings](https://invent.kde.org/plasma/systemsettings).

## Contributing

We appreciate your interest in contributing! To report a bug, please use the Plasma Workspace bug tracker at [Issues · Sonic-DE/plasma-workspace-sonic](https://github.com/Sonic-DE/plasma-workspace-sonic/issues).

To make a source code contribution, fork the repo then create a pull request with your changes.

## Build instructions

sonic-system-info uses CMake like most SonicDE projects, so you can build it like this:

```bash
mkdir build
cd build
cmake ..
make
```

or with ninja:

```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
```
