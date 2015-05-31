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

#define LARGURA_TELA 150
#define ALTURA_TELA 48

void Logo();
void Menu(int escolhaMenu);
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla);
void CorConsolePadrao();
void barraInferior();


int main()
{
	struct Bala bala[QUANTIDADE_BALAS];

	int estadoJogo = ESTADO_JOGO_MENU;
	int escolhaMenu = ESCOLHA_MENU_INICIAR;
	int balasAtiradas = 0, balasDestruidas =0, balasAtivas = 0;
	
	bool projetil_em_movimento = false;

	int jogador_x = Console::WindowWidth * 0.5f;
	int jogador_y = ALTURA_TELA-5;

	array<String^>^ barraInferior = gcnew array<String^>(3);

	for (int a = 0; a < 3; a++){
		for (int b = 0; b < LARGURA_TELA; b++){
			barraInferior[a] += " ";
		}
	}

	Console::SetWindowSize(LARGURA_TELA, ALTURA_TELA);
	Console::SetBufferSize(LARGURA_TELA, ALTURA_TELA);
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

			// MUNDAN€A DE MODO 
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
				else if (tecla.Key == ConsoleKey::Spacebar )
				{
					bala[balasAtiradas].balaEmMovimento= true;
					bala[balasAtiradas].x = jogador_x + 4;
					bala[balasAtiradas].y = jogador_y - 1;
					balasAtiradas++;	
				}
			}

			// DESENHA
			CorConsolePadrao();

			Console::Clear();

			Console::BackgroundColor = ConsoleColor::DarkCyan;
			Console::SetCursorPosition(0, Console::WindowHeight - 3);
			Console::Write(barraInferior[0]);			
			Console::BackgroundColor = ConsoleColor::DarkBlue;
			Console::SetCursorPosition(0, Console::WindowHeight - 2);
			Console::Write(barraInferior[1]);
			Console::SetCursorPosition(0, Console::WindowHeight - 1);
			Console::Write(barraInferior[2]);

			Console::BackgroundColor = ConsoleColor::Black;
			Console::ForegroundColor = ConsoleColor::Magenta;
			ConsoleHelper::ImprimirASCIIExtended(jogador_x , jogador_y- 2,  "   Û Û  ");
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y - 1,  "  ÛÛ ÛÛ");
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y,      " ÛÛ   ÛÛ ");

			/*Console::SetCursorPosition(jogador_x, jogador_y + 2);
			Console::WriteLine(balasAtivas);*/
			
			balasAtivas = balasAtiradas - balasDestruidas;

			for (int a = balasDestruidas ; a < balasAtiradas; a++){
				/*Console::SetCursorPosition(jogador_x + 10, jogador_y + 2);
				Console::WriteLine(balasAtiradas);
				Console::SetCursorPosition(jogador_x + 13, jogador_y + 2);
				Console::WriteLine("a = " + a);*/

				if (bala[a].balaEmMovimento){
					bala[a].y--;
					
					if (bala[a].y == 0)
					{
						bala[a].balaEmMovimento = false;
						balasDestruidas++;
					}
					

					Console::ForegroundColor = ConsoleColor::White;
					Console::SetCursorPosition(bala[a].x, bala[a].y);
					ConsoleHelper::ImprimirASCIIExtended("Û");
					Console::SetCursorPosition(bala[a].x, bala[a].y + 1);
					ConsoleHelper::ImprimirASCIIExtended("Û");




					CorConsolePadrao();
				}
			}
			Threading::Thread::Sleep(16);
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
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 1 , "  ____                                  _   _   _             _    ");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 2, " |  _ \\  ___ _ __ ___   ___  _ __      / \\ | |_| |_ __ _  ___| | __");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 3, " | | | |/ _ \\ '_ ` _ \\ / _ \\| '_ \\    / _ \\| __| __/ _` |/ __| |/ /");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 4, " | |_| |  __/ | | | | | (_) | | | |  / ___ \\ |_| || (_| | (__|   < ");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 5, " |____/ \\___|_| |_| |_|\\___/|_| |_| /_/   \\_\\__|\\__\\__,_|\\___|_|\\_\\");
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
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.45f, ALTURA_TELA * 0.45f, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "INICIAR");
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.45f , ALTURA_TELA * 0.45f + 1, ConsoleColor::Black, ConsoleColor::White, "RANKING");
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f + 1, ALTURA_TELA * 0.45f + 2, ConsoleColor::Black, ConsoleColor::White, "SAIR");
		break;

	case ESCOLHA_MENU_RANKING:
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f , ALTURA_TELA * 0.45f , ConsoleColor::Black, ConsoleColor::White, "INICIAR");
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f, ALTURA_TELA * 0.45f + 1, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "RANKING");
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f + 1, ALTURA_TELA * 0.45f + 2, ConsoleColor::Black, ConsoleColor::White, "SAIR");
		break;

	case ESCOLHA_MENU_SAIR:
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f, ALTURA_TELA * 0.45f , ConsoleColor::Black, ConsoleColor::White, "INICIAR");
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f, ALTURA_TELA * 0.45f + 1, ConsoleColor::Black, ConsoleColor::White, "RANKING");
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f + 1, ALTURA_TELA * 0.45f + 2, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "SAIR");
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
void barraInferior()
{


}