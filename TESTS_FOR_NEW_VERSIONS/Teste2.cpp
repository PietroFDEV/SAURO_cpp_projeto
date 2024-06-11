#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NOME 100
#define MAX_ALUNOS 100
#define MAX_CURSOS 100
#define MAX_VENCIMENTOS 100
#define MAX_MATRICULAS 100

typedef struct {
    int id;
    char nome[MAX_NOME];
    char senha[MAX_NOME];
} Aluno;

typedef struct {
    int id;
    char nome[MAX_NOME];
    float precoPorMes;
    int duracaoMeses; // Número de meses que o curso vai durar
} Curso;

typedef struct {
    int id;
    int idAluno;
    int idCurso;
    time_t dataExpiracao; // Data de expiração para a matrícula
} Matricula;

typedef struct {
    int idVencimento;
    int idMatricula;
    time_t dataVencimento;
    float valorParcela;
    int pago; // Boolean para indicar se a parcela foi paga
} Vencimento;

// Protótipos de Função
void cadastrar_aluno(Aluno alunos[], int *num_alunos);
void cadastrar_curso(Curso cursos[], int *num_cursos);
void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos, Vencimento vencimentos[], int *num_vencimentos);
void carregar_alunos(Aluno alunos[], int *num_alunos);
void carregar_cursos(Curso cursos[], int *num_cursos);
void carregar_matriculas(Matricula matriculas[], int *num_matriculas);
void carregar_vencimentos(Vencimento vencimentos[], int *num_vencimentos);
void editar_aluno(Aluno alunos[], int num_alunos);
void editar_curso(Curso cursos[], int num_cursos);
void editar_matricula(Matricula matriculas[], int num_matriculas, int idMatricula, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos);
void excluir_aluno(Aluno alunos[], int *num_alunos, Matricula matriculas[], int *num_matriculas);
void excluir_curso(Curso cursos[], int *num_cursos, Matricula matriculas[], int *num_matriculas);
void excluir_matricula(Matricula matriculas[], int *num_matriculas, int idAluno);
void exibir_alunos(Aluno alunos[], int num_alunos);
void exibir_cursos(Curso cursos[], int num_cursos);
void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos, Matricula matriculas[], int num_matriculas);
void exibir_vencimentos(Matricula matriculas[], int num_matriculas, Vencimento vencimentos[], int num_vencimentos);
void limpar_console();
void menu_admin(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas, Vencimento vencimentos[], int num_vencimentos);
void menu_aluno(int idAluno, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas, Vencimento vencimentos[], int num_vencimentos);
void menu_login();
int login(Aluno alunos[], int num_alunos);
int encontrar_proximo_id(Aluno alunos[], int num_alunos);
int encontrar_proximo_id_curso(Curso cursos[], int num_cursos);
int encontrar_proximo_id_matricula(Matricula matriculas[], int num_matriculas);
int encontrar_proximo_id_vencimento(Vencimento vencimentos[], int num_vencimentos);
char *encontrar_nome_aluno(int idAluno, Aluno alunos[], int num_alunos);
char *encontrar_nome_curso(int idCurso, Curso cursos[], int num_cursos);
void salvar_alunos(Aluno alunos[], int num_alunos);
void salvar_cursos(Curso cursos[], int num_cursos);
void salvar_matriculas(Matricula matriculas[], int num_matriculas);
void salvar_vencimentos(Vencimento vencimentos[], int num_vencimentos);
time_t calcular_data_expiracao(int duracaoMeses);
void criar_vencimentos(Vencimento vencimentos[], int *num_vencimentos, Matricula matricula, Curso curso);

void cadastrar_aluno(Aluno alunos[], int *num_alunos);
void editar_aluno(Aluno alunos[], int num_alunos);
void excluir_aluno(Aluno alunos[], int *num_alunos, Matricula matriculas[], int *num_matriculas);
void exibir_alunos(Aluno alunos[], int num_alunos);
void salvar_alunos(Aluno alunos[], int num_alunos);
void carregar_alunos(Aluno alunos[], int *num_alunos);

void cadastrar_curso(Curso cursos[], int *num_cursos);
void editar_curso(Curso cursos[], int num_cursos);
void excluir_curso(Curso cursos[], int *num_cursos, Matricula matriculas[], int *num_matriculas);
void exibir_cursos(Curso cursos[], int num_cursos);
void salvar_cursos(Curso cursos[], int num_cursos);
void carregar_cursos(Curso cursos[], int *num_cursos);

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos);
void editar_matricula(Matricula matriculas[], int num_matriculas, int idMatricula, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos);
void excluir_matricula(Matricula matriculas[], int *num_matriculas, int idAluno);
void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos, Matricula matriculas[], int num_matriculas);
void salvar_matriculas(Matricula matriculas[], int num_matriculas);
void carregar_matriculas(Matricula matriculas[], int *num_matriculas);

void cadastrar_vencimento(Vencimento vencimentos[], int *num_vencimentos, Matricula matriculas[], int num_matriculas);
void editar_vencimento(Vencimento vencimentos[], int num_vencimentos);
void excluir_vencimento(Vencimento vencimentos[], int *num_vencimentos);
void salvar_vencimentos(Vencimento vencimentos[], int num_vencimentos);
void carregar_vencimentos(Vencimento vencimentos[], int *num_vencimentos);

void login_aluno(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int *num_matriculas, Vencimento vencimentos[], int *num_vencimentos);
void login_admin(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int *num_matriculas, Vencimento vencimentos[], int *num_vencimentos);


void limpar_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int login(Aluno alunos[], int num_alunos) {
    char nome[MAX_NOME];
    char senha[MAX_NOME];
    printf("Digite o nome de usuário: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = 0; // Remove o caractere de nova linha
    printf("Digite a senha: ");
    fgets(senha, MAX_NOME, stdin);
    senha[strcspn(senha, "\n")] = 0; // Remove o caractere de nova linha

    if (strcmp("admin", nome) == 0 && strcmp("admin", senha) == 0) {
        return -1; // Usuário administrador
    }

    for (int i = 0; i < num_alunos; i++) {
        if (strcmp(alunos[i].nome, nome) == 0 && strcmp(alunos[i].senha, senha) == 0) {
            return alunos[i].id; // Usuário aluno
        }
    }

    return 0; // Falha no login
}

void menu_login() {
    printf("\n======= Menu de Login =======\n");
    printf("1. Entrar\n");
    printf("2. Sair\n");
    printf("Escolha uma opção: ");
}

void menu_aluno(int idAluno, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas, Vencimento vencimentos[], int num_vencimentos) {
    int opcao;
    do {
        printf("\n======= Menu do Aluno =======\n");
        printf("1. Ver cursos disponíveis\n");
        printf("2. Ver minhas matrículas\n");
        printf("3. Ver vencimentos\n");
        printf("4. Adicionar nova matrícula\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada
        limpar_console();

        switch (opcao) {
            case 1:
                exibir_cursos(cursos, num_cursos);
                break;
            case 2:
                exibir_matriculas(idAluno, cursos, num_cursos, alunos, num_alunos, matriculas, num_matriculas);
                break;
            case 3:
                exibir_vencimentos(matriculas, num_matriculas, vencimentos, num_vencimentos);
                break;
            case 4:
                cadastrar_matricula(matriculas, &num_matriculas, idAluno, cursos, num_cursos, vencimentos, &num_vencimentos);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 5);
}

void menu_admin(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas, Vencimento vencimentos[], int num_vencimentos) {
    int opcao_admin_menu;
    int opcao_admin;
    do {
        printf("\n======= Menu de Admin =======\n");
        printf("1. Gerenciar alunos\n");
        printf("2. Gerenciar cursos\n");
        printf("3. Gerenciar matrículas\n");
        printf("4. Gerenciar vencimentos\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao_admin_menu);
        getchar(); // Limpa o buffer de entrada
        limpar_console();

        switch (opcao_admin_menu) {
            case 1:
                // Gerenciamento de Alunos
                printf("\n======= Gerenciamento de Alunos =======\n");
                printf("1. Adicionar aluno\n");
                printf("2. Editar aluno\n");
                printf("3. Excluir aluno\n");
                printf("4. Ver alunos\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao_admin);
                getchar(); // Limpa o buffer de entrada
                switch (opcao_admin) {
                    case 1:
                        cadastrar_aluno(alunos, &num_alunos);
                        break;
                    case 2:
                        editar_aluno(alunos, num_alunos);
                        break;
                    case 3:
                        excluir_aluno(alunos, &num_alunos, matriculas, &num_matriculas);
                        break;
                    case 4:
                        exibir_alunos(alunos, num_alunos);
                        break;
                    default:
                        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                }
                break;
            case 2:
                // Gerenciamento de Cursos
                printf("\n======= Gerenciamento de Cursos =======\n");
                printf("1. Adicionar curso\n");
                printf("2. Editar curso\n");
                printf("3. Excluir curso\n");
                printf("4. Ver cursos\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao_admin);
                getchar(); // Limpa o buffer de entrada
                switch (opcao_admin) {
                    case 1:
                        cadastrar_curso(cursos, &num_cursos);
                        break;
                    case 2:
                        editar_curso(cursos, num_cursos);
                        break;
                    case 3:
                        excluir_curso(cursos, &num_cursos, matriculas, &num_matriculas);
                        break;
                    case 4:
                        exibir_cursos(cursos, num_cursos);
                        break;
                    default:
                        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                }
                break;
            case 3:
                // Gerenciamento de Matrículas
                printf("\n======= Gerenciamento de Matrículas =======\n");
                printf("1. Adicionar matrícula\n");
                printf("2. Editar matrícula\n");
                printf("3. Excluir matrícula\n");
                printf("4. Ver matrículas\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao_admin);
                getchar(); // Limpa o buffer de entrada
                switch (opcao_admin) {
                    case 1:
                        printf("Opção inválida para administrador. Use a opção de alunos para adicionar matrículas.\n");
                        break;
                    case 2: {
                        int idMatricula;
                        printf("Digite o ID da matrícula a ser editada: ");
                        scanf("%d", &idMatricula);
                        editar_matricula(matriculas, num_matriculas, idMatricula, cursos, num_cursos, alunos, num_alunos);
                        break;
                    }
                    case 3:
                        excluir_matricula(matriculas, &num_matriculas, 0); // Passando 0 para excluir matrícula sem verificar aluno
                        break;
                    case 4:
                        for (int i = 0; i < num_alunos; i++) {
                            exibir_matriculas(alunos[i].id, cursos, num_cursos, alunos, num_alunos, matriculas, num_matriculas);
                        }
                        break;
                    default:
                        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                }
                break;
            case 4:
                // Gerenciamento de Vencimentos
                printf("\n======= Gerenciamento de Vencimentos =======\n");
                exibir_vencimentos(matriculas, num_matriculas, vencimentos, num_vencimentos);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao_admin_menu != 5);
}

int encontrar_proximo_id(Aluno alunos[], int num_alunos) {
    int max_id = 0;
    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].id > max_id) {
            max_id = alunos[i].id;
        }
    }
    return max_id + 1;
}

int encontrar_proximo_id_curso(Curso cursos[], int num_cursos) {
    int max_id = 0;
    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].id > max_id) {
            max_id = cursos[i].id;
        }
    }
    return max_id + 1;
}

int encontrar_proximo_id_matricula(Matricula matriculas[], int num_matriculas) {
    int max_id = 0;
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id > max_id) {
            max_id = matriculas[i].id;
        }
    }
    return max_id + 1;
}

int encontrar_proximo_id_vencimento(Vencimento vencimentos[], int num_vencimentos) {
    int max_id = 0;
    for (int i = 0; i < num_vencimentos; i++) {
        if (vencimentos[i].idVencimento > max_id) {
            max_id = vencimentos[i].idVencimento;
        }
    }
    return max_id + 1;
}

char *encontrar_nome_aluno(int idAluno, Aluno alunos[], int num_alunos) {
    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].id == idAluno) {
            return alunos[i].nome;
        }
    }
    return NULL;
}

char *encontrar_nome_curso(int idCurso, Curso cursos[], int num_cursos) {
    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].id == idCurso) {
            return cursos[i].nome;
        }
    }
    return NULL;
}

time_t calcular_data_expiracao(int duracaoMeses) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    t->tm_mon += duracaoMeses;
    return mktime(t);
}

void criar_vencimentos(Vencimento vencimentos[], int *num_vencimentos, Matricula matricula, Curso curso) {
    for (int i = 0; i < curso.duracaoMeses; i++) {
        vencimentos[*num_vencimentos].idVencimento = encontrar_proximo_id_vencimento(vencimentos, *num_vencimentos);
        vencimentos[*num_vencimentos].idMatricula = matricula.id;
        vencimentos[*num_vencimentos].valorParcela = curso.precoPorMes;
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        t->tm_mon += i;
        vencimentos[*num_vencimentos].dataVencimento = mktime(t);
        vencimentos[*num_vencimentos].pago = 0; // Inicia como não pago
        (*num_vencimentos)++;
    }
}

void cadastrar_aluno(Aluno alunos[], int *num_alunos) {
    printf("Digite o nome do aluno: ");
    fgets(alunos[*num_alunos].nome, MAX_NOME, stdin);
    alunos[*num_alunos].nome[strcspn(alunos[*num_alunos].nome, "\n")] = 0; // Remove o caractere de nova linha

    printf("Digite a senha do aluno: ");
    fgets(alunos[*num_alunos].senha, MAX_NOME, stdin);
    alunos[*num_alunos].senha[strcspn(alunos[*num_alunos].senha, "\n")] = 0; // Remove o caractere de nova linha

    alunos[*num_alunos].id = encontrar_proximo_id(alunos, *num_alunos);

    (*num_alunos)++;
    salvar_alunos(alunos, *num_alunos);
    printf("Aluno cadastrado com sucesso!\n");
}

void cadastrar_curso(Curso cursos[], int *num_cursos) {
    printf("Digite o nome do curso: ");
    fgets(cursos[*num_cursos].nome, MAX_NOME, stdin);
    cursos[*num_cursos].nome[strcspn(cursos[*num_cursos].nome, "\n")] = 0; // Remove o caractere de nova linha

    printf("Digite o preço por mês do curso: ");
    scanf("%f", &cursos[*num_cursos].precoPorMes);

    printf("Digite a duração do curso em meses: ");
    scanf("%d", &cursos[*num_cursos].duracaoMeses);
    getchar(); // Limpa o buffer de entrada

    cursos[*num_cursos].id = encontrar_proximo_id_curso(cursos, *num_cursos);

    (*num_cursos)++;
    salvar_cursos(cursos, *num_cursos);
    printf("Curso cadastrado com sucesso!\n");
}

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos, Vencimento vencimentos[], int *num_vencimentos) {
    int idCurso;
    printf("Digite o ID do curso: ");
    scanf("%d", &idCurso);
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].id == idCurso) {
            matriculas[*num_matriculas].id = encontrar_proximo_id_matricula(matriculas, *num_matriculas);
            matriculas[*num_matriculas].idAluno = idAluno;
            matriculas[*num_matriculas].idCurso = idCurso;
            matriculas[*num_matriculas].dataExpiracao = calcular_data_expiracao(cursos[i].duracaoMeses);

            // Criar vencimentos
            cadastrar_vencimento(vencimentos, num_vencimentos, &matriculas[*num_matriculas], *num_matriculas);

            (*num_matriculas)++;
            salvar_matriculas(matriculas, *num_matriculas);
            printf("Matrícula cadastrada com sucesso!\n");
            return;
        }
    }

    printf("Curso não encontrado.\n");
}

void carregar_alunos(Aluno alunos[], int *num_alunos) {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) {
        *num_alunos = 0;
        return;
    }

    *num_alunos = 0;
    while (fscanf(file, "%d %[^\n] %[^\n]", &alunos[*num_alunos].id, alunos[*num_alunos].nome, alunos[*num_alunos].senha) == 3) {
        (*num_alunos)++;
    }

    fclose(file);
}

void carregar_cursos(Curso cursos[], int *num_cursos) {
    FILE *file = fopen("cursos.txt", "r");
    if (file == NULL) {
        *num_cursos = 0;
        return;
    }

    *num_cursos = 0;
    while (fscanf(file, "%d %[^\n] %f %d", &cursos[*num_cursos].id, cursos[*num_cursos].nome, &cursos[*num_cursos].precoPorMes, &cursos[*num_cursos].duracaoMeses) == 4) {
        (*num_cursos)++;
    }

    fclose(file);
}

void carregar_matriculas(Matricula matriculas[], int *num_matriculas) {
    FILE *file = fopen("matriculas.txt", "r");
    if (file == NULL) {
        *num_matriculas = 0;
        return;
    }

    *num_matriculas = 0;
    while (fscanf(file, "%d %d %d %ld", &matriculas[*num_matriculas].id, &matriculas[*num_matriculas].idAluno, &matriculas[*num_matriculas].idCurso, &matriculas[*num_matriculas].dataExpiracao) == 4) {
        (*num_matriculas)++;
    }

    fclose(file);
}

void carregar_vencimentos(Vencimento vencimentos[], int *num_vencimentos) {
    FILE *file = fopen("vencimentos.txt", "r");
    if (file == NULL) {
        *num_vencimentos = 0;
        return;
    }

    *num_vencimentos = 0;
    while (fscanf(file, "%d %d %f %ld %d", &vencimentos[*num_vencimentos].idVencimento, &vencimentos[*num_vencimentos].idMatricula, &vencimentos[*num_vencimentos].valorParcela, &vencimentos[*num_vencimentos].dataVencimento, &vencimentos[*num_vencimentos].pago) == 5) {
        (*num_vencimentos)++;
    }

    fclose(file);
}

void salvar_alunos(Aluno alunos[], int num_alunos) {
    FILE *file = fopen("alunos.txt", "w");
    if (file == NULL) {
        printf("Erro ao salvar alunos.\n");
        return;
    }

    for (int i = 0; i < num_alunos; i++) {
        fprintf(file, "%d %s %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }

    fclose(file);
}

void salvar_cursos(Curso cursos[], int num_cursos) {
    FILE *file = fopen("cursos.txt", "w");
    if (file == NULL) {
        printf("Erro ao salvar cursos.\n");
        return;
    }

    for (int i = 0; i < num_cursos; i++) {
        fprintf(file, "%d %s %.2f %d\n", cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].duracaoMeses);
    }

    fclose(file);
}

void salvar_matriculas(Matricula matriculas[], int num_matriculas) {
    FILE *file = fopen("matriculas.txt", "w");
    if (file == NULL) {
        printf("Erro ao salvar matrículas.\n");
        return;
    }

    for (int i = 0; i < num_matriculas; i++) {
        fprintf(file, "%d %d %d %ld\n", matriculas[i].id, matriculas[i].idAluno, matriculas[i].idCurso, matriculas[i].dataExpiracao);
    }

    fclose(file);
}

void salvar_vencimentos(Vencimento vencimentos[], int num_vencimentos) {
    FILE *file = fopen("vencimentos.txt", "w");
    if (file == NULL) {
        printf("Erro ao salvar vencimentos.\n");
        return;
    }

    for (int i = 0; i < num_vencimentos; i++) {
        fprintf(file, "%d %d %.2f %ld %d\n", vencimentos[i].idVencimento, vencimentos[i].idMatricula, vencimentos[i].valorParcela, vencimentos[i].dataVencimento, vencimentos[i].pago);
    }

    fclose(file);
}

void exibir_alunos(Aluno alunos[], int num_alunos) {
    printf("\n======= Lista de Alunos =======\n");
    for (int i = 0; i < num_alunos; i++) {
        printf("ID: %d\n", alunos[i].id);
        printf("Nome: %s\n", alunos[i].nome);
        printf("-------------------------\n");
    }
}

void exibir_cursos(Curso cursos[], int num_cursos) {
    printf("\n======= Lista de Cursos =======\n");
    for (int i = 0; i < num_cursos; i++) {
        printf("ID: %d\n", cursos[i].id);
        printf("Nome: %s\n", cursos[i].nome);
        printf("Preço por Mês: %.2f\n", cursos[i].precoPorMes);
        printf("Duração (meses): %d\n", cursos[i].duracaoMeses);
        printf("-------------------------\n");
    }
}

void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos, Matricula matriculas[], int num_matriculas) {
    printf("\n======= Matrículas do Aluno %d =======\n", idAluno);
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].idAluno == idAluno) {
            char *nomeCurso = encontrar_nome_curso(matriculas[i].idCurso, cursos, num_cursos);
            printf("ID: %d\n", matriculas[i].id);
            printf("Curso: %s\n", nomeCurso);
            printf("Data de Expiração: %s", ctime(&matriculas[i].dataExpiracao));
            printf("-------------------------\n");
        }
    }
}

void exibir_vencimentos(Matricula matriculas[], int num_matriculas, Vencimento vencimentos[], int num_vencimentos) {
    printf("\n======= Lista de Vencimentos =======\n");
    for (int i = 0; i < num_vencimentos; i++) {
        printf("ID Vencimento: %d\n", vencimentos[i].idVencimento);
        printf("ID Matrícula: %d\n", vencimentos[i].idMatricula);
        printf("Valor da Parcela: %.2f\n", vencimentos[i].valorParcela);
        printf("Data de Vencimento: %s", ctime(&vencimentos[i].dataVencimento));
        printf("Pago: %s\n", vencimentos[i].pago ? "Sim" : "Não");
        printf("-------------------------\n");
    }
}

void editar_aluno(Aluno alunos[], int num_alunos) {
    int id;
    printf("Digite o ID do aluno a ser editado: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].id == id) {
            printf("Digite o novo nome do aluno (ou pressione Enter para manter o atual): ");
            char novo_nome[MAX_NOME];
            fgets(novo_nome, MAX_NOME, stdin);
            if (strcmp(novo_nome, "\n") != 0) {
                novo_nome[strcspn(novo_nome, "\n")] = 0; // Remove o caractere de nova linha
                strcpy(alunos[i].nome, novo_nome);
            }

            printf("Digite a nova senha do aluno (ou pressione Enter para manter a atual): ");
            char nova_senha[MAX_NOME];
            fgets(nova_senha, MAX_NOME, stdin);
            if (strcmp(nova_senha, "\n") != 0) {
                nova_senha[strcspn(nova_senha, "\n")] = 0; // Remove o caractere de nova linha
                strcpy(alunos[i].senha, nova_senha);
            }

            salvar_alunos(alunos, num_alunos);
            printf("Aluno editado com sucesso!\n");
            return;
        }
    }

    printf("Aluno não encontrado.\n");
}

void editar_curso(Curso cursos[], int num_cursos) {
    int id;
    printf("Digite o ID do curso a ser editado: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].id == id) {
            printf("Digite o novo nome do curso (ou pressione Enter para manter o atual): ");
            char novo_nome[MAX_NOME];
            fgets(novo_nome, MAX_NOME, stdin);
            if (strcmp(novo_nome, "\n") != 0) {
                novo_nome[strcspn(novo_nome, "\n")] = 0; // Remove o caractere de nova linha
                strcpy(cursos[i].nome, novo_nome);
            }

            printf("Digite o novo preço por mês do curso (ou pressione Enter para manter o atual): ");
            char novo_preco[20];
            fgets(novo_preco, 20, stdin);
            if (strcmp(novo_preco, "\n") != 0) {
                cursos[i].precoPorMes = atof(novo_preco);
            }

            printf("Digite a nova duração do curso em meses (ou pressione Enter para manter a atual): ");
            char nova_duracao[20];
            fgets(nova_duracao, 20, stdin);
            if (strcmp(nova_duracao, "\n") != 0) {
                cursos[i].duracaoMeses = atoi(nova_duracao);
            }

            salvar_cursos(cursos, num_cursos);
            printf("Curso editado com sucesso!\n");
            return;
        }
    }

    printf("Curso não encontrado.\n");
}

void editar_matricula(Matricula matriculas[], int num_matriculas, int idMatricula, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos) {
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id == idMatricula) {
            printf("Digite o novo ID do aluno (ou pressione Enter para manter o atual): ");
            char novo_idAluno[20];
            fgets(novo_idAluno, 20, stdin);
            if (strcmp(novo_idAluno, "\n") != 0) {
                matriculas[i].idAluno = atoi(novo_idAluno);
            }

            printf("Digite o novo ID do curso (ou pressione Enter para manter o atual): ");
            char novo_idCurso[20];
            fgets(novo_idCurso, 20, stdin);
            if (strcmp(novo_idCurso, "\n") != 0) {
                matriculas[i].idCurso = atoi(novo_idCurso);
                // Atualizar a data de expiração com base na nova duração do curso
                for (int j = 0; j < num_cursos; j++) {
                    if (cursos[j].id == matriculas[i].idCurso) {
                        matriculas[i].dataExpiracao = calcular_data_expiracao(cursos[j].duracaoMeses);
                        break;
                    }
                }
            }

            salvar_matriculas(matriculas, num_matriculas);
            printf("Matrícula editada com sucesso!\n");
            return;
        }
    }

    printf("Matrícula não encontrada.\n");
}

void excluir_aluno(Aluno alunos[], int *num_alunos, Matricula matriculas[], int *num_matriculas) {
    int id;
    printf("Digite o ID do aluno a ser excluído: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    // Verifica se o aluno está matriculado em algum curso
    for (int i = 0; i < *num_matriculas; i++) {
        if (matriculas[i].idAluno == id) {
            printf("O aluno está matriculado em um curso e não pode ser excluído.\n");
            return;
        }
    }

    int index = -1;
    for (int i = 0; i < *num_alunos; i++) {
        if (alunos[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Aluno não encontrado.\n");
        return;
    }

    for (int i = index; i < *num_alunos - 1; i++) {
        alunos[i] = alunos[i + 1];
    }

    (*num_alunos)--;
    salvar_alunos(alunos, *num_alunos);
    printf("Aluno excluído com sucesso!\n");
}

void excluir_curso(Curso cursos[], int *num_cursos, Matricula matriculas[], int *num_matriculas) {
    int id;
    printf("Digite o ID do curso a ser excluído: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    // Verifica se o curso tem alunos matriculados
    for (int i = 0; i < *num_matriculas; i++) {
        if (matriculas[i].idCurso == id) {
            printf("Existem alunos matriculados neste curso. Não é possível excluí-lo.\n");
            return;
        }
    }

    int index = -1;
    for (int i = 0; i < *num_cursos; i++) {
        if (cursos[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Curso não encontrado.\n");
        return;
    }

    for (int i = index; i < *num_cursos - 1; i++) {
        cursos[i] = cursos[i + 1];
    }

    (*num_cursos)--;
    salvar_cursos(cursos, *num_cursos);
    printf("Curso excluído com sucesso!\n");
}

void excluir_matricula(Matricula matriculas[], int *num_matriculas, int idAluno) {
    int id;
    printf("Digite o ID da matrícula a ser excluída: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    int index = -1;
    for (int i = 0; i < *num_matriculas; i++) {
        if (matriculas[i].id == id) {
            // Verifica se a matrícula pertence ao aluno
            if (idAluno != 0 && matriculas[i].idAluno != idAluno) {
                printf("A matrícula não pertence ao aluno.\n");
                return;
            }
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Matrícula não encontrada.\n");
        return;
    }

    for (int i = index; i < *num_matriculas - 1; i++) {
        matriculas[i] = matriculas[i + 1];
    }

    (*num_matriculas)--;
    salvar_matriculas(matriculas, *num_matriculas);
    printf("Matrícula excluída com sucesso!\n");
}

void cadastrar_vencimento(Vencimento vencimentos[], int *num_vencimentos, Matricula matriculas[], int num_matriculas) {
    if (*num_vencimentos >= MAX_VENCIMENTOS) {
        printf("Número máximo de vencimentos atingido.\n");
        return;
    }

    Vencimento novo_vencimento;
    novo_vencimento.idVencimento = (*num_vencimentos > 0) ? vencimentos[*num_vencimentos - 1].idVencimento + 1 : 1;

    printf("Digite o ID da matrícula: ");
    scanf("%d", &novo_vencimento.idMatricula);
    getchar(); // Limpa o buffer de entrada

    // Verifica se a matrícula existe
    int matricula_existe = 0;
    for (int i = 0; i < num_matriculas; i++) {
        if (matriculas[i].id == novo_vencimento.idMatricula) {
            matricula_existe = 1;
            break;
        }
    }

    if (!matricula_existe) {
        printf("Matrícula não encontrada.\n");
        return;
    }

    printf("Digite o valor do vencimento: ");
    scanf("%f", &novo_vencimento.valorParcela);
    getchar(); // Limpa o buffer de entrada

    char novo_vencimento_dataVencimento[11]; // Array to store the date
    printf("Digite a data de vencimento (YYYY-MM-DD): ");
    fgets(novo_vencimento_dataVencimento, sizeof(novo_vencimento_dataVencimento), stdin);
    novo_vencimento_dataVencimento[strcspn(novo_vencimento_dataVencimento, "\n")] = 0; // Remove o caractere de nova linha

    // Convert string date to time_t (if needed) and assign to novo_vencimento.dataVencimento
    // Example:
    // novo_vencimento.dataVencimento = convert_string_to_time_t(novo_vencimento_dataVencimento);

    novo_vencimento.pago = 0; // Por padrão, o vencimento não está pago

    vencimentos[*num_vencimentos] = novo_vencimento;
    (*num_vencimentos)++;
    salvar_vencimentos(vencimentos, *num_vencimentos);

    printf("Vencimento cadastrado com sucesso!\n");
}


void editar_vencimento(Vencimento vencimentos[], int num_vencimentos) {
    int id;
    printf("Digite o ID do vencimento a ser editado: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < num_vencimentos; i++) {
        if (vencimentos[i].idVencimento == id) {
            printf("Digite o novo valor do vencimento (ou pressione Enter para manter o atual): ");
            char novo_valor[20];
            fgets(novo_valor, 20, stdin);
            if (strcmp(novo_valor, "\n") != 0) {
                vencimentos[i].valor = atof(novo_valor);
            }

            printf("Digite a nova data de vencimento (YYYY-MM-DD) (ou pressione Enter para manter a atual): ");
            char nova_data[11];
            fgets(nova_data, 11, stdin);
            if (strcmp(nova_data, "\n") != 0) {
                nova_data[strcspn(nova_data, "\n")] = 0; // Remove o caractere de nova linha
                strcpy(vencimentos[i].dataVencimento, nova_data);
            }

            printf("Digite o novo status de pagamento (1 para pago, 0 para não pago) (ou pressione Enter para manter o atual): ");
            char novo_status[2];
            fgets(novo_status, 2, stdin);
            if (strcmp(novo_status, "\n") != 0) {
                vencimentos[i].pago = atoi(novo_status);
            }

            salvar_vencimentos(vencimentos, num_vencimentos);
            printf("Vencimento editado com sucesso!\n");
            return;
        }
    }

    printf("Vencimento não encontrado.\n");
}

void excluir_vencimento(Vencimento vencimentos[], int *num_vencimentos) {
    int id;
    printf("Digite o ID do vencimento a ser excluído: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    int index = -1;
    for (int i = 0; i < *num_vencimentos; i++) {
        if (vencimentos[i].idVencimento == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Vencimento não encontrado.\n");
        return;
    }

    for (int i = index; i < *num_vencimentos - 1; i++) {
        vencimentos[i] = vencimentos[i + 1];
    }

    (*num_vencimentos)--;
    salvar_vencimentos(vencimentos, *num_vencimentos);
    printf("Vencimento excluído com sucesso!\n");
}

void salvar_vencimentos(Vencimento vencimentos[], int num_vencimentos) {
    FILE *file = fopen("vencimentos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de vencimentos.\n");
        return;
    }

    for (int i = 0; i < num_vencimentos; i++) {
        fprintf(file, "%d %d %.2f %d %s\n", vencimentos[i].idVencimento, vencimentos[i].idMatricula, vencimentos[i].valor, vencimentos[i].pago, vencimentos[i].dataVencimento);
    }

    fclose(file);
}

void carregar_vencimentos(Vencimento vencimentos[], int *num_vencimentos) {
    FILE *file = fopen("vencimentos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de vencimentos.\n");
        return;
    }

    *num_vencimentos = 0;
    while (fscanf(file, "%d %d %f %d %s\n", &vencimentos[*num_vencimentos].idVencimento, &vencimentos[*num_vencimentos].idMatricula, &vencimentos[*num_vencimentos].valor, &vencimentos[*num_vencimentos].pago, vencimentos[*num_vencimentos].dataVencimento) != EOF) {
        (*num_vencimentos)++;
    }

    fclose(file);
}

void login_aluno(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int *num_matriculas, Vencimento vencimentos[], int *num_vencimentos) {
    int id;
    char senha[MAX_NOME];

    printf("Digite o ID do aluno: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    printf("Digite a senha: ");
    fgets(senha, MAX_NOME, stdin);
    senha[strcspn(senha, "\n")] = 0; // Remove o caractere de nova linha

    // Verificar se o ID e a senha são válidos
    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].id == id && strcmp(alunos[i].senha, senha) == 0) {
            int opcao_aluno;
            do {
                printf("\n======= Menu do Aluno =======\n");
                printf("1. Ver Cursos\n");
                printf("2. Ver Matrículas\n");
                printf("3. Ver Vencimentos\n");
                printf("4. Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao_aluno);
                getchar(); // Limpa o buffer de entrada

                switch (opcao_aluno) {
                    case 1:
                        exibir_cursos(cursos, num_cursos);
                        break;
                    case 2:
                        exibir_matriculas(id, cursos, num_cursos, alunos, num_alunos, matriculas, *num_matriculas);
                        break;
                    case 3:
                        exibir_vencimentos(matriculas, *num_matriculas, vencimentos, *num_vencimentos);
                        break;
                    case 4:
                        printf("Saindo...\n");
                        break;
                    default:
                        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                }
            } while (opcao_aluno != 4);
            return;
        }
    }

    printf("ID ou senha inválidos.\n");
}

void login_admin(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int *num_matriculas, Vencimento vencimentos[], int *num_vencimentos) {
    int id;
    char senha[MAX_NOME];

    printf("Digite o ID do administrador: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    printf("Digite a senha do administrador: ");
    fgets(senha, MAX_NOME, stdin);
    senha[strcspn(senha, "\n")] = 0; // Remove o caractere de nova linha

    // Verificar se o usuário é um administrador
    if (id == 1 && strcmp(senha, "admin") == 0) {
        int opcao_admin;
        do {
            printf("\n======= Menu do Administrador =======\n");
            printf("1. Gerenciar Alunos\n");
            printf("2. Gerenciar Cursos\n");
            printf("3. Gerenciar Matrículas\n");
            printf("4. Gerenciar Vencimentos\n");
            printf("5. Sair\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_admin);
            getchar(); // Limpa o buffer de entrada

            switch (opcao_admin) {
                case 1: {
                    int opcao_aluno;
                    do {
                        printf("\n======= Gerenciar Alunos =======\n");
                        printf("1. Cadastrar Aluno\n");
                        printf("2. Editar Aluno\n");
                        printf("3. Excluir Aluno\n");
                        printf("4. Listar Alunos\n");
                        printf("5. Voltar\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcao_aluno);
                        getchar(); // Limpa o buffer de entrada

                        switch (opcao_aluno) {
                            case 1:
                                cadastrar_aluno(alunos, &num_alunos);
                                break;
                            case 2:
                                editar_aluno(alunos, num_alunos);
                                break;
                            case 3:
                                excluir_aluno(alunos, &num_alunos, matriculas, num_matriculas);
                                break;
                            case 4:
                                exibir_alunos(alunos, num_alunos);
                                break;
                            case 5:
                                printf("Voltando...\n");
                                break;
                            default:
                                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                        }
                    } while (opcao_aluno != 5);
                    break;
                }
                case 2: {
                    int opcao_curso;
                    do {
                        printf("\n======= Gerenciar Cursos =======\n");
                        printf("1. Cadastrar Curso\n");
                        printf("2. Editar Curso\n");
                        printf("3. Excluir Curso\n");
                        printf("4. Listar Cursos\n");
                        printf("5. Voltar\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcao_curso);
                        getchar(); // Limpa o buffer de entrada

                        switch (opcao_curso) {
                            case 1:
                                cadastrar_curso(cursos, &num_cursos);
                                break;
                            case 2:
                                editar_curso(cursos, num_cursos);
                                break;
                            case 3:
                                excluir_curso(cursos, &num_cursos, matriculas, num_matriculas);
                                break;
                            case 4:
                                exibir_cursos(cursos, num_cursos);
                                break;
                            case 5:
                                printf("Voltando...\n");
                                break;
                            default:
                                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                        }
                    } while (opcao_curso != 5);
                    break;
                }
                case 3: {
                    int opcao_matricula;
                    do {
                        printf("\n======= Gerenciar Matrículas =======\n");
                        printf("1. Cadastrar Matrícula\n");
                        printf("2. Editar Matrícula\n");
                        printf("3. Excluir Matrícula\n");
                        printf("4. Listar Matrículas\n");
                        printf("5. Voltar\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcao_matricula);
                        getchar(); // Limpa o buffer de entrada

                        switch (opcao_matricula) {
                            case 1:
    cadastrar_matricula(matriculas, num_matriculas, 0, cursos, num_cursos, vencimentos, num_vencimentos);
    break;
                            case 2: {
                                int idMatricula;
                                printf("Digite o ID da matrícula a ser editada: ");
                                scanf("%d", &idMatricula);
                                getchar(); // Limpa o buffer de entrada
                                editar_matricula(matriculas, *num_matriculas, idMatricula, cursos, num_cursos, alunos, num_alunos);
                                break;
                            }
                            case 3:
                                excluir_matricula(matriculas, num_matriculas, 0);
                                break;
                            case 4:
                                exibir_matriculas(0, cursos, num_cursos, alunos, num_alunos, matriculas, *num_matriculas);
                                break;
                            case 5:
                                printf("Voltando...\n");
                                break;
                            default:
                                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                        }
                    } while (opcao_matricula != 5);
                    break;
                }
                case 4: {
                    int opcao_vencimento;
                    do {
                        printf("\n======= Gerenciar Vencimentos =======\n");
                        printf("1. Cadastrar Vencimento\n");
                        printf("2. Editar Vencimento\n");
                        printf("3. Excluir Vencimento\n");
                        printf("4. Listar Vencimentos\n");
                        printf("5. Voltar\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcao_vencimento);
                        getchar(); // Limpa o buffer de entrada

                        switch (opcao_vencimento) {
                            case 1:
                                cadastrar_vencimento(vencimentos, num_vencimentos, matriculas, *num_matriculas);
                                break;
                            case 2:
                                editar_vencimento(vencimentos, *num_vencimentos);
                                break;
                            case 3:
                                excluir_vencimento(vencimentos, num_vencimentos);
                                break;
                            case 4:
                                exibir_vencimentos(matriculas, *num_matriculas, vencimentos, *num_vencimentos);
                                break;
                            case 5:
                                printf("Voltando...\n");
                                break;
                            default:
                                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                        }
                    } while (opcao_vencimento != 5);
                    break;
                }
                case 5:
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha uma opção válida.\n");
            }
        } while (opcao_admin != 5);
    } else {
        printf("ID ou senha do administrador inválidos.\n");
    }
}

int main() {
    Aluno alunos[MAX_ALUNOS];
    int num_alunos = 0;
    Curso cursos[MAX_CURSOS];
    int num_cursos = 0;
    Matricula matriculas[MAX_MATRICULAS];
    int num_matriculas = 0;
    Vencimento vencimentos[MAX_VENCIMENTOS];
    int num_vencimentos = 0;

    carregar_alunos(alunos, &num_alunos);
    carregar_cursos(cursos, &num_cursos);
    carregar_matriculas(matriculas, &num_matriculas);
    carregar_vencimentos(vencimentos, &num_vencimentos);

    int opcao;
    do {
        printf("\n======= Menu Principal =======\n");
        printf("1. Login Aluno\n");
        printf("2. Login Administrador\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1:
                login_aluno(alunos, num_alunos, cursos, num_cursos, matriculas, &num_matriculas, vencimentos, &num_vencimentos);
                break;
            case 2:
                login_admin(alunos, num_alunos, cursos, num_cursos, matriculas, &num_matriculas, vencimentos, &num_vencimentos);
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 3);

    return 0;
}
