/** ======================================================================== **\
	Unit Tests

| Description:
	The library with functions for unit testing.

| License:
	Copyright (c) 2017 Double V

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

#if defined _inc_tdw_utest
	#undef _inc_tdw_utest
#endif

#if defined _TDW_UTEST
	#endinput
#endif

#define _TDW_UTEST

#include "amx\amx_header"

// -----------------------------------------------------------------------------
// Macros

#define u@$ 0]);
#define __:%0$ u@(u[_:%0 u@$

#define TEST(%1) uts@T:%1$

#define uts@T: __:uts@T0:uts@T1:$
// Seprate parameters
#define uts@T0:uts@T1:%8$%1,%2$ %8$forward utc@%2_%1();public utc@%2_%1()for(new bool:_utc@%1=(u_test_start("\0\0"#%1),print(!"[ RUN        ] "#%1) || true); _utc@%1; _utc@%1 = false)
#define uts@T1:%8$%1$           %8$forward ut_@%1();public ut_@%1()for(new bool:_utc@%1=(u_test_start("\0\0"#%1),print(!"[ RUN        ] "#%1) || true); _utc@%1; _utc@%1 = false)

//  Remove trailing spaces
#define utc@%0\32;%1(%2) utc@%0%1(%2)
#define ut_@%0\32;%1(%2) ut_@%0%1(%2)
#define _utc@%0\32;%1) _utc@%0%1)

// Assertions
//
#define ASSERT(%1) utest_assert((%1), uts@b:%1)
// Boolean
#define ASSERT_TRUE(%1) utest_assert(((%1) == bool:0), uts@b:%1)
#define ASSERT_FALSE(%1) utest_assert(((%1) != bool:0), uts@b:!(%1))
#define ASSERT_NULL(%1) utest_assert((_:(%1) == _:0), uts@b:%1==NULL)
// Conditionals
#define ASSERT_EQU(%1,%2) ASSERT(%1 == %2)
#define ASSERT_NEQ(%1,%2) ASSERT(%1 != %2)
#define ASSERT_LSS(%1,%2) ASSERT(%1 < %2)
#define ASSERT_LEQ(%1,%2) ASSERT(%1 <= %2)
#define ASSERT_GTR(%1,%2) ASSERT(%1 > %2)
#define ASSERT_GEQ(%1,%2) ASSERT(%1 >= %2)

// PP Parser
#define uts@b: _:uts@p1:uts@p2:uts@qt:uts@nq:
// Parse the parenthesis of a function
// \x28 in ASCII-code is the left parenthesis
// \x29 in ASCII-code is the right parenthesis
#define uts@p1:uts@p2:uts@qt:uts@nq:%1(%2) uts@p1:uts@p2:uts@qt:uts@nq:%1\x28;%2)
#define uts@p2:uts@qt:uts@nq:%1)%2)        uts@p2:uts@qt:uts@nq:%1\x29;%2)
// Parse the quotes
// \x22 in ASCII-code is the quote
#define uts@qt:uts@nq:%0"%1"%2)            uts@qt:uts@nq:%0\x22;%1\x22;%2)
#define uts@nq:%1)                         "\"%1\"")

// -----------------------------------------------------------------------------
// Setup

enum UnitTestCase
{
	UnitTestCase_Fails,
	UnitTestCase_Tests,
	UnitTestCase_PrevAddr
};

static stock
	TDW_g_sHdr[AMX_HDR],        // AMX header
	TDW_g_sCurArrayAddr,        // Current address of the array.
	TDW_g_sPrevFailAddr,        // Address of previous fail.
	TDW_g_sNumFails,            // Number of fails.
	TDW_g_sNumTests,            // Number of tests.
	bool:TDW_g_sInTest = false;

// -----------------------------------------------------------------------------
// API Functions

stock u_test_start(data_array[])
{
	// Reset the cells in the array.
	data_array[0] =
	data_array[1] = 0;
	// Store the address of the array.
	TDW_g_sCurArrayAddr = ref(data_array);
}

stock utest_run(&num_tests, &num_fails)
{
	GetAmxHeader(TDW_g_sHdr);

	new
		name[32],
		num_publics = GetNumPublics(TDW_g_sHdr);

	// Reset the variables.
	TDW_g_sCurArrayAddr =
	TDW_g_sPrevFailAddr = 0;

	// Find all functions with unit tests
	do
	{
		if (GetPublicNameFromIndex(num_publics, name) &&
			strcmp(name, !"ut_@", _, 4) == 0)
		{
			num_fails = TDW_g_sNumFails;

			TDW_g_sInTest = true;
			CallLocalFunction(name, !"");
			TDW_g_sInTest = false;

			if (num_fails != TDW_g_sNumFails)
			{
				// TDW_g_sCurTestAddr is an address of unit test name. The name
				// starts with two empty cells to store the previous address
				// and the number of fails
				//
				// In first cell we store the previous address
				WriteAmxMemory(TDW_g_sCurArrayAddr, TDW_g_sPrevFailAddr);
				// In the next cell we store the number of unit test fails
				WriteAmxMemory(TDW_g_sCurArrayAddr + 4, TDW_g_sNumFails - num_fails);
				// Store the current address (see below)
				TDW_g_sPrevFailAddr = TDW_g_sCurArrayAddr;
			}
		}
	}
	while (num_publics--);

	num_tests = TDW_g_sNumTests;
	num_fails = TDW_g_sNumFails;
}

stock utest_run_all()
{
	new
		num_tests,
		num_fails;

	utest_run(num_tests, num_fails);

	if (num_fails)
	{
		while (TDW_g_sPrevFailAddr != 0)
		{
			printf("%s %d",
				deref(TDW_g_sPrevFailAddr + 8), // Address of the name
				ReadAmxMemory(TDW_g_sPrevFailAddr + 4)); // Get the number of fails
			TDW_g_sPrevFailAddr = ReadAmxMemory(TDW_g_sPrevFailAddr);
		}
	}

	printf("Result: %d %d", num_tests, num_fails);
}

stock utest_run_case(utc[UnitTestCase], const case_name[])
{
	GetAmxHeader(TDW_g_sHdr);

	new
		name[32],
		num_publics = GetNumPublics(TDW_g_sHdr),
		buffer[32] = "utc@",
		len,
		prev_tests,
		prev_fails;

	strcat(buffer, case_name);
	len = strlen(buffer);
	buffer[len++] = '_';

	prev_tests = TDW_g_sNumTests;
	prev_fails = TDW_g_sNumFails;

	printf("[------------] Starting test case: %s", case_name);

	// Reset the variables.
	utc[UnitTestCase_PrevAddr] =
	TDW_g_sCurArrayAddr =
	TDW_g_sNumTests =
	TDW_g_sNumFails = 0;

	// Find all functions with unit tests
	do
	{
		if (GetPublicNameFromIndex(num_publics, name) &&
			strcmp(name, buffer, _, len) == 0)
		{
			utc[UnitTestCase_Fails] = TDW_g_sNumFails;

			TDW_g_sInTest = true;
			CallLocalFunction(name, !"");
			TDW_g_sInTest = false;

			if (utc[UnitTestCase_Fails] != TDW_g_sNumFails)
			{
				// TDW_g_sCurTestAddr is an address of unit test name. The name
				// starts with two empty cells to store the previous address
				// and the number of fails
				//
				// In first cell we store the previous address
				WriteAmxMemory(TDW_g_sCurArrayAddr, utc[UnitTestCase_PrevAddr]);
				// In the next cell we store the number of unit test fails
				WriteAmxMemory(TDW_g_sCurArrayAddr + 4, TDW_g_sNumFails - utc[UnitTestCase_Fails]);
				// Store the current address (see below)
				utc[UnitTestCase_PrevAddr] = TDW_g_sCurArrayAddr;
			}
		}
	}
	while (num_publics--);

	printf("[------------] End testing");

	utc[UnitTestCase_Tests] = TDW_g_sNumTests;
	utc[UnitTestCase_Fails] = TDW_g_sNumFails;

	TDW_g_sNumTests = prev_tests;
	TDW_g_sNumFails = prev_fails;
}

stock utest_print_case(utc[UnitTestCase])
{
	while (utc[UnitTestCase_PrevAddr] != 0)
	{
		printf("%s %d",
			deref(utc[UnitTestCase_PrevAddr] + 8), // Address of the name
			ReadAmxMemory(utc[UnitTestCase_PrevAddr] + 4)); // Get the number of fails
		utc[UnitTestCase_PrevAddr] = ReadAmxMemory(utc[UnitTestCase_PrevAddr]);
	}
}

stock utest_test(bool:x, exp[])
{
	++TDW_g_sNumTests;
	if (_:x == 0)
	{
		++TDW_g_sNumFails;
		printf("[   FAILED   ] %s", exp);
	}
}
