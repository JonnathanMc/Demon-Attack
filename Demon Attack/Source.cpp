#include "ConsoleHelper.h"
#include<stdlib.h>
#include<time.h>

using namespace System;

#define ESTADO_JOGO_MENU 0
#define ESTADO_JOGO_GAMEPLAY 1
#define ESTADO_JOGO_RANKING 2
#define ESTADO_JOGO_GAMEOVER 3
#define ESTADO_JOGO_WIN 4

#define ESCOLHA_MENU_INICIAR 0
#define ESCOLHA_MENU_SAIR 1

#define SELECTED_FONTE_COLOR ConsoleColor::Green
#define SELECTED_BACKGROUND_COLOR ConsoleColor::Blue

#define VIDA_INIMIGOS_FASE_1 1
#define VIDA_INIMIGOS_FASE_2 2
#define VIDA_INIMIGOS_FASE_3 3

#define LARGURA_TELA 145
#define ALTURA_TELA 70

void Logo();
void Menu(int escolhaMenu);
int MenuOpcoesSelecao(int opcao, ConsoleKeyInfo tecla);
void CorConsolePadrao();
void barraInferior();
void imprimirInimigo(int posX, int posY, int direcaoY, int fase);
void imprimirPersonagem(int posX, int posY);


int aux = 0, direcaoAleatoria;
float tempoAleatorio = 0, tempoDecorrido = 0;
time_t proximaAtualizacaoTempob = clock();
int QUANTIDADE_INIMIGOS = 3;

int main()
{
	struct Bala{
		int x;
		int y;
		bool balaEmMovimento = false;
		bool balaNoCano = true;
	};
	struct Player{
		int x = Console::WindowWidth * 0.5f;
		int y = ALTURA_TELA - 5;
		bool vivo = true;
		int vidas = 3;
	};
	struct Inimigo{
		bool inimigoAtivo = true;
		int x, y, yInicial, direcaoY = 1, direcaoX = 1;
		int vidas = 3;
		float tempoDecorrido = 0;
		bool atirar = false;

		time_t proximaAtualizacaoInimigoX = clock();
		time_t proximaAtualizacaoInimigoY = clock();
		time_t tempoProximoMovimento = clock();

		struct Projetil{
			int x, y;
			bool projetilEmMovimento = false;
		};
		Projetil projetil;
		int atirador = 0;
		struct Aux{
			time_t proximoTiroInimigo = clock();
			float contadorTempoTiro = 0;		
			bool auxTiroInimigo = false;
		};
		Aux aux;
	};

	struct Bala projetil;
	struct Inimigo inimigo[4];
	struct Inimigo enemy;
	struct Player player;

	int fase = 1;
	int pontos = 0;
	bool noInimigo = false;
	bool iniciarFase = true;
	int estadoJogo = ESTADO_JOGO_MENU;
	int escolhaMenu = ESCOLHA_MENU_INICIAR;

	time_t proximoTiroInimigo = clock();
	int auxTiroInimigo = 0;
	float contadorTempoTiro = 0;
	int auxPassarFase = 1;

	Char desenho_inimigo2 = (Char)9600;
	time_t proximaAtualizacaoProjetil = clock(), proximaAtualizacaoTempo = clock();

	array<String^>^ barraInferior = gcnew array<String^>(3);

	srand(time(NULL));

	////////////////////// INICIO PROCESSAMENTO ///////////////////////////////////////////


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
			if (iniciarFase){
				int numero;
				if (fase == 3){
					QUANTIDADE_INIMIGOS = 4;
				}
				else{
					QUANTIDADE_INIMIGOS = 3;
				}

				int distancia = 9;

				for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
					inimigo[i].x = rand() % (LARGURA_TELA - 11);
					inimigo[i].y = inimigo[i].yInicial = ALTURA_TELA * 0.5f + distancia;
					inimigo[i].direcaoX = rand() % 2;
					inimigo[i].direcaoY = rand() % 3;
					inimigo[i].inimigoAtivo = true;
					inimigo[i].atirar = false;
					inimigo[i].projetil.projetilEmMovimento = false;
					distancia -= 9;

					if (fase == 1){
						inimigo[i].vidas = VIDA_INIMIGOS_FASE_1;
					}
					else if (fase == 2){
						inimigo[i].vidas = VIDA_INIMIGOS_FASE_2;
					}
					else if (fase == 3){
						inimigo[i].vidas = VIDA_INIMIGOS_FASE_3;
					}
				}
				iniciarFase = false;
				auxTiroInimigo = 0;
			}

			//// PERSONAGEM
			if (Console::KeyAvailable)
			{
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::A && player.x > 0)
				{
					player.x--;
				}
				else if (tecla.Key == ConsoleKey::D && player.x + 9 < LARGURA_TELA)
				{
					player.x++;
				}
				else if (tecla.Key == ConsoleKey::Spacebar && projetil.balaEmMovimento == false)
				{
					projetil.balaEmMovimento = true;
					projetil.balaNoCano = false;
					projetil.x = player.x + 4;
					projetil.y = player.y - 1;
				}
				else if (tecla.Key == ConsoleKey::Y)
				{
					iniciarFase = true;
				}

				else if (tecla.Key == ConsoleKey::P)
				{
					for (int i = 0; i < QUANTIDADE_INIMIGOS - 1; i++){
						inimigo[i].atirar = true;
						//inimigo[i].aux.auxTiroInimigo == true;
					}
				}
				else if (tecla.Key == ConsoleKey::O){
					//inimigo[enemy.atirador].atirar = false;
					auxTiroInimigo = 0;
				}

				else if (tecla.Key == ConsoleKey::Z){
					fase = 1;
					iniciarFase = true;
				}
				else if (tecla.Key == ConsoleKey::X){
					fase = 2;
					iniciarFase = true;
				}
				else if (tecla.Key == ConsoleKey::C){
					fase = 3;
					iniciarFase = true;
				}
				else if (tecla.Key == ConsoleKey::J){
					player.vivo = false;
				}
			}

			// PROJETILLLLL ///////////////////////////////////
			if (projetil.balaEmMovimento)
			{
				if (clock() > proximaAtualizacaoProjetil){
					projetil.y -= 2;
					proximaAtualizacaoProjetil = clock() + 20;
				}

				if (projetil.y == 0)
				{
					projetil.balaEmMovimento = false;
				}
			}
			else
				projetil.balaNoCano = true;

			//INIMIGO  ////////////////////////////////////////////////////////

			//// MOVIMENTACAO EM X
			for (int x = 0; x < QUANTIDADE_INIMIGOS; x++){

				if (inimigo[x].x + 10 >= LARGURA_TELA){
					inimigo[x].direcaoX = 0;
				}
				if (inimigo[x].x <= 1){
					inimigo[x].direcaoX = 1;
				}
				if (inimigo[x].atirar == false){
					if (clock() > inimigo[x].proximaAtualizacaoInimigoX && inimigo[x].direcaoX == 1){
						inimigo[x].x += 1;
						inimigo[x].proximaAtualizacaoInimigoX = clock() + 30;
					}
					if (clock() > inimigo[x].proximaAtualizacaoInimigoX && inimigo[x].direcaoX == 0){
						inimigo[x].x -= 1;
						inimigo[x].proximaAtualizacaoInimigoX = clock() + 30;
					}
				}

				if (clock() > inimigo[x].tempoProximoMovimento){
					inimigo[x].tempoProximoMovimento = clock() + 750;
					inimigo[x].tempoDecorrido++;

				}
				if (inimigo[x].tempoDecorrido >= ((rand() % 2 + 1) - ((rand() % 6 + 1) / 10.0f))){
					inimigo[x].direcaoX = rand() % 2;
					if (x == 0){
						if (inimigo[x].x < player.x && inimigo[x].direcaoX == 0){
							if (rand() % 10 >= 4)
								inimigo[x].direcaoX = 1;
						}
						else if (inimigo[x].x > player.x && inimigo[x].direcaoX == 1)
						{
							if (rand() % 10 >= 4)
								inimigo[x].direcaoX = 0;
						}
					}
					inimigo[x].tempoDecorrido = 0;
				}

				///////// MOVIMENTACAO EM Y //
				// DIREÇÃO: 0 -> BAIXO       1 -> RETO        2 -> CIMA

				if (inimigo[x].atirar == false){
					if (clock() > inimigo[x].proximaAtualizacaoInimigoY && inimigo[x].direcaoY == 0){ ///BAIXO
						inimigo[x].y++;
						inimigo[x].proximaAtualizacaoInimigoY = clock() + 30;
					}
					if (inimigo[x].direcaoY == 1){  //RETO
						inimigo[x].direcaoY = rand() % 3;
					}
					if (clock() > inimigo[x].proximaAtualizacaoInimigoY && inimigo[x].direcaoY == 2){  ///CIMA
						inimigo[x].y--;
						inimigo[x].proximaAtualizacaoInimigoY = clock() + 30;

					}
				}

				if (inimigo[x].y <= inimigo[x].yInicial - 4){
					inimigo[x].direcaoY = 0;
				}
				else if (inimigo[x].y >= inimigo[x].yInicial + 4){
					inimigo[x].direcaoY = 2;
				}
			}

			// COLISÃO INIMIGO
			if (projetil.balaEmMovimento){
				for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
					if (inimigo[i].inimigoAtivo){
						if (projetil.x >= inimigo[i].x && projetil.x <= inimigo[i].x + 9){
							if (projetil.y <= inimigo[i].y && projetil.y >= inimigo[i].y - 4){
								projetil.balaEmMovimento = false;
								inimigo[i].vidas--;
								pontos += 10;
							}
						}
					}
					if (inimigo[i].vidas <= 0){
						inimigo[i].inimigoAtivo = false;
						inimigo[i].atirar = false;
					}

				}
			}
			//// TIRO DO INIMIGO
			
			
			for (int j = 0; j < QUANTIDADE_INIMIGOS - 1; j++){

				if (inimigo[j].inimigoAtivo){

					if ((inimigo[j].x > (player.x - 5) && inimigo[j].x < (player.x + 10)) 
						&& inimigo[j].aux.auxTiroInimigo == false){

						inimigo[j].aux.auxTiroInimigo = true;
					}
					if (inimigo[j].aux.auxTiroInimigo == true){
						if (clock() > inimigo[j].aux.proximoTiroInimigo){
							inimigo[j].aux.contadorTempoTiro += 0.1;
							inimigo[j].aux.proximoTiroInimigo = clock() + 100;
						}
						if (j == 2){
							if (inimigo[j].aux.contadorTempoTiro >= 1.2f){
								inimigo[j].aux.contadorTempoTiro = 0;
								inimigo[j].atirar = true;
							}
						}
						else{
							if (inimigo[j].aux.contadorTempoTiro >= 0.8f){
								inimigo[j].aux.contadorTempoTiro = 0;
								inimigo[j].atirar = true;
							}
						}
					}
					//
					if (inimigo[j].atirar == true){
						if (clock() > inimigo[j].aux.proximoTiroInimigo){
							inimigo[j].aux.contadorTempoTiro += 0.1;
							inimigo[j].aux.proximoTiroInimigo = clock() + 100;
						}
						if (inimigo[j].aux.contadorTempoTiro >= 0.1f){
							inimigo[j].projetil.projetilEmMovimento = true;
							inimigo[j].aux.contadorTempoTiro = 0;
							inimigo[j].atirar = false;
						}
						inimigo[j].projetil.x = inimigo[j].x + 3;
						inimigo[j].projetil.y = inimigo[j].y + 1;
					}
				}

				if (inimigo[j].projetil.projetilEmMovimento){
					inimigo[j].projetil.y++;
				}
				if (inimigo[j].projetil.projetilEmMovimento == true && inimigo[j].projetil.y >= ALTURA_TELA - 5){
					inimigo[j].projetil.projetilEmMovimento = false;
					inimigo[j].aux.auxTiroInimigo = false;
					
				}

				//// COLISAO DA BALA DO INIMIGO NO PLAYER
				if (inimigo[j].projetil.projetilEmMovimento)
				{
					if ((inimigo[j].projetil.x > player.x || inimigo[j].projetil.x + 4 > player.x)
						&& inimigo[j].projetil.x < player.x + 9)
					{
						if (inimigo[j].projetil.y > player.y - 3 && inimigo[j].projetil.y < player.y)
						{
							inimigo[j].projetil.projetilEmMovimento = false;
							player.vidas--;
						}
					}
				}
				
			}

			/*
			if (inimigo[0].inimigoAtivo){

				if ((inimigo[0].x > (player.x - 5) && inimigo[0].x < (player.x + 10)) && auxTiroInimigo == 0){
					auxTiroInimigo = 1;

				}
				if (auxTiroInimigo == 1){
					if (clock() > proximoTiroInimigo){
						contadorTempoTiro += 0.1;
						proximoTiroInimigo = clock() + 100;
					}
					if (contadorTempoTiro >= 0.7f){
						contadorTempoTiro = 0;
						inimigo[0].atirar = true;
					}
				}
				if (inimigo[0].atirar == true){
					if (clock() > proximoTiroInimigo){
						contadorTempoTiro += 0.1;
						proximoTiroInimigo = clock() + 100;
					}
					if (contadorTempoTiro >= 0.1f){
						inimigo[0].projetil.projetilEmMovimento = true;
						contadorTempoTiro = 0;
						inimigo[0].atirar = false;
					}
					inimigo[0].projetil.x = inimigo[0].x + 3;
					inimigo[0].projetil.y = inimigo[0].y + 1;
				}
			}

			if (inimigo[0].projetil.projetilEmMovimento){
				inimigo[0].projetil.y++;
			}
			if (inimigo[0].projetil.projetilEmMovimento == true && inimigo[0].projetil.y >= ALTURA_TELA - 5){
				inimigo[0].projetil.projetilEmMovimento = false;
				auxTiroInimigo = 0;
				//inimigo[0].atirar = false;
			}

			//// COLISAO DA BALA DO INIMIGO NO PLAYER
			if (inimigo[0].projetil.projetilEmMovimento)
			{
				if ((inimigo[0].projetil.x > player.x || inimigo[0].projetil.x + 4 > player.x)
					&& inimigo[0].projetil.x < player.x + 9)
				{
					if (inimigo[0].projetil.y > player.y - 3 && inimigo[0].projetil.y < player.y)
					{
						inimigo[0].projetil.projetilEmMovimento = false;
						player.vidas--;
					}
				}
			}
			*/ ////////////

			/// GERENCIAMENTO

			if (player.vidas <= 0)
				player.vivo = false;

			for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
				if (i == enemy.atirador){
					if (!inimigo[i].inimigoAtivo && inimigo[i].projetil.projetilEmMovimento == false){
						enemy.atirador++;
						break;
					}
				}
			}
			if (!player.vivo)
				estadoJogo = ESTADO_JOGO_GAMEOVER;

			auxPassarFase = 1;
			for (int i = 0; i < QUANTIDADE_INIMIGOS; i++)
			{
				if (inimigo[i].inimigoAtivo == true)
					auxPassarFase = 0;
			}
			if (auxPassarFase == 1){
				if (fase == 1)
					fase = 2;
				else if (fase == 2)
					fase = 3;
				else if (fase == 3)
					estadoJogo = ESTADO_JOGO_WIN;

				if (fase == 1 || fase == 2 || fase == 3)
					iniciarFase = true;
				player.vidas++;
			}

			//////////////////////////////////////////// DESENHA ////////////////////////////////////////////////////
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
			imprimirPersonagem(player.x, player.y);

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
				Console::SetCursorPosition(player.x + 4, player.y - 2);
				ConsoleHelper::ImprimirASCIIExtended("Û");
				Console::SetCursorPosition(player.x + 4, player.y - 1);
				ConsoleHelper::ImprimirASCIIExtended("Û");
			}

			// DESENHA O INIMIGO


			for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
				if (inimigo[i].inimigoAtivo){

					imprimirInimigo(inimigo[i].x, inimigo[i].y, inimigo[i].direcaoY, fase);

				}
			}
			// PROJETIL DO INIMIGO
			for (int j = 0; j < QUANTIDADE_INIMIGOS - 1; j++)
			{
				if (inimigo[j].projetil.projetilEmMovimento == true){

					Console::ForegroundColor = ConsoleColor::Cyan;
					Console::SetCursorPosition(inimigo[j].projetil.x, inimigo[j].projetil.y);
					if (fase == 1)
						ConsoleHelper::ImprimirASCIIExtended("Û Û");
					if (fase >= 2)
						ConsoleHelper::ImprimirASCIIExtended("Û Û Û");
				}
			}

			Console::SetCursorPosition(10, 10);
			Console::Write("Pontos: " + pontos);

			Console::SetCursorPosition(10, 11);
			Console::Write("Vidas: " + player.vidas);

			//////////// TESTES

			/*for (int i = 0; i < QUANTIDADE_INIMIGOS; i++){
				if (inimigo[i].inimigoAtivo){

					Console::ForegroundColor = ConsoleColor::Cyan;
					Console::SetCursorPosition(inimigo[i].x + 1, inimigo[i].y - 8);
					Console::Write("Vidas: " + inimigo[i].vidas);
				}
			}*/

			Threading::Thread::Sleep(10);
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

		case ESTADO_JOGO_GAMEOVER:
			Console::Clear();
			Console::SetCursorPosition(LARGURA_TELA * 0.5f, 10);
			Console::Write("GameOver");
			Console::SetCursorPosition((LARGURA_TELA * 0.5f) - 20, 30);
			Console::WriteLine("Deseja jogar novamente?: ");
			Console::SetCursorPosition((LARGURA_TELA * 0.5f) + 5, 29);
			Console::WriteLine("S: Sim ");
			Console::SetCursorPosition((LARGURA_TELA * 0.5f) + 5, 31);
			Console::WriteLine("N: Não ");

			tecla = Console::ReadKey(true);
			if (tecla.Key == ConsoleKey::S)
			{
				estadoJogo = ESTADO_JOGO_GAMEPLAY;
				fase = 1;
				player.vidas = 3;
				player.vivo = true;
				iniciarFase = true;
				pontos = 0;
			}
			if (tecla.Key == ConsoleKey::N)
			{
				exit(0);
			}
			break;
		case ESTADO_JOGO_WIN:
			Console::Clear();
			Console::SetCursorPosition(10, 10);
			Console::Write("YOU WIN");
			Console::SetCursorPosition(10, 30);
			Console::WriteLine("Deseja jogar novamente?: ");
			Console::SetCursorPosition(10, 31);
			Console::WriteLine("S: Sim ");
			Console::SetCursorPosition(10, 32);
			Console::WriteLine("N: Não ");

			tecla = Console::ReadKey(true);
			if (tecla.Key == ConsoleKey::S)
			{
				estadoJogo = ESTADO_JOGO_GAMEPLAY;
				fase = 1;
				player.vidas = 3;
				player.vivo = true;
				iniciarFase = true;
				pontos = 0;
			}
			if (tecla.Key == ConsoleKey::N)
			{
				exit(0);
			}
			break;
		}

	}

	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void imprimirInimigo(int posX, int posY, int direcaoY, int fase)
{
	ConsoleColor corPrincipal;
	ConsoleColor corSecundaria;
	if (fase == 1){
		corPrincipal = ConsoleColor::Red;
		corSecundaria = ConsoleColor::Green;
	}
	else if (fase == 2){
		corPrincipal = ConsoleColor::Cyan;
		corSecundaria = ConsoleColor::Blue;
	}
	else if (fase == 3){
		corPrincipal = ConsoleColor::Green;
		corSecundaria = ConsoleColor::Red;
	}
	if (direcaoY == 2){
		Console::ForegroundColor = corSecundaria;
		Console::SetCursorPosition(posX, posY - 3);
		ConsoleHelper::ImprimirASCIIExtended("    Û Û   ");
		Console::ForegroundColor = corPrincipal;
		Console::SetCursorPosition(posX, posY - 2);
		ConsoleHelper::ImprimirASCIIExtended(" ÛÛÛ Û ÛÛÛ");
		//Console::ForegroundColor = ConsoleColor::DarkMagenta;
		Console::SetCursorPosition(posX, posY - 1);
		ConsoleHelper::ImprimirASCIIExtended(" Û ÛÛÛÛÛ Û");
		//Console::ForegroundColor = ConsoleColor::Green;
		Console::SetCursorPosition(posX, posY);
		ConsoleHelper::ImprimirASCIIExtended("Û   Û Û   Û");
	}
	else if (direcaoY == 0)
	{
		Console::ForegroundColor = corSecundaria;
		Console::SetCursorPosition(posX, posY - 4);
		ConsoleHelper::ImprimirASCIIExtended("Û         Û");
		Console::SetCursorPosition(posX, posY - 3);
		ConsoleHelper::ImprimirASCIIExtended(" Û  Û Û  Û");
		Console::ForegroundColor = corPrincipal;
		Console::SetCursorPosition(posX, posY - 2);
		ConsoleHelper::ImprimirASCIIExtended(" ÛÛÛ Û ÛÛÛ");
		Console::SetCursorPosition(posX, posY - 1);
		ConsoleHelper::ImprimirASCIIExtended("   ÛÛÛÛÛ  ");
		Console::SetCursorPosition(posX, posY);
		ConsoleHelper::ImprimirASCIIExtended("    Û Û   ");
	
	}
	
}
void imprimirPersonagem(int posX, int posY)
{
	Console::BackgroundColor = ConsoleColor::Black;
	Console::ForegroundColor = ConsoleColor::Magenta;
	ConsoleHelper::ImprimirASCIIExtended(posX, posY - 2, "   Û Û  ");
	ConsoleHelper::ImprimirASCIIExtended(posX, posY - 1, "  ÛÛ ÛÛ");
	ConsoleHelper::ImprimirASCIIExtended(posX, posY, " ÛÛ   ÛÛ ");
}

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
