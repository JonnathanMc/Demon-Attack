#include "ConsoleHelper.h"


using namespace System;

#define ESTADO_JOGO_MENU 0
#define ESTADO_JOGO_GAMEPLAY 1
#define ESTADO_JOGO_GAMEOVER 2


#define ESCOLHA_MENU_INICIAR 0
#define ESCOLHA_MENU_RANKING 1
#define ESCOLHA_MENU_SAIR 2

int main()
{
	const Char JOGADOR_GRAFICO = (Char)9608;
	int estadoJogo = ESTADO_JOGO_MENU;
	int escolhaMenu = ESCOLHA_MENU_INICIAR;

	int jogador_x = 45;
	int jogador_y = 25;
	int projetil_x = 0;
	int projetil_y = 0;

	bool projetil_em_movimento = false;


	Console::SetWindowSize(100, 30);
	Console::SetBufferSize(100, 30);
	Console::CursorVisible = true;

	for (;;){
		switch (estadoJogo)
		{
		case ESTADO_JOGO_MENU:

			Console::Clear();

			Console::BackgroundColor = ConsoleColor::Black;
			Console::ForegroundColor = ConsoleColor::White;

			ConsoleHelper::ImprimirASCIIExtended(15, 1, "  ____                                  _   _   _             _    ");
			ConsoleHelper::ImprimirASCIIExtended(15, 2, " |  _ \\  ___ _ __ ___   ___  _ __      / \\ | |_| |_ __ _  ___| | __");
			ConsoleHelper::ImprimirASCIIExtended(15, 3, " | | | |/ _ \\ '_ ` _ \\ / _ \\| '_ \\    / _ \\| __| __/ _` |/ __| |/ /");
			ConsoleHelper::ImprimirASCIIExtended(15, 4, " | |_| |  __/ | | | | | (_) | | | |  / ___ \\ |_| || (_| | (__|   < ");
			ConsoleHelper::ImprimirASCIIExtended(15, 5, " |____/ \\___|_| |_| |_|\\___/|_| |_| /_/   \\_\\__|\\__\\__,_|\\___|_|\\_\\");



			switch (escolhaMenu)
			{
			case ESCOLHA_MENU_INICIAR:
				ConsoleHelper::ImprimirASCIIExtended(30, 15, ConsoleColor::White, ConsoleColor::Yellow, "INICIAR");
				ConsoleHelper::ImprimirASCIIExtended(30, 16, ConsoleColor::Black, ConsoleColor::White, "RANKING");
				ConsoleHelper::ImprimirASCIIExtended(30, 17, ConsoleColor::Black, ConsoleColor::White, "SAIR");
				break;

			case ESCOLHA_MENU_RANKING:

				ConsoleHelper::ImprimirASCIIExtended(30, 15, ConsoleColor::Black, ConsoleColor::White, "INICIAR");
				ConsoleHelper::ImprimirASCIIExtended(30, 16, ConsoleColor::White, ConsoleColor::Yellow, "RANKING");
				ConsoleHelper::ImprimirASCIIExtended(30, 17, ConsoleColor::Black, ConsoleColor::White, "SAIR");

				break;

			case ESCOLHA_MENU_SAIR:
				ConsoleHelper::ImprimirASCIIExtended(30, 15, ConsoleColor::Black, ConsoleColor::White, "INICIAR");
				ConsoleHelper::ImprimirASCIIExtended(30, 16, ConsoleColor::Black, ConsoleColor::White, "RANKING");
				ConsoleHelper::ImprimirASCIIExtended(30, 17, ConsoleColor::White, ConsoleColor::Yellow, "SAIR");

				break;
			}
			Console::WriteLine(escolhaMenu);

			ConsoleKeyInfo tecla;
			tecla = Console::ReadKey();

			if (tecla.Key == ConsoleKey::S)
			{
				escolhaMenu++;
				if (escolhaMenu == 3)
					escolhaMenu = 0;
			}
			if (tecla.Key == ConsoleKey::W)
			{
				if (escolhaMenu == 0)
					escolhaMenu = 2;
				else
					escolhaMenu--;

			}

			if (escolhaMenu == ESCOLHA_MENU_INICIAR && tecla.Key == ConsoleKey::Enter)
			{
				estadoJogo = ESTADO_JOGO_GAMEPLAY;
			}
			if (escolhaMenu == ESCOLHA_MENU_RANKING && tecla.Key == ConsoleKey::Enter)
			{
				//estadoJogo = ESTADO_JOGO_GAMEPLAY;
			}
			if (escolhaMenu == ESCOLHA_MENU_SAIR && tecla.Key == ConsoleKey::Enter)
			{
				//estadoJogo = ESTADO_JOGO_GAMEPLAY;
			}

			break;

			//GAMEPLAY

		case ESTADO_JOGO_GAMEPLAY:

			if (Console::KeyAvailable)
			{
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::A)
				{
					jogador_x--;
				}
				else if (tecla.Key == ConsoleKey::D)
				{
					jogador_x++;
				}
				/*
								else if (tecla.Key == ConsoleKey::W)
								{
								jogador_y--;
								}
								else if (tecla.Key == ConsoleKey::S)
								{
								jogador_y++;
								}*/
				else if (tecla.Key == ConsoleKey::Spacebar)
				{
					projetil_em_movimento = true;
					projetil_x = jogador_x;
					projetil_y = jogador_y - 1;
				}
			}
			if (projetil_em_movimento){

				projetil_y--;
				if (projetil_y == 0){
					projetil_em_movimento = false;
				}
			}


			// DESENHA
			/*Console::BackgroundColor = ConsoleColor::White;
			Console::ForegroundColor = ConsoleColor::Black;
			Console::Clear();

			Console::SetCursorPosition(jogador_x, jogador_y);
			Console::WriteLine(JOGADOR_GRAFICO);*/
			Console::BackgroundColor = ConsoleColor::White;
			Console::ForegroundColor = ConsoleColor::Black;
			Console::Clear();

			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y, ConsoleColor::Black, ConsoleColor::White, " █▄█ ");
			Threading::Thread::Sleep(10);

			break;
		}
	}

	//system("pause");
	return 0;
}