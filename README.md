# PE File Parser

A lightweight Windows command-line tool written in C that parses and displays the structure of Portable Executable (PE) files.

## What It Does

Given a PE file path (`.exe`, `.dll`, `.sys`), the tool reads it from disk and prints a structured breakdown of its internal layout, including:

- **File Header** — architecture (x32/x64), file type (EXE/DLL/SYS), section count
- **Optional Header** — entry point, image base, code size, checksum, and more
- **Data Directories** — addresses and sizes of Export, Import, Resource, Exception, Relocation, TLS, and IAT tables
- **Sections** — name, size, RVA, virtual address, relocations, and memory permissions for each section

## Usage

```
PeParser.exe <path_to_pe_file>
```

**Example:**
```
PeParser.exe C:\Windows\System32\notepad.exe
```

## Requirements

- Windows OS
- MSVC or MinGW compiler (builds with standard Win32 headers)

## Building

Open the project in Visual Studio and build it 

## Notes

- The tool reads the file entirely into heap memory before parsing.
- RVAs (Relative Virtual Addresses) are printed alongside computed in-memory addresses based on the load buffer.
- No third-party dependencies — uses only `Windows.h` and `winternl.h`.
