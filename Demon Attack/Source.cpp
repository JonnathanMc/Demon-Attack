#include "ConsoleHelper.h"
#include<stdlib.h>
#include<time.h>

using namespace System;

struct Bala{
	int x;
	int y;
	bool balaEmMovimento = false;
	bool balaNoCano = true;
};
struct Inimigo{
	int x, y;

};

#define ESTADO_JOGO_MENU 0
#define ESTADO_JOGO_GAMEPLAY 1
#define ESTADO_JOGO_RANKING 2
#define ESTADO_JOGO_GAMEOVER 3

#define ESCOLHA_MENU_INICIAR 0
#define ESCOLHA_MENU_SAIR 1

#define SELECTED_FONTE_COLOR ConsoleColor::Green
#define SELECTED_BACKGROUND_COLOR ConsoleColor::Blue

#define QUANTIDADE_BALAS 10

#define LARGURA_TELA 140
#define ALTURA_TELA 70

void Logo();
void Menu(int escolhaMenu);
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla);
void CorConsolePadrao();
void barraInferior();

int main()
{
	struct Bala bala[QUANTIDADE_BALAS], projetil;

	struct Inimigo i1;
	i1.x = 10;
	i1.y = 30;

	int estadoJogo = ESTADO_JOGO_MENU;
	int escolhaMenu = ESCOLHA_MENU_INICIAR;
	int balasAtiradas = 0, balasDestruidas = 0, balasAtivas = 0;
	int jogador_x = Console::WindowWidth * 0.5f;
	int jogador_y = ALTURA_TELA - 5;
	Char desenho_inimigo2 = (Char)9600;

	time_t proximaAtualizacaoInimigo = clock();

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

			// MUNDANЂA DE MODO 
			if (escolhaMenu == ESCOLHA_MENU_INICIAR && tecla.Key == ConsoleKey::Enter)
			{
				estadoJogo = ESTADO_JOGO_GAMEPLAY;
			}
		
			if (escolhaMenu == ESCOLHA_MENU_SAIR && tecla.Key == ConsoleKey::Enter)
			{
				exit(0);
			}

			break;

			//GAMEPLAY
		case ESTADO_JOGO_GAMEPLAY:
			//// PERSONAGEM
			if (Console::KeyAvailable)
			{
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::A && jogador_x > 0)
				{
					jogador_x--;
				}
				else if (tecla.Key == ConsoleKey::D && jogador_x + 9 < LARGURA_TELA)
				{
					jogador_x++;

				}
				else if (tecla.Key == ConsoleKey::Spacebar && projetil.balaEmMovimento == false)
				{
					
					projetil.balaEmMovimento = true;
					projetil.balaNoCano = false;
					projetil.x = jogador_x + 4;
					projetil.y = jogador_y - 1;
				}
			}

			//INIMIGO
			if (clock() > proximaAtualizacaoInimigo && i1.x + 13 < LARGURA_TELA){
				i1.x++;
				proximaAtualizacaoInimigo = clock() + 10;
			}

			////////////////////////// DESENHA ////////////////////////////////////////////////////
			CorConsolePadrao();

			Console::Clear();
			//DESENHA A BARRA INFERIOR
			Console::BackgroundColor = ConsoleColor::DarkCyan;
			Console::SetCursorPosition(0, Console::WindowHeight - 3);
			Console::Write(barraInferior[0]);
			Console::BackgroundColor = ConsoleColor::DarkBlue;
			Console::SetCursorPosition(0, Console::WindowHeight - 2);
			Console::Write(barraInferior[1]);
			Console::SetCursorPosition(0, Console::WindowHeight - 1);
			Console::Write(barraInferior[2]);

			// DESENHA O PERSONAGEM
			Console::BackgroundColor = ConsoleColor::Black;
			Console::ForegroundColor = ConsoleColor::Magenta;
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y - 2, "   Ы Ы  ");
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y - 1, "  ЫЫ ЫЫ");
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y,     " ЫЫ   ЫЫ ");

			if (projetil.balaEmMovimento)
			{
				projetil.y--;
				if (projetil.y == 0)
				{
					projetil.balaEmMovimento = false;
					projetil.balaNoCano = true;

				}
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(projetil.x, projetil.y);
				ConsoleHelper::ImprimirASCIIExtended("Ы");
				Console::SetCursorPosition(projetil.x, projetil.y + 1);
				ConsoleHelper::ImprimirASCIIExtended("Ы");
				CorConsolePadrao();
			}
			if (projetil.balaNoCano)
			{
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(jogador_x + 4, jogador_y - 2);
				ConsoleHelper::ImprimirASCIIExtended("Ы");
				Console::SetCursorPosition(jogador_x + 4, jogador_y - 1);
				ConsoleHelper::ImprimirASCIIExtended("Ы");
			}

			// DESENHA O INIMIGO
			Console::ForegroundColor = ConsoleColor::Red;
			Console::SetCursorPosition(i1.x, i1.y - 3);
			
			ConsoleHelper::ImprimirASCIIExtended("   Ы   Ы " );
			Console::SetCursorPosition(i1.x, i1.y - 2);
			ConsoleHelper::ImprimirASCIIExtended("  Ы  Ы  Ы  ");
			Console::SetCursorPosition(i1.x, i1.y - 1);
			ConsoleHelper::ImprimirASCIIExtended("Ы   Ы Ы   Ы ");
			
			Console::SetCursorPosition(i1.x, i1.y);
			ConsoleHelper::ImprimirASCIIExtended("  Ы     Ы     ");
			
			Threading::Thread::Sleep(14);
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
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 1, "  ____                                  _   _   _             _    ");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 2, " |  _ \\  ___ _ __ ___   ___  _ __      / \\ | |_| |_ __ _  ___| | __");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 3, " | | | |/ _ \\ '_ ` _ \\ / _ \\| '_ \\    / _ \\| __| __/ _` |/ __| |/ /");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 4, " | |_| |  __/ | | | | | (_) | | | |  / ___ \\ |_| || (_| | (__|   < ");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f, ALTURA_TELA *0.10f + 5, " |____/ \\___|_| |_| |_|\\___/|_| |_| /_/   \\_\\__|\\__\\__,_|\\___|_|\\_\\");

	Console::ForegroundColor = ConsoleColor::Red;
	
	/*ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f + 2, ALTURA_TELA *0.40f + 1,    " #        ##");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f , ALTURA_TELA * 0.4f + 2, " ####  ##  #### ");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f - 2, ALTURA_TELA *0.40f + 3, "  #    ######    # ");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f + 6, ALTURA_TELA *0.40f + 4, "....");
	ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f + 6 , ALTURA_TELA *0.40f + 5, ".  .");*/
	//ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA * 0.25f + 4, ALTURA_TELA *0.50f + 6, " .    .");
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
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f + 1, ALTURA_TELA * 0.45f + 2, ConsoleColor::Black, ConsoleColor::White, "SAIR");
		break;
	case ESCOLHA_MENU_SAIR:
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f, ALTURA_TELA * 0.45f, ConsoleColor::Black, ConsoleColor::White, "INICIAR");	
		ConsoleHelper::ImprimirASCIIExtended(LARGURA_TELA *0.45f + 1, ALTURA_TELA * 0.45f + 2, SELECTED_BACKGROUND_COLOR, SELECTED_FONTE_COLOR, "SAIR");
		break;
	}
}
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla)
{
	if (tecla.Key == ConsoleKey::S || tecla.Key == ConsoleKey::DownArrow)
	{
		opcao++;
		if (opcao == 2)
			opcao = 0;
	}
	if (tecla.Key == ConsoleKey::W || tecla.Key == ConsoleKey::UpArrow)
	{
		if (opcao == 0)
			opcao = 1;
		else
			opcao--;
	}

	return opcao;
}
