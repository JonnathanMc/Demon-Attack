#include "ConsoleHelper.h"
#include<stdlib.h>

using namespace System;

struct Bala{
	int x;
	int y;
	bool balaEmMovimento;
};

#define ESTADO_JOGO_MENU 0
#define ESTADO_JOGO_GAMEPLAY 1
#define ESTADO_JOGO_RANKING 2
#define ESTADO_JOGO_GAMEOVER 3

#define ESCOLHA_MENU_INICIAR 0
#define ESCOLHA_MENU_RANKING 1
#define ESCOLHA_MENU_SAIR 2

#define SELECTED_FONTE_COLOR ConsoleColor::Green
#define SELECTED_BACKGROUND_COLOR ConsoleColor::Blue

#define QUANTIDADE_BALAS 10

void Logo();
void Menu(int escolhaMenu);
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla);
void CorConsolePadrao();


int main()
{
	struct Bala bala[QUANTIDADE_BALAS];

	int estadoJogo = ESTADO_JOGO_MENU;
	int escolhaMenu = ESCOLHA_MENU_INICIAR;

	int jogador_x = 45;
	int jogador_y = 25;
	

	int balasAtiradas = 0, balasDestruidas =0, balasAtivas = 0;

	int altura_tela, largura_tela;

	bool projetil_em_movimento = false;

	largura_tela = 100;
	altura_tela = 30;

	Console::SetWindowSize(100, 30);
	Console::SetBufferSize(100, 30);
	Console::CursorVisible = false;

	for (;;){
		switch (estadoJogo)
		{
		case ESTADO_JOGO_MENU:
			Console::Clear();

			CorConsolePadrao();

			Logo();
			Menu(escolhaMenu);

			ConsoleKeyInfo tecla;
			tecla = Console::ReadKey();
			escolhaMenu = MenuOpcoesSelecao(escolhaMenu, tecla);

			CorConsolePadrao();

			// MUNDANÄA DE MODO 
			if (escolhaMenu == ESCOLHA_MENU_INICIAR && tecla.Key == ConsoleKey::Enter)
			{
				estadoJogo = ESTADO_JOGO_GAMEPLAY;
			}
			if (escolhaMenu == ESCOLHA_MENU_RANKING && tecla.Key == ConsoleKey::Enter)
			{
				estadoJogo = ESTADO_JOGO_RANKING;
			}
			if (escolhaMenu == ESCOLHA_MENU_SAIR && tecla.Key == ConsoleKey::Enter)
			{
				exit(0);
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

				else if (tecla.Key == ConsoleKey::Spacebar)
				{
					bala[balasAtiradas].balaEmMovimento= true;
					bala[balasAtiradas].x = jogador_x + 1;
					bala[balasAtiradas].y = jogador_y - 1;
					balasAtiradas++;
					//Console::SetCursorPosition(10, 10);

				}
			}

			// DESENHA

			CorConsolePadrao();

			Console::Clear();
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y, ConsoleColor::Black, ConsoleColor::Magenta, "€‹€");

			Console::SetCursorPosition(jogador_x, jogador_y + 2);
			Console::WriteLine(balasAtivas);
			
			balasAtivas = balasAtiradas - balasDestruidas;

			for (int a = balasDestruidas ; a < balasAtiradas; a++){
				Console::SetCursorPosition(jogador_x + 10, jogador_y + 2);
				Console::WriteLine(balasAtiradas);
				Console::SetCursorPosition(jogador_x + 13, jogador_y + 2);
				Console::WriteLine("a = " + a);

				if (bala[a].balaEmMovimento){
					bala[a].y--;
					
					if (bala[a].y == 0)
					{
						bala[a].balaEmMovimento = false;
						balasDestruidas++;
						
					}
					Console::SetCursorPosition(jogador_x + 2, jogador_y + 2);
					Console::WriteLine(bala[a].x);
					Console::SetCursorPosition(jogador_x + 6, jogador_y + 2);
					Console::WriteLine(bala[a].y);

					Console::SetCursorPosition(bala[a].x, bala[a].y);
					Console::BackgroundColor = ConsoleColor::DarkGray;
					Console::ForegroundColor = ConsoleColor::Red;
					Console::WriteLine("A");

					CorConsolePadrao();
					
					
					//ConsoleHelper::ImprimirASCIIExtended(bala[a].x, bala[a].y, ConsoleColor::DarkGray, ConsoleColor::Red, "A");
				}
			}
			Threading::Thread::Sleep(32);
			break;

		case ESTADO_JOGO_RANKING:
			Console::Clear();

			ConsoleHelper::ImprimirASCIIExtended(40, 3, "MODULO EM CONTRUCAO!!");
			ConsoleHelper::ImprimirASCIIExtended(30, 25, "Pressione ENTER para voltar ao menu");
			tecla = Console::ReadKey(true);
			if (tecla.Key == ConsoleKey::Enter)
			{
				estadoJogo = ESTADO_JOGO_MENU;
			}
			break;

		}
	}

	//system("pause");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Logo()
{
	ConsoleHelper::ImprimirASCIIExtended(15, 1, "  ____                                  _   _   _             _    ");
	ConsoleHelper::ImprimirASCIIExtended(15, 2, " |  _ \\  ___ _ __ ___   ___  _ __      / \\ | |_| |_ __ _  ___| | __");
	ConsoleHelper::ImprimirASCIIExtended(15, 3, " | | | |/ _ \\ '_ ` _ \\ / _ \\| '_ \\    / _ \\| __| __/ _` |/ __| |/ /");
	ConsoleHelper::ImprimirASCIIExtended(15, 4, " | |_| |  __/ | | | | | (_) | | | |  / ___ \\ |_| || (_| | (__|   < ");
	ConsoleHelper::ImprimirASCIIExtended(15, 5, " |____/ \\___|_| |_| |_|\\___/|_| |_| /_/   \\_\\__|\\__\\__,_|\\___|_|\\_\\");
}
void CorConsolePadrao()
{
	Console::BackgroundColor = ConsoleColor::Black;
	Console::ForegroundColor = ConsoleColor::White;
}

void Menu(int escolhaMenu)
{
	switch (escolhaMenu)
	{
	case ESCOLHA_MENU_INICIAR:
		ConsoleHelper::ImprimirASCIIExtended(43, 15, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "INICIAR");
		ConsoleHelper::ImprimirASCIIExtended(43, 16, ConsoleColor::Black, ConsoleColor::White, "RANKING");
		ConsoleHelper::ImprimirASCIIExtended(44, 17, ConsoleColor::Black, ConsoleColor::White, "SAIR");
		break;

	case ESCOLHA_MENU_RANKING:
		ConsoleHelper::ImprimirASCIIExtended(43, 15, ConsoleColor::Black, ConsoleColor::White, "INICIAR");
		ConsoleHelper::ImprimirASCIIExtended(43, 16, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "RANKING");
		ConsoleHelper::ImprimirASCIIExtended(44, 17, ConsoleColor::Black, ConsoleColor::White, "SAIR");
		break;

	case ESCOLHA_MENU_SAIR:
		ConsoleHelper::ImprimirASCIIExtended(43, 15, ConsoleColor::Black, ConsoleColor::White, "INICIAR");
		ConsoleHelper::ImprimirASCIIExtended(43, 16, ConsoleColor::Black, ConsoleColor::White, "RANKING");
		ConsoleHelper::ImprimirASCIIExtended(44, 17, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "SAIR");
		break;
	}
}
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla)
{
	if (tecla.Key == ConsoleKey::S || tecla.Key == ConsoleKey::DownArrow)
	{
		opcao++;
		if (opcao == 3)
			opcao = 0;
	}
	if (tecla.Key == ConsoleKey::W || tecla.Key == ConsoleKey::UpArrow)
	{
		if (opcao == 0)
			opcao = 2;
		else
			opcao--;
	}

	return opcao;
}