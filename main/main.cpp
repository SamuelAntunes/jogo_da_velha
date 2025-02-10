#include <iostream>
#include <cstdlib>
#include <string>
#include <limits>
#include <ctime>   // Para srand() e time()

#ifdef _WIN32
    #include <windows.h>   // Para Sleep()
#else
    #include <unistd.h>    // Para usleep()
#endif

using namespace std;

// ===================
// Códigos de cores ANSI
// ===================
#ifndef _WIN32
    #define RESET   "\033[0m"
    #define RED     "\033[31m"
    #define GREEN   "\033[32m"
    #define YELLOW  "\033[33m"
    #define BLUE    "\033[34m"
    #define MAGENTA "\033[35m"
    #define CYAN    "\033[36m"
#else
    // Se o terminal do Windows não suportar cores ANSI, pode deixá-las vazias.
    #define RESET   ""
    #define RED     ""
    #define GREEN   ""
    #define YELLOW  ""
    #define BLUE    ""
    #define MAGENTA ""
    #define CYAN    ""
#endif

// ===================
// Funções Auxiliares
// ===================

// Limpa a tela (compatível com Windows e Unix-like)
void clearScreen() {
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}

// Aguarda que o usuário pressione ENTER
void waitForEnter() {
    cout << "\nPressione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Pausa a execução por um determinado número de milissegundos.
// Em Windows, usamos Sleep(ms); em Unix-like, usleep(ms * 1000);
void delay(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000); // usleep espera microsegundos
    #endif
}

// Exibe uma splash screen animada
void splashScreen() {
    clearScreen();
    cout << "\n\n";
    cout << "      #####################################################\n";
    cout << "      #                                                   #\n";
    cout << "      #             BEM VINDO AO JOGO DA VELHA            #\n";
    cout << "      #                                                   #\n";
    cout << "      #####################################################\n\n";
    delay(2000); // Pausa de 2 segundos
}

// Exibe um cabeçalho para as telas do jogo
void printHeader() {
    cout << "=====================================================\n";
    cout << "               JOGO DA VELHA - BATALHA               \n";
    cout << "=====================================================\n\n";
}

// ===================
// Funções do Jogo
// ===================

// Inicializa o tabuleiro com o caractere '-' para indicar posição vazia
void initializeBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            board[i][j] = '-';
        }
    }
}

// Exibe o tabuleiro com uma grade ASCII aprimorada e cores
void printBoard(char board[3][3]) {
    cout << "\n";
    for (int i = 0; i < 3; i++){
        cout << "      ";
        for (int j = 0; j < 3; j++){
            char cell = board[i][j];
            if(cell == 'X')
                cout << RED << cell << RESET;
            else if(cell == 'O')
                cout << GREEN << cell << RESET;
            else
                cout << YELLOW << cell << RESET;
            
            if(j < 2)
                cout << " | ";
        }
        cout << "\n";
        if(i < 2)
            cout << "     ---+---+---\n";
    }
}

// Exibe um mapa de posições para auxiliar os jogadores
void printPositionMap() {
    cout << "\n      MAPA DE POSICOES:\n";
    cout << "      7 | 8 | 9 \n";
    cout << "     ---+---+---\n";
    cout << "      4 | 5 | 6 \n";
    cout << "     ---+---+---\n";
    cout << "      1 | 2 | 3 \n";
}

// Verifica se há vencedor no tabuleiro.
// Retorna 1 se 'X' venceu, 2 se 'O' venceu, ou 0 se não houver vencedor.
int checkWinner(char board[3][3]) {
    // Verifica linhas
    for (int i = 0; i < 3; i++){
        if(board[i][0] != '-' && board[i][0] == board[i][1] && board[i][1] == board[i][2]){
            return (board[i][0] == 'X') ? 1 : 2;
        }
    }
    // Verifica colunas
    for (int j = 0; j < 3; j++){
        if(board[0][j] != '-' && board[0][j] == board[1][j] && board[1][j] == board[2][j]){
            return (board[0][j] == 'X') ? 1 : 2;
        }
    }
    // Verifica diagonais
    if(board[0][0] != '-' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return (board[0][0] == 'X') ? 1 : 2;
    if(board[0][2] != '-' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return (board[0][2] == 'X') ? 1 : 2;
    
    return 0;
}

// Exibe um banner de vitória com uma aparência chamativa
void printVictoryBanner(const string &player, char symbol) {
    cout << "\n****************************************\n";
    cout << "*                                      *\n";
    cout << "*          VENCEDOR: " << player << " (" << symbol << ")          *\n";
    cout << "*                                      *\n";
    cout << "****************************************\n";
}

// ===================
// Inteligência Artificial (Modo Solo)
// ===================

// Função auxiliar: testa se uma posição vazia pode levar à vitória para um dado símbolo
bool testMove(char board[3][3], int row, int col, char symbol) {
    board[row][col] = symbol;
    int res = checkWinner(board);
    board[row][col] = '-';
    if (symbol == 'O' && res == 2) return true;
    if (symbol == 'X' && res == 1) return true;
    return false;
}

// Função que retorna a posição (1 a 9) escolhida pela máquina
int getComputerMove(char board[3][3], int posMapping[9][2]) {
    // 1. Tenta encontrar um movimento vencedor (jogar com 'O')
    for (int pos = 1; pos <= 9; pos++) {
        int row = posMapping[pos - 1][0];
        int col = posMapping[pos - 1][1];
        if(board[row][col] == '-' && testMove(board, row, col, 'O')) {
            return pos;
        }
    }
    // 2. Tenta bloquear a vitória do jogador (jogar com 'X')
    for (int pos = 1; pos <= 9; pos++) {
        int row = posMapping[pos - 1][0];
        int col = posMapping[pos - 1][1];
        if(board[row][col] == '-' && testMove(board, row, col, 'X')) {
            return pos;
        }
    }
    // 3. Se o centro estiver livre (posição 5 -> índice 4)
    if(board[1][1] == '-') return 5;
    // 4. Escolhe um dos cantos, se disponível (posições 1, 3, 7, 9)
    int corners[4] = {1, 3, 7, 9};
    for (int i = 0; i < 4; i++){
        int pos = corners[i];
        int row = posMapping[pos - 1][0];
        int col = posMapping[pos - 1][1];
        if(board[row][col] == '-') return pos;
    }
    // 5. Caso contrário, escolhe aleatoriamente dentre as posições vazias
    int available[9], count = 0;
    for (int pos = 1; pos <= 9; pos++){
        int row = posMapping[pos - 1][0];
        int col = posMapping[pos - 1][1];
        if(board[row][col] == '-') {
            available[count++] = pos;
        }
    }
    if(count > 0) {
        int index = rand() % count;
        return available[index];
    }
    return 0;
}

// ===================
// Modo Multiplayer (dois jogadores)
// ===================

void playGame(const string &player1, const string &player2, int &score1, int &score2) {
    char board[3][3];
    initializeBoard(board);
    int turn = 1;       // 1 para player1 (X), 2 para player2 (O)
    int moves = 0;
    int winner = 0;
    
    // Mapeamento das posições (1 a 9) para as coordenadas da matriz:
    // 1 -> (2,0), 2 -> (2,1), 3 -> (2,2),
    // 4 -> (1,0), 5 -> (1,1), 6 -> (1,2),
    // 7 -> (0,0), 8 -> (0,1), 9 -> (0,2)
    int posMapping[9][2] = {
        {2, 0}, {2, 1}, {2, 2},
        {1, 0}, {1, 1}, {1, 2},
        {0, 0}, {0, 1}, {0, 2}
    };
    
    while(moves < 9 && winner == 0) {
        clearScreen();
        printHeader();
        cout << "Pontuacao: " << BLUE << player1 << " (X): " << score1 << RESET 
             << "  vs  " << MAGENTA << player2 << " (O): " << score2 << RESET << "\n";
        cout << "\nJogada " << moves + 1 << "\n";
        printBoard(board);
        printPositionMap();
        
        string currentPlayer = (turn == 1) ? player1 : player2;
        char symbol = (turn == 1) ? 'X' : 'O';
        
        int pos;
        bool valid = false;
        while(!valid) {
            cout << "\n" << currentPlayer << " (" << ((symbol == 'X') ? RED : GREEN) << symbol << RESET 
                 << "), escolha uma posicao (1-9): ";
            cin >> pos;
            
            if(cin.fail() || pos < 1 || pos > 9) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Tente novamente.\n";
            } else {
                int row = posMapping[pos - 1][0];
                int col = posMapping[pos - 1][1];
                if(board[row][col] != '-') {
                    cout << "Posicao ja ocupada. Escolha outra.\n";
                } else {
                    board[row][col] = symbol;
                    valid = true;
                }
            }
        }
        
        moves++;
        winner = checkWinner(board);
        turn = (turn == 1) ? 2 : 1;
    }
    
    clearScreen();
    printHeader();
    printBoard(board);
    if(winner == 1) {
        cout << "\n" << player1 << " (X) venceu a partida!\n";
        score1++;
        printVictoryBanner(player1, 'X');
    } else if(winner == 2) {
        cout << "\n" << player2 << " (O) venceu a partida!\n";
        score2++;
        printVictoryBanner(player2, 'O');
    } else {
        cout << "\nEmpate!\n";
    }
    
    cout << "\nPontuacao Atual: " << BLUE << player1 << " (X): " << score1 << RESET 
         << "  vs  " << MAGENTA << player2 << " (O): " << score2 << RESET << "\n";
    waitForEnter();
}

// ===================
// Modo Solo (jogador vs. máquina)
// ===================

void playGameSolo(const string &player, int &playerScore, int &machineScore) {
    char board[3][3];
    initializeBoard(board);
    int turn = 1;       // 1 para o jogador (X), 2 para a máquina (O)
    int moves = 0;
    int winner = 0;
    
    // Mapeamento das posições (1 a 9) para as coordenadas da matriz:
    int posMapping[9][2] = {
        {2, 0}, {2, 1}, {2, 2},
        {1, 0}, {1, 1}, {1, 2},
        {0, 0}, {0, 1}, {0, 2}
    };
    
    while(moves < 9 && winner == 0) {
        clearScreen();
        printHeader();
        cout << "Pontuacao: " << BLUE << player << " (X): " << playerScore << RESET 
             << "  vs  " << MAGENTA << "Maquina" << " (O): " << machineScore << RESET << "\n";
        cout << "\nJogada " << moves + 1 << "\n";
        printBoard(board);
        printPositionMap();
        
        if(turn == 1) {
            // Turno do jogador
            int pos;
            bool valid = false;
            while(!valid) {
                cout << "\n" << player << " (X), escolha uma posicao (1-9): ";
                cin >> pos;
                if(cin.fail() || pos < 1 || pos > 9) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Entrada invalida. Tente novamente.\n";
                } else {
                    int row = posMapping[pos - 1][0];
                    int col = posMapping[pos - 1][1];
                    if(board[row][col] != '-') {
                        cout << "Posicao ja ocupada. Escolha outra.\n";
                    } else {
                        board[row][col] = 'X';
                        valid = true;
                    }
                }
            }
        } else {
            // Turno da máquina
            cout << "\nA maquina esta jogando...\n";
            delay(1000); // Aguarda 1 segundo para simular "pensamento"
            int compPos = getComputerMove(board, posMapping);
            int row = posMapping[compPos - 1][0];
            int col = posMapping[compPos - 1][1];
            board[row][col] = 'O';
            cout << "\nA maquina escolheu a posicao " << compPos << ".\n";
            delay(1000);
        }
        
        moves++;
        winner = checkWinner(board);
        turn = (turn == 1) ? 2 : 1;
    }
    
    clearScreen();
    printHeader();
    printBoard(board);
    if(winner == 1) {
        cout << "\n" << player << " (X) venceu a partida!\n";
        playerScore++;
        printVictoryBanner(player, 'X');
    } else if(winner == 2) {
        cout << "\nA maquina (O) venceu a partida!\n";
        machineScore++;
        printVictoryBanner("Maquina", 'O');
    } else {
        cout << "\nEmpate!\n";
    }
    
    cout << "\nPontuacao Atual: " << BLUE << player << " (X): " << playerScore << RESET 
         << "  vs  " << MAGENTA << "Maquina" << " (O): " << machineScore << RESET << "\n";
    waitForEnter();
}

// ===================
// Menu Principal
// ===================

void mainMenu() {
    int choice;
    while(true) {
        clearScreen();
        splashScreen();
        printHeader();
        cout << "1 - Jogar Multiplayer\n";
        cout << "2 - Jogar Solo (vs. Maquina)\n";
        cout << "3 - Sobre o jogo\n";
        cout << "4 - Sair\n";
        cout << "\nEscolha uma opcao: ";
        cin >> choice;
        
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }
        
        if(choice == 1) {
            string player1, player2;
            cout << "\nDigite o nome do Jogador 1 (X): ";
            cin >> player1;
            cout << "Digite o nome do Jogador 2 (O): ";
            cin >> player2;
            int score1 = 0, score2 = 0;
            char playAgain;
            do {
                playGame(player1, player2, score1, score2);
                cout << "\nDeseja jogar novamente? (s/n): ";
                cin >> playAgain;
            } while(playAgain == 's' || playAgain == 'S');
        } else if(choice == 2) {
            string player;
            cout << "\nDigite o seu nome (Voce sera o X): ";
            cin >> player;
            int playerScore = 0, machineScore = 0;
            char playAgain;
            do {
                playGameSolo(player, playerScore, machineScore);
                cout << "\nDeseja jogar novamente? (s/n): ";
                cin >> playAgain;
            } while(playAgain == 's' || playAgain == 'S');
        } else if(choice == 3) {
            clearScreen();
            printHeader();
            cout << "Jogo da Velha desenvolvido em C++.\n";
            cout << "Versao com modos Multiplayer e Solo (vs. Maquina).\n";
            waitForEnter();
        } else if(choice == 4) {
            cout << "\nObrigado por jogar! Ate mais.\n";
            break;
        } else {
            cout << "\nOpcao invalida. Tente novamente.\n";
            waitForEnter();
        }
    }
}

int main() {
    // Inicializa a semente para números aleatórios
    srand(static_cast<unsigned int>(time(0)));
    mainMenu();
    return 0;
}
