/** ======================================================================== **\
| Version:
0.5.1

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


#if defined _inc_tdw_test
	#undef _inc_tdw_test
#endif

#if defined _TDW_TSLIB__TEST
	#endinput
#endif

#define _TDW_TSLIB__TEST

#include "tdw_test/detect/crashdetect"

#define TEST%0(%1) forward _wT@%1();public _wT@%1()

#define ASSERT_TRUE%0(%1) _TDW_TEST_(bool:(%1),#%1)
#define ASSERT_FALSE%0(%1) _TDW_TEST_(bool:(%1),#%1)
// Binary
#define ASSERT_EQU%0(%1,%2) _TDW_TEST_(bool:((%1) == (%2)), (#%1 == %2))
#define ASSERT_NEQ%0(%1,%2) _TDW_TEST_(bool:((%1) != (%2)), (#%1 != %2))
#define ASSERT_LSS%0(%1,%2) _TDW_TEST_(bool:((%1) < (%2)), (#%1 < %2))
#define ASSERT_LEQ%0(%1,%2) _TDW_TEST_(bool:((%1) <= (%2)), (#%1 <= %2))
#define ASSERT_GTR%0(%1,%2) _TDW_TEST_(bool:((%1) > (%2)), (#%1 > %2))
#define ASSERT_GEQ%0(%1,%2) _TDW_TEST_(bool:((%1) >= (%2)), (#%1 >= %2))


#if defined FILTERSCRIPT
	#if defined TSH@OnFilterScriptInit
		forward TSH@OnFilterScriptInit();
	#endif

	public
		OnFilterScriptInit()
	{
		// We need to load the hook because the tests are slow.
		#if defined TSH@OnFilterScriptInit
			TSH@OnFilterScriptInit();
		#endif
		Test_Run();
		return 1;
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit TSH@OnFilterScriptInit
#else
	#if defined TSH@OnGameModeInit
		forward TSH@OnGameModeInit();
	#endif

	public
		OnGameModeInit()
	{
		// We need to load the hook because the tests are slow.
		#if defined TSH@OnGameModeInit
			TSH@OnGameModeInit();
		#endif
		Test_Run();
		return 1;
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit TSH@OnGameModeInit
#endif


static stock
	TDW_g_sOffsetDat,
	TDW_g_sOffsetPublicTable,
	TDW_g_sOffsetNativeTable,
	TDW_g_sNumberOfPublics,
	TDW_g_sFunctionHaveAssertions;



static stock
	Test_Init()
{
	new addr;

	const
		AMX_HEADER_OFFSET_PFT = 32,
		AMX_HEADER_OFFSET_NFT = 36;

	#emit lctrl 1
	#emit neg
	#emit stor.pri TDW_g_sOffsetDat

	#emit add.c AMX_HEADER_OFFSET_PFT
	#emit stor.s.pri addr
	#emit lref.s.pri addr
	#emit load.s.alt addr
	#emit add
	#emit const.alt AMX_HEADER_OFFSET_PFT
	#emit sub
	#emit stor.pri TDW_g_sOffsetPublicTable

	#emit load.pri TDW_g_sOffsetDat
	#emit add.c AMX_HEADER_OFFSET_NFT
	#emit stor.s.pri addr
	#emit lref.s.pri addr
	#emit load.s.alt addr
	#emit add
	#emit const.alt AMX_HEADER_OFFSET_NFT
	#emit sub
	#emit stor.pri TDW_g_sOffsetNativeTable

	#emit load.alt TDW_g_sOffsetPublicTable
	#emit sub
	#emit shr.c.pri 3
	#emit stor.pri TDW_g_sNumberOfPublics

	return 0;
}


static stock
	Test_Run()
{
	Test_Init();

	print(!"\n========= Unit Test Run =========");

	const
		// TDW_g_sTag = TAG_PACK('@','T','w','_'),
		TDW_g_sTag = ('_')|('w'<<8)|('T'<<16)|('@'<<24),
		TDW_MAX_FUNCTION_NAME = 32;

	new
		index,
		addr,
		tmp,
		count_of_tests,
		count_of_passed,
		count_of_failed,
		function_name[TDW_MAX_FUNCTION_NAME],
		ret_value,
		time_ms;

	while (index < TDW_g_sNumberOfPublics)
	{
		addr = (index << 3) + TDW_g_sOffsetPublicTable + 4;

		#emit lref.s.pri addr
		#emit load.alt TDW_g_sOffsetDat
		#emit add
		#emit stor.s.pri addr

		#emit lref.s.pri addr
		#emit stor.s.pri tmp

		if (tmp == TDW_g_sTag)
		{
			++count_of_tests;

			// Reset the variable.
			TDW_g_sFunctionHaveAssertions = 0;

			// Get the name without of the tag.
			#emit load.s.pri addr
			#emit add.c 4
			#emit stor.s.pri addr

			// terminate the buffer of the function name.
			function_name[0] = '\0';

			// read the name of a function into buffer.
			ReadMemoryString(function_name, addr);

			printf("[ RUN        ] %s", function_name);

			// get the function address to call it.
			#emit load.pri TDW_g_sOffsetPublicTable
			#emit load.s.alt index
			#emit shl.c.alt 3
			#emit add
			#emit stor.s.pri addr
			#emit lref.s.pri addr
			#emit stor.s.pri addr

			time_ms = GetTickCount();

			// Call the function from the address
			#emit push.c 0

			// Push the return address
			#emit lctrl 6
			#emit add.c 0x1C
			#emit push.pri

			// Go to the address
			#emit load.s.pri addr
			#emit sctrl 6

			// Get the returned value of the function
			// #emit stor.s.pri ret_value

			time_ms = GetTickCount() - time_ms;

			if (TDW_g_sFunctionHaveAssertions)
			{
				printf("[   FAILED   ] %s (%d ms)", function_name, time_ms);
				++count_of_failed;
			}
			else
			{
				printf("[         OK ] %s (%d ms)", function_name, time_ms);
				++count_of_passed;
			}
		}
		++index;
	}

	// Display the result of tests.
	print(!"============ Summary ============");
	printf("\tCount of all tests: %d", count_of_tests);
	printf("\tCount of passed:    %d", count_of_passed);
	printf("\tCount of failed:    %d", count_of_failed);
	print(!"============== End ==============");

	return 1;
}


static stock
	ReadMemoryString(dest[], address)
{
	new chr, i;
	do
	{
		#emit lref.s.pri address
		#emit const.alt 0xff
		#emit and
		#emit stor.s.pri chr

		dest[i++] = chr;

		#emit inc.s address
	}
	while (chr != '\0');
}


stock
	_TDW_TEST_(bool:x, const c_str[])
{
	if (x == false)
	{
		printf("\t* Assertion failed: %s", c_str);
		TDW_g_sFunctionHaveAssertions += 1;
	}
	return 0;
}