#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

#define MAX_NOME 100

typedef struct {
  int id;
  char nome[MAX_NOME];
  char senha[MAX_NOME];
}
Aluno;

typedef struct {
  int id;
  char nome[MAX_NOME];
  float precoPorMes;
  int numParcelas;
}
Curso;

typedef struct {
  int id;
  int idAluno;
  int idCurso;
  time_t expirationDate; // Expiration date for the registration
}
Matricula;

// Function Prototypes
void login_menu();
int login(Aluno alunos[], int num_alunos);
void student_menu(int idAluno, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void admin_menu(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void cadastrar_aluno(Aluno alunos[], int * num_alunos);
void editar_aluno(Aluno alunos[], int num_alunos);
void excluir_aluno(Aluno alunos[], int * num_alunos, Matricula matriculas[], int num_matriculas);
void cadastrar_curso(Curso cursos[], int * num_cursos);
void editar_curso(Curso cursos[], int num_cursos);
void excluir_curso(Curso cursos[], int * num_cursos, Matricula matriculas[], int num_matriculas);
void exibir_alunos(Aluno alunos[], int num_alunos);
void exibir_cursos(Curso cursos[], int num_cursos);
void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void adicionar_matricula(Matricula matriculas[], int * num_matriculas, int idAluno, Curso cursos[], int num_cursos);
void remover_matricula(Matricula matriculas[], int * num_matriculas, int idAluno);
int find_next_id(Aluno alunos[], int num_alunos);
int find_next_id_curso(Curso cursos[], int num_cursos);
int find_next_id_matricula(Matricula matriculas[], int num_matriculas);
void save_alunos(Aluno alunos[], int num_alunos);
void load_alunos(Aluno alunos[], int * num_alunos);
void save_cursos(Curso cursos[], int num_cursos);
void load_cursos(Curso cursos[], int * num_cursos);
void save_matriculas(Matricula matriculas[], int num_matriculas);
void load_matriculas(Matricula matriculas[], int * num_matriculas);
time_t calculate_expiration_date();

// Function Definitions

void login_menu() {
  printf("\n======= Login Menu =======\n");
  printf("1. Login\n");
  printf("2. Leave\n");
  printf("Choose an option: ");
}

int login(Aluno alunos[], int num_alunos) {
  char nome[MAX_NOME];
  char senha[MAX_NOME];
  printf("Enter username: ");
  fgets(nome, MAX_NOME, stdin);
  nome[strcspn(nome, "\n")] = 0; // Remove newline character
  printf("Enter password: ");
  fgets(senha, MAX_NOME, stdin);
  senha[strcspn(senha, "\n")] = 0; // Remove newline character

  if (strcmp("admin", nome) == 0 && strcmp("admin", senha) == 0) {
    return -1; // Admin user
  }

  for (int i = 0; i < num_alunos; i++) {
    if (strcmp(alunos[i].nome, nome) == 0 && strcmp(alunos[i].senha, senha) == 0) {
      return alunos[i].id; // Student user
    }
  }

  return 0; // Login failed
}

void student_menu(int idAluno, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
  int opcao;
  do {
    printf("\n======= Student Menu =======\n");
    printf("1. View available courses\n");
    printf("2. View my registrations\n");
    printf("3. Add new registration\n");
    printf("4. Logout\n");
    printf("Choose an option: ");
    scanf("%d", & opcao);
    getchar(); // Clear input buffer

    switch (opcao) {
    case 1:
      exibir_cursos(cursos, num_cursos);
      break;
    case 2:
      exibir_matriculas(idAluno, cursos, num_cursos, matriculas, num_matriculas);
      break;
    case 3:
      adicionar_matricula(matriculas, & num_matriculas, idAluno, cursos, num_cursos);
      break;
    case 4:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid option. Please choose a valid option.\n");
    }
  } while (opcao != 4);
}

void exibir_cursos(Curso cursos[], int num_cursos) {
  printf("\n======= Cursos Cadastrados =======\n");
  for (int i = 0; i < num_cursos; i++) {
    printf("ID: %d, Nome: %s, Preço do curso: %.2f, Parcelas: %d, Valor da Parcela: %.2f\n",
      cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas,
      cursos[i].precoPorMes / cursos[i].numParcelas);
  }
}

void admin_menu(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
  int opcao;
  do {
    printf("\n======= Admin Menu =======\n");
    printf("1. Manage students\n");
    printf("2. Manage courses\n");
    printf("3. Manage registrations\n");
    printf("4. Logout\n");
    printf("Choose an option: ");
    scanf("%d", & opcao);
    getchar(); // Clear input buffer

    switch (opcao) {
    case 1:
      // Student Management
      printf("\n======= Student Management =======\n");
      printf("1. Add student\n");
      printf("2. Edit student\n");
      printf("3. Delete student\n");
      printf("4. View students\n");
      printf("Choose an option: ");
      scanf("%d", & opcao);
      getchar(); // Clear input buffer
      switch (opcao) {
      case 1:
        cadastrar_aluno(alunos, & num_alunos);
        break;
      case 2:
        editar_aluno(alunos, num_alunos);
        break;
      case 3:
        excluir_aluno(alunos, & num_alunos, matriculas, num_matriculas);
        break;
      case 4:
        exibir_alunos(alunos, num_alunos);
        break;
      default:
        printf("Invalid option. Please choose a valid option.\n");
      }
      break;
    case 2:
      // Course Management
      printf("\n======= Course Management =======\n");
      printf("1. Add course\n");
      printf("2. Edit course\n");
      printf("3. Delete course\n");
      printf("4. View courses\n");
      printf("Choose an option: ");
      scanf("%d", & opcao);
      getchar(); // Clear input buffer
      switch (opcao) {
      case 1:
        cadastrar_curso(cursos, & num_cursos);
        break;
      case 2:
        editar_curso(cursos, num_cursos);
        break;
      case 3:
        excluir_curso(cursos, & num_cursos, matriculas, num_matriculas);
        break;
      case 4:
        exibir_cursos(cursos, num_cursos);
        break;
      default:
        printf("Invalid option. Please choose a valid option.\n");
      }
      break;
    case 3:
      // Registration Management
      printf("\n======= Registration Management =======\n");
      printf("1. View registrations\n");
      printf("2. Remove registration\n");
      printf("Choose an option: ");
      scanf("%d", & opcao);
      getchar(); // Clear input buffer
      switch (opcao) {
      case 1:
        exibir_matriculas(-1, cursos, num_cursos, matriculas, num_matriculas); // Display all registrations
        break;
      case 2:
        printf("Enter student ID to remove registration: ");
        int id;
        scanf("%d", & id);
        getchar(); // Clear input buffer
        remover_matricula(matriculas, & num_matriculas, id);
        break;
      default:
        printf("Invalid option. Please choose a valid option.\n");
      }
      break;
    case 4:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid option. Please choose a valid option.\n");
    }
  } while (opcao != 4);
}

void cadastrar_aluno(Aluno alunos[], int * num_alunos) {
  printf("Enter student name: ");
  fgets(alunos[ * num_alunos].nome, MAX_NOME, stdin);
  alunos[ * num_alunos].nome[strcspn(alunos[ * num_alunos].nome, "\n")] = 0; // Remove newline character
  printf("Enter password: ");
  fgets(alunos[ * num_alunos].senha, MAX_NOME, stdin);
  alunos[ * num_alunos].senha[strcspn(alunos[ * num_alunos].senha, "\n")] = 0; // Remove newline character
  alunos[ * num_alunos].id = find_next_id(alunos, * num_alunos);
  ( * num_alunos) ++;
  save_alunos(alunos, * num_alunos);
  printf("Student added successfully!\n");
}

void editar_aluno(Aluno alunos[], int num_alunos) {
  printf("Enter student ID to edit: ");
  int id;
  scanf("%d", & id);
  getchar(); // Clear input buffer
  for (int i = 0; i < num_alunos; i++) {
    if (alunos[i].id == id) {
      printf("Enter new name: ");
      fgets(alunos[i].nome, MAX_NOME, stdin);
      alunos[i].nome[strcspn(alunos[i].nome, "\n")] = 0; // Remove newline character
      printf("Enter new password: ");
      fgets(alunos[i].senha, MAX_NOME, stdin);
      alunos[i].senha[strcspn(alunos[i].senha, "\n")] = 0; // Remove newline character
      save_alunos(alunos, num_alunos);
      printf("Student information updated successfully!\n");
      return;
    }
  }
  printf("Student with ID %d not found.\n", id);
}

void excluir_aluno(Aluno alunos[], int * num_alunos, Matricula matriculas[], int num_matriculas) {
  printf("Enter student ID to delete: ");
  int id;
  scanf("%d", & id);
  getchar(); // Clear input buffer
  int found = 0;
  for (int i = 0; i < * num_alunos; i++) {
    if (alunos[i].id == id) {
      for (int j = 0; j < num_matriculas; j++) {
        if (matriculas[j].idAluno == id) {
          // Remove associated registrations
          for (int k = j; k < num_matriculas - 1; k++) {
            matriculas[k] = matriculas[k + 1];
          }
          num_matriculas--;
        }
      }
      // Remove student
      for (int j = i; j < * num_alunos - 1; j++) {
        alunos[j] = alunos[j + 1];
      }
      ( * num_alunos) --;
      save_alunos(alunos, * num_alunos);
      save_matriculas(matriculas, num_matriculas);
      found = 1;
      printf("Student deleted successfully!\n");
      break;
    }
  }
  if (!found) {
    printf("Student with ID %d not found.\n", id);
  }
}

void cadastrar_curso(Curso cursos[], int * num_cursos) {
  printf("Enter course name: ");
  fgets(cursos[ * num_cursos].nome, MAX_NOME, stdin);
  cursos[ * num_cursos].nome[strcspn(cursos[ * num_cursos].nome, "\n")] = 0; // Remove newline character
  printf("Enter price per month: ");
  scanf("%f", & cursos[ * num_cursos].precoPorMes);
  getchar(); // Clear input buffer
  printf("Enter number of installments: ");
  scanf("%d", & cursos[ * num_cursos].numParcelas);
  getchar(); // Clear input buffer
  cursos[ * num_cursos].id = find_next_id_curso(cursos, * num_cursos);
  ( * num_cursos) ++;
  save_cursos(cursos, * num_cursos);
  printf("Course added successfully!\n");
}

void editar_curso(Curso cursos[], int num_cursos) {
  printf("Enter course ID to edit: ");
  int id;
  scanf("%d", & id);
  getchar(); // Clear input buffer
  for (int i = 0; i < num_cursos; i++) {
    if (cursos[i].id == id) {
      printf("Enter new name: ");
      fgets(cursos[i].nome, MAX_NOME, stdin);
      cursos[i].nome[strcspn(cursos[i].nome, "\n")] = 0; // Remove newline character
      printf("Enter new price per month: ");
      scanf("%f", & cursos[i].precoPorMes);
      getchar(); // Clear input buffer
      printf("Enter new number of installments: ");
      scanf("%d", & cursos[i].numParcelas);
      getchar(); // Clear input buffer
      save_cursos(cursos, num_cursos);
      printf("Course information updated successfully!\n");
      return;
    }
  }
  printf("Course with ID %d not found.\n", id);
}

void excluir_curso(Curso cursos[], int * num_cursos, Matricula matriculas[], int num_matriculas) {
  printf("Enter course ID to delete: ");
  int id;
  scanf("%d", & id);
  getchar(); // Clear input buffer
  int found = 0;
  for (int i = 0; i < * num_cursos; i++) {
    if (cursos[i].id == id) {
      for (int j = 0; j < num_matriculas; j++) {
        if (matriculas[j].idCurso == id) {
          // Remove associated registrations
          for (int k = j; k < num_matriculas - 1; k++) {
            matriculas[k] = matriculas[k + 1];
          }
          num_matriculas--;
        }
      }
      // Remove course
      for (int j = i; j < * num_cursos - 1; j++) {
        cursos[j] = cursos[j + 1];
      }
      ( * num_cursos) --;
      save_cursos(cursos, * num_cursos);
      save_matriculas(matriculas, num_matriculas);
      found = 1;
      printf("Course deleted successfully!\n");
      break;
    }
  }
  if (!found) {
    printf("Course with ID %d not found.\n", id);
  }
}

void exibir_alunos(Aluno alunos[], int num_alunos) {
  printf("\n======= Registered Students =======\n");
  for (int i = 0; i < num_alunos; i++) {
    printf("ID: %d, Name: %s\n", alunos[i].id, alunos[i].nome);
  }
}

void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
  printf("\n======= Registrations =======\n");
  for (int i = 0; i < num_matriculas; i++) {
    if (idAluno == -1 || matriculas[i].idAluno == idAluno) {
      printf("Student ID: %d, Course ID: %d, Expiration Date: %s", matriculas[i].idAluno, matriculas[i].idCurso, ctime( & matriculas[i].expirationDate));
    }
  }
}

void adicionar_matricula(Matricula matriculas[], int * num_matriculas, int idAluno, Curso cursos[], int num_cursos) {
  int idCurso;
  printf("Enter course ID to register: ");
  scanf("%d", & idCurso);
  getchar(); // Clear input buffer
  int found = 0;
  for (int i = 0; i < num_cursos; i++) {
    if (cursos[i].id == idCurso) {
      matriculas[ * num_matriculas].idAluno = idAluno;
      matriculas[ * num_matriculas].idCurso = idCurso;
      matriculas[ * num_matriculas].expirationDate = calculate_expiration_date();
      ( * num_matriculas) ++;
      save_matriculas(matriculas, * num_matriculas);
      found = 1;
      printf("Registration added successfully!\n");
      break;
    }
  }
  if (!found) {
    printf("Course with ID %d not found.\n", idCurso);
  }
}

void remover_matricula(Matricula matriculas[], int * num_matriculas, int idAluno) {
  int found = 0;
  for (int i = 0; i < * num_matriculas; i++) {
    if (matriculas[i].idAluno == idAluno) {
      for (int j = i; j < * num_matriculas - 1; j++) {
        matriculas[j] = matriculas[j + 1];
      }
      ( * num_matriculas) --;
      save_matriculas(matriculas, * num_matriculas);
      found = 1;
      printf("Registration removed successfully!\n");
      break;
    }
  }
  if (!found) {
    printf("No registration found for student ID %d.\n", idAluno);
  }
}

int find_next_id(Aluno alunos[], int num_alunos) {
  if (num_alunos == 0) {
    return 1; // Start with ID 1 if no students are registered
  } else {
    return alunos[num_alunos - 1].id + 1; // Increment last ID
  }
}

int find_next_id_curso(Curso cursos[], int num_cursos) {
  if (num_cursos == 0) {
    return 1; // Start with ID 1 if no courses are registered
  } else {
    return cursos[num_cursos - 1].id + 1; // Increment last ID
  }
}

int find_next_id_matricula(Matricula matriculas[], int num_matriculas) {
  if (num_matriculas == 0) {
    return 1; // Start with ID 1 if no registrations are made
  } else {
    return matriculas[num_matriculas - 1].id + 1; // Increment last ID
  }
}

void save_alunos(Aluno alunos[], int num_alunos) {
  FILE * file = fopen("alunos.txt", "w");
  if (file != NULL) {
    for (int i = 0; i < num_alunos; i++) {
      fprintf(file, "%d %s %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
    fclose(file);
  } else {
    printf("Error saving students data.\n");
  }
}

void load_alunos(Aluno alunos[], int * num_alunos) {
  FILE * file = fopen("alunos.txt", "r");
  if (file != NULL) {
    * num_alunos = 0;
    while (fscanf(file, "%d %[^\n] %[^\n]\n", & alunos[ * num_alunos].id, alunos[ * num_alunos].nome, alunos[ * num_alunos].senha) != EOF) {
      ( * num_alunos) ++;
    }
    fclose(file);
  } else {
    printf("Error loading students data.\n");
  }
}

void save_cursos(Curso cursos[], int num_cursos) {
  FILE * file = fopen("cursos.txt", "w");
  if (file != NULL) {
    for (int i = 0; i < num_cursos; i++) {
      fprintf(file, "%d %s %.2f %d\n", cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas);
    }
    fclose(file);
  } else {
    printf("Error saving courses data.\n");
  }
}

void load_cursos(Curso cursos[], int * num_cursos) {
  FILE * file = fopen("cursos.txt", "r");
  if (file != NULL) {
    * num_cursos = 0;
    while (fscanf(file, "%d %[^\n] %f %d\n", & cursos[ * num_cursos].id, cursos[ * num_cursos].nome, & cursos[ * num_cursos].precoPorMes, & cursos[ * num_cursos].numParcelas) != EOF) {
      ( * num_cursos) ++;
    }
    fclose(file);
  } else {
    printf("Error loading courses data.\n");
  }
}

void save_matriculas(Matricula matriculas[], int num_matriculas) {
  FILE * file = fopen("matriculas.txt", "w");
  if (file != NULL) {
    for (int i = 0; i < num_matriculas; i++) {
      fprintf(file, "%d %d %ld\n", matriculas[i].idAluno, matriculas[i].idCurso, matriculas[i].expirationDate);
    }
    fclose(file);
  } else {
    printf("Error saving registrations data.\n");
  }
}

void load_matriculas(Matricula matriculas[], int * num_matriculas) {
  FILE * file = fopen("matriculas.txt", "r");
  if (file != NULL) {
    * num_matriculas = 0;
    while (fscanf(file, "%d %d %ld\n", & matriculas[ * num_matriculas].idAluno, & matriculas[ * num_matriculas].idCurso, & matriculas[ * num_matriculas].expirationDate) != EOF) {
      ( * num_matriculas) ++;
    }
    fclose(file);
  } else {
    printf("Error loading registrations data.\n");
  }
}

time_t calculate_expiration_date() {
  time_t now = time(NULL);
  struct tm * current_time = localtime( & now);
  current_time -> tm_mon++; // Increment month since January is represented by 0
  current_time -> tm_year += current_time -> tm_mon / 12; // Adjust year if month exceeds 12
  current_time -> tm_mon %= 12; // Ensure month is within 0-11
  return mktime(current_time);
}

int main() {
  Aluno alunos[100];
  Curso cursos[100];
  Matricula matriculas[100];
  int num_alunos = 0, num_cursos = 0, num_matriculas = 0;
  load_alunos(alunos, & num_alunos);
  load_cursos(cursos, & num_cursos);
  load_matriculas(matriculas, & num_matriculas);

  int opcao;
  do {
    login_menu();
    scanf("%d", & opcao);
    getchar(); // Clear input buffer

    switch (opcao) {
    case 1: {
      int id = login(alunos, num_alunos);
      if (id == -1) {
        admin_menu(alunos, num_alunos, cursos, num_cursos, matriculas, num_matriculas);
      } else if (id > 0) {
        student_menu(id, alunos, num_alunos, cursos, num_cursos, matriculas, num_matriculas);
      } else {
        printf("Invalid username or password. Please try again.\n");
      }
      break;
    }
    case 2:
      printf("Exiting program...\n");
      break;
    default:
      printf("Invalid option. Please choose a valid option.\n");
    }
  } while (opcao != 2);

  return 0;
}
