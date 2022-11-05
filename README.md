# Windows x86 Debugger

![C++](docs/badges/C++.svg)
[![CMake](docs/badges/Made-with-CMake.svg)](https://cmake.org)
[![Visual-Studio](docs/badges/Made-with-Visual-Studio.svg)](https://visualstudio.microsoft.com/)
[![Windows](docs/badges/Microsoft-Windows.svg)](https://www.microsoft.com/en-ie/windows)
![License](docs/badges/License-MIT.svg)

## Introduction

![Cover](Cover.png)

A tiny **Windows x86** debugging framework written in *C++20* that supports software breakpoints and hardware breakpoints. It can be used to create custom debuggers. Some design patterns came from [*GleeBug*](https://github.com/x64dbg/GleeBug).

## Getting Started

### Prerequisites

- Install [*Visual Studio 2022*](https://visualstudio.microsoft.com).
- Install [*CMake*](https://cmake.org).
- Set the `PATH` environment variables.

### Building

Set the location to the project folder and run:

```bash
mkdir -p build
cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build .
```

## Usage

Users can create derived classes inheriting from `Debugger` class and override or implement provided event callbacks.

- `Debugger` does not provide any implementation for event callbacks whose names start with `cb`.
- `Debugger` provides the basic implementation for event callbacks whose names start with `On`.

```c++
class MyDebugger : public Debugger {
private:
    void cbCreateProcess(const CREATE_PROCESS_DEBUG_INFO& details,
                         const Process& process) override {
        std::cout << std::format("The process {} has been created.",
                                 process.Id())
                  << std::endl;
    }

    void cbExitProcess(const EXIT_PROCESS_DEBUG_INFO& details,
                       const Process& process) override {
        std::cout << std::format("The process {} has exited.",
                                 process.Id())
                  << std::endl;
    }
};
```

## Documents

Code comments follow [*Doxygen*](https://www.doxygen.nl) specification.

### Class Diagram

```mermaid
classDiagram

class RegisterIndex {
    <<enumeration>>
    EAX
    EBX
    ECX
    EDX
}

class Register {
    Set(int)
    Reset()
    Get() int
}

Register --> RegisterIndex

class Flag {
    <<enumeration>>
    CF
    AF
    PF
    ZF
}

class FlagRegister {
    SetCF()
    ResetCF()
    CF() bool
}

Register <|-- FlagRegister
FlagRegister ..> Flag

class DebugStatusRegister {
    SetB0()
    ResetB0()
    B0() bool
}

Register <|-- DebugStatusRegister

class DebugControlRegister {
    SetL0()
    ResetL0()
    L0() bool
    SetRW0(val)
    RW0() int
}

Register <|-- DebugControlRegister

class Registers {
    Register EAX
    FlagRegister EFLAGS
    DebugStatusRegister DR6
    DebugControlRegister DR7
}

Registers o-- Register

class Breakpoint {
    int address
}

class HardwareBreakpointSlot {
    <<enumeration>>
    DR0
    DR1
    DR2
    DR3
}

class HardwareBreakpointType {
    <<enumeration>>
    Execute
    Write
    ReadWrite
}

class HardwareBreakpointSize {
    <<enumeration>>
    Byte
    Word
    Dword
}

class HardwareBreakpoint {
    HardwareBreakpointSlot slot
    HardwareBreakpointType access
    HardwareBreakpointSize size
}

Breakpoint <|-- HardwareBreakpoint
HardwareBreakpoint --> HardwareBreakpointSlot
HardwareBreakpoint --> HardwareBreakpointType
HardwareBreakpoint --> HardwareBreakpointSize

class SoftwareBreakpoint {
    byte origin
}

Breakpoint <|-- SoftwareBreakpoint

class Thread {
    Suspend()
    Resume()
    StepInto()
    SetHardwareBreakpoint(addr, slot, type, size)
    DeleteHardwareBreakpoint(slot)
}

Thread *-- HardwareBreakpoint
Thread --> Registers

class Process {
    Suspend()
    Resume()
    FindThread(id) Thread
    NewThread(thread)
    RemoveThread(thread)
    SetSoftwareBreakpoint(addr, callback)
    DeleteSoftwareBreakpoint(addr)
    FindSoftwareBreakpoint(addr) SoftwareBreakpoint
    SetHardwareBreakpoint(addr, slot, type, size, callback)
    DeleteHardwareBreakpoint(addr)
    FindHardwareBreakpoint(addr) HardwareBreakpoint
    WriteMemory(addr, data)
    ReadMemory(addr, size) vector~byte~
}

Process *-- Thread
Process *-- SoftwareBreakpoint

class Debugger {
    Create(file, cmd)
    Attach(proc)
    Start()
    Detach()
    Stop()
}

Debugger o-- Process
```

## License

Distributed under the *MIT License*. See `LICENSE` for more information.

## Contact

- ***Chen Zhenshuo***

  > ***GitHub***: https://github.com/czs108
  >
  > ***E-Mail***: chenzs108@outlook.com
  >
  > ***WeChat***: chenzs108

- ***Liu Guowen***

  > ***GitHub***: https://github.com/lgw1995
  >
  > ***E-Mail***: liu.guowen@outlook.com