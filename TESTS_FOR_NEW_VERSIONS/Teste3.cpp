#include <stdio.h>
#include <string.h>

#define MAX_ALUNOS 100
#define MAX_CURSOS 100
#define MAX_MATRICULAS 100
#define MAX_NOME 50

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

// Function prototypes
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
int obter_proximo_id(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);

// Function implementations
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
        printf("1. Ver Alunos\n");
        printf("2. Adicionar Aluno\n");
        printf("3. Atualizar Aluno\n");
        printf("4. Excluir Aluno\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                exibir_alunos(alunos, *num_alunos);
                break;
            case 2: {
                printf("Digite o nome do aluno: ");
                fgets(alunos[*num_alunos].nome, MAX_NOME, stdin);
                alunos[*num_alunos].nome[strcspn(alunos[*num_alunos].nome, "\n")] = 0; // Remover nova linha
                printf("Digite a senha do aluno: ");
                fgets(alunos[*num_alunos].senha, MAX_NOME, stdin);
                alunos[*num_alunos].senha[strcspn(alunos[*num_alunos].senha, "\n")] = 0; // Remover nova linha
                alunos[*num_alunos].id = obter_proximo_id(alunos, *num_alunos, NULL, 0, NULL, 0);
                (*num_alunos)++;
                salvar_dados_alunos(alunos, *num_alunos);
                printf("Aluno cadastrado com sucesso!\n");
                break;
            }
            case 3: {
                int id;
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
                        salvar_dados_alunos(alunos, *num_alunos);
                        printf("Aluno atualizado com sucesso!\n");
                        break;
                    }
                }
                break;
            }
            case 4: {
                int id;
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
                        salvar_dados_alunos(alunos, *num_alunos);
                        printf("Aluno excluído com sucesso!\n");
                        break;
                    }
                }
                break;
            }
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
        printf("1. Ver Cursos\n");
        printf("2. Adicionar Curso\n");
        printf("3. Atualizar Curso\n");
        printf("4. Excluir Curso\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                exibir_cursos(cursos, *num_cursos);
                break;
            case 2: {
                printf("Digite o nome do curso: ");
                fgets(cursos[*num_cursos].nome, MAX_NOME, stdin);
                cursos[*num_cursos].nome[strcspn(cursos[*num_cursos].nome, "\n")] = 0; // Remover nova linha
                printf("Digite o preço por mês do curso: ");
                scanf("%f", &cursos[*num_cursos].precoPorMes);
                getchar(); // Limpar buffer de entrada
                printf("Digite o número de parcelas do curso: ");
                scanf("%d", &cursos[*num_cursos].numParcelas);
                getchar(); // Limpar buffer de entrada
                cursos[*num_cursos].id = obter_proximo_id(NULL, 0, cursos, *num_cursos, NULL, 0);
                (*num_cursos)++;
                salvar_dados_cursos(cursos, *num_cursos);
                printf("Curso cadastrado com sucesso!\n");
                break;
            }
            case 3: {
                int id;
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
                        getchar(); // Limpar buffer de entrada
                        printf("Digite o novo número de parcelas do curso: ");
                        scanf("%d", &cursos[i].numParcelas);
                        getchar(); // Limpar buffer de entrada
                        salvar_dados_cursos(cursos, *num_cursos);
                        printf("Curso atualizado com sucesso!\n");
                        break;
                    }
                }
                break;
            }
            case 4: {
                int id;
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
                        salvar_dados_cursos(cursos, *num_cursos);
                        printf("Curso excluído com sucesso!\n");
                        break;
                    }
                }
                break;
            }
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
        printf("1. Ver Matrículas\n");
        printf("2. Adicionar Matrícula\n");
        printf("3. Atualizar Matrícula\n");
        printf("4. Excluir Matrícula\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer de entrada

        switch (opcao) {
            case 1:
                exibir_matriculas(matriculas, *num_matriculas, alunos, num_alunos, cursos, num_cursos);
                break;
            case 2: {
                int idAluno, idCurso;
                printf("Digite o ID do aluno: ");
                scanf("%d", &idAluno);
                getchar(); // Limpar buffer de entrada
                printf("Digite o ID do curso: ");
                scanf("%d", &idCurso);
                getchar(); // Limpar buffer de entrada
                matriculas[*num_matriculas].id = obter_proximo_id(alunos, num_alunos, cursos, num_cursos, matriculas, *num_matriculas);
                matriculas[*num_matriculas].idAluno = idAluno;
                matriculas[*num_matriculas].idCurso = idCurso;
                (*num_matriculas)++;
                salvar_dados_matriculas(matriculas, *num_matriculas);
                printf("Matrícula cadastrada com sucesso!\n");
                break;
            }
            case 3:
                atualizar_matricula(matriculas, *num_matriculas, cursos, num_cursos);
                break;
            case 4:
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

void exibir_alunos(Aluno alunos[], int num_alunos) {
    printf("\n======= Alunos Cadastrados =======\n");
    for (int i = 0; i < num_alunos; i++) {
        printf("ID: %d, Nome: %s, Senha: %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
}

void exibir_cursos(Curso cursos[], int num_cursos) {
    printf("\n======= Cursos Cadastrados =======\n");
    for (int i = 0; i < num_cursos; i++) {
        printf("ID: %d, Nome: %s, Preço por Mês: %.2f, Parcelas: %d, Valor da Parcela: %.2f\n",
               cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas,
               cursos[i].precoPorMes / cursos[i].numParcelas);
    }
}

void exibir_matriculas(Matricula matriculas[], int num_matriculas, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos) {
    printf("\n======= Matrículas Cadastradas =======\n");
    for (int i = 0; i < num_matriculas; i++) {
        for (int j = 0; j < num_alunos; j++) {
            if (alunos[j].id == matriculas[i].idAluno) {
                char nome_aluno[MAX_NOME];
                strcpy(nome_aluno, alunos[j].nome);
                for (int k = 0; k < num_cursos; k++) {
                    if (cursos[k].id == matriculas[i].idCurso) {
                        printf("Aluno: %s, Curso: %s, Preço por Mês: %.2f, Parcelas: %d, Valor da Parcela: %.2f\n",
                               nome_aluno, cursos[k].nome, cursos[k].precoPorMes, cursos[k].numParcelas,
                               cursos[k].precoPorMes / cursos[k].numParcelas);
                        break;
                    }
                }
                break;
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
        fscanf(file, "%f\n%d\n", &cursos[*num_cursos].precoPorMes, &cursos[*num_cursos].numParcelas);
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
    while (fscanf(file, "%d\n", &matriculas[*num_matriculas].id) != EOF) {
        fscanf(file, "%d\n%d\n", &matriculas[*num_matriculas].idAluno, &matriculas[*num_matriculas].idCurso);
        (*num_matriculas)++;
    }
    fclose(file);
}

void exibir_menu_principal(int tipoUsuario) {
    if (tipoUsuario == 0) {
        printf("\n======= Menu Principal =======\n");
        printf("1. CRUD Alunos\n");
        printf("2. CRUD Cursos\n");
        printf("3. CRUD Matrículas\n");
        printf("4. Fazer Logout\n");
        printf("Escolha uma opção: ");
    } else {
        printf("\n======= Menu Principal =======\n");
        printf("1.Ver Matrículas\n");
        printf("2. Cadastrar Matrícula\n");
        printf("3. Atualizar Matrícula\n");
        printf("4. Excluir Matrícula\n");
        printf("5. Fazer Logout\n");
        printf("Escolha uma opção: ");
    }
}

int fazer_login(Aluno alunos[], int num_alunos) {
    char username[MAX_NOME];
    char password[MAX_NOME];
    printf("Username: ");
    fgets(username, MAX_NOME, stdin);
    username[strcspn(username, "\n")] = 0; // Remove newline character
    printf("Password: ");
    fgets(password, MAX_NOME, stdin);
    password[strcspn(password, "\n")] = 0; // Remove newline character

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        printf("Login as admin successful!\n");
        return 0; // Admin user
    }

    for (int i = 0; i < num_alunos; i++) {
        if (strcmp(alunos[i].nome, username) == 0 && strcmp(alunos[i].senha, password) == 0) {
            printf("Login successful! Welcome, %s!\n", alunos[i].nome);
            return alunos[i].id; // Return student ID as user ID
        }
    }

    printf("Login failed. Invalid username or password.\n");
    return -1; // Login failed
}

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos) {
    int idCurso;
    printf("Cursos disponíveis:\n");
    exibir_cursos(cursos, num_cursos);
    printf("Digite o ID do curso desejado: ");
    scanf("%d", &idCurso);
    getchar(); // Limpar buffer de entrada

    matriculas[*num_matriculas].id = obter_proximo_id(NULL, 0, cursos, num_cursos, matriculas, *num_matriculas);
    matriculas[*num_matriculas].idAluno = idAluno;
    matriculas[*num_matriculas].idCurso = idCurso;
    (*num_matriculas)++;
    salvar_dados_matriculas(matriculas, *num_matriculas);
    printf("Matrícula cadastrada com sucesso!\n");
}

void atualizar_matricula(Matricula matriculas[], int num_matriculas, Curso cursos[], int num_cursos) {
    int id;
    printf("Digite o ID da matrícula a ser atualizada: ");
    scanf("%d", &id);
    getchar(); // Limpar buffer de entrada
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id == id) {
            int idCurso;
            printf("Cursos disponíveis:\n");
            exibir_cursos(cursos, num_cursos);
            printf("Digite o ID do novo curso: ");
            scanf("%d", &idCurso);
            getchar(); // Limpar buffer de entrada
            matriculas[i].idCurso = idCurso;
            salvar_dados_matriculas(matriculas, num_matriculas);
            printf("Matrícula atualizada com sucesso!\n");
            return;
        }
    }
    printf("Matrícula não encontrada.\n");
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
            salvar_dados_matriculas(matriculas, *num_matriculas);
            printf("Matrícula excluída com sucesso!\n");
            return;
        }
    }
    printf("Matrícula não encontrada.\n");
}

int main() {
    Aluno alunos[MAX_ALUNOS];
    Curso cursos[MAX_CURSOS];
    Matricula matriculas[MAX_MATRICULAS];
    int num_alunos = 0;
    int num_cursos = 0;
    int num_matriculas = 0;

    carregar_dados_alunos(alunos, &num_alunos);
    carregar_dados_cursos(cursos, &num_cursos);
    carregar_dados_matriculas(matriculas, &num_matriculas);

    int userType;
    int userId;
    int opcao;
    do {
        printf("\n======= Sistema de Gestão de Matrículas =======\n");
        userId = fazer_login(alunos, num_alunos);
        if (userId != -1) {
            userType = 1; // Regular user
            exibir_menu_principal(userType);
            
            scanf("%d", &opcao);
            getchar(); // Limpar buffer de entrada
            switch (opcao) {
                case 1:
                    exibir_matriculas(matriculas, num_matriculas, alunos, num_alunos, cursos, num_cursos);
                    break;
                case 2:
                    cadastrar_matricula(matriculas, &num_matriculas, userId, cursos, num_cursos);
                    break;
                case 3:
                    atualizar_matricula(matriculas, num_matriculas, cursos, num_cursos);
                    break;
                case 4:
                    excluir_matricula(matriculas, &num_matriculas);
                    break;
                case 5:
                    printf("Fazendo Logout...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        } else if(userId == 0) {
            userType = 0; // Admin user
            exibir_menu_principal(userType);
            int opcao;
            scanf("%d", &opcao);
            getchar(); // Limpar buffer de entrada
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
                    printf("Fazendo Logout...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        }
    } while (userType == 0 ? userId != -1 : opcao != 4);

    return 0;
}


