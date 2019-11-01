#include "stdafx.h"
#include "Utility.h"

std::string SymbolLookup::GetSymbolString(void * Address) const
{
	if (Address == nullptr)
		return "";

	DWORD Displacement = 0;
	DWORD64 Addr = reinterpret_cast<DWORD64>(Address);

	SymbolBuffer Symbol;
	char Buffer[StrBufferSize] = { 0, };

	IMAGEHLP_LINE64 line;
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	SymFromAddr(Handle, Addr, 0, &Symbol);

	BOOL Ret = SymGetLineFromAddr64(Handle, Addr, &Displacement, &line);

	if (Ret)
	{
		sprintf_s(Buffer, StrBufferSize, "%s(%d) : %s", line.FileName, line.LineNumber, Symbol.Name);
	}
	else
	{
		sprintf_s(Buffer, StrBufferSize, "No line info : %s", Symbol.Name);
	}

	return std::string(Buffer);
}

void DISPLAY_STACKTRACE()
{
	CallStack Stack;
	SymbolLookup Lookup;
	for (size_t i = 2; i < Stack.GetCount(); ++i)
	{
		const std::string& str = Lookup.GetSymbolString(Stack[i]);
		std::cout << str << std::endl;
	}
}


void DISPLAY_STACKONPOINT()
{
	CallStack Stack;
	SymbolLookup Lookup;
	const std::string& str = Lookup.GetSymbolString(Stack[2]);
	std::cout << str << std::endl;
}