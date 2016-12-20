#include <a_samp>

#include <tdw_benchmark>

benchmark FirstFunction[1](var1, var2)
{
	var2 = 8;
	var1 = var2;
	printf("var1: %d | var2: %d", var1, var2);
}

benchmark SecondFunction[8](i)
{
	while (i != 1_000) {
		#emit nop
		#emit nop
		#emit nop
		#emit nop
		++i;
	}
}


main()
{
	benchmark_run();
}


public
	OnBenchPerformed(function_name[], result_time)
{
	p:0("Cool! %s %d", function_name, result_time);
}
