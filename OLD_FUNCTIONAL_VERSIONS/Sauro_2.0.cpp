#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_SENHA 20

typedef struct {
    int id;
    char nome[MAX_NOME];
} Aluno;

typedef struct {
    int id;
    char nome[MAX_NOME];
    float precoPorMes;
} Curso;

typedef struct {
    int idAluno;
    int idCurso;
} Matricula;

typedef struct {
    char nome[MAX_NOME];
    char senha[MAX_SENHA];
    int tipo; // 0 para admin, 1 para aluno
    int idAluno; // ID do aluno, se for do tipo aluno
} Usuario;

// Estrutura para n�s de lista simplesmente encadeada
typedef struct NodoAluno {
    Aluno aluno;
    struct NodoAluno* proximo;
} NodoAluno;

typedef struct NodoCurso {
    Curso curso;
    struct NodoCurso* proximo;
} NodoCurso;

typedef struct NodoMatricula {
    Matricula matricula;
    struct NodoMatricula* proximo;
} NodoMatricula;

// Estrutura para Pilha
typedef struct NodoPilha {
    Matricula matricula;
    struct NodoPilha* proximo;
} NodoPilha;

typedef struct {
    NodoPilha* topo;
} Pilha;

// Estrutura para Fila
typedef struct NodoFila {
    Matricula matricula;
    struct NodoFila* proximo;
} NodoFila;

typedef struct {
    NodoFila* frente;
    NodoFila* tras;
} Fila;

// Fun��es de inicializa��o de Pilha e Fila
void inicializarPilha(Pilha* pilha) {
    pilha->topo = NULL;
}

void inicializarFila(Fila* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

void limpar_tela() {
    system("clear");  // Use "cls" para Windows
}

// Fun��o de login
int login(Usuario usuarios[], int num_usuarios, Usuario *usuario_logado) {
    char nome[MAX_NOME];
    char senha[MAX_SENHA];

    printf("Nome de usu�rio: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0'; // Remover a nova linha

    printf("Senha: ");
    fgets(senha, MAX_SENHA, stdin);
    senha[strcspn(senha, "\n")] = '\0'; // Remover a nova linha

    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            *usuario_logado = usuarios[i];
            return 1;
        }
    }

    printf("Usu�rio ou senha incorretos.\n");
    return 0;
}

void cadastrar_alunos_cursos(NodoAluno** alunos, int* num_alunos, NodoCurso** cursos, int* num_cursos) {
    int opcao;
    do {
        printf("\n");
        printf("======= Menu de Cadastro =======\n");
        printf("1. Cadastrar aluno\n");
        printf("2. Cadastrar curso\n");
        printf("3. Voltar ao menu principal\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer de entrada

        switch (opcao) {
            case 1: {
                // Cadastrar aluno
                NodoAluno* novo_aluno = (NodoAluno*)malloc(sizeof(NodoAluno));
                printf("Digite o ID do aluno: ");
                scanf("%d", &novo_aluno->aluno.id);
                getchar(); // Limpar o buffer de entrada
                printf("Digite o nome do aluno: ");
                fgets(novo_aluno->aluno.nome, MAX_NOME, stdin);
                novo_aluno->aluno.nome[strcspn(novo_aluno->aluno.nome, "\n")] = '\0'; // Remover nova linha
                novo_aluno->proximo = *alunos;
                *alunos = novo_aluno;
                (*num_alunos)++;
                printf("Aluno cadastrado com sucesso!\n");
                break;
            }
            case 2: {
                // Cadastrar curso
                NodoCurso* novo_curso = (NodoCurso*)malloc(sizeof(NodoCurso));
                printf("Digite o ID do curso: ");
                scanf("%d", &novo_curso->curso.id);
                getchar(); // Limpar o buffer de entrada
                printf("Digite o nome do curso: ");
                fgets(novo_curso->curso.nome, MAX_NOME, stdin);
                novo_curso->curso.nome[strcspn(novo_curso->curso.nome, "\n")] = '\0'; // Remover nova linha
                printf("Digite o pre�o por m�s do curso: ");
                scanf("%f", &novo_curso->curso.precoPorMes);
                novo_curso->proximo = *cursos;
                *cursos = novo_curso;
                (*num_cursos)++;
                printf("Curso cadastrado com sucesso!\n");
                break;
            }
            case 3:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
    } while (opcao != 3);
}

// Fun��o para cadastrar matr�cula
void cadastrar_matricula(Pilha* pilha, Fila* fila, NodoAluno* alunos, NodoCurso* cursos) {
    int idAluno, idCurso;
    printf("Digite o ID do aluno: ");
    scanf("%d", &idAluno);
    printf("Digite o ID do curso: ");
    scanf("%d", &idCurso);

    // Verificar se o aluno e o curso existem
    NodoAluno* aluno = alunos;
    NodoCurso* curso = cursos;
    while (aluno != NULL && aluno->aluno.id != idAluno) {
        aluno = aluno->proximo;
    }
    while (curso != NULL && curso->curso.id != idCurso) {
        curso = curso->proximo;
    }
    if (aluno == NULL || curso == NULL) {
        printf("Aluno ou curso n�o encontrado.\n");
        return;
    }

    // Preencher os detalhes da matr�cula
    Matricula matricula;
    matricula.idAluno = idAluno;
    matricula.idCurso = idCurso;

    // Adicionar na pilha
    NodoPilha* novoNodoPilha = (NodoPilha*)malloc(sizeof(NodoPilha));
    novoNodoPilha->matricula = matricula;
    novoNodoPilha->proximo = pilha->topo;
    pilha->topo = novoNodoPilha;

    // Adicionar na fila
    NodoFila* novoNodoFila = (NodoFila*)malloc(sizeof(NodoFila));
    novoNodoFila->matricula = matricula;
    novoNodoFila->proximo = NULL;
    if (fila->tras == NULL) {
        fila->frente = novoNodoFila;
        fila->tras = novoNodoFila;
    } else {
        fila->tras->proximo = novoNodoFila;
        fila->tras = novoNodoFila;
    }

    printf("Matr�cula cadastrada com sucesso!\n");
}

// Fun��o para exibir alunos
void exibir_alunos(NodoAluno* alunos) {
    printf("\n======= Alunos Cadastrados =======\n");
    NodoAluno* atual = alunos;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s\n", atual->aluno.id, atual->aluno.nome);
        atual = atual->proximo;
    }
}

// Fun��o para exibir cursos
void exibir_cursos(NodoCurso* cursos) {
    printf("\n======= Cursos Cadastrados =======\n");
    NodoCurso* atual = cursos;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s, Pre�o: %.2f\n", atual->curso.id, atual->curso.nome, atual->curso.precoPorMes);
        atual = atual->proximo;
    }
}

// Fun��o para exibir matr�culas (Pilha)
void exibir_matriculas_pilha(Pilha* pilha) {
    printf("\n======= Matr�culas (Pilha) =======\n");
    NodoPilha* atual = pilha->topo;
    while (atual != NULL) {
        printf("ID Aluno: %d, ID Curso: %d\n", atual->matricula.idAluno, atual->matricula.idCurso);
        atual = atual->proximo;
    }
}

// Fun��o para exibir matr�culas (Fila)
void exibir_matriculas_fila(Fila* fila) {
    printf("\n======= Matr�culas (Fila) =======\n");
    NodoFila* atual = fila->frente;
    while (atual != NULL) {
        printf("ID Aluno: %d, ID Curso: %d\n", atual->matricula.idAluno, atual->matricula.idCurso);
        atual = atual->proximo;
    }
}

// Fun��o para exibir matr�culas de um aluno espec�fico
void exibir_matriculas_aluno(Pilha* pilha, Fila* fila, int idAluno) {
       printf("\n======= Matr�culas do Aluno %d =======\n", idAluno);
    NodoPilha* atualPilha = pilha->topo;
    while (atualPilha != NULL) {
        if (atualPilha->matricula.idAluno == idAluno) {
            printf("ID Curso: %d\n", atualPilha->matricula.idCurso);
        }
        atualPilha = atualPilha->proximo;
    }
    NodoFila* atualFila = fila->frente;
    while (atualFila != NULL) {
        if (atualFila->matricula.idAluno == idAluno) {
            printf("ID Curso: %d\n", atualFila->matricula.idCurso);
        }
        atualFila = atualFila->proximo;
    }
}

int main() {
    // Inicializa��o de vari�veis
    NodoAluno* alunos = NULL;
    int num_alunos = 0;
    NodoCurso* cursos = NULL;
    int num_cursos = 0;
    Pilha pilha_matriculas;
    Fila fila_matriculas;
    Usuario usuarios[2]; // Definindo dois usu�rios (admin e aluno)
    usuarios[0].tipo = 0; // Tipo 0 para admin
    strcpy(usuarios[0].nome, "admin");
    strcpy(usuarios[0].senha, "admin");
    usuarios[1].tipo = 1; // Tipo 1 para aluno
    strcpy(usuarios[1].nome, "aluno");
    strcpy(usuarios[1].senha, "12345");

    // Loop do menu de login
    Usuario usuario_logado;
    int login_sucesso = 0;
    do {
        limpar_tela();
        printf("======= Menu de Login =======\n");
        login_sucesso = login(usuarios, 2, &usuario_logado);
    } while (!login_sucesso);

    // Loop do menu principal
    int opcao;
    do {
        limpar_tela();
        if (usuario_logado.tipo == 0) {
            // Menu para admin
            printf("Bem-vindo, admin!\n");
            printf("\n======= Menu Principal =======\n");
            printf("1. Cadastrar alunos e cursos\n");
            printf("2. Cadastrar matr�cula\n");
            printf("3. Ver alunos cadastrados\n");
            printf("4. Ver cursos cadastrados\n");
            printf("5. Ver matr�culas (Pilha)\n");
            printf("6. Ver matr�culas (Fila)\n");
            printf("7. Sair\n");
            printf("Escolha uma op��o: ");
            scanf("%d", &opcao);
            getchar(); // Limpar o buffer de entrada
        } else {
            // Menu para aluno
            printf("Bem-vindo, %s!\n", usuario_logado.nome);
            printf("\n======= Menu Principal =======\n");
            printf("1. Ver minhas matr�culas\n");
            printf("2. Sair\n");
            printf("Escolha uma op��o: ");
            scanf("%d", &opcao);
            getchar(); // Limpar o buffer de entrada
        }

        switch (opcao) {
            case 1:
                if (usuario_logado.tipo == 0) {
                    // Admin
                    cadastrar_alunos_cursos(&alunos, &num_alunos, &cursos, &num_cursos);
                } else {
                    // Aluno
                    exibir_matriculas_aluno(&pilha_matriculas, &fila_matriculas, usuario_logado.idAluno);
                }
                break;
            case 2:
                if (usuario_logado.tipo == 0) {
                    // Admin
                    cadastrar_matricula(&pilha_matriculas, &fila_matriculas, alunos, cursos);
                } else {
                    // Aluno
                    printf("Op��o inv�lida.\n");
                }
                break;
            case 3:
                if (usuario_logado.tipo == 0) {
                    // Admin
                    exibir_alunos(alunos);
                } else {
                    // Aluno
                    printf("Op��o inv�lida.\n");
                }
                break;
            case 4:
                if (usuario_logado.tipo == 0) {
                    // Admin
                    exibir_cursos(cursos);
                } else {
                    // Aluno
                    printf("Op��o inv�lida.\n");
                }
                break;
            case 5:
                if (usuario_logado.tipo == 0) {
                    // Admin
                    exibir_matriculas_pilha(&pilha_matriculas);
                } else {
                    // Aluno
                    printf("Op��o inv�lida.\n");
                }
                break;
            case 6:
                if (usuario_logado.tipo == 0) {
                    // Admin
                    exibir_matriculas_fila(&fila_matriculas);
                } else {
                    // Aluno
                    printf("Op��o inv�lida.\n");
                }
                break;
            case 7:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
    } while (opcao != 7);

    // Libera��o de mem�ria (se houver)
    NodoAluno* tempAluno;
    while (alunos != NULL) {
        tempAluno = alunos;
        alunos = alunos->proximo;
        free(tempAluno);
    }

    NodoCurso* tempCurso;
    while (cursos != NULL) {
        tempCurso = cursos;
        cursos = cursos->proximo;
        free(tempCurso);
    }

    NodoPilha* tempPilha;
    while (pilha_matriculas.topo != NULL) {
        tempPilha = pilha_matriculas.topo;
        pilha_matriculas.topo = pilha_matriculas.topo->proximo;
        free(tempPilha);
    }

    NodoFila* tempFila;
    while (fila_matriculas.frente != NULL) {
        tempFila = fila_matriculas.frente;
        fila_matriculas.frente = fila_matriculas.frente->proximo;
        free(tempFila);
    }

    return 0;
}


