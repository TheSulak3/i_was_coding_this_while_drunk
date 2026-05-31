#include "../Hooks.hpp"
#include "../Cheat/CCheat.hpp"

#include <d3dx9.h>
#include <unordered_map>

#pragma comment(lib, "d3dx9.lib")

BOOL __stdcall Hooks::Cheat::hkDllMain( HMODULE hModule, DWORD ulReason, LPVOID lpReserved )
{
	printf( "[ t.me/violanes ] intercepting dllmain...\n" );

	//DisableThreadLibraryCalls( hModule );
	// add vectored exception handler

	HANDLE hProcess = GetCurrentProcess( );
	if ( GetPriorityClass( hProcess ) != 128 && GetPriorityClass( hProcess ) != 256 )
		SetPriorityClass( hProcess, 128 );

	*reinterpret_cast< HMODULE* >( 0x407E2E0C ) = hModule;

	CCheat* pCheat = new CCheat( );
	pCheat->Init( );

	return 1;
}

int __cdecl Hooks::Cheat::hkRand( )
{
	return 0;
}

int __fastcall Hooks::Cheat::hkGetNetVar( void* thisptr, int, uint32_t iClassHash, uint32_t iVarHash )
{
	static size_t iIndex = 50;

	// Default remapping rules:
	//   idx <  496: identity     (idx -> idx)
	//   idx < 1336: +1 shift     (idx -> idx + 1)
	//   idx >= 1336: -13 shift   (idx -> idx - 13)
	// Entries below override those defaults.
	static const std::unordered_map< size_t, size_t > aOverrides =
	{
		// 369-380 rotation (exceptions to identity default)
		{ 369, 380 }, { 370, 369 }, { 371, 370 }, { 372, 371 }, { 373, 372 }, { 374, 373 },
		{ 375, 374 }, { 376, 375 }, { 377, 376 }, { 378, 377 }, { 379, 378 }, { 380, 379 },

		// 572-575 shuffle (exceptions to +1 default)
		{ 572, 574 }, { 573, 575 }, { 574, 576 }, { 575, 573 },

		// 1301-1335 complex remap (exceptions to +1 default)
		{ 1301, 1305 }, { 1302, 1306 }, { 1303, 1307 }, { 1304, 1308 }, { 1305, 1309 },
		{ 1306, 1310 }, { 1307, 1311 }, { 1308, 1302 }, { 1309, 1302 }, { 1310, 1303 },
		{ 1311, 1303 }, { 1312, 1304 }, { 1313, 1304 }, { 1314, 1314 }, { 1315, 1314 },
		{ 1316, 1312 }, { 1317, 1312 }, { 1318, 1313 }, { 1319, 1313 }, { 1320, 1315 },
		{ 1321, 1315 }, { 1322, 1316 }, { 1323, 1316 }, { 1324, 1317 }, { 1325, 1317 },
		{ 1326, 1318 }, { 1327, 1318 }, { 1328, 1319 }, { 1329, 1319 }, { 1330, 1320 },
		{ 1331, 1320 }, { 1332, 1321 }, { 1333, 1321 }, { 1334, 1322 }, { 1335, 1322 },

		// 1478-1480 shuffle (exceptions to -13 default)
		{ 1478, 1467 }, { 1479, 1465 }, { 1480, 1466 },
	};

	auto it = aOverrides.find( iIndex );
	size_t iMapped;
	if ( it != aOverrides.end( ) )
		iMapped = it->second;
	else if ( iIndex < 496 )
		iMapped = iIndex;
	else if ( iIndex < 1336 )
		iMapped = iIndex + 1;
	else
		iMapped = iIndex - 13;

	reinterpret_cast< int* >( thisptr )[ iMapped ] += o_GetNetVar( thisptr, iClassHash, iVarHash );

	iIndex++;

	return 0xDEADC0DE;
}

void __fastcall Hooks::Cheat::hkInitFonts( void* thisptr, int, int pDevice )
{
	printf( "initing fonts lol\n" );

	*reinterpret_cast< int* >( thisptr ) = pDevice;
	*reinterpret_cast< std::string* >( ( uint32_t ) thisptr + 0x18 ) = "Verdana";
	*reinterpret_cast< uint32_t* >( ( uint32_t ) thisptr + 0x30 ) = 12;

	using GetScreenSize_t = void ( __thiscall* ) ( void***, int*, int* );
	using FontMgrConstructor_t = int ( __cdecl* ) ( void**, int* );
	using FontMgrDestructor_t = int ( __thiscall* ) ( uint32_t, void* );
	using InitFont_t = void ( __thiscall* ) ( uint32_t, const char* );

	void*** pSurface = ( *reinterpret_cast< void***** >( 0x40808990 ) )[ 12 ];
	if ( !pSurface )
	{
		printf( "failed to get surface interface\n" );
		return;
	}

	int iWidth = 0, iHeight = 0;

	( reinterpret_cast< GetScreenSize_t >( pSurface[ 0 ][ 44 ] ) )( pSurface, &iWidth, &iHeight );

	printf( "screen w/h : %d %d\n", iWidth, iHeight );

	D3DXCreateTexture( reinterpret_cast< LPDIRECT3DDEVICE9 >( pDevice ), iWidth, iHeight, -1, 1024,
		D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, reinterpret_cast< LPDIRECT3DTEXTURE9* >( ( uint32_t ) thisptr + 0x34 ) );

	void* pFontMgr[ 2 ];

	( reinterpret_cast< FontMgrConstructor_t >( 0x404CBD80 ) )( pFontMgr, &pDevice );
	( reinterpret_cast< FontMgrDestructor_t >( 0x404CBE40 ) )( ( uint32_t ) thisptr + 0x10, pFontMgr[ 0 ] );

	( reinterpret_cast< InitFont_t >( ( reinterpret_cast< void**** >( ( uint32_t ) thisptr + 0x10 ) )[ 0 ][ 0 ][ 0 ] ) )
		( ( uint32_t ) thisptr + 0x10, ( reinterpret_cast< std::string* >( ( uint32_t ) thisptr + 0x18 ) )->c_str( ) );

	printf( "fonts done :)) hope it helps\n" );
}

void __cdecl Hooks::Cheat::hkRunInitializer( void* thisptr, void* pVtable )
{
	printf( "asked for initializer \n" /*pVtable, ( *( uint32_t** ) pVtable )[ 2 ]*/ );

	using InitFunction_t = void ( __thiscall* ) ( void* );
	( reinterpret_cast< InitFunction_t >( ( *( uint32_t** ) pVtable )[ 2 ] ) )( pVtable );
}

void __fastcall Hooks::Cheat::hkSendRequest( void* pRequestMgr, int, std::string pResponse, uint32_t prolly_size, bool )
{
	printf( "send request %zu - %s <> 0x%X 0x%p [from 0x%p]\n",
		pResponse.length( ), pResponse.c_str( ), prolly_size, &pResponse, _ReturnAddress( ) );
}
