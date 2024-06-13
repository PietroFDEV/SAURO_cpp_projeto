#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 100
#define MAX_LINHA 256

typedef struct Aluno {
    int id;
    char nome[MAX_NOME];
    char senha[MAX_NOME];
    struct Aluno* next;
} Aluno;

typedef struct Curso {
    int id;
    char nome[MAX_NOME];
    float precoPorMes;
    int numParcelas;
    struct Curso* next;
} Curso;

typedef struct Vencimento {
	int id;
	int matriculaId;
    time_t data;
    float valor;
    int pago;
    struct Vencimento* next;
} Vencimento;


typedef struct Matricula {
    int id;
    int idAluno;
    int idCurso;
    int meses;
    Vencimento* vencimentos;
    struct Matricula* next;
} Matricula;

// Protótipos de Funcao
void cadastrar_aluno(Aluno** alunos);
void cadastrar_curso(Curso** cursos);
void cadastrar_matricula(Matricula** matriculas, int idAluno, Curso* cursos, Vencimento** vencimentos);
Aluno* carregar_alunos();
Curso* carregar_cursos();
Vencimento* carregar_vencimentos();
void editar_aluno(Aluno* alunos);
void editar_curso(Curso* cursos);
void editar_matricula(Matricula* matricula, Curso* cursos, Aluno* alunos, int idMatricula);
void excluir_aluno(Aluno** alunos, Matricula** matriculas);
void excluir_curso(Curso** cursos, Matricula** matriculas);
void excluir_matricula(Matricula** matriculas, int idAluno);
void exibir_alunos(Aluno* alunos);
void exibir_cursos(Curso* cursos);
void exibir_matriculas(int idAluno, Curso* cursos, Aluno* alunos, Matricula* matriculas, Vencimento* vencimentos);
void limpar_console();
void menu_admin(Aluno** alunos, Curso** cursos, Matricula** matriculas, Vencimento** vencimentos);
void menu_aluno(int idAluno, Aluno* alunos, Curso* cursos, Matricula* matriculas, Vencimento* vencimentos);
void menu_login();
void menu_relatorios(Curso* cursos, Aluno* alunos, Matricula* matriculas, Vencimento* vencimentos);
int login(Aluno* alunos);
int encontrar_proximo_id(Aluno* alunos);
int encontrar_proximo_id_curso(Curso* cursos);
int encontrar_proximo_id_matricula(Matricula* matriculas);
int encontrar_proximo_id_vencimento(Vencimento* vencimentos);
time_t parse_date(const char* date_str);
Matricula* carregar_matriculas(Curso* cursos);
Aluno* encontrar_aluno_por_id(int idAluno, Aluno* alunos);
Curso* encontrar_curso_por_id(int idCurso, Curso* cursos);
Matricula* encontrar_matricula_por_id(int idMatricula, Matricula* matriculas);
Vencimento* carregar_vencimentos_aluno(int idMatricula);
void salvar_alunos(Aluno* alunos);
void salvar_cursos(Curso* cursos);
void salvar_matriculas(Matricula* matriculas);
time_t calcular_data_expiracao();
int custom_strptime(const char *buf, const char *format, struct tm *tm);

// Funcões relacionadas aos vencimentos
Vencimento* criar_vencimento(int id, time_t data, float valor, int pago);
void exibir_vencimentos(Vencimento* vencimentos, Aluno* alunos, Matricula* matriculas);
void salvar_vencimentos(Vencimento* vencimentos);
void liberar_vencimentos(Vencimento* vencimentos);
void swap(Vencimento* a, Vencimento* b);
void excluir_vencimento_admin(Matricula* matriculas, Vencimento* vencimentos);
void menu_admin_vencimentos(Matricula* matriculas, Curso* cursos, Aluno* alunos, Vencimento* vencimentos);
void editar_vencimento_admin(Matricula* matriculas);
void criar_vencimento_admin(Matricula* matriculas, Curso* cursos);
void pagar_vencimento(int idAluno, Matricula* matriculas, Aluno* alunos);

void pressione_enter(){
	printf("\nPressione Enter para continuar...\n");
	getchar(); 
}

int custom_strptime(const char *buf, const char *format, struct tm *tm) {
    int day, month, year;
    if (sscanf(buf, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0;
    }
    tm->tm_mday = day;
    tm->tm_mon = month - 1;
    tm->tm_year = year - 1900; // ano minimo 1900
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1;
    return 1;
}

Vencimento* criar_vencimento(int id, time_t data, float valor, int pago) {
    Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
    novo_vencimento->id = id;
    novo_vencimento->data = data;
    novo_vencimento->valor = valor;
    novo_vencimento->pago = pago;
    novo_vencimento->next = NULL;
    return novo_vencimento;
}

void swap(Vencimento* a, Vencimento* b) {
    int tempId = a->id;
    int tempMatriculaId = a->matriculaId;
    time_t tempData = a->data;
    float tempValor = a->valor;
    int tempPago = a->pago;

    a->id = b->id;
    a->matriculaId = b->matriculaId;
    a->data = b->data;
    a->valor = b->valor;
    a->pago = b->pago;

    b->id = tempId;
    b->matriculaId = tempMatriculaId;
    b->data = tempData;
    b->valor = tempValor;
    b->pago = tempPago;
}

void exibir_vencimentos(Vencimento* vencimentos, Aluno* alunos, Matricula* matriculas) {
	printf("\n======= Vencimentos =======\n");
	Vencimento* currentV = vencimentos;
	if(currentV != NULL){
		while(currentV != NULL){
		Matricula* matricula = encontrar_matricula_por_id(currentV->matriculaId, matriculas);
    	Aluno* aluno = encontrar_aluno_por_id(matricula->idAluno, alunos);
    	char data_str[100];
        struct tm* data = localtime(&currentV->data);
        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data);
        printf("Matricula: %d - Aluno: %s - Vencimento: %d - Data: %s - Valor: %.2f - Pago: %s\n", matricula->id, aluno->nome, currentV->id, data_str, currentV->valor, currentV->pago == 0 ? "Nao" : "Sim");
		currentV = currentV->next;
		}
		
	} else {
		printf("\nNenhum vencimento encontrado.");
	}
	
}

void salvar_vencimentos(Vencimento *vencimentos) {
    FILE* file = fopen("vencimentos.txt", "w");
    if (!file) {
        perror("Erro ao abrir o arquivo de vencimentos");
        return;
    }
    
    while(vencimentos != NULL){
    	fprintf(file, "%d,%d,%ld,%.2f,%d\n", vencimentos->id, vencimentos->matriculaId, vencimentos->data, vencimentos->valor, vencimentos->pago);
    	vencimentos = vencimentos->next;
	}
    
    fclose(file);
}

void menu_admin_vencimentos(Matricula* matriculas, Curso* cursos, Aluno* alunos, Vencimento* vencimentos) {
    int opcao;
    do {
    	limpar_console();
        printf("======= Menu Administrativo -> Gerenciamento de Vencimentos =======\n");
        printf("1. Exibir vencimentos\n");
        printf("2. Excluir vencimento\n");
        printf("3. Voltar\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();
		limpar_console();
        switch (opcao) {
			case 1:{
            	printf("======= Menu Administrativo -> Gerenciamento de Vencimentos -> Exibir =======\n");
			    exibir_vencimentos(vencimentos, alunos, matriculas);
			    pressione_enter();
			    break;
			}
            case 2:
            	printf("======= Menu Administrativo -> Gerenciamento de Vencimentos -> Excluir =======\n");
                excluir_vencimento_admin(matriculas, vencimentos);
                pressione_enter();
                break;
            case 3:
                break;
            default:
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
        }
    } while (opcao != 3);
}

void menu_relatorios(Curso* cursos, Aluno* alunos, Matricula* matriculas, Vencimento* vencimentos){
	int opcao;
	do {
		limpar_console();
        printf("======= Menu Administrativo -> Relatorios =======\n");
        printf("1. Relatorio de Valores Recebidos\n");
        printf("2. Relatorio de Matriculas\n");
        printf("3. Voltar\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();
		limpar_console();
		switch (opcao) {
			case 1: {
			
				printf("======= Menu Administrativo -> Relatorios -> Valores =======\n");
				int trocado;
			    Vencimento *ptr1;
			    Vencimento *lptr = NULL;
			
			    if (vencimentos == NULL)
			        return;
			
			    do {
			        trocado = 0;
			        ptr1 = vencimentos;
			
			        while (ptr1->next != lptr) {
			            if (ptr1->valor < ptr1->next->valor) { 
			                swap(ptr1, ptr1->next);
			                trocado = 1;
			            }
			            ptr1 = ptr1->next;
			        }
			        lptr = ptr1;
			    } while (trocado);
			    
			    printf("Relatorio de valores recebidos (ordenado por valor em ordem decrescente):\n");
			    Vencimento* current = vencimentos;
			    float valorTotal = NULL;
			    while (current != NULL) {
			    	if(current->pago == 1){
			    		char data_str[100];
				        struct tm* data = localtime(&current->data);
				        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data);
			    		printf("ID matricula: %d|Data: %s|Valor: R$%.2f|Pago: %s\n", 
			            current->matriculaId, data_str, current->valor, current->pago == 0 ? "Nao" : "Sim");
			            valorTotal = valorTotal + current->valor;
					}
			        current = current->next;
			    }
			    printf("Valor total recebido: R$%f\n", valorTotal);
			    pressione_enter();
				break;
			}
			default:
				printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
		}
	} while (opcao != 3);
}

void liberar_vencimentos(Vencimento* vencimentos) {
    while (vencimentos != NULL) {
        Vencimento* temp = vencimentos;
        vencimentos = vencimentos->next;
        free(temp);
    }
}

void excluir_vencimento_admin(Matricula* matriculas, Vencimento* vencimentos) {
    int idMatricula;
    printf("Informe o ID da matricula: ");
    scanf("%d", &idMatricula);

    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->id == idMatricula) {
        	int idVencimento;
            printf("Informe o id do vencimento a excluir: ");
            scanf("%d", &idVencimento);
            Vencimento* previous = NULL;
            Vencimento* vencimento = vencimentos;
            while(vencimento != NULL){
            	if(vencimento->id == idVencimento){
            		if(previous == NULL){
            			vencimentos = vencimento->next;
					} else {
						previous->next = vencimento->next;
					}
					free(vencimento);
					salvar_vencimentos(vencimentos);
					printf("\nVencimento excluido com sucesso.\n");
					return;
				}
				previous = vencimento;
				vencimento = vencimento->next;
			}
			printf("\nVencimento nao encontrado.\n");
        }
        matricula = matricula->next;
    }
    printf("\nMatricula nao encontrada.\n");
}

void editar_vencimento_admin(Matricula* matriculas) {
    int idMatricula;
    printf("Informe o ID da matricula: ");
    scanf("%d", &idMatricula);

    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->id == idMatricula) {
            time_t data;
            struct tm tm_data;
            char data_str[100];

            printf("Informe a data do vencimento a editar (dd/mm/aaaa): ");
            scanf("%s", data_str);
            if (!custom_strptime(data_str, "%d/%m/%Y", &tm_data)) {
                printf("Formato de data invalido.\n");
                return;
            }
            data = mktime(&tm_data);

            Vencimento* vencimento = matricula->vencimentos;
            while (vencimento != NULL) {
                if (difftime(vencimento->data, data) == 0) {
                    printf("Informe o novo valor do vencimento: ");
                    scanf("%f", &vencimento->valor);

                    printf("Informe o novo status de pagamento (0 para nao pago, 1 para pago): ");
                    scanf("%d", &vencimento->pago);

                    printf("\nVencimento editado com sucesso.\n");
                    return;
                }
                vencimento = vencimento->next;
            }
            printf("\nVencimento nao encontrado.\n");
            return;
        }
        matricula = matricula->next;
    }
    printf("\nMatricula nao encontrada.\n");
}

void criar_vencimento_admin(Matricula* matriculas, Curso* cursos) {
    int idMatricula;
    printf("Informe o ID da matricula: ");
    scanf("%d", &idMatricula);

    Matricula* matricula = matriculas;
    if(matricula != NULL){
    	while (matricula != NULL) {
	        if (matricula->id == idMatricula) {
	            time_t now = time(NULL);
	            struct tm tm_data;
	            char data_str[100];
	            float valor;
	            int pago;
	
	            printf("Informe a data do vencimento (dd/mm/aaaa): ");
	            scanf("%s", data_str);
	            if (!custom_strptime(data_str, "%d/%m/%Y", &tm_data)) {
	                printf("Formato de data invalido.\n");
	                return;
	            }
	            time_t data = mktime(&tm_data);

	            printf("Informe o valor do vencimento: ");
	            scanf("%f", &valor);
	
	            printf("Informe o status de pagamento (0 para nao pago, 1 para pago): ");
	            scanf("%d", &pago);
	
	            printf("\nVencimento criado com sucesso.\n");
	            return;
	        }
        	matricula = matricula->next;
    	}
    	return;
	} else {
		printf("\nMatricula nao encontrada.\n");
		return;
	}
}

void pagar_vencimento(int idAluno, Matricula* matriculas, Aluno* alunos, Vencimento* vencimentos) {
    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->idAluno == idAluno) {
        	Aluno* aluno = encontrar_aluno_por_id(matricula->idAluno, alunos);
        	        	
        	int idVenc;
        	printf("\nInforme o ID do vencimento que deseja pagar: ");
	        scanf("%d", &idVenc);
        	
            Vencimento* vencimento = vencimentos;
            
            while(vencimento != NULL){
            	if(vencimento->matriculaId == matricula->id && vencimento->id == idVenc){
            		vencimento->pago = 1;
            		salvar_vencimentos(vencimentos);
	                printf("Vencimento pago com sucesso.\n");
	                pressione_enter();
	                return;
				}
            	vencimento = vencimento->next;
			}
			printf("\nId de vencimento nao encontrado.");
            pressione_enter();
            return;
        }
        matricula = matricula->next;
    }
    printf("Nao ha vencimentos pendentes para pagar.\n");
}

void limpar_console() {
  #ifdef _WIN32
  system("cls");
  #else
  system("clear");
  #endif
}

int login(Aluno* alunos) {
	char nome[MAX_NOME];
	char senha[MAX_NOME];
	printf("======= Login =======\n\n");
	printf("Digite o nome de usuario: ");
	fgets(nome, MAX_NOME, stdin);
	nome[strcspn(nome, "\n")] = 0; // Remove o caractere de nova linha
	printf("Digite a senha: ");
	fgets(senha, MAX_NOME, stdin);
	senha[strcspn(senha, "\n")] = 0; // Remove o caractere de nova linha
	
	if (strcmp("admin", nome) == 0 && strcmp("admin", senha) == 0) {
		return -1; // Usuario administrador
	}
	
	Aluno* current = alunos;
	while (current != NULL) {
	    if (strcmp(current->nome, nome) == 0 && strcmp(current->senha, senha) == 0) {
	    	return current->id; // Usuario aluno
	    }
	    current = current->next;
	}
  
	printf("\nUsuario ou senha errados, tente novamente.");
    pressione_enter();
	return 0; // Falha no login
}

void menu_login() {
  printf("======= Menu de Login =======\n");
  printf("1. Entrar\n");
  printf("2. Fechar programa\n");
  printf("\nEscolha uma opcao: ");
}

void menu_aluno(int idAluno, Aluno* alunos, Curso* cursos, Matricula* matriculas, Vencimento** vencimentos) {
	Aluno* logado = alunos;
	while(logado != NULL){
		if(logado->id == idAluno){
			int opcao;
				do {
					limpar_console();
					printf("======= Menu do Aluno - %s =======\n", logado->nome);
				    printf("1. Ver cursos disponiveis\n");
				    printf("2. Ver minhas matriculas\n");
				    printf("3. Cadastrar em um curso\n");
				    printf("4. Ver vencimentos\n");
				    printf("5. Pagar vencimento\n");
				    printf("6. Sair\n");
				    printf("\nEscolha uma opcao: ");
				    scanf("%d", & opcao);
				    getchar();
				    limpar_console();
				
				    switch (opcao) {
					    case 1:
					    	exibir_cursos(cursos);
					    	pressione_enter();
					      	break;
					    case 2:
					    	exibir_matriculas(idAluno, cursos, alunos, matriculas, *vencimentos);
					      	break;
					    case 3:
							cadastrar_matricula(&matriculas, idAluno, cursos, vencimentos);
							pressione_enter();
					      	break;
					    case 4: {
					    	Matricula* currentMatricula = matriculas;
					    	if(currentMatricula != NULL){
					    		while (currentMatricula != NULL) {
					    			if(currentMatricula->idAluno == idAluno){
					    				Vencimento* currentV = *vencimentos;
								    	while(currentV != NULL){
							    			if(currentV->matriculaId == currentMatricula->id){
							    				char data_str[100];
										        struct tm* data = localtime(&currentV->data);
										        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data);
										        printf("Matricula: %d|Aluno: %s|Vencimento ID: %d|Data: %s|Valor: %.2f|Pago: %s\n", currentMatricula->id, logado->nome, currentV->id, data_str, currentV->valor, currentV->pago == 0 ? "Nao" : "Sim");
											}
											currentV = currentV->next;
										}
										pressione_enter();
									}
									currentMatricula = currentMatricula->next;
								}
							} else {
								printf("Nenhum vencimento encontrado.\n");
								pressione_enter();
							}
							break;
						}
					    case 5: {
					    	printf("======= Pagar vencimento =======\n");
					    	Matricula* currentMatricula = matriculas;
					    	if(currentMatricula != NULL){
					    		while (currentMatricula != NULL) {
					    			if(currentMatricula->idAluno == idAluno){
					    				Vencimento* currentV = *vencimentos;
								    	while(currentV != NULL){
							    			if(currentV->matriculaId == currentMatricula->id){
							    				char data_str[100];
										        struct tm* data = localtime(&currentV->data);
										        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data);
										        printf("Matricula: %d|Aluno: %s|Vencimento ID: %d|Data: %s|Valor: %.2f|Pago: %s\n", currentMatricula->id, logado->nome, currentV->id, data_str, currentV->valor, currentV->pago == 0 ? "Nao" : "Sim");
											}
											currentV = currentV->next;
										}
									}
									currentMatricula = currentMatricula->next;
								}
							} else {
								printf("Nenhum vencimento encontrado.\n");
								pressione_enter();
	  							break;
							}
					    	pagar_vencimento(idAluno, matriculas, alunos, *vencimentos);
					    	break;
						}
					    case 6:
					      printf("Saindo...\n");
					      break;
					    default:
					      printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
					}
				} while (opcao != 6);
		}
		logado = logado->next;
	}
  
}

void menu_admin(Aluno** alunos, Curso** cursos, Matricula** matriculas, Vencimento** vencimentos) {
    int opcao_admin_menu;
    do {
    	limpar_console();
        printf("======= Menu Administrativo =======\n");
        printf("1. Gerenciamento de Alunos\n");
        printf("2. Gerenciamento de Cursos\n");
        printf("3. Gerenciamento de Matriculas\n");
        printf("4. Gerenciamento de Vencimentos\n");
        printf("5. Relatorios\n");
        printf("6. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao_admin_menu);
        getchar();
        limpar_console();
        int opcao_admin;

        switch (opcao_admin_menu) {
        case 1:
            // Gerenciamento de Alunos
            printf("======= Menu Administrativo -> Gerenciamento de Alunos =======\n");
            printf("1. Adicionar aluno\n");
            printf("2. Editar aluno\n");
            printf("3. Excluir aluno\n");
            printf("4. Ver alunos\n");
            printf("5. Voltar\n");
            printf("\nEscolha uma opcao: ");
            scanf("%d", &opcao_admin);
            getchar();
            limpar_console();
            switch (opcao_admin) {
            case 1:
                cadastrar_aluno(alunos);
                pressione_enter();
                break;
            case 2:
                editar_aluno(*alunos);
                pressione_enter();
                break;
            case 3:
            	exibir_alunos(*alunos);
            	printf("\n======= Menu Administrativo -> Gerenciamento de Alunos -> Excluir Aluno =======\n");
                excluir_aluno(alunos, matriculas);
                pressione_enter();
                break;
            case 4:
                exibir_alunos(*alunos);
                 pressione_enter();
                break;
            case 5:
            	break;
            default:
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
            }
            break;
        case 2:
            // Gerenciamento de Cursos
            printf("======= Menu Administrativo -> Gerenciamento de Cursos =======\n");
            printf("1. Adicionar curso\n");
            printf("2. Editar curso\n");
            printf("3. Excluir curso\n");
            printf("4. Ver cursos\n");
            printf("5. Voltar\n");
            printf("\nEscolha uma opcao: ");
            scanf("%d", &opcao_admin);
            getchar();
            limpar_console();
            switch (opcao_admin) {
            case 1:
                cadastrar_curso(cursos);
                pressione_enter();
                break;
            case 2:
                editar_curso(*cursos);
                pressione_enter();
                break;
            case 3:
            	exibir_cursos(*cursos);
                excluir_curso(cursos, matriculas);
                pressione_enter();
                break;
            case 4:
                exibir_cursos(*cursos);
                pressione_enter();
                break;
            case 5:
            	printf("Voltando..\n");
            	break;
            default:
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
            }
            break;
        case 3:
            // Gerenciamento de Matriculas
            printf("======= Menu Administrativo -> Gerenciamento de Matriculas =======\n");
            printf("1. Ver matriculas\n");
            printf("2. Remover matricula\n");
            printf("3. Editar matricula\n");
            printf("4. Voltar\n");
            printf("\nEscolha uma opcao: ");
            scanf("%d", &opcao_admin);
            getchar();
            limpar_console();
            switch (opcao_admin) {
            case 1:
                exibir_matriculas(-1, *cursos, *alunos, *matriculas, *vencimentos);
                break;
            case 2:
                printf("Digite o ID da matricula para remover: ");
                int id;
                scanf("%d", &id);
                getchar();
                excluir_matricula(matriculas, id);
                pressione_enter();
                break;
            case 3:
                printf("Digite o ID da matricula para editar: ");
                int idMatricula;
                scanf("%d", &idMatricula);
                getchar();
                editar_matricula(*matriculas, *cursos, *alunos, idMatricula);
                pressione_enter();
                break;
            case 4:
            	printf("Voltando..\n");
            	break;
            default:
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
            }
            break;
        case 4:
            menu_admin_vencimentos(*matriculas, *cursos, *alunos, *vencimentos);
            break;
        case 5:
        	menu_relatorios(*cursos, *alunos, *matriculas, *vencimentos);
        	break;
        case 6:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
            getchar();
        }
    } while (opcao_admin_menu != 6);
}

void exibir_cursos(Curso* cursos) {
  limpar_console();
  printf("======= Cursos Disponiveis =======\n");
  Curso* current = cursos;
  if(current != NULL){
	while (current != NULL) {
		printf("ID: %d, Nome: %s, Preco por mes: %.2f, Parcelas: %d\n", current->id, current->nome, current->precoPorMes, current->numParcelas);
	    current = current->next;
    }
  } else {
  	printf("\nNenhum curso encontrado.");
  	return;
  }
  
}

void exibir_alunos(Aluno* alunos) {
  limpar_console();
  printf("======= Alunos Cadastrados =======\n");
  Aluno* current = alunos;
  while (current != NULL) {
    printf("ID: %d, Nome: %s, Senha: %s\n", current->id, current->nome, current->senha);
    current = current->next;
  }
}

void exibir_matriculas(int idAluno, Curso* cursos, Aluno* alunos, Matricula* matriculas, Vencimento* vencimentos) {
    if (idAluno != -1) {
    	limpar_console();
        printf("======= Matriculas =======\n");
        Matricula* current = matriculas;
        if(current != NULL){
        	while (current != NULL) {
            if (idAluno == -1 || current->idAluno == idAluno) {
                Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
                Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
                printf("ID da Matricula: %d, ID do Aluno: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
                   current->id, current->idAluno, 
                   aluno != NULL ? aluno->nome : "Aluno nao encontrado", 
                   curso != NULL ? curso->nome : "Curso nao encontrado", 
                   current->meses
				);
            }
            current = current->next;            
        }
        pressione_enter();
		} else {
			printf("\nNenhuma matricula encontrada.");
			return;
		}
        
    } else {
    	int opcao;
    	do{
    		limpar_console();
	        printf("======= Matriculas =======\n");
	        printf("1. Exibir todas as matriculas\n");
	        printf("2. Filtrar por ID do Aluno\n");
	        printf("3. Filtrar por ID do Curso\n");
	        printf("4. Filtrar por Numero de Meses\n");
	        printf("5. Voltar\n");
	        printf("\nEscolha uma opcao: ");
	
	        
	        scanf("%d", &opcao);
	        getchar();
	        limpar_console();
			
	        Matricula* current;
	        switch (opcao) {
	            case 1:
	                current = matriculas;
	                if(current != NULL){
	                	while (current != NULL) {
		                    Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
		                    Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
		                    printf("ID da Matricula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
		                    current->id, 
		                    aluno != NULL ? aluno->nome : "Aluno nao encontrado", 
		                    curso != NULL ? curso->nome : "Curso nao encontrado", 
		                    current->meses);
		                           
		                    current = current->next;
	                	}
					} else {
						printf("Nenhuma matricula encontrada.\n");
					}
	                pressione_enter();
	                break;
	            case 2:
	                {
	                	exibir_alunos(alunos);
	                    int idAlunoFiltro;
	                    printf("Digite o ID do Aluno: ");
	                    scanf("%d", &idAlunoFiltro);
	                    getchar();
	                    limpar_console();
	                    current = matriculas;
	                    while (current != NULL) {
	                        if (current->idAluno == idAlunoFiltro) {
	                            Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
	                            Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
	                            printf("\nID da Matricula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
	                                current->id, 
	                                aluno != NULL ? aluno->nome : "Aluno nao encontrado", 
	                                curso != NULL ? curso->nome : "Curso nao encontrado", 
	                                current->meses
								);
								
								Matricula* currentMatricula = matriculas;
						    	if(currentMatricula != NULL){
						    		while (currentMatricula != NULL) {
						    			if(currentMatricula->idAluno == idAlunoFiltro){
						    				Vencimento* currentV = vencimentos;
									    	while(currentV != NULL){
								    			if(currentV->matriculaId == currentMatricula->id){
								    				char data_str[100];
											        struct tm* data = localtime(&currentV->data);
											        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data);
											        printf("Matricula: %d|Vencimento ID: %d|Data: %s|Valor: %.2f|Pago: %s\n", currentMatricula->id, currentV->id, data_str, currentV->valor, currentV->pago == 0 ? "Nao" : "Sim");
												}
												currentV = currentV->next;
											}
										}
										currentMatricula = currentMatricula->next;
									}
								} else {
									printf("Nenhum vencimento encontrado.\n");
								}
	                        }
	                        current = current->next;
	                    }
	                }
	                pressione_enter();
	                break;
	            case 3:
	                {
	                    int idCursoFiltro;
	                    printf("Digite o ID do Curso: ");
	                    scanf("%d", &idCursoFiltro);
	                    getchar();
	                    limpar_console();
	                    current = matriculas;
	                    while (current != NULL) {
	                        if (current->idCurso == idCursoFiltro) {
	                            Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
	                            Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
	                            printf("ID da Matricula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
	                                   current->id, 
	                                   aluno != NULL ? aluno->nome : "Aluno nao encontrado", 
	                                   curso != NULL ? curso->nome : "Curso nao encontrado", 
	                                   current->meses);
	                        }
	                        current = current->next;
	                    }
	                }
	                pressione_enter();
	                break;
	            case 4:
	                {
	                	exibir_cursos(cursos);
	                    int mesesFiltro;
	                    printf("Digite o numero de meses: ");
	                    scanf("%d", &mesesFiltro);
	                    getchar();
	                    limpar_console();
	                    current = matriculas;
	                    while (current != NULL) {
	                        if (current->meses == mesesFiltro) {
	                            Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
	                            Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
	                            printf("ID da Matricula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
	                                   current->id, 
	                                   aluno != NULL ? aluno->nome : "Aluno nao encontrado", 
	                                   curso != NULL ? curso->nome : "Curso nao encontrado", 
	                                   current->meses);
	                        }
	                        current = current->next;
	                    }
	                }
	                pressione_enter();
	                break;
	            case 5:
	                printf("Retornando ao menu anterior...\n");
	                break;
	            default:
	                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
	        }
    	}while(opcao != 5);
    }
}

void cadastrar_aluno(Aluno** alunos) {
    Aluno* novo_aluno = (Aluno*)malloc(sizeof(Aluno));
    novo_aluno->id = encontrar_proximo_id(*alunos);
    printf("======= Menu Administrativo -> Gerenciamento de Alunos -> Cadastro =======\n");
    printf("\nDigite o nome do aluno: ");
    fgets(novo_aluno->nome, MAX_NOME, stdin);
    novo_aluno->nome[strcspn(novo_aluno->nome, "\n")] = 0; 
    printf("Digite a senha do aluno: ");
    fgets(novo_aluno->senha, MAX_NOME, stdin);
    novo_aluno->senha[strcspn(novo_aluno->senha, "\n")] = 0;
    novo_aluno->next = *alunos;
    *alunos = novo_aluno;
    salvar_alunos(*alunos);
    printf("\nAluno cadastrado com sucesso.\n");
}

void cadastrar_curso(Curso** cursos) {
  Curso* novo_curso = (Curso*)malloc(sizeof(Curso));
  novo_curso->id = encontrar_proximo_id_curso(*cursos);
  printf("Digite o nome do curso: ");
  fgets(novo_curso->nome, MAX_NOME, stdin);
  novo_curso->nome[strcspn(novo_curso->nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite o preco por mes do curso: ");
  scanf("%f", &novo_curso->precoPorMes);
  printf("Digite o numero de parcelas do curso: ");
  scanf("%d", &novo_curso->numParcelas);
  getchar();
  novo_curso->next = *cursos;
  *cursos = novo_curso;
  salvar_cursos(*cursos);
  printf("\nCurso cadastrado com sucesso.\n");
}

void cadastrar_matricula(Matricula** matriculas, int idAluno, Curso* cursos, Vencimento** vencimentos) {
    Matricula* nova_matricula = (Matricula*)malloc(sizeof(Matricula));
    nova_matricula->id = encontrar_proximo_id_matricula(*matriculas);
    nova_matricula->idAluno = idAluno;
    exibir_cursos(cursos);
    printf("Digite o ID do curso: ");
    int idCurso;
    scanf("%d", &idCurso);
    getchar();

	Curso* currentC = cursos;
	while(currentC != NULL){
		if(currentC->id == idCurso){
			nova_matricula->idCurso = idCurso;
		    int numMeses = currentC->numParcelas;
		    
		    nova_matricula->meses = numMeses;
		
		    time_t hoje = time(NULL);
		    for (int i = 0; i < numMeses; i++) {
		        Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
		        novo_vencimento->id = i + 1;
		        novo_vencimento->matriculaId = nova_matricula->id;
		        novo_vencimento->pago = 0;
		        novo_vencimento->data = hoje + (i * 30 * 24 * 60 * 60);
		        novo_vencimento->valor = currentC->precoPorMes;
		        novo_vencimento->next = *vencimentos;
		        *vencimentos = novo_vencimento;
		    }
		    nova_matricula->next = *matriculas;
		    *matriculas = nova_matricula;
		    salvar_matriculas(*matriculas);
		    salvar_vencimentos(*vencimentos);
		    printf("\nMatricula cadastrada com sucesso.\n");
		    return;
		}
		currentC = currentC->next;
	}
	printf("\nCurso nao encontrado.\n");
	return;
}

void editar_aluno(Aluno* alunos) {
  exibir_alunos(alunos);
  int id;
  printf("\nDigite o ID do aluno a ser editado: ");
  scanf("%d", &id);
  getchar();
  Aluno* current = alunos;
  while (current != NULL) {
    if (current->id == id) {
      printf("Digite o novo nome do aluno: ");
      fgets(current->nome, MAX_NOME, stdin);
      current->nome[strcspn(current->nome, "\n")] = 0; // Remove o caractere de nova linha
      printf("Digite a nova senha do aluno: ");
      fgets(current->senha, MAX_NOME, stdin);
      current->senha[strcspn(current->senha, "\n")] = 0; // Remove o caractere de nova linha
      salvar_alunos(alunos);
      printf("\nAluno editado com sucesso.\n");
      return;
    }
    current = current->next;
  }
  printf("\nAluno nao encontrado.\n");
}

void editar_curso(Curso* cursos) {
  exibir_cursos(cursos);
  int id;
  printf("\nDigite o ID do curso a ser editado: ");
  scanf("%d", &id);
  getchar();
  Curso* current = cursos;
  while (current != NULL) {
    if (current->id == id) {
      printf("Digite o novo nome do curso: ");
      fgets(current->nome, MAX_NOME, stdin);
      current->nome[strcspn(current->nome, "\n")] = 0; 
      printf("Digite o novo preco por mes do curso: ");
      scanf("%f", &current->precoPorMes);
      printf("Digite o novo numero de parcelas do curso: ");
      scanf("%d", &current->numParcelas);
      getchar();
      salvar_cursos(cursos);
      printf("\nCurso editado com sucesso.\n");
      return;
    }
    current = current->next;
  }
  printf("\nCurso nao encontrado.\n");
}

void editar_matricula(Matricula* matricula, Curso* cursos, Aluno* alunos, int idMatricula) {
	Matricula* currentM = matricula;
	while(currentM != NULL){
		if(currentM->id == idMatricula){
			int cursoId;
		    printf("\nDigite o novo ID do curso: ");
		    scanf("%d", &cursoId);
		    getchar();
		    Curso* currentC = cursos;
		    while(currentC != NULL){
		    	if(currentC->id == cursoId){
		    		currentM->idCurso = cursoId;
		    		printf("Digite a nova quantidade de meses para a matricula: ");
				    int numMeses;
				    scanf("%d", &numMeses);
				    getchar();
				
				    liberar_vencimentos(currentM->vencimentos);
				    currentM->vencimentos = NULL;
				
				    time_t hoje = time(NULL);
				    for (int i = 0; i < numMeses; i++) {
				        Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
				        novo_vencimento->data = hoje + (i * 30 * 24 * 60 * 60);
				        novo_vencimento->valor = currentC->precoPorMes;
				        novo_vencimento->next = NULL;
				
				        if (i == 0) {
				            currentM->vencimentos = novo_vencimento;
				        } else {
				            Vencimento* ultimo = currentM->vencimentos;
				            while (ultimo->next != NULL) {
				                ultimo = ultimo->next;
				            }
				            ultimo->next = novo_vencimento;
				        }
				    }
				
				    salvar_matriculas(currentM);
				    printf("\nMatricula editada com sucesso.\n");
				    return;
				}
				currentC = currentC->next;
			}
			printf("\nCurso nao encontrado.\n");
		}
		currentM = currentM->next;
	}
	printf("\nMatricula nao encontrada.\n");
}

void excluir_aluno(Aluno** alunos, Matricula** matriculas) {
  int id;
  printf("\nDigite o ID do aluno a ser excluido: ");
  scanf("%d", &id);
  getchar();
  Aluno* current = *alunos;
  Aluno* previous = NULL;
  while (current != NULL) {
    if (current->id == id) {
      if (previous == NULL) {
        *alunos = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      salvar_alunos(*alunos);
      excluir_matricula(matriculas, id);
      printf("\nAluno excluido com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("\nAluno nao encontrado.\n");
}

void excluir_curso(Curso** cursos, Matricula** matriculas) {
  int id;
  printf("Digite o ID do curso a ser excluido: ");
  scanf("%d", &id);
  getchar();
  Curso* current = *cursos;
  Curso* previous = NULL;
  while (current != NULL) {
    if (current->id == id) {
      if (previous == NULL) {
        *cursos = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      salvar_cursos(*cursos);
      printf("\nCurso excluido com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("\nCurso nao encontrado.\n");
}

void excluir_matricula(Matricula** matriculas, int idMatricula) {
    Matricula* current = *matriculas;
    Matricula* previous = NULL;
    while (current != NULL) {
        if (current->idAluno == idMatricula) {
            if (previous == NULL) {
                *matriculas = current->next;
            } else {
                previous->next = current->next;
            }
            liberar_vencimentos(current->vencimentos);
            free(current);
            salvar_matriculas(*matriculas);
            printf("\nMatricula excluida com sucesso.\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("\nMatricula nao encontrada.\n");
	return;
}

int encontrar_proximo_id(Aluno* alunos) {
  int max_id = 0;
  Aluno* current = alunos;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

int encontrar_proximo_id_curso(Curso* cursos) {
  int max_id = 0;
  Curso* current = cursos;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

int encontrar_proximo_id_matricula(Matricula* matriculas) {
  int max_id = 0;
  Matricula* current = matriculas;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

int encontrar_proximo_id_vencimento(Vencimento* vencimentos) {
  int max_id = 0;
  Vencimento* current = vencimentos;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

Vencimento* carregar_vencimentos(){
	FILE* file = fopen("vencimentos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de vencimentos");
        return NULL;
    }
    
    Vencimento* head = NULL;
    Vencimento* current = NULL;
    char linha[MAX_LINHA];
    
    while (fgets(linha, sizeof(linha), file)) {
        Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
        sscanf(linha, "%d,%d,%ld,%f,%d", &novo_vencimento->id, &novo_vencimento->matriculaId, &novo_vencimento->data, &novo_vencimento->valor, &novo_vencimento->pago);
        novo_vencimento->next = NULL;

        if (!head) {
            head = novo_vencimento;
        } else {
            current->next = novo_vencimento;
        }
        current = novo_vencimento;
    }
    
    fclose(file);
    return head;
}

Vencimento* carregar_vencimentos_aluno(int idMatricula) {
    FILE* file = fopen("vencimentos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de vencimentos");
        return NULL;
    }

    Vencimento* head = NULL;
    Vencimento* current = NULL;
    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), file)) {
        int id;
		int matriculaId;
        time_t data;
        float valor;
        int pago;

        sscanf(linha, "%d,%d,%ld,%f,%d", &id, &matriculaId, &data, &valor, &pago);
        if (matriculaId == idMatricula) {
            Vencimento* novo_vencimento = criar_vencimento(id, data, valor, pago);
            if (!head) {
                head = novo_vencimento;
            } else {
                current->next = novo_vencimento;
            }
            current = novo_vencimento;
        }
    }

    fclose(file);
    return head;
}

Aluno* carregar_alunos() {
    FILE* file = fopen("alunos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de alunos");
        return NULL;
    }

    Aluno* head = NULL;
    Aluno* current = NULL;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        Aluno* novo_aluno = (Aluno*)malloc(sizeof(Aluno));
        sscanf(linha, "%d,%99[^,],%99[^\n]", &novo_aluno->id, novo_aluno->nome, novo_aluno->senha);
        novo_aluno->next = NULL;

        if (!head) {
            head = novo_aluno;
        } else {
            current->next = novo_aluno;
        }
        current = novo_aluno;
    }

    fclose(file);
    return head;
}

Curso* carregar_cursos() {
    FILE* file = fopen("cursos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de cursos");
        return NULL;
    }

    Curso* head = NULL;
    Curso* current = NULL;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        Curso* novo_curso = (Curso*)malloc(sizeof(Curso));
        sscanf(linha, "%d,%99[^,],%f,%d", &novo_curso->id, novo_curso->nome, &novo_curso->precoPorMes, &novo_curso->numParcelas);
        novo_curso->next = NULL;

        if (!head) {
            head = novo_curso;
        } else {
            current->next = novo_curso;
        }
        current = novo_curso;
    }

    fclose(file);
    return head;
}

time_t parse_date(const char* date_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    if (sscanf(date_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) {
        return (time_t)-1;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    return mktime(&tm);
}

Matricula* carregar_matriculas(Curso* cursos) {
    FILE* file = fopen("matriculas.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de matriculas");
        return NULL;
    }

    Matricula* head = NULL;
    Matricula* current = NULL;
    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), file)) {
        Matricula* nova_matricula = (Matricula*)malloc(sizeof(Matricula));

        sscanf(linha, "%d,%d,%d,%d", &nova_matricula->id, &nova_matricula->idAluno, &nova_matricula->idCurso, &nova_matricula->meses);
        nova_matricula->next = NULL;
        nova_matricula->vencimentos = carregar_vencimentos_aluno(nova_matricula->id);

        if (!nova_matricula->vencimentos) {
            Curso* curso = encontrar_curso_por_id(nova_matricula->idCurso, cursos);
            if (curso) {
                time_t now = time(NULL);
                for (int i = 0; i < nova_matricula->meses; ++i) {
                    time_t vencimento_data = now + (i * 30 * 24 * 3600);
                }
            }
        }

        if (!head) {
            head = nova_matricula;
        } else {
            current->next = nova_matricula;
        }
        current = nova_matricula;
    }

    fclose(file);
    return head;
}

Aluno* encontrar_aluno_por_id(int idAluno, Aluno* alunos) {
    while (alunos != NULL) {
        if (alunos->id == idAluno) {
            return alunos;
        }
        alunos = alunos->next;
    }
    return NULL;
}

Matricula* encontrar_matricula_por_id(int idMatricula, Matricula* matriculas){
	while(matriculas != NULL){
		if(matriculas->id == idMatricula){
			return matriculas;
		}
		matriculas = matriculas->next;
	}
	return NULL;
}

Curso* encontrar_curso_por_id(int idCurso, Curso* cursos) {
    while (cursos != NULL) {
        if (cursos->id == idCurso) {
            return cursos;
        }
        cursos = cursos->next;
    }
    return NULL;
}

void salvar_alunos(Aluno* alunos) {
  FILE * arquivo = fopen("alunos.txt", "w");
  if (arquivo != NULL) {
    while (alunos != NULL) {
      fprintf(arquivo, "%d,%s,%s\n", alunos->id, alunos->nome, alunos->senha);
      alunos = alunos->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados dos alunos.\n");
  }
}

void salvar_cursos(Curso* cursos) {
  FILE * arquivo = fopen("cursos.txt", "w");
  if (arquivo != NULL) {
    while (cursos != NULL) {
      fprintf(arquivo, "%d,%s,%.2f,%d\n", cursos->id, cursos->nome, cursos->precoPorMes, cursos->numParcelas);
      cursos = cursos->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados dos cursos.\n");
  }
}

void salvar_matriculas(Matricula* matriculas) {
  FILE *arquivo = fopen("matriculas.txt", "w");
  if (arquivo != NULL) {
    while (matriculas != NULL) {
      fprintf(arquivo, "%d,%d,%d,%d\n", matriculas->id, matriculas->idAluno, matriculas->idCurso, matriculas->meses);
      matriculas = matriculas->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados das matriculas.\n");
  }
}

time_t calcular_data_expiracao() {
  time_t agora = time(NULL);
  struct tm * tempo_atual = localtime( & agora);
  tempo_atual -> tm_mon++;
  tempo_atual -> tm_year += tempo_atual -> tm_mon / 12;
  tempo_atual -> tm_mon %= 12;
  return mktime(tempo_atual);
}

int main() {
    Aluno* alunos = NULL;
    Curso* cursos = NULL;
    Matricula* matriculas = NULL;
    Vencimento* vencimentos = NULL;

    int opcao;
    do {
        Aluno* alunos = carregar_alunos();
    	Curso* cursos = carregar_cursos();
    	Matricula* matriculas = carregar_matriculas(cursos);
    	Vencimento* vencimentos = carregar_vencimentos();
        limpar_console();
    	
        menu_login();
        scanf("%d", &opcao);
        getchar();
        limpar_console();

        switch (opcao) {
            case 1: {
                int id = login(alunos);
                limpar_console();
                if (id == -1) {
                    menu_admin(&alunos, &cursos, &matriculas, &vencimentos);
                } else if (id > 0) {
                    menu_aluno(id, alunos, cursos, matriculas, &vencimentos);
                } else {
                    printf("Nome de usuario ou senha invalidos. Por favor, tente novamente.\n");
                }
                break;
            }
            case 2:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
        }
    } while (opcao != 2);

    return 0;
}
