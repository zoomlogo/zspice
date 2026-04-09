# TODO.
- refactor model code to make it cleaner
  add stamping helper functions

- non linear dc analysis
 - mosfet
  - mosfet capacitance & Vbs diode
  - mosfet tests
 - jfet
- docs

- make sweeps return a data struct instead of directly writing csv file
- add functions to write specific components to that csv file
- add a function to write that data to csv file
- add a sweep for values (e.g. sweep over resistance)

- ac small signal models
 - diode
 - bjt
 - mosfet
 - jfet

- transient analysis

- optimization
 - compute using sparse matrices (KLU)
 - multithreading support on sweeps (use sbuf_t)

- parsing
 - python api (ctypes)
 - python impl first, then C later
 - expression parsing
 - netlist parsing
 - (opt) custom netlist parsing

- full fledged models (parser injections)

# Far Future.
- gui
- digital analysis

# Other.
Use grep if you don't have ripgrep.
```bash
rg "TODO|XXX|FIXME"
```
