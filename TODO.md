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

### CHANGE THESE
Load this buffer into vim's location list `:lbuffer`
```
src/component/cccs.c:39:19:	usize ni = c->id2;	// current index
src/component/cccs.c:88:19:	usize ni = c->id2;	// current index
src/component/voltage_source.c:107:27:	if (isnan(c->V.frequency))	// sweep source
src/analysis/dc_helper.c:51:22:			c->D._Vj = c->D.Vj;	// store the old value
src/analysis/dc_helper.c:52:32:			diode_limit(c, Vj, &c->D.Vj);	// overwrite
src/component/diode.c:145:19:	usize n0 = c->id0;	// anode
src/component/diode.c:146:19:	usize n1 = c->id1;	// cathode
src/util/zmth.h:39:13:		return -12;	// log10(1e-12)
src/component/component.h:63:17:	enum ctype type;	//!< The type of component stored.
src/component/component.h:141:42:extern const dc_stamp_f DC_STAMPS[_C_LEN];	///< The DC Stamp Lookup Table.
src/component/component.h:150:42:extern const ac_stamp_f AC_STAMPS[_C_LEN];	///< The AC Stamp Lookup Table.
src/analysis/analysis.h:30:25:	sweep_type_e sweep_type;	///< The sweep type.
src/analysis/analysis.h:31:28:	usize sweeped_component_id;	///< The voltage source to sweep.
src/analysis/analysis.h:32:19:	f64 start_voltage;	///< Start voltage.
src/analysis/analysis.h:33:18:	f64 stop_voltage;	///< Stop voltage.
src/analysis/analysis.h:37:22:	const char *filename;	///< Output CSV filename.
src/analysis/analysis.h:38:17:	usize *node_ids;	///< The list of node ids to output.
src/analysis/analysis.h:47:25:	sweep_type_e sweep_type;	///< The sweep type.
src/analysis/analysis.h:48:21:	f64 start_frequency;	///< Start frequency.
src/analysis/analysis.h:49:20:	f64 stop_frequency;	///< Stop frequency.
src/analysis/analysis.h:53:22:	const char *filename;	///< Output CSV filename.
src/analysis/analysis.h:54:17:	usize *node_ids;	///< The list of node ids to output.
src/analysis/analysis.h:56:19:	usize ref_node_id;	///< The id of the reference node to compute the gain w.r.t..
src/component/current_source.c:90:27:	if (isnan(c->I.frequency))	// sweep source
src/util/lu.h:29:28:#define LU_TOLERANCE MIN_TOL	///< The max tolerance for a value to be considered 0.
src/analysis/dc.c:102:9:			break;	// converged
src/component/ccvs.c:43:19:	usize ni = c->id2;	// current index
src/component/ccvs.c:99:19:	usize ni = c->id2;	// current index
src/core/circuit.h:16:18:	usize node_count;	///< The number of nodes present.
src/core/circuit.h:19:26:	usize component_capacity;	///< Space allocated for the components.
src/core/circuit.h:20:23:	usize component_count;	///< Number of components present.
src/core/circuit.h:21:25:	component_t *components;	///< Dynamic array of components.
src/core/circuit.h:24:19:	env_t default_env;	///< The default simulation environment.
src/core/environment.h:9:32:#define K_BOLTZMANN 1.380649e-23	///< The Boltzmann constant, \f(k\f).
src/core/environment.h:10:29:#define K_CHARGE 1.602176e-19	///< The elementary charge, \f(q\f).
src/core/environment.h:17:17:	f64 temperature;	///< Global Temperature, \f(T\f), of the circuit in Kelvin.
```
