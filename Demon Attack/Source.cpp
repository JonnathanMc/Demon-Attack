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
	bool inimigoAtivo = true;
	int x, y, yInicial, direcaoY = 1, direcaoX = 1;
	time_t proximaAtualizacaoInimigo = clock();
	int vidas = 3;
};

#define ESTADO_JOGO_MENU 0
#define ESTADO_JOGO_GAMEPLAY 1
#define ESTADO_JOGO_RANKING 2
#define ESTADO_JOGO_GAMEOVER 3

#define ESCOLHA_MENU_INICIAR 0
#define ESCOLHA_MENU_SAIR 1

#define SELECTED_FONTE_COLOR ConsoleColor::Green
#define SELECTED_BACKGROUND_COLOR ConsoleColor::Blue

#define QUANTIDADE_INIMIGOS 3

#define LARGURA_TELA 145
#define ALTURA_TELA 70

void Logo();
void Menu(int escolhaMenu);
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla);
void CorConsolePadrao();
void barraInferior();

int main()
{
	struct Bala projetil;
	struct Inimigo inimigo[QUANTIDADE_INIMIGOS];


	int pontos = 0;
	bool noInimigo = false;

	int estadoJogo = ESTADO_JOGO_MENU;
	int escolhaMenu = ESCOLHA_MENU_INICIAR;
	int balasAtiradas = 0, balasDestruidas = 0, balasAtivas = 0;
	int jogador_x = Console::WindowWidth * 0.5f;
	int jogador_y = ALTURA_TELA - 5;
	Char desenho_inimigo2 = (Char)9600;

	time_t  proximaAtualizacaoProjetil = clock();

	array<String^>^ barraInferior = gcnew array<String^>(3);
	
	srand(time(NULL));



	////////////////////// INICIO PROCESSAMENTO ///////////////////////////////////////////
	inimigo[0].x = rand() % (LARGURA_TELA - 11);
	inimigo[0].y = inimigo[0].yInicial = ALTURA_TELA * 0.5f + 14;
	inimigo[0].direcaoX = rand() % 2;

	inimigo[1].x = rand() % (LARGURA_TELA - 11);
	inimigo[1].y = inimigo[1].yInicial = ALTURA_TELA * 0.5f;
	inimigo[1].direcaoX = rand() % 2;

	inimigo[2].x = rand() % (LARGURA_TELA - 11) ;
	inimigo[2].y = inimigo[2].yInicial = ALTURA_TELA * 0.5f - 14;
	inimigo[2].direcaoX = rand() % 2;

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

			// PROJETILLLLL ///////////////////////////////////
			if (projetil.balaEmMovimento)
			{

				/*if (clock() > proximaAtualizacaoProjetil){
					projetil.y -= 1;
					proximaAtualizacaoProjetil = clock() + 50;
				}*/
				projetil.y -= 2;
				
				if (projetil.y == 0)
				{
					projetil.balaEmMovimento = false;
				}
			}
			else
				projetil.balaNoCano = true;

			//INIMIGO // DIREÇÃO: 0 -> BAIXO       1 -> RETO        2 -> CIMA
			for (int x = 0; x < QUANTIDADE_INIMIGOS; x++){
				
				if (inimigo[x].x + 10 >= LARGURA_TELA){
					inimigo[x].direcaoX = 0;
				}
				if (inimigo[x].x <= 1){
					inimigo[x].direcaoX = 1;
				}

				if (clock() > inimigo[x].proximaAtualizacaoInimigo && inimigo[x].direcaoX == 1){
					inimigo[x].x++;
					//inimigo[x].proximaAtualizacaoInimigo = clock() + 30;
				}
				if (clock() > inimigo[x].proximaAtualizacaoInimigo && inimigo[x].direcaoX == 0){
					inimigo[x].x--;
					//inimigo[x].proximaAtualizacaoInimigo = clock() + 30;
				}


				///////// MOVIMENTACAO EM Y //

				if (clock() > inimigo[x].proximaAtualizacaoInimigo && inimigo[x].direcaoY == 0){ ///BAIXO
					inimigo[x].y++;
					inimigo[x].proximaAtualizacaoInimigo = clock() + 30;
					
					
				}

				if (inimigo[x].direcaoY == 1){  //RETO
					inimigo[x].direcaoY = rand() % 2;
					
				}

				if (clock() > inimigo[x].proximaAtualizacaoInimigo && inimigo[x].direcaoY == 2){  ///CIMA
					inimigo[x].y--;
					inimigo[x].proximaAtualizacaoInimigo = clock() + 30;
					
				}

				if (inimigo[x].y <= inimigo[x].yInicial - 4){
					inimigo[x].direcaoY = 0;
				}
				else if (inimigo[x].y >= inimigo[x].yInicial + 4){
					inimigo[x].direcaoY = 2;
				}
				

				
			}

			if (projetil.balaEmMovimento){
				for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
					if (inimigo[i].inimigoAtivo){
						if (projetil.x >= inimigo[i].x && projetil.x <= inimigo[i].x + 9){
							if (projetil.y <= inimigo[i].y && projetil.y >= inimigo[i].y-4){
								
								projetil.balaEmMovimento = false;
								inimigo[i].vidas--;
								pontos++;
							
							}
						}
					}	
					if (inimigo[i].vidas <= 0)
						inimigo[i].inimigoAtivo = false;
				}
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
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y - 2, "   Û Û  ");
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y - 1, "  ÛÛ ÛÛ");
			ConsoleHelper::ImprimirASCIIExtended(jogador_x, jogador_y, " ÛÛ   ÛÛ ");

			if (projetil.balaEmMovimento)
			{
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(projetil.x, projetil.y);
				ConsoleHelper::ImprimirASCIIExtended("Û");
				Console::SetCursorPosition(projetil.x, projetil.y + 1);
				ConsoleHelper::ImprimirASCIIExtended("Û");
				CorConsolePadrao();
			}
			if (projetil.balaNoCano)
			{
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(jogador_x + 4, jogador_y - 2);
				ConsoleHelper::ImprimirASCIIExtended("Û");
				Console::SetCursorPosition(jogador_x + 4, jogador_y - 1);
				ConsoleHelper::ImprimirASCIIExtended("Û");
			}

			// DESENHA O INIMIGO
			
			for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
				if (inimigo[i].inimigoAtivo){
					Console::ForegroundColor = ConsoleColor::Green;
					Console::SetCursorPosition(inimigo[i].x, inimigo[i].y - 3);
					ConsoleHelper::ImprimirASCIIExtended("    Û Û   ");
					Console::ForegroundColor = ConsoleColor::Red;
					Console::SetCursorPosition(inimigo[i].x, inimigo[i].y - 2);
					ConsoleHelper::ImprimirASCIIExtended(" ÛÛÛ Û ÛÛÛ");
					//Console::ForegroundColor = ConsoleColor::DarkMagenta;
					Console::SetCursorPosition(inimigo[i].x, inimigo[i].y - 1);
					ConsoleHelper::ImprimirASCIIExtended(" Û ÛÛÛÛÛ Û");
					//Console::ForegroundColor = ConsoleColor::Green;
					Console::SetCursorPosition(inimigo[i].x, inimigo[i].y);
					ConsoleHelper::ImprimirASCIIExtended("    Û Û   ");
				}
			}
			Console::SetCursorPosition(10, 10);
			Console::Write(" " + pontos);
		

			for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
				if (inimigo[i].inimigoAtivo){
					/*Console::SetCursorPosition(inimigo[i].x - 8, inimigo[i].y - 5);
					Console::Write("YINI: " + inimigo[i].yInicial);*/
					Console::SetCursorPosition(inimigo[i].x + 1, inimigo[i].y - 6);
					Console::Write("DireçãoY: " + inimigo[i].direcaoY);
					/*Console::SetCursorPosition(inimigo[i].x - 8, inimigo[i].y - 3);
					Console::Write("Vidas: " + inimigo[i].vidas);
					Console::SetCursorPosition(inimigo[i].x - 3, inimigo[i].y-2);
					Console::Write(inimigo[i].x);
					Console::SetCursorPosition(inimigo[i].x - 3, inimigo[i].y - 1);
					Console::Write(inimigo[i].y);
					Console::SetCursorPosition(inimigo[i].x - 3, inimigo[i].y );
					Console::Write(inimigo[i].inimigoAtivo);
					*/
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
