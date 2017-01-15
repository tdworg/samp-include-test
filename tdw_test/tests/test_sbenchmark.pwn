#include <a_samp>
#include <tdw_benchmark>

main()
{
	new i;

	sbenchmark:postinc() {
		i++;
	}

	sbenchmark:preinc() {
		++i;
	}
}
