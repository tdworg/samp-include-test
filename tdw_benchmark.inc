/** ======================================================================== **\
	Benchmark

| Description:
Allows for the possibility to compare the performance of different
algorithms (functions).

Example:
	benchmark SomeFunction[1000](local)
	{
		local = 5;
		printf("%d", local);
	}

| Version:
1.0.0

| License:
Copyright (c) 2016 Double V

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgement in the product documentation would be
	appreciated but is not required.
2.	Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.
3.	This notice may not be removed or altered from any source distribution.
\** ======================================================================== **/

#if defined _inc_tdw_benchmark
	#undef _inc_tdw_benchmark
#endif

#if defined _TDW_BENCHMARK
	#endinput
#endif

#define _TDW_BENCHMARK

const _BENCHMARK_TIMER_MAJOR=10_000;
const _BENCHMARK_TIMER_MINOR=1_000;

forward OnBenchPerformed(function_name[], result_time);

#if !defined TDW_MAX_DEFAULT_ITERATIONS
	#define TDW_MAX_DEFAULT_ITERATIONS (10000)
#endif

#include "tdw_debug"

#define u@$ 0]);
#define __:%0$ u@(u[_:%0 u@$

#define benchmark%1(%2) bm@r:%1(%2)
#define bm@r: __:bm@r0:bm@r1:$
#define bm@r0:bm@r1:%8$%1[%3]%0(%2) %8$forward public bmf@%1();public bmf@%1()for(new bm@l:bmv@current,%2;_:bmv@current<(%3);++bmv@current)
#define bm@r1:%8$%1(%2) %8$forward public bmf@%1(bmv@it);public bmf@%1(bmv@it)for(new bm@l:bmv@current,%2;_:bmv@current<bmv@it;++bmv@current)

#define bmf@%0\32;%1(%2) bmf@%0%1(%2)

#define bm@l:%1,; %1;

#define sbenchmark:%1(%3) \
	for(new %1@nonei=1,%1@nonet,%1@nonett,%1@iterMi=0,%1@iterMj=_:C:0,%3;%1@nonei--!=0;printf(#%1": %dms [%dx%d]",%1@nonett,\
		_BENCHMARK_TIMER_MAJOR,_BENCHMARK_TIMER_MINOR))\
		for(%1@iterMj=0;%1@iterMj<(_BENCHMARK_TIMER_MAJOR);%1@nonett+=GetTickCount()-%1@nonet,++%1@iterMj)\
			for(%1@iterMi=0,%1@nonet=GetTickCount();%1@iterMi<(_BENCHMARK_TIMER_MINOR);++%1@iterMi)
#define C:%1,; %1;

// -----------------------------------------------------------------------------
// Vars
// -----------------------------------------------------------------------------

static stock
	TDW_g_sOffset@Dat,
	TDW_g_sOffset@PFT,
	TDW_g_sOffset@NFT,
	TDW_g_sNumberOfPublics,
	TDW_g_sOnBenchPerformed[] = !"OnBenchPerformed",
	TDW_g_sCallbacks;

// -----------------------------------------------------------------------------
// Internal
// -----------------------------------------------------------------------------

static stock
	benchmark_init()
{
	new
		addr;

	const
		AMX_HDR_OFFSET_PFT = 32,
		AMX_HDR_OFFSET_NFT = 36;

	#emit lctrl 1
	#emit neg
	#emit stor.pri TDW_g_sOffset@Dat

	#emit add.c AMX_HDR_OFFSET_PFT
	#emit stor.s.pri addr
	#emit lref.s.pri addr
	#emit load.s.alt addr
	#emit add
	#emit const.alt AMX_HDR_OFFSET_PFT
	#emit sub
	#emit stor.pri TDW_g_sOffset@PFT

	#emit load.pri TDW_g_sOffset@Dat

	#emit add.c AMX_HDR_OFFSET_NFT
	#emit stor.s.pri addr
	#emit lref.s.pri addr
	#emit load.s.alt addr
	#emit add
	#emit const.alt AMX_HDR_OFFSET_NFT
	#emit sub
	#emit stor.pri TDW_g_sOffset@NFT

	#emit load.alt TDW_g_sOffset@PFT
	#emit sub
	#emit shr.c.pri 3
	#emit stor.pri TDW_g_sNumberOfPublics

	return 0;
}

stock
	benchmark_readString(dest[], address)
{
	new
		cur_char,
		i;
	do {
		#emit lref.s.pri address
		#emit const.alt 0xff
		#emit and
		#emit stor.s.pri cur_char

		dest[i++] = cur_char;

		#emit inc.s address
	} while (cur_char != '\0');
	return
		dest[i] = '\0',
		0;
}


#if defined FILTERSCRIPT
	public
		OnFilterScriptInit()
	{
		if (funcidx(TDW_g_sOnBenchPerformed) != -1) {
			TDW_g_sCallbacks |= 1;
		}
		return
		#if defined TDWBM@OnFilterScriptInit
			TDWBM@OnFilterScriptInit();
		#else
			1;
		#endif
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit TDWBM@OnFilterScriptInit

	#if defined TDWBM@OnFilterScriptInit
		forward TDWBM@OnFilterScriptInit();
	#endif
#else
	public
		OnGameModeInit()
	{
		if (funcidx(TDW_g_sOnBenchPerformed) != -1) {
			TDW_g_sCallbacks |= 1;
		}
		return
		#if defined TDWBM@OnGameModeInit
			TDWBM@OnGameModeInit();
		#else
			1;
		#endif
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit TDWBM@OnGameModeInit

	#if defined TDWBM@OnGameModeInit
		forward TDWBM@OnGameModeInit();
	#endif
#endif

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

stock
	benchmark_run()
{
	if (TDW_g_sOffset@Dat == 0) {
		benchmark_init();
	}
	const
		TDW_MAX_FUNCTION_NAME = 32,
		prefix = ('b') | ('m' << 8) | ('f' << 16) | ('@' << 24), // bmf@
		TDW_MAX_DEFAULT_ITERATIONS_PRE = TDW_MAX_DEFAULT_ITERATIONS;
	static const
		message[] = "[benchmark]: \"%s\" took %dms to execute.";
	new
		idx,
		addr,
		tmp,
		function_name[TDW_MAX_FUNCTION_NAME],
		time@start,
		time@result;
	while (idx < TDW_g_sNumberOfPublics) {
		addr = (idx << 3) + TDW_g_sOffset@PFT + 4;

		#emit lref.s.pri addr
		#emit load.alt TDW_g_sOffset@Dat
		#emit add
		#emit stor.s.pri addr
		#emit lref.s.pri addr
		#emit stor.s.pri tmp

		if (tmp == prefix) {
			#emit load.s.pri addr
			#emit add.c 4
			#emit stor.s.pri addr

			benchmark_readString(function_name, addr);

			#emit load.pri TDW_g_sOffset@PFT
			#emit load.s.alt idx
			#emit shl.c.alt 3
			#emit add
			#emit stor.s.pri addr
			#emit lref.s.pri addr
			#emit stor.s.pri addr

			time@start = GetTickCount();

			#emit push.c TDW_MAX_DEFAULT_ITERATIONS_PRE
			#emit push.c 4

			#emit lctrl 6
			#emit add.c 0x1C
			#emit push.pri

			#emit load.s.pri addr
			#emit sctrl 6

			time@result = GetTickCount() - time@start;

			p:0(message, function_name, time@result);

			if (TDW_g_sCallbacks & 1) {
				CallLocalFunction(TDW_g_sOnBenchPerformed, !"si",
					function_name, time@result
				);
			}
		}
		++idx;
	}
	return 0;
}
