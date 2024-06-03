//#########################################
//# Autor: Pietro
//# Projeto SAURO
//#########################################

#include <stdio.h>
#include <stdlib.h> // utilizar o CLS para limpar o console
#include <string.h> // utlizaar fgets e getchar

// Caso queira deixar o maximo de contatos, alterar por aqui, fica mais tranquilo
#define MAX_CONTATOS 5
#define MAX_NOME 50
#define MAX_TELEFONE 20
#define MAX_SENHA 20

struct CONTATO {
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
};

struct CONTATO {
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
};

struct CONTATO {
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
};

struct USUARIO {
	char login[MAX_NOME];
	char senha[MAX_SENHA];
};

// função logar, em vez de deixar isso setado no MAIN()
void exibir_menu() {
	char login[100];
	char senha[100];
    printf("\n");
    printf("Informe o Usuario\n");
    scanf("%s", login);
    
    if()
}

// função para aparecer as opções do menu, em vez de deixar isso setado no MAIN()
void exibir_menu() {
    printf("\n");
    printf("1. Adicionar contato\n");
    printf("2. Buscar contato\n");
    printf("3. Editar contato\n");
    printf("4. Deletar contato\n");
    printf("5. Mostrar todos os contatos\n");
    printf("6. Sair\n");
    printf("\nEscolha uma opcao: ");
}

// adiciona um ccontato caso N seja menos que MAX_CONTATOS
int adicionar_contato(struct CONTATO lista[], int n) {
    if (n >= MAX_CONTATOS) {
    	system("CLS");
        printf("Lista de contatos cheia.\n");
        return n;
    }
    printf("Digite o nome: ");
    fgets(lista[n].nome, MAX_NOME, stdin);
    
    printf("Digite o telefone: ");
    fgets(lista[n].telefone, MAX_TELEFONE, stdin);
    
    n++;
    system("CLS");
    printf("Contato adicionado com sucesso.\n");
    return n;
}

// Busca o contato trazendo seu nome e telefone, buscnando pelo seu nome
void buscar_contato(struct CONTATO lista[], int n) {
    char nome[MAX_NOME];
    printf("Digite o nome a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
        	system("CLS");
            printf("Contato encontrado:\n");
            printf("Nome: %s", lista[i].nome);
            printf("Telefone: %s", lista[i].telefone);
            return;
        }
    }
    system("CLS");
    printf("Contato nao encontrado.\n");
}

// Busca todos os contatos trazendo seu nome e telefone
void buscar_todos(struct CONTATO lista[], int n) {
	if(n > 0){
	    for (int i = 0; i < n; i++) {
	        printf("Contato %i encontrado:\n", (i + 1));
	        printf("Nome: %s", lista[i].nome);
	        printf("Telefone: %s\n\n", lista[i].telefone);
	    }
	    return;
    }
    system("CLS");
    printf("Nenhum contato encontrado.\n");
}

// Edita um contato buscando ele pelo seu nome
int editar_contato(struct CONTATO lista[], int n) {
    char nome[MAX_NOME];
    printf("Digite o nome a editar: ");
    fgets(nome, MAX_NOME, stdin);
    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
            printf("Digite o novo nome: ");
            fgets(lista[i].nome, MAX_NOME, stdin);
            printf("Digite o novo telefone: ");
            fgets(lista[i].telefone, MAX_TELEFONE, stdin);
            system("CLS");
            printf("Contato editado com sucesso.\n");
            return 1;
        }
    }
    system("CLS");
    printf("Contato nao encontrado.\n");
    return 0;
}

// Deleta o contato buscando ele por seu nome
int deletar_contato(struct CONTATO lista[], int n) {
    char nome[MAX_NOME];
    printf("Digite o nome a deletar: ");
    fgets(nome, MAX_NOME, stdin);
    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
            for (int j = i; j < n - 1; j++) {
                strcpy(lista[j].nome, lista[j+1].nome);
                strcpy(lista[j].telefone, lista[j+1].telefone);
            }
            n--;
            system("CLS");
            printf("Contato deletado com sucesso.\n");
            return n;
        }
    }
    system("CLS");
    printf("Contato nao encontrado.\n");
    return n;
}

// rotina rodada em loop sempre que inicia, até que o usuário escolha o numero 5 nas opções
int main() {
    CONTATO lista[MAX_CONTATOS];
    int quantidadeContatos = 0;
    int opcao;
    
    
    // LOGIN
    do {
    	
	}while ()
    
    
	// MENU para ADMIN
    do {
        exibir_menu();
        scanf("%d", &opcao);
        getchar();
		system("CLS");
        switch (opcao) {
            case 1:
                quantidadeContatos = adicionar_contato(lista, quantidadeContatos);
                break;
            case 2:
                buscar_contato(lista, quantidadeContatos);
                break;
            case 3:
                editar_contato(lista, quantidadeContatos);
                break;
            case 4:
                quantidadeContatos = deletar_contato(lista, quantidadeContatos);
                break;
            case 5:
            	buscar_todos(lista, quantidadeContatos);
            	break;
            case 6:
            	system("CLS");
                printf("Saindo...\n");
                break;
            default:
            	system("CLS");
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 6);

    return 0;
}
