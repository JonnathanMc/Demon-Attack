#pragma once
#pragma managed
using namespace System;

ref class ConsoleHelper
{

private:
	static Text::Encoding ^ enc437 = Text::Encoding::GetEncoding(437);
	static Text::Encoding ^ encUTF = Text::Encoding::GetEncoding("UTF-16");

public:
	static void ImprimirASCIIExtended(String^ str)
	{
		array<unsigned char>^ bytes = encUTF->GetBytes(str);

		int tam = (int)(bytes->Length * 0.5f);
		array<unsigned char>^ tmp = gcnew array<unsigned char>(tam);
		int  i, j;
		for (i = 0, j = 0; i < bytes->Length; i += 2, j++)
			tmp[j] = bytes[i];

		Console::Write(enc437->GetString(tmp));
	}

	static void ImprimirASCIIExtended(int coluna, int linha, String^ str)
	{
		Console::SetCursorPosition(coluna, linha);
		ImprimirASCIIExtended(str);
	}

	static void ImprimirASCIIExtended(int coluna,
		int linha,
		ConsoleColor corFundo,
		ConsoleColor corFonte,
		String^ str)
	{
		Console::SetCursorPosition(coluna, linha);
		Console::BackgroundColor = corFundo;
		Console::ForegroundColor = corFonte;
		ImprimirASCIIExtended(str);
	}
	
};
