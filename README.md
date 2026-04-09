# zspice.
zspice is an analog circuit simulator inspired by SPICE engines.  it is an
recreational learning project, to understand how SPICE engines work, and how
common components are modelled.

for the mock-up version check [this repo](https://codeberg.org/zoomlogo/aacs).
there i tried making a quick resistor network solver in python, then _tried_
porting it to C++

## Build Instructions.
Requires CMake, also Ninja is nice to have.
```bash
git clone https://github.com/zoomlogo/zspice
cd zspice
mkdir build && cd build
cmake ..
ninja
./test_run           # to run the tests
./zspice             # actual program
```

## Doc Build Instructions.
Requires doxygen.
```bash
doxygen  # output in docs/
```

## TODO
See [TODO](./TODO.md) for info.

## COPYING
See [COPYING](./COPYING) for info.

## Resources
There are some of the resources I referred while implementing some aspects of
the program.
- [visualization of diode converging](https://www.desmos.com/calculator/rgm1u8j8ik)
- [qucs technical docs](https://guitorri.github.io/qucs-web/docs/technical.pdf)
- [berkeley spice paper](https://www2.eecs.berkeley.edu/Pubs/TechRpts/1973/ERL-m-382.pdf)
- [MNA explanation](https://cheever.domains.swarthmore.edu/Ref/mna/MNA_All.html)
- [wikipedia - MNA](https://en.wikipedia.org/wiki/Modified_nodal_analysis)
- [ngspice manual](https://ngspice.sourceforge.io/docs/ngspice-45-manual.pdf)
- Modelling the Bipolar Transistor, Ian E. Getreu
