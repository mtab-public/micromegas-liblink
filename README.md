# LibraryLink for micrOMEGAs

This folder provides a **Mathematica LibraryLink interface** to **micrOMEGAs** for computing dark matter relic density without permanent I/O.  

## Files

- `micromegas_dll.c` — C library implementing LibraryLink functions  
- `micromegas.m` — Mathematica script to compile and load the library  

## Requirements

- Mathematica 10+  
- C compiler (GCC/Clang)  
- micrOMEGAs installed at `$TMP/micromegas/dmfv/main_dmfv`  
- `uuid` library (`-luuid`)  

## Usage

1. Compile and load the library:

```<< "micromegas.m"```

2. Define 25 model parameters:

```params = {Rel11, Rel12, ..., lH3};```

3. Compute relic density:

```relicDensity = omega[params]```

- Input: 1D tensor of 25 real values corresponding to model parameters  
- Output: Relic density (real number)  

## Notes

- Avoids permanent I/O via process substitution  
- Adjust micrOMEGAs path in `micromegas_dll.c` if necessary
