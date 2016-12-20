/** ======================================================================== **\
	Debug

| Description:
The library contains some function for debugging code.

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

#if defined _inc_tdw_debug
	#undef _inc_tdw_debug
#endif

#if defined _TDW_DEBUG
	#endinput
#endif

#define _TDW_DEBUG

#if !defined _DEBUG
	#define _DEBUG (0)
#endif

#define p:%1(%2)%0\10; debug_print@%1(%2)%0$

#define debug_print@0(%2)%0$ printf(%2)%0

#if _DEBUG >= 1
	#define debug_print@1(%2)%0$ printf(%2)%0
#else
	#define debug_print@1(%2)%0$
#endif
#if _DEBUG >= 2
	#define debug_print@2(%2)%0$ printf(%2)%0
#else
	#define debug_print@2(%2)%0$
#endif
#if _DEBUG >= 3
	#define debug_print@3(%2)%0$ printf(%2)%0
#else
	#define debug_print@3(%2)%0$
#endif
#if _DEBUG >= 4
	#define debug_print@4(%2)%0$ printf(%2)%0
#else
	#define debug_print@4(%2)%0$
#endif
#if _DEBUG >= 5
	#define debug_print@5(%2)%0$ printf(%2)%0
#else
	#define debug_print@5(%2)%0$
#endif

#define debug_print@e(%2)%0$ printf("\a[TDW][    Error    ]: "%2)%0
#define debug_print@w(%2)%0$ printf("[TDW][   Warning   ]: "%2)%0
#define debug_print@i(%2)%0$ printf("[TDW][ Information ]: "%2)%0
