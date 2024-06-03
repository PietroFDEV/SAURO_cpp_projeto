#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

// Defini��o da estrutura de aluno
typedef struct {
    int id;
    char name[MAX_NOME];
} Student;

// Defini��o da estrutura de curso
typedef struct {
    int id;
    char name[MAX_NOME];
    float pricePerMonth; // Pre�o por m�s
} Course;

// Defini��o da estrutura de matr�cula
typedef struct {
    int studentId;
    int courseId;
} Matricula;

void cadastrar_alunos_cursos(Student alunos[], int *num_alunos, Course cursos[], int *num_cursos) {
    int opcao;
    do{
	    printf("\n");
	    printf("======= Menu de Cadastro =======\n");
	    printf("1. Cadastrar aluno\n");
	    printf("2. Cadastrar curso\n");
	    printf("3. Voltar ao menu principal\n");
	    printf("Escolha uma op��o: ");
	    scanf("%d", &opcao);
	    getchar(); // Clear input buffer
	
	    switch (opcao) {
	        case 1:
	            // Cadastrar aluno
	            printf("Digite o ID do aluno: ");
	            scanf("%d", &alunos[*num_alunos].id);
	            getchar(); // Clear input buffer
	            printf("Digite o nome do aluno: ");
	            fgets(alunos[*num_alunos].name, MAX_NOME, stdin);
	            (*num_alunos)++;
	            printf("Aluno cadastrado com sucesso!\n");
	            break;
	        case 2:
	            // Cadastrar curso
	            printf("Digite o ID do curso: ");
	            scanf("%d", &cursos[*num_cursos].id);
	            getchar(); // Clear input buffer
	            printf("Digite o nome do curso: ");
	            fgets(cursos[*num_cursos].name, MAX_NOME, stdin);
		        printf("Digite o pre�o por m�s do curso: ");
		        scanf("%f", &cursos[*num_cursos].pricePerMonth);
	            (*num_cursos)++;
	            printf("Curso cadastrado com sucesso!\n");
	            break;
	        default:
	            printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
	    }
    } while (opcao != 3);
}


// Fun��o para cadastrar matr�culas
void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, Student alunos[], int num_alunos, Course cursos[], int num_cursos) {
    int studentId, courseId;
    printf("Digite o ID do aluno: ");
    scanf("%d", &studentId);
    printf("Digite o ID do curso: ");
    scanf("%d", &courseId);
    
    // Verificar se o aluno e o curso existem
    int alunoIndex = -1;
    int cursoIndex = -1;
    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].id == studentId) {
            alunoIndex = i;
            break;
        }
    }
    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].id == courseId) {
            cursoIndex = i;
            break;
        }
    }
    if (alunoIndex == -1 || cursoIndex == -1) {
        printf("Aluno ou curso n�o encontrado.\n");
        return;
    }

    // Preencher os detalhes da matr�cula
    matriculas[*num_matriculas].studentId = studentId;
    matriculas[*num_matriculas].courseId = courseId;
    (*num_matriculas)++;
    printf("Matr�cula cadastrada com sucesso!\n");
}

// Fun��o para exibir menu principal
void exibir_menu_principal() {
    printf("\n======= Menu Principal =======\n");
    printf("1. Cadastrar alunos e cursos\n");
    printf("2. Cadastrar matr�cula\n");
    printf("3. Ver alunos cadastrados\n");
    printf("4. Ver cursos cadastrados\n");
    printf("5. Sair\n");
    printf("Escolha uma op��o: ");
}

// Fun��o principal
int main() {
    // Declara��o de vari�veis
    Student alunos[100]; // Array para armazenar alunos (limite de 100)
    int num_alunos = 0; // N�mero atual de alunos cadastrados
    Course cursos[100]; // Array para armazenar cursos (limite de 100)
    int num_cursos = 0; // N�mero atual de cursos cadastrados
    Matricula matriculas[100]; // Array para armazenar matr�culas (limite de 100)
    int num_matriculas = 0; // N�mero atual de matr�culas

    int opcao;

    // Loop do menu principal
    do {
        exibir_menu_principal();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer de entrada

        switch (opcao) {
            case 1:
                // Cadastrar alunos e cursos
                cadastrar_alunos_cursos(alunos, &num_alunos, cursos, &num_cursos);
                break;
            case 2:
                // Cadastrar matr�cula
                cadastrar_matricula(matriculas, &num_matriculas, alunos, num_alunos, cursos, num_cursos);
                break;
            case 3:
                // Ver alunos cadastrados
                printf("\n======= Alunos Cadastrados =======\n");
                for (int i = 0; i < num_alunos; i++) {
                    printf("ID: %d, Nome: %s\n", alunos[i].id, alunos[i].name);
                }
                break;
            case 4:
                // Ver cursos cadastrados
                printf("\n======= Cursos Cadastrados =======\n");
                for (int i = 0; i < num_cursos; i++) {
                    printf("ID: %d, Nome: %s, Pre�o: %d\n", cursos[i].id, cursos[i].name, cursos[i].pricePerMonth);
                }
                break;
            case 5:
                // Sair do programa
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
    } while (opcao != 5);

    return 0;
}

