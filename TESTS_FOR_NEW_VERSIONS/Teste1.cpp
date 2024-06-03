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
    int id;
    int idAluno;
    int idCurso;
} Matricula;

int obter_proximo_id(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void crud_alunos(Aluno alunos[], int *num_alunos);
void crud_cursos(Curso cursos[], int *num_cursos);
void crud_matriculas(Matricula matriculas[], int *num_matriculas, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos);
void exibir_alunos(Aluno alunos[], int num_alunos);
void exibir_cursos(Curso cursos[], int num_cursos);
void exibir_matriculas(Matricula matriculas[], int num_matriculas, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos);
void salvar_dados_alunos(Aluno alunos[], int num_alunos);
void carregar_dados_alunos(Aluno alunos[], int *num_alunos);
void salvar_dados_cursos(Curso cursos[], int num_cursos);
void carregar_dados_cursos(Curso cursos[], int *num_cursos);
void salvar_dados_matriculas(Matricula matriculas[], int num_matriculas);
void carregar_dados_matriculas(Matricula matriculas[], int *num_matriculas);
int fazer_login(Aluno alunos[], int num_alunos);
void exibir_menu_principal(int tipoUsuario);
void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos);
void atualizar_matricula(Matricula matriculas[], int num_matriculas, Curso cursos[], int num_cursos);
void excluir_matricula(Matricula matriculas[], int *num_matriculas);

int obter_proximo_id(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
    int max_id = 0;
    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].id > max_id) {
            max_id = alunos[i].id;
        }
    }
    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].id > max_id) {
            max_id = cursos[i].id;
        }
    }
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id > max_id) {
            max_id = matriculas[i].id;
        }
    }
    return max_id + 1;
}

void crud_alunos(Aluno alunos[], int *num_alunos) {
    int opcao;
    do {
        printf("\n======= CRUD Alunos =======\n");
        printf("1. Ver alunos\n");
        printf("2. Adicionar aluno\n");
        printf("3. Atualizar aluno\n");
        printf("4. Excluir aluno\n");
        printf("5. Voltar ao menu principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                // Ver alunos
                exibir_alunos(alunos, *num_alunos);
                break;
            case 2:
                // Adicionar aluno
                alunos[*num_alunos].id = obter_proximo_id(alunos, *num_alunos, NULL, 0, NULL, 0);
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
            case 3:
                // Atualizar aluno
                exibir_alunos(alunos, *num_alunos);
                int id;
                printf("Digite o ID do aluno a ser atualizado: ");
                scanf("%d", &id);
                getchar(); // Limpar buffer de entrada
                for (int i = 0; i < *num_alunos; i++) {
                    if (alunos[i].id == id) {
                        printf("Digite o novo nome do aluno: ");
                        fgets(alunos[i].nome, MAX_NOME, stdin);
                        alunos[i].nome[strcspn(alunos[i].nome, "\n")] = 0; // Remover nova linha
                        printf("Digite a nova senha do aluno: ");
                        fgets(alunos[i].senha, MAX_NOME, stdin);
                        alunos[i].senha[strcspn(alunos[i].senha, "\n")] = 0; // Remover nova linha
                        printf("Aluno atualizado com sucesso!\n");
                        salvar_dados_alunos(alunos, *num_alunos);
                        break;
                    }
                }
                break;
            case 4:
                // Excluir aluno
                exibir_alunos(alunos, *num_alunos);
                printf("Digite o ID do aluno a ser excluído: ");
                scanf("%d", &id);
                getchar(); // Limpar buffer de entrada
                for (int i = 0; i < *num_alunos; i++) {
                    if (alunos[i].id == id) {
                        for (int j = i; j < *num_alunos - 1; j++) {
                            alunos[j] = alunos[j + 1];
                        }
                        (*num_alunos)--;
                        printf("Aluno excluído com sucesso!\n");
                        salvar_dados_alunos(alunos, *num_alunos);
                        break;
                    }
                }
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 5);
}

void crud_cursos(Curso cursos[], int *num_cursos) {
    int opcao;
    do {
        printf("\n======= CRUD Cursos =======\n");
        printf("1. Ver cursos\n");
        printf("2. Adicionar curso\n");
        printf("3. Atualizar curso\n");
        printf("4. Excluir curso\n");
        printf("5. Voltar ao menu principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                // Ver cursos
                exibir_cursos(cursos, *num_cursos);
                break;
            case 2:
                // Adicionar curso
                cursos[*num_cursos].id = obter_proximo_id(NULL, 0, cursos, *num_cursos, NULL, 0);
                printf("Digite o nome do curso: ");
                fgets(cursos[*num_cursos].nome, MAX_NOME, stdin);
                cursos[*num_cursos].nome[strcspn(cursos[*num_cursos].nome, "\n")] = 0; // Remover nova linha
                printf("Digite o preço por mês do curso: ");
                scanf("%f", &cursos[*num_cursos].precoPorMes);
                printf("Digite o número de parcelas: ");
                scanf("%d", &cursos[*num_cursos].numParcelas);
                getchar(); // Limpar buffer de entrada
                (*num_cursos)++;
                printf("Curso cadastrado com sucesso!\n");
                salvar_dados_cursos(cursos, *num_cursos);
                break;
            case 3:
                // Atualizar curso
                exibir_cursos(cursos, *num_cursos);
                int id;
                printf("Digite o ID do curso a ser atualizado: ");
                scanf("%d", &id);
                getchar(); // Limpar buffer de entrada
                for (int i = 0; i < *num_cursos; i++) {
                    if (cursos[i].id == id) {
                        printf("Digite o novo nome do curso: ");
                        fgets(cursos[i].nome, MAX_NOME, stdin);
                        cursos[i].nome[strcspn(cursos[i].nome, "\n")] = 0; // Remover nova linha
                        printf("Digite o novo preço por mês do curso: ");
                        scanf("%f", &cursos[i].precoPorMes);
                        printf("Digite o novo número de parcelas: ");
                        scanf("%d", &cursos[i].numParcelas);
                        getchar(); // Limpar buffer de entrada
                        printf("Curso atualizado com sucesso!\n");
                        salvar_dados_cursos(cursos, *num_cursos);
                        break;
                    }
                }
                break;
            case 4:
                // Excluir curso
                exibir_cursos(cursos, *num_cursos);
                printf("Digite o ID do curso a ser excluído: ");
                scanf("%d", &id);
                getchar(); // Limpar buffer de entrada
                for (int i = 0; i < *num_cursos; i++) {
                    if (cursos[i].id == id) {
                        for (int j = i; j < *num_cursos - 1; j++) {
                            cursos[j] = cursos[j + 1];
                        }
                        (*num_cursos)--;
                        printf("Curso excluído com sucesso!\n");
                        salvar_dados_cursos(cursos, *num_cursos);
                        break;
                    }
                }
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 5);
}

void crud_matriculas(Matricula matriculas[], int *num_matriculas, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos) {
    int opcao;
    do {
        printf("\n======= CRUD Matrículas =======\n");
        printf("1. Ver matrículas\n");
        printf("2. Adicionar matrícula\n");
        printf("3. Atualizar matrícula\n");
        printf("4. Excluir matrícula\n");
        printf("5. Voltar ao menu principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                // Ver matrículas
                exibir_matriculas(matriculas, *num_matriculas, alunos, num_alunos, cursos, num_cursos);
                break;
            case 2:
                // Adicionar matrícula
                matriculas[*num_matriculas].id = obter_proximo_id(NULL, 0, NULL, 0, matriculas, *num_matriculas);
                printf("Digite o ID do aluno: ");
                scanf("%d", &matriculas[*num_matriculas].idAluno);
                printf("Digite o ID do curso: ");
                scanf("%d", &matriculas[*num_matriculas].idCurso);
                getchar(); // Limpar buffer de entrada
                (*num_matriculas)++;
                printf("Matrícula cadastrada com sucesso!\n");
                salvar_dados_matriculas(matriculas, *num_matriculas);
                break;
            case 3:
                // Atualizar matrícula
                atualizar_matricula(matriculas, *num_matriculas, cursos, num_cursos);
                salvar_dados_matriculas(matriculas, *num_matriculas);
                break;
            case 4:
                // Excluir matrícula
                excluir_matricula(matriculas, num_matriculas);
                salvar_dados_matriculas(matriculas, *num_matriculas);
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 5);
}

void exibir_alunos(Aluno alunos[], int num_alunos) {
    printf("\n======= Alunos Cadastrados =======\n");
    for (int i = 0; i < num_alunos; i++) {
        printf("ID: %d, Nome: %s, Senha: %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
}

void exibir_cursos(Curso cursos[], int num_cursos) {
    printf("\n======= Cursos Cadastrados =======\n");
    for (int i = 0; i < num_cursos; i++) {
        printf("ID: %d, Nome: %s, Preço por mês: %.2f, Número de parcelas: %d\n",
               cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas);
    }
}

void exibir_matriculas(Matricula matriculas[], int num_matriculas, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos) {
    printf("\n======= Matrículas Cadastradas =======\n");
    for (int i = 0; i < num_matriculas; i++) {
        int idAluno = matriculas[i].idAluno;
        int idCurso = matriculas[i].idCurso;
        char nomeAluno[MAX_NOME];
        char nomeCurso[MAX_NOME];
        float precoCurso;
        int numParcelas;

        // Encontrar nome do aluno
        for (int j = 0; j < num_alunos; j++) {
            if (alunos[j].id == idAluno) {
                strcpy(nomeAluno, alunos[j].nome);
                break;
            }
        }

        // Encontrar nome do curso, preço e número de parcelas
        for (int j = 0; j < num_cursos; j++) {
            if (cursos[j].id == idCurso) {
                strcpy(nomeCurso, cursos[j].nome);
                precoCurso = cursos[j].precoPorMes;
                numParcelas = cursos[j].numParcelas;
                break;
            }
        }

        printf("ID Matrícula: %d, Aluno: %s, Curso: %s, Preço por mês: %.2f, Número de parcelas: %d\n",
               matriculas[i].id, nomeAluno, nomeCurso, precoCurso, numParcelas);
    }
}

void salvar_dados_alunos(Aluno alunos[], int num_alunos) {
    FILE *file = fopen("alunos.txt", "w");
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
    while (fscanf(file, "%d\n%[^\n]\n%[^\n]\n", &alunos[*num_alunos].id, alunos[*num_alunos].nome, alunos[*num_alunos].senha) != EOF) {
        (*num_alunos)++;
    }
    fclose(file);
}

void salvar_dados_cursos(Curso cursos[], int num_cursos) {
    FILE *file = fopen("cursos.txt", "w");
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
    while (fscanf(file, "%d\n%[^\n]\n%f\n%d\n", &cursos[*num_cursos].id, cursos[*num_cursos].nome, &cursos[*num_cursos].precoPorMes, &cursos[*num_cursos].numParcelas) != EOF) {
        (*num_cursos)++;
    }
    fclose(file);
}

void salvar_dados_matriculas(Matricula matriculas[], int num_matriculas) {
    FILE *file = fopen("matriculas.txt", "w");
    for (int i = 0; i < num_matriculas; i++) {
        fprintf(file, "%d\n%d\n%d\n", matriculas[i].id, matriculas[i].idAluno, matriculas[i].idCurso);
    }
    fclose(file);
}

void carregar_dados_matriculas(Matricula matriculas[], int *num_matriculas) {
    FILE *file = fopen("matriculas.txt", "r");
    if (file == NULL) {
        printf("Arquivo de matrículas não encontrado. Inicializando lista vazia.\n");
        return;
    }
    while (fscanf(file, "%d\n%d\n%d\n", &matriculas[*num_matriculas].id, &matriculas[*num_matriculas].idAluno, &matriculas[*num_matriculas].idCurso) != EOF) {
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
    if (tipoUsuario == -1) {
        printf("\n======= Menu Principal =======\n");
        printf("1. CRUD Alunos\n");
        printf("2. CRUD Cursos\n");
        printf("3. CRUD Matrículas\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
    } else if(tipoUsuario != 0) {
        printf("\n======= Menu Principal =======\n");
        printf("1. Ver Matrículas\n");
        printf("2. Adicionar Matrícula\n");
        printf("3. Atualizar Matrícula\n");
        printf("4. Excluir Matrícula\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
    }
}

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos) {
    matriculas[*num_matriculas].id = obter_proximo_id(NULL, 0, NULL, 0, matriculas, *num_matriculas);
    matriculas[*num_matriculas].idAluno = idAluno;
    printf("Digite o ID do curso: ");
    scanf("%d", &matriculas[*num_matriculas].idCurso);
    getchar(); // Limpar buffer de entrada
    (*num_matriculas)++;
    printf("Matrícula cadastrada com sucesso!\n");
    salvar_dados_matriculas(matriculas, *num_matriculas);
}

void atualizar_matricula(Matricula matriculas[], int num_matriculas, Curso cursos[], int num_cursos) {
    int id;
    printf("Digite o ID da matrícula a ser atualizada: ");
    scanf("%d", &id);
    getchar(); // Limpar buffer de entrada
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id == id) {
            printf("Digite o novo ID do curso: ");
            scanf("%d", &matriculas[i].idCurso);
            getchar(); // Limpar buffer de entrada
            printf("Matrícula atualizada com sucesso!\n");
            break;
        }
    }
}

void excluir_matricula(Matricula matriculas[], int *num_matriculas) {
    int id;
    printf("Digite o ID da matrícula a ser excluída: ");
    scanf("%d", &id);
    getchar(); // Limpar buffer de entrada
    for (int i = 0; i < *num_matriculas; i++) {
        if (matriculas[i].id == id) {
            for (int j = i; j < *num_matriculas - 1; j++) {
                matriculas[j] = matriculas[j + 1];
            }
            (*num_matriculas)--;
            printf("Matrícula excluída com sucesso!\n");
            break;
        }
    }
}

int main() {
    Aluno alunos[100];
    Curso cursos[100];
    Matricula matriculas[100];
    int num_alunos = 0;
    int num_cursos = 0;
    int num_matriculas = 0;
    int tipoUsuario;
    int opcao;

    carregar_dados_alunos(alunos, &num_alunos);
    carregar_dados_cursos(cursos, &num_cursos);
    carregar_dados_matriculas(matriculas, &num_matriculas);

    tipoUsuario = fazer_login(alunos, num_alunos);
    if (tipoUsuario == -1) {
        printf("Encerrando o programa.\n");
        return 0;
    }
    

    do {
        exibir_menu_principal(tipoUsuario == 0 ? 0 : 1);
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        if (tipoUsuario == -1) {
            // Admin
            switch (opcao) {
                case 1:
                    crud_alunos(alunos, &num_alunos);
                    break;
                case 2:
                    crud_cursos(cursos, &num_cursos);
                    break;
                case 3:
                    crud_matriculas(matriculas, &num_matriculas, alunos, num_alunos, cursos, num_cursos);
                    break;
                case 4:
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        } else if(tipoUsuario != 0) {
            // User
            switch (opcao) {
                case 1:
                    exibir_matriculas(matriculas, num_matriculas, alunos, num_alunos, cursos, num_cursos);
                    break;
                case 2:
                    cadastrar_matricula(matriculas, &num_matriculas, tipoUsuario, cursos, num_cursos);
                    break;
                case 3:
                    atualizar_matricula(matriculas, num_matriculas, cursos, num_cursos);
                    break;
                case 4:
                    excluir_matricula(matriculas, &num_matriculas);
                    break;
                case 5:
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        } else {
        	printf("Usuário inválido. Por favor, Tente novamente.\n");
		}
    } while (opcao != 4 && opcao != 5);

    return 0;
}

