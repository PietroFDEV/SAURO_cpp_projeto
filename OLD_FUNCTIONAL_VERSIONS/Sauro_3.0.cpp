#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 100

typedef struct {
    int id;
    char nome[MAX_NOME];
    char senha[MAX_NOME];
} Aluno;

typedef struct {
    int id;
    char nome[MAX_NOME];
    float precoPorMes;
    int numParcelas;
} Curso;

typedef struct {
    int idAluno;
    int idCurso;
} Matricula;

// Function Prototypes
void cadastrar_alunos_cursos(Aluno alunos[], int *num_alunos, Curso cursos[], int *num_cursos);
void exibir_cursos(Curso cursos[], int num_cursos);
void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void salvar_dados_alunos(Aluno alunos[], int num_alunos);
void carregar_dados_alunos(Aluno alunos[], int *num_alunos);
void salvar_dados_cursos(Curso cursos[], int num_cursos);
void carregar_dados_cursos(Curso cursos[], int *num_cursos);
void salvar_dados_matriculas(Matricula matriculas[], int num_matriculas);
void carregar_dados_matriculas(Matricula matriculas[], int *num_matriculas);
int fazer_login(Aluno alunos[], int num_alunos);
void exibir_menu_principal(int tipoUsuario);
void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos);

// Function Definitions
void cadastrar_alunos_cursos(Aluno alunos[], int *num_alunos, Curso cursos[], int *num_cursos) {
    int opcao;
    do {
        printf("\n======= Menu de Cadastro =======\n");
        printf("1. Cadastrar aluno\n");
        printf("2. Cadastrar curso\n");
        printf("3. Voltar ao menu principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                // Cadastrar aluno
                printf("Digite o ID do aluno: ");
                scanf("%d", &alunos[*num_alunos].id);
                getchar(); // Limpar buffer de entrada
                printf("Digite o nome do aluno: ");
                fgets(alunos[*num_alunos].nome, MAX_NOME, stdin);
                alunos[*num_alunos].nome[strcspn(alunos[*num_alunos].nome, "\n")] = 0; // Remover nova linha
                printf("Digite a senha do aluno: ");
                fgets(alunos[*num_alunos].senha, MAX_NOME, stdin);
                alunos[*num_alunos].senha[strcspn(alunos[*num_alunos].senha, "\n")] = 0; // Remover nova linha
                (*num_alunos)++;
                printf("Aluno cadastrado com sucesso!\n");
                salvar_dados_alunos(alunos, *num_alunos);
                break;
            case 2:
                // Cadastrar curso
                printf("Digite o ID do curso: ");
                scanf("%d", &cursos[*num_cursos].id);
                getchar(); // Limpar buffer de entrada
                printf("Digite o nome do curso: ");
                fgets(cursos[*num_cursos].nome, MAX_NOME, stdin);
                cursos[*num_cursos].nome[strcspn(cursos[*num_cursos].nome, "\n")] = 0; // Remover nova linha
                printf("Digite o preço por mês do curso: ");
                scanf("%f", &cursos[*num_cursos].precoPorMes);
                printf("Digite o número de parcelas: ");
                scanf("%d", &cursos[*num_cursos].numParcelas);
                (*num_cursos)++;
                printf("Curso cadastrado com sucesso!\n");
                salvar_dados_cursos(cursos, *num_cursos);
                break;
            case 3:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 3);
}

void exibir_cursos(Curso cursos[], int num_cursos) {
    printf("\n======= Cursos Cadastrados =======\n");
    for (int i = 0; i < num_cursos; i++) {
        printf("ID: %d, Nome: %s, Preço por Mês: %.2f, Parcelas: %d, Valor da Parcela: %.2f\n",
               cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas,
               cursos[i].precoPorMes / cursos[i].numParcelas);
    }
}

void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
    printf("\n======= Matrículas do Aluno =======\n");
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].idAluno == idAluno) {
            for (int j = 0; j < num_cursos; j++) {
                if (cursos[j].id == matriculas[i].idCurso) {
                    printf("Curso: %s, Preço por Mês: %.2f, Parcelas: %d, Valor da Parcela: %.2f\n",
                           cursos[j].nome, cursos[j].precoPorMes, cursos[j].numParcelas,
                           cursos[j].precoPorMes / cursos[j].numParcelas);
                    break;
                }
            }
        }
    }
}

void salvar_dados_alunos(Aluno alunos[], int num_alunos) {
    FILE *file = fopen("alunos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar alunos.\n");
        return;
    }
    for (int i = 0; i < num_alunos; i++) {
        fprintf(file, "%d\n%s\n%s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
    fclose(file);
}

void carregar_dados_alunos(Aluno alunos[], int *num_alunos) {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) {
        printf("Arquivo de alunos não encontrado. Inicializando lista vazia.\n");
        return;
    }
    while (fscanf(file, "%d\n", &alunos[*num_alunos].id) != EOF) {
        fgets(alunos[*num_alunos].nome, MAX_NOME, file);
        alunos[*num_alunos].nome[strcspn(alunos[*num_alunos].nome, "\n")] = 0; // Remover nova linha
        fgets(alunos[*num_alunos].senha, MAX_NOME, file);
        alunos[*num_alunos].senha[strcspn(alunos[*num_alunos].senha, "\n")] = 0; // Remover nova linha
        (*num_alunos)++;
    }
    fclose(file);
}

void salvar_dados_cursos(Curso cursos[], int num_cursos) {
    FILE *file = fopen("cursos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar cursos.\n");
        return;
    }
    for (int i = 0; i < num_cursos; i++) {
        fprintf(file, "%d\n%s\n%.2f\n%d\n", cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas);
    }
    fclose(file);
}

void carregar_dados_cursos(Curso cursos[], int *num_cursos) {
    FILE *file = fopen("cursos.txt", "r");
    if (file == NULL) {
        printf("Arquivo de cursos não encontrado. Inicializando lista vazia.\n");
        return;
    }
    while (fscanf(file, "%d\n", &cursos[*num_cursos].id) != EOF) {
        fgets(cursos[*num_cursos].nome, MAX_NOME, file);
        cursos[*num_cursos].nome[strcspn(cursos[*num_cursos].nome, "\n")] = 0; // Remover nova linha
        fscanf(file, "%f\n", &cursos[*num_cursos].precoPorMes);
        fscanf(file, "%d\n", &cursos[*num_cursos].numParcelas);
        (*num_cursos)++;
    }
    fclose(file);
}

void salvar_dados_matriculas(Matricula matriculas[], int num_matriculas) {
    FILE *file = fopen("matriculas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar matrículas.\n");
        return;
    }
    for (int i = 0; i < num_matriculas; i++) {
        fprintf(file, "%d\n%d\n", matriculas[i].idAluno, matriculas[i].idCurso);
    }
    fclose(file);
}

void carregar_dados_matriculas(Matricula matriculas[], int *num_matriculas) {
    FILE *file = fopen("matriculas.txt", "r");
    if (file == NULL) {
        printf("Arquivo de matrículas não encontrado. Inicializando lista vazia.\n");
        return;
    }
    while (fscanf(file, "%d\n%d\n", &matriculas[*num_matriculas].idAluno, &matriculas[*num_matriculas].idCurso) != EOF) {
        (*num_matriculas)++;
    }
    fclose(file);
}

int fazer_login(Aluno alunos[], int num_alunos) {
    char nome[MAX_NOME];
    char senha[MAX_NOME];
    printf("Digite o nome de usuário: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = 0; // Remover nova linha
    printf("Digite a senha: ");
    fgets(senha, MAX_NOME, stdin);
    senha[strcspn(senha, "\n")] = 0; // Remover nova linha

	if (strcmp("admin", nome) == 0  && strcmp("admin", senha) == 0)
	{
		return -1;
	}
	
    for (int i = 0; i < num_alunos; i++) {
        if (strcmp(alunos[i].nome, nome) == 0 && strcmp(alunos[i].senha, senha) == 0) {
            return alunos[i].id; // Retornar ID do aluno
        }
    }

    return 0;
}

void exibir_menu_principal(int tipoUsuario) {
    printf("\n======= Menu Principal =======\n");
    if (tipoUsuario == -1) {
        printf("1. Cadastrar alunos e cursos\n");
        printf("2. Cadastrar matrícula\n");
        printf("3. Ver alunos cadastrados\n");
        printf("4. Ver cursos cadastrados\n");
        printf("5. Sair\n");
    } else {
        printf("1. Ver cursos disponíveis\n");
        printf("2. Ver matrículas\n");
        printf("3. Cadastrar nova matrícula\n");
        printf("4. Sair\n");
    }
    printf("Escolha uma opção: ");
}

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos) {
    int idCurso;
    printf("Digite o ID do curso: ");
    scanf("%d", &idCurso);
    matriculas[*num_matriculas].idAluno = idAluno;
    matriculas[*num_matriculas].idCurso = idCurso;
    (*num_matriculas)++;
    printf("Matrícula cadastrada com sucesso!\n");
    salvar_dados_matriculas(matriculas, *num_matriculas);
}

int main() {
    Aluno alunos[100]; // Array para armazenar alunos (limite de 100)
    int num_alunos = 0; // Número atual de alunos cadastrados
    Curso cursos[100]; // Array para armazenar cursos (limite de 100)
    int num_cursos = 0; // Número atual de cursos cadastrados
    Matricula matriculas[100]; // Array para armazenar matrículas (limite de 100)
    int num_matriculas = 0; // Número atual de matrículas

    // Carregar dados dos arquivos
    carregar_dados_alunos(alunos, &num_alunos);
    carregar_dados_cursos(cursos, &num_cursos);
    carregar_dados_matriculas(matriculas, &num_matriculas);

    int opcao;
    int idAluno = -1;

    // Loop do login e menu principal
    do {
        idAluno = fazer_login(alunos, num_alunos);

        if (idAluno == -1) {
            // Menu para administrador
            do {
                exibir_menu_principal(-1);
                scanf("%d", &opcao);
                getchar(); // Limpar o buffer de entrada

                switch (opcao) {
                    case 1:
                        // Cadastrar alunos e cursos
                        cadastrar_alunos_cursos(alunos, &num_alunos, cursos, &num_cursos);
                        break;
                    case 2:
                        // Cadastrar matrícula
                        cadastrar_matricula(matriculas, &num_matriculas, -1, cursos, num_cursos);
                        break;
                    case 3:
                        // Ver alunos cadastrados
                        printf("\n======= Alunos Cadastrados =======\n");
                        for (int i = 0; i < num_alunos; i++) {
                            printf("ID: %d, Nome: %s, Senha: %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
                        }
                        break;
                    case 4:
                        // Ver cursos cadastrados
                        exibir_cursos(cursos, num_cursos);
                        break;
                    case 5:
                        printf("Saindo do menu do administrador...\n");
                        break;
                    default:
                        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                }
            } while (opcao != 5);
        } else if(idAluno != 0) {
            // Menu para aluno logado
            do {
                exibir_menu_principal(1);
                scanf("%d", &opcao);
                getchar(); // Limpar o buffer de entrada

                switch (opcao) {
                    case 1:
                        // Ver cursos disponíveis
                        exibir_cursos(cursos, num_cursos);
                        break;
                    case 2:
                        // Ver matrículas
                        exibir_matriculas(idAluno, cursos, num_cursos, matriculas, num_matriculas);
                        break;
                    case 3:
                        // Cadastrar nova matrícula
                        cadastrar_matricula(matriculas, &num_matriculas, idAluno, cursos, num_cursos);
                        break;
                    case 4:
                        printf("Saindo do menu do aluno...\n");
                        break;
                    default:
                        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                }
            } while (opcao != 4);
        }
        else {
        	printf("Usuário inválido. Por favor, Tente novamente.\n");
		}
    } while (1);

    return 0;
}

