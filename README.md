# OTAVauleControl (simulator)

This repository contains a simulated version of the OTAVauleSlave project adapted to compile and run on a host. The simulation implements simple HAL stubs for LoRa/WiFi/GSM, a basic IPC/LoRa receive path, and a valve actuation simulation.

Build and run
---------------

Requirements: CMake and a C compiler (gcc/clang).

Commands:

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./otavalve
```

What changed
------------

- Fixed multiple prototype and header errors so the code compiles.
- Added simple HAL simulation in `lorahal.c` (simulated send/receive).
- Added `CMakeLists.txt` and `main()` to run the simulation.

Notes
-----

This is a simulation and avoids hardware-specific logic. I preserved the original execution flow but fixed many prototype mismatches and obvious bugs so it builds and demonstrates a receive->parse->actuate->ack cycle.
Control the valve using the Miro-contoller and communication modules and automate them accordingly.
