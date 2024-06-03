#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int id;
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
                printf("Aluno adicionado com sucesso!\n");
                salvar_dados_alunos(alunos, *num_alunos);
                break;
            case 3:
                // Atualizar aluno
                exibir_alunos(alunos, *num_alunos);
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
    int id;
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
                printf("Curso adicionado com sucesso!\n");
                salvar_dados_cursos(cursos, *num_cursos);
                break;
            case 3:
                // Atualizar curso
                exibir_cursos(cursos, *num_cursos);
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
    int id;
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
                cadastrar_matricula(matriculas, num_matriculas, 0, cursos, num_cursos); // Admin idAluno = 0
                break;
            case 3:
                // Atualizar matrícula
                atualizar_matricula(matriculas, *num_matriculas, cursos, num_cursos);
                break;
            case 4:
                // Excluir matrícula
                excluir_matricula(matriculas, num_matriculas);
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 5);
}

void exibir_menu_principal(int tipoUsuario) {
    printf("\n======= Menu Principal =======\n");
    if (tipoUsuario == 0) { // Admin
        printf("1. CRUD Alunos\n");
        printf("2. CRUD Cursos\n");
        printf("3. CRUD Matrículas\n");
        printf("4. Sair\n");
    } else { // User
        printf("1. Ver matrículas\n");
        printf("2. Adicionar matrícula\n");
        printf("3. Atualizar matrícula\n");
        printf("4. Excluir matrícula\n");
        printf("5. Sair\n");
    }
    printf("Escolha uma opção: ");
}

int fazer_login(Aluno alunos[], int num_alunos) {
    char nome[MAX_NOME];
    char senha[MAX_NOME];

    printf("Digite o nome: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = 0; // Remover nova linha
    printf("Digite a senha: ");
    fgets(senha, MAX_NOME, stdin);
    senha[strcspn(senha, "\n")] = 0; // Remover nova linha

    if (strcmp(nome, "admin") == 0 && strcmp(senha, "admin") == 0) {
        return 0; // Admin ID
    }

    for (int i = 0; i < num_alunos; i++) {
        if (strcmp(alunos[i].nome, nome) == 0 && strcmp(alunos[i].senha, senha) == 0) {
            return alunos[i].id;
        }
    }

    printf("Nome ou senha incorretos. Tente novamente.\n");
    return -1;
}

void salvar_dados_alunos(Aluno alunos[], int num_alunos) {
    FILE *file = fopen("alunos.txt", "w");
    for (int i = 0; i < num_alunos; i++) {
        fprintf(file, "%d,%s,%s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
    fclose(file);
}

void carregar_dados_alunos(Aluno alunos[], int *num_alunos) {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) return;
    while (fscanf(file, "%d,%[^,],%s\n", &alunos[*num_alunos].id, alunos[*num_alunos].nome, alunos[*num_alunos].senha) != EOF) {
        (*num_alunos)++;
    }
    fclose(file);
}

void salvar_dados_cursos(Curso cursos[], int num_cursos) {
    FILE *file = fopen("cursos.txt", "w");
    for (int i = 0; i < num_cursos; i++) {
        fprintf(file, "%d,%s,%f,%d\n", cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas);
    }
    fclose(file);
}

void carregar_dados_cursos(Curso cursos[], int *num_cursos) {
    FILE *file = fopen("cursos.txt", "r");
    if (file == NULL) return;
    while (fscanf(file, "%d,%[^,],%f,%d\n", &cursos[*num_cursos].id, cursos[*num_cursos].nome, &cursos[*num_cursos].precoPorMes, &cursos[*num_cursos].numParcelas) != EOF) {
        (*num_cursos)++;
    }
    fclose(file);
}

void salvar_dados_matriculas(Matricula matriculas[], int num_matriculas) {
    FILE *file = fopen("matriculas.txt", "w");
    for (int i = 0; i < num_matriculas; i++) {
        fprintf(file, "%d,%d,%d\n", matriculas[i].id, matriculas[i].idAluno, matriculas[i].idCurso);
    }
    fclose(file);
}

void carregar_dados_matriculas(Matricula matriculas[], int *num_matriculas) {
    FILE *file = fopen("matriculas.txt", "r");
    if (file == NULL) return;
    while (fscanf(file, "%d,%d,%d\n", &matriculas[*num_matriculas].id, &matriculas[*num_matriculas].idAluno, &matriculas[*num_matriculas].idCurso) != EOF) {
        (*num_matriculas)++;
    }
    fclose(file);
}

void exibir_alunos(Aluno alunos[], int num_alunos) {
    printf("\n======= Lista de Alunos =======\n");
    for (int i = 0; i < num_alunos; i++) {
        printf("ID: %d, Nome: %s, Senha: %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
}

void exibir_cursos(Curso cursos[], int num_cursos) {
    printf("\n======= Lista de Cursos =======\n");
    for (int i = 0; i < num_cursos; i++) {
        printf("ID: %d, Nome: %s, Preço por Mês: %.2f, Parcelas: %d\n", cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas);
    }
}

void exibir_matriculas(Matricula matriculas[], int num_matriculas, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos) {
    printf("\n======= Lista de Matrículas =======\n");
    for (int i = 0; i < num_matriculas; i++) {
        printf("ID: %d, ID Aluno: %d, ID Curso: %d\n", matriculas[i].id, matriculas[i].idAluno, matriculas[i].idCurso);
    }
}

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos) {
    matriculas[*num_matriculas].id = obter_proximo_id(NULL, 0, NULL, 0, matriculas, *num_matriculas);
    matriculas[*num_matriculas].idAluno = idAluno;
    exibir_cursos(cursos, num_cursos);
    printf("Digite o ID do curso: ");
    scanf("%d", &matriculas[*num_matriculas].idCurso);
    getchar(); // Limpar buffer de entrada
    (*num_matriculas)++;
    printf("Matrícula cadastrada com sucesso!\n");
    salvar_dados_matriculas(matriculas, *num_matriculas);
}

void atualizar_matricula(Matricula matriculas[], int num_matriculas, Curso cursos[], int num_cursos) {
    int id;
    exibir_matriculas(matriculas, num_matriculas, NULL, 0, cursos, num_cursos);
    printf("Digite o ID da matrícula a ser atualizada: ");
    scanf("%d", &id);
    getchar(); // Limpar buffer de entrada
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id == id) {
            printf("Digite o novo ID do curso: ");
            scanf("%d", &matriculas[i].idCurso);
            getchar(); // Limpar buffer de entrada
            printf("Matrícula atualizada com sucesso!\n");
            salvar_dados_matriculas(matriculas, num_matriculas);
            break;
        }
    }
}

void excluir_matricula(Matricula matriculas[], int *num_matriculas) {
    int id;
    exibir_matriculas(matriculas, *num_matriculas, NULL, 0, NULL, 0);
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
            salvar_dados_matriculas(matriculas, *num_matriculas);
            break;
        }
    }
}

void main() {
    Aluno alunos[100];
    Curso cursos[100];
    Matricula matriculas[100];
    int num_alunos = 0, num_cursos = 0, num_matriculas = 0;

    carregar_dados_alunos(alunos, &num_alunos);
    carregar_dados_cursos(cursos, &num_cursos);
    carregar_dados_matriculas(matriculas, &num_matriculas);

    int idUsuario;

    while (1) {
        idUsuario = fazer_login(alunos, num_alunos);

        if (idUsuario != -1) {
            break;
        }

        int sair;
        printf("Deseja tentar novamente? (1 para Sim, 0 para Não): ");
        scanf("%d", &sair);
        getchar(); // Limpar buffer de entrada
        if (sair == 0) {
            printf("Saindo do programa...\n");
            return;
        }
    }

    int opcao;
    do {
        exibir_menu_principal(idUsuario == 0 ? 0 : 1);
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        if (idUsuario == 0) { // Admin
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
                    printf("Saindo do programa...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        } else { // User
            switch (opcao) {
                case 1:
                    exibir_matriculas(matriculas, num_matriculas, alunos, num_alunos, cursos, num_cursos);
                    break;
                case 2:
                    cadastrar_matricula(matriculas, &num_matriculas, idUsuario, cursos, num_cursos);
                    break;
                case 3:
                    atualizar_matricula(matriculas, num_matriculas, cursos, num_cursos);
                    break;
                case 4:
                    excluir_matricula(matriculas, &num_matriculas);
                    break;
                case 5:
                    printf("Saindo do programa...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        }
    } while (opcao != (idUsuario == 0 ? 4 : 5));
}

