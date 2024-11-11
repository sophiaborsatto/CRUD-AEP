#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#define users_maximos 50
//--------------------------------------------------------------------------------------------------------------------
struct usuario {
    char nome[50];
    char senha[50];
};
//--------------------------------------------------------------------------------------------------------------------
int i;
//--------------------------------------------------------------------------------------------------------------------
void linhaCol(int lin, int col) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col-1, lin-1});
}
//--------------------------------------------------------------------------------------------------------------------
enum {
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};
//--------------------------------------------------------------------------------------------------------------------
enum {
	_BLACK=0, _BLUE=16, _GREEN=32, _CYAN=48, _RED=64, _MAGENTA=80, _BROWN=96, _LIGHTGRAY=112, _DARKGRAY=128, 
	_LIGHTBLUE=144, _LIGHTGREEN=160, _LIGHTCYAN=176, _LIGHTRED=192, _LIGHTMAGENTA=208, _YELLOW=224, _WHITE=240
};
//--------------------------------------------------------------------------------------------------------------------
void textColor(int letras, int fundo) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letras + fundo);
}
//--------------------------------------------------------------------------------------------------------------------
void box(int lin1, int col1, int lin2, int col2) { // caixa
    int i, j;
    linhaCol(lin1, col1);
    printf(" ");
    for (i = col1 + 1; i < col2; i++) {
        printf("=");
    }
    printf(" ");

    for (i = lin1 + 1; i < lin2; i++) {
        linhaCol(i, col1);
        printf("'");
        for (j = col1 + 1; j < col2; j++) {
            linhaCol(i, j);
            printf(" ");
        }
        linhaCol(i, col2);
        printf("'");
    }

    linhaCol(lin2, col1);
    printf(" ");
    for (i = col1 + 1; i < col2; i++) {
        printf("=");
    }
    printf(" ");
}
//--------------------------------------------------------------------------------------------------------------------
struct usuario u[users_maximos];
//--------------------------------------------------------------------------------------------------------------------
int total_usuarios = 0;
//--------------------------------------------------------------------------------------------------------------------
void cifra_cesar(char *senha) { // CRIPTOGRAFIA: numeros de 0 - 9 e letras de A - Z e a - z (+13 de deslocamento de caracteres)
    for (i = 0; senha[i] != '\0'; i++) {
        if (senha[i] >= 'A' && senha[i] <= 'Z') {
            senha[i] = ((senha[i] - 'A' + 13) % 26) + 'A';
        } else if (senha[i] >= 'a' && senha[i] <= 'z') {
            senha[i] = ((senha[i] - 'a' + 13) % 26) + 'a';
        } else if (senha[i] >= '0' && senha[i] <= '9') {
            senha[i] = ((senha[i] - '0' + 13) % 10) + '0';
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
void salvar_user() {
    FILE *file = fopen("usuarios.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar!\n");
        return;
    }
    for (i = 0; i < total_usuarios; i++) {
        fprintf(file, "%s %s\n", u[i].nome, u[i].senha);
    }
    fclose(file);
}
//--------------------------------------------------------------------------------------------------------------------
void carregar_user() {
    FILE *file = fopen("usuarios.txt", "r");
    if (file == NULL) {
        return;
    }
    total_usuarios = 0;
    while (fscanf(file, "%s %s", u[total_usuarios].nome, u[total_usuarios].senha) != EOF) {
        total_usuarios++;
    }
    fclose(file);
}
//--------------------------------------------------------------------------------------------------------------------
int selecionar_user() { // Exibir e escolher um usuario pela lista
    int selecionado = 0;
    int tecla;

    while (1) {
        system("cls");
        textColor(WHITE, _BLUE);

        box(5, 20, total_usuarios + 9, 60);

        linhaCol(6, 22);
        printf("Selecione um usuario usando as setas:");
        
        for (i = 0; i < total_usuarios; i++) { // Exibe os usuarios e destaca o selecionado
            linhaCol(8 + i, 25);
            if (i == selecionado) {
                textColor(BLUE, _CYAN); 
            } else {
                textColor(WHITE, _BLUE);
            }
            printf(" %s", u[i].nome);
            textColor(WHITE, _BLACK);
        }

        tecla = getch(); // Mexer com as setas
        if (tecla == 224) {
            tecla = getch();
            if (tecla == 72) {  // Seta para cima
                if (selecionado <= 0) {
                    selecionado = total_usuarios - 1; // Do primeiro usuario vai pro ultimo
                } else {
                    selecionado--;
                }
            } else if (tecla == 80) {  // Seta para baixo
                if (selecionado >= total_usuarios - 1) { // Do ultimo usuario vai pro primeiro
                    selecionado = 0;
                } else {
                    selecionado++;
                }
            }
        } else if (tecla == 13) {  // Enter para confirmar
            return selecionado;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
void cadastrar() {
    system("cls");
    textColor(WHITE, _BLUE);
    box(10, 20, 18, 60);
    linhaCol(11, 22);
    printf("         CADASTRO DE USUARIOS\n");
    
    linhaCol(20, 1);
    textColor(WHITE, _BLACK);
    printf("Caso deseje cancelar o cadastro: pressione \"ESC\", caso deseje continuar: pressione qualquer outra tecla.");
    if (getch() == 27) {
    	linhaCol(21, 1);
    	printf("\nCadastro cancelado com sucesso! Pressione qualquer tecla para voltar ao menu.");
    	getch();
		return;
    }
    linhaCol(20, 1);
    for (i = 0; i < 120; i++) {
        printf(" ");
    }
    
    textColor(WHITE, _BLUE);
    linhaCol(13, 22);
    printf("Digite o nome do usuário: ");
    scanf("%49s", u[total_usuarios].nome);

    linhaCol(14, 22);
    printf("Digite a senha: ");
    scanf("%49s", u[total_usuarios].senha);
    
    linhaCol(16, 22);
    printf("Usuário cadastrado com sucesso!\n");

    cifra_cesar(u[total_usuarios].senha);
    total_usuarios++;
    salvar_user();

	textColor(WHITE, _BLACK);
	linhaCol(20, 1);
    printf("\nPressione qualquer tecla para voltar ao menu.");
    getch();
}
//--------------------------------------------------------------------------------------------------------------------
void listar() {
    system("cls");
    textColor(WHITE, _BLUE);
    box(5, 20, total_usuarios + 9, 60);
    linhaCol(6, 22);
    printf("        USUÁRIOS CADASTRADOS:");
    
    for (i = 0; i < total_usuarios; i++) {
        linhaCol(8 + i, 25);
        textColor(WHITE, _BLUE);
        printf("%i - %s", i + 1, u[i].nome);
    }
    textColor(WHITE, _BLACK);
    linhaCol(total_usuarios + 9, 1);
    printf("\n\nPressione qualquer tecla para voltar ao menu.");
    getch();
}
//--------------------------------------------------------------------------------------------------------------------
void atualizar() {
    int usuarioSelecionado = selecionar_user();
    char senhaAtual[50];
    
    system("cls");
    textColor(WHITE, _BLUE);
    box(5, 20, 12, strlen(u[usuarioSelecionado].nome) + 56);

    linhaCol(6, 22);
    printf("     Atualização de Usuário: %s", u[usuarioSelecionado].nome);
    
    linhaCol(8, 22);
    printf("Digite a senha atual: ");
    scanf("%49s", senhaAtual);
    cifra_cesar(senhaAtual);

    if (strcmp(u[usuarioSelecionado].senha, senhaAtual) == 0) {
        linhaCol(9, 22);
        printf("Digite a nova senha: ");
        scanf("%49s", u[usuarioSelecionado].senha);
        cifra_cesar(u[usuarioSelecionado].senha);
        salvar_user();

        linhaCol(11, 22);
        printf("Usuário atualizado com sucesso!");
    } else {
        linhaCol(10, 22);
        printf("Senha incorreta! Tente novamente");
    }

    linhaCol(12, 1);
    textColor(WHITE, _BLACK);
    printf("\n\nPressione qualquer tecla para voltar ao menu.");
    getch();
}
//--------------------------------------------------------------------------------------------------------------------
void deletar() {
    int usuarioSelecionado = selecionar_user();
    char senhaDel[50];

    system("cls");
    textColor(WHITE, _BLUE);
    box(5, 20, 12, strlen(u[usuarioSelecionado].nome) + 56);

    linhaCol(6, 22);
    printf("     Deletar Usuário: %s", u[usuarioSelecionado].nome);

    linhaCol(8, 22);
    printf("Digite a senha: ");
    scanf("%49s", senhaDel);
    cifra_cesar(senhaDel);

    if (strcmp(u[usuarioSelecionado].senha, senhaDel) == 0) {
        for (i = usuarioSelecionado; i < total_usuarios - 1; i++) {
            u[i] = u[i + 1];
        }
        total_usuarios--;
        salvar_user();

        linhaCol(10, 22);
        printf("Usuário deletado com sucesso!");
    } else {
        linhaCol(10, 22);
        printf("Senha incorreta! Tente novamente.");
    }

    linhaCol(12, 1);
    textColor(WHITE, _BLACK);
    printf("\n\nPressione qualquer tecla para voltar ao menu.");
    getch();
}
//--------------------------------------------------------------------------------------------------------------------
void menu() {
    int opcao = 1;
    int tecla;

    textColor(WHITE, _BLACK);
    system("cls"); 

    textColor(WHITE, _BLUE);
    box(10, 20, 20, 60);

    linhaCol(11, 31);
    printf("SEJA BEM-VINDO AO CRUD! ");    
    linhaCol(13, 22);
    printf(" Escolha uma das opção abaixo:");

    while (1) {
        for (i = 1; i <= 5; i++) {
            linhaCol(14 + i, 27); 

            if (i == opcao) {
                textColor(BLUE, _CYAN); 
            } else {
                textColor(WHITE, _BLUE); 
            }

            switch (i) {
                case 1: printf("     Cadastrar Usuário       "); break;
                case 2: printf("     Lista de Usuários       "); break;
                case 3: printf("     Atualizar Usuário       "); break;
                case 4: printf("     Deletar Usuário         "); break;
                case 5: printf("     Sair                    "); break;
            }
        }

        tecla = getch();

        if (tecla == 224) { 
            tecla = getch();
            if (tecla == 72) {
            	if (opcao <= 1) {
            		opcao = 6;
				}
                opcao--;
            } else if (tecla == 80) {
            	if (opcao >= 5) {
            		opcao = 0;
				}
                opcao++;
            }
        } else if (tecla == 13) { 
            break;
        }
    }

    textColor(WHITE, _BLACK);
    switch (opcao) {
        case 1: cadastrar(); break;
        case 2: listar(); break;
        case 3: atualizar(); break;
        case 4: deletar(); break;
        case 5: 
            linhaCol(23, 22);
            textColor(WHITE, _BLACK);
            return;
    }

    menu();
}
//--------------------------------------------------------------------------------------------------------------------
int main() {
    setlocale(LC_ALL, "portuguese");
    carregar_user();
    menu();
    return 0;
}
