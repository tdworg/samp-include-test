# TDW Test
A collection of libraries for testing, debugging and benchmarking scripts.

### Installation
1. Download library.
2. All files move into folder `pawno/includes`. But, you can use the flag `-i<path>` to specify an alternative path.
3. Include it:
```PAWN
#include <a_samp> // standart SAMP library
#include <tdw_debug>
```

### Example
```pawn
#define _DEBUG 0
#include <tdw_debug>

main()
{
	p:0("Debug level is zero");
	p:1("Debug level is greater than zero");
}
```
