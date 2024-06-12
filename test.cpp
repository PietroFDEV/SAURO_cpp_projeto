#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Aluno {
    int id_aluno;
    char nome[100];
    char senha[50];
    struct Aluno* prox;
} Aluno;

typedef struct Curso {
    int id_curso;
    char nome[100];
    float mensalidade;
    int num_parcelas;
    struct Curso* prox;
} Curso;

typedef struct Matricula {
    int id_matricula;
    int id_aluno;
    int id_curso;
    struct Matricula* prox;
} Matricula;

typedef struct Mensalidade {
    int id_mensalidade;
    int id_matricula;
    time_t data_vencimento;
    float valor_parcela;
    int pago; // 1 para pago, 0 para não pago
    struct Mensalidade* prox;
} Mensalidade;

Aluno* alunos = NULL;
Curso* cursos = NULL;
Matricula* matriculas = NULL;
Mensalidade* mensalidades = NULL;

// Funções para manipulação de Aluno
Aluno* buscarAluno(int id) {
    Aluno* atual = alunos;
    while (atual != NULL) {
        if (atual->id_aluno == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

Aluno* buscarAlunoPorNome(char* nome) {
    Aluno* atual = alunos;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void criarAluno(int id, char* nome, char* senha) {
    Aluno* novo = (Aluno*)malloc(sizeof(Aluno));
    novo->id_aluno = id;
    strcpy(novo->nome, nome);
    strcpy(novo->senha, senha);
    novo->prox = alunos;
    alunos = novo;
}

void listarAlunos() {
    Aluno* atual = alunos;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s\n", atual->id_aluno, atual->nome);
        atual = atual->prox;
    }
}

// Funções para manipulação de Curso
Curso* buscarCurso(int id) {
    Curso* atual = cursos;
    while (atual != NULL) {
        if (atual->id_curso == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void criarCurso(int id, char* nome, float mensalidade, int num_parcelas) {
    Curso* novo = (Curso*)malloc(sizeof(Curso));
    novo->id_curso = id;
    strcpy(novo->nome, nome);
    novo->mensalidade = mensalidade;
    novo->num_parcelas = num_parcelas;
    novo->prox = cursos;
    cursos = novo;
}

void listarCursos() {
    Curso* atual = cursos;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s, Mensalidade: %.2f, Parcelas: %d\n", atual->id_curso, atual->nome, atual->mensalidade, atual->num_parcelas);
        atual = atual->prox;
    }
}

// Funções para manipulação de Matricula
Matricula* buscarMatricula(int id) {
    Matricula* atual = matriculas;
    while (atual != NULL) {
        if (atual->id_matricula == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void criarMatricula(int id, int id_aluno, int id_curso) {
    Matricula* novo = (Matricula*)malloc(sizeof(Matricula));
    novo->id_matricula = id;
    novo->id_aluno = id_aluno;
    novo->id_curso = id_curso;
    novo->prox = matriculas;
    matriculas = novo;
}

void listarMatriculas() {
    Matricula* atual = matriculas;
    while (atual != NULL) {
        printf("ID: %d, Aluno ID: %d, Curso ID: %d\n", atual->id_matricula, atual->id_aluno, atual->id_curso);
        atual = atual->prox;
    }
}

// Funções para manipulação de Mensalidade
Mensalidade* buscarMensalidade(int id) {
    Mensalidade* atual = mensalidades;
    while (atual != NULL) {
        if (atual->id_mensalidade == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void criarMensalidade(int id, int id_matricula, time_t data_vencimento, float valor_parcela) {
    Mensalidade* novo = (Mensalidade*)malloc(sizeof(Mensalidade));
    novo->id_mensalidade = id;
    novo->id_matricula = id_matricula;
    novo->data_vencimento = data_vencimento;
    novo->valor_parcela = valor_parcela;
    novo->pago = 0; // Inicia como não pago
    novo->prox = mensalidades;
    mensalidades = novo;
}

void listarMensalidades() {
    Mensalidade* atual = mensalidades;
    while (atual != NULL) {
        char buffer[26];
        struct tm* tm_info;
        tm_info = localtime(&atual->data_vencimento);
        strftime(buffer, 26, "%Y-%m-%d", tm_info);
        printf("ID: %d, Matricula ID: %d, Vencimento: %s, Valor: %.2f, Pago: %d\n", atual->id_mensalidade, atual->id_matricula, buffer, atual->valor_parcela, atual->pago);
        atual = atual->prox;
    }
}

void pagarMensalidade(int id) {
    Mensalidade* mensalidade = buscarMensalidade(id);
    if (mensalidade != NULL) {
        mensalidade->pago = 1;
        printf("Mensalidade %d paga com sucesso.\n", id);
    } else {
        printf("Mensalidade não encontrada.\n");
    }
}

// Funções de administração
void cadastrarAlunoCurso(int id_aluno, int id_curso) {
    Aluno* aluno = buscarAluno(id_aluno);
    Curso* curso = buscarCurso(id_curso);
    if (aluno != NULL && curso != NULL) {
        int id_matricula = rand(); // Gera um ID aleatório para a matrícula
        criarMatricula(id_matricula, id_aluno, id_curso);
        time_t data_atual = time(NULL);
        struct tm* tm_info = localtime(&data_atual);
        for (int i = 0; i < curso->num_parcelas; i++) {
            tm_info->tm_mon += 1;
            time_t data_vencimento = mktime(tm_info);
            int id_mensalidade = rand(); // Gera um ID aleatório para a mensalidade
            criarMensalidade(id_mensalidade, id_matricula, data_vencimento, curso->mensalidade);
        }
        printf("Aluno %d cadastrado no curso %d com sucesso.\n", id_aluno, id_curso);
    } else {
        printf("Aluno ou curso não encontrado.\n");
    }
}

// Funções de usuário
void visualizarMatriculas(int id_aluno) {
    Matricula* atual = matriculas;
    while (atual != NULL) {
        if (atual->id_aluno == id_aluno) {
            printf("ID Matricula: %d, ID Curso: %d\n", atual->id_matricula, atual->id_curso);
            Mensalidade* mensalidade_atual = mensalidades;
            while (mensalidade_atual != NULL) {
                if (mensalidade_atual->id_matricula == atual->id_matricula) {
                    char buffer[26];
                    struct tm* tm_info;
                    tm_info = localtime(&mensalidade_atual->data_vencimento);
                    strftime(buffer, 26, "%Y-%m-%d", tm_info);
                    printf("ID Mensalidade: %d, Vencimento: %s, Valor: %.2f, Pago: %d\n", mensalidade_atual->id_mensalidade, buffer, mensalidade_atual->valor_parcela, mensalidade_atual->pago);
                }
                mensalidade_atual = mensalidade_atual->prox;
            }
        }
        atual = atual->prox;
    }
}

// Funções para salvar e carregar dados
void salvarAlunos() {
    FILE* arquivo = fopen("alunos.dat", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar alunos");
        return;
    }
    Aluno* atual = alunos;
    while (atual != NULL) {
        fwrite(atual, sizeof(Aluno), 1, arquivo);
        atual = atual->prox;
    }
    fclose(arquivo);
}

void carregarAlunos() {
    FILE* arquivo = fopen("alunos.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para carregar alunos");
        return;
    }
    Aluno temp;
    while (fread(&temp, sizeof(Aluno), 1, arquivo)) {
        criarAluno(temp.id_aluno, temp.nome, temp.senha);
    }
    fclose(arquivo);
}

void salvarCursos() {
    FILE* arquivo = fopen("cursos.dat", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar cursos");
        return;
    }
    Curso* atual = cursos;
    while (atual != NULL) {
        fwrite(atual, sizeof(Curso), 1, arquivo);
        atual = atual->prox;
    }
    fclose(arquivo);
}

void carregarCursos() {
    FILE* arquivo = fopen("cursos.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para carregar cursos");
        return;
    }
    Curso temp;
    while (fread(&temp, sizeof(Curso), 1, arquivo)) {
        criarCurso(temp.id_curso, temp.nome, temp.mensalidade, temp.num_parcelas);
    }
    fclose(arquivo);
}

void salvarMatriculas() {
    FILE* arquivo = fopen("matriculas.dat", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar matrículas");
        return;
    }
    Matricula* atual = matriculas;
    while (atual != NULL) {
        fwrite(atual, sizeof(Matricula), 1, arquivo);
        atual = atual->prox;
    }
    fclose(arquivo);
}

void carregarMatriculas() {
    FILE* arquivo = fopen("matriculas.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para carregar matrículas");
        return;
    }
    Matricula temp;
    while (fread(&temp, sizeof(Matricula), 1, arquivo)) {
        criarMatricula(temp.id_matricula, temp.id_aluno, temp.id_curso);
    }
    fclose(arquivo);
}

void salvarMensalidades() {
    FILE* arquivo = fopen("mensalidades.dat", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar mensalidades");
        return;
    }
    Mensalidade* atual = mensalidades;
    while (atual != NULL) {
        fwrite(atual, sizeof(Mensalidade), 1, arquivo);
        atual = atual->prox;
    }
    fclose(arquivo);
}

void carregarMensalidades() {
    FILE* arquivo = fopen("mensalidades.dat", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para carregar mensalidades");
        return;
    }
    Mensalidade temp;
    while (fread(&temp, sizeof(Mensalidade), 1, arquivo)) {
        criarMensalidade(temp.id_mensalidade, temp.id_matricula, temp.data_vencimento, temp.valor_parcela);
        Mensalidade* nova = buscarMensalidade(temp.id_mensalidade);
        if (nova != NULL) {
            nova->pago = temp.pago; // Atualiza o status de pagamento
        }
    }
    fclose(arquivo);
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int login(char* nome, char* senha) {
    if (strcmp(nome, "admin") == 0 && strcmp(senha, "admin") == 0) {
        return 1; // Admin
    }
    Aluno* aluno = buscarAlunoPorNome(nome);
    if (aluno != NULL && strcmp(aluno->senha, senha) == 0) {
        return 2; // Aluno
    }
    return 0; // Falha no login
}

int main() {
    carregarAlunos();
    carregarCursos();
    carregarMatriculas();
    carregarMensalidades();

    char nome[100], senha[50];
    int opcao, id, id_aluno, id_curso;
    float mensalidade;
    int num_parcelas;
    int tipo_usuario;

    limparTela();
    printf("Login:\n");
    printf("Nome: ");
    scanf("%s", nome);
    printf("Senha: ");
    scanf("%s", senha);

    tipo_usuario = login(nome, senha);
    if (tipo_usuario == 0) {
        printf("Falha no login.\n");
        return 1;
    }

    while (1) {
        limparTela();
        if (tipo_usuario == 1) { // Admin
            printf("1. Criar Aluno\n");
            printf("2. Listar Alunos\n");
            printf("3. Criar Curso\n");
            printf("4. Listar Cursos\n");
            printf("5. Cadastrar Aluno em Curso\n");
            printf("6. Listar Matriculas\n");
            printf("7. Listar Mensalidades\n");
            printf("8. Pagar Mensalidade\n");
            printf("0. Sair\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            switch (opcao) {
                case 1:
                    printf("ID do Aluno: ");
                    scanf("%d", &id);
                    printf("Nome: ");
                    scanf("%s", nome);
                    printf("Senha: ");
                    scanf("%s", senha);
                    criarAluno(id, nome, senha);
                    break;
                case 2:
                    listarAlunos();
                    break;
                case 3:
                    printf("ID do Curso: ");
                    scanf("%d", &id);
                    printf("Nome: ");
                    scanf("%s", nome);
                    printf("Mensalidade: ");
                    scanf("%f", &mensalidade);
                    printf("Número de Parcelas: ");
                    scanf("%d", &num_parcelas);
                    criarCurso(id, nome, mensalidade, num_parcelas);
                    break;
                case 4:
                    listarCursos();
                    break;
                case 5:
                    printf("ID do Aluno: ");
                    scanf("%d", &id_aluno);
                    printf("ID do Curso: ");
                    scanf("%d", &id_curso);
                    cadastrarAlunoCurso(id_aluno, id_curso);
                    break;
                case 6:
                    listarMatriculas();
                    break;
                case 7:
                    listarMensalidades();
                    break;
                case 8:
                    printf("ID da Mensalidade: ");
                    scanf("%d", &id);
                    pagarMensalidade(id);
                    break;
                case 0:
                    salvarAlunos();
                    salvarCursos();
                    salvarMatriculas();
                    salvarMensalidades();
                    exit(0);
                default:
                    printf("Opção inválida!\n");
            }
        } else if (tipo_usuario == 2) { // Aluno
            printf("1. Visualizar Matrículas\n");
            printf("0. Sair\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            switch (opcao) {
                case 1:
                    printf("ID do Aluno: ");
                    scanf("%d", &id_aluno);
                    visualizarMatriculas(id_aluno);
                    break;
                case 0:
                    salvarAlunos();
                    salvarCursos();
                    salvarMatriculas();
                    salvarMensalidades();
                    exit(0);
                default:
                    printf("Opção inválida!\n");
            }
        }
        printf("Pressione Enter para continuar...\n");
        getchar();
        getchar();
    }

    return 0;
}

