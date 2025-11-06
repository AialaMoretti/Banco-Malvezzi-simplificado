#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// ==================== DEFINIÇÕES ====================
// Constantes simbólicas para indicar o status da conta
#define ATIVA 1
#define ENCERRADA 0

// ==================== STRUCTS ====================
// Estrutura que representa um cliente do banco
typedef struct {
    int codigo;                // Identificador do cliente
    char nome[50];             // Nome completo
    char email[50];            // E-mail
    char cpf[20];              // CPF (identificador único)
    char telefone[20];         // Telefone
    char dataNascimento[20];   // Data de nascimento
    char dataCadastro[20];     // Data em que foi criado o cadastro
    char agencia[20];          // Agência da conta
    int status;                // Status da conta: 1 = ativa, 0 = encerrada
} Cliente;

// Estrutura que representa uma conta bancária
typedef struct {
    int numero;         // Número da conta
    Cliente cliente;    // Dados do cliente (struct Cliente)
    float saldo;        // Saldo disponível
    float limite;       // Limite adicional (opcional)
    float saldoTotal;   // Soma do saldo + limite
} Conta;

// ==================== DECLARAÇÃO DE FUNÇÕES ====================
// Funções principais do sistema
void menu();
void infoCliente(Cliente cliente);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();
void consultarConta();
void atualizarDados();
void encerrarConta();

// Funções auxiliares
float atualizaSaldoTotal(Conta conta);
Conta *buscarContaPorNumero(int numero);
Conta *buscarContaPorCPF(char *cpf);

// Funções de operação bancária
void sacar(Conta *conta, float valor);
void depositar(Conta *conta, float valor);
void transferir(Conta *origem, Conta *destino, float valor);

// ==================== VARIÁVEIS GLOBAIS ====================
// Vetor que armazena todas as contas criadas
static Conta contas[50];       // Capacidade máxima: 50 contas
static int contador_contas = 0; // Contador de contas criadas
static int contador_clientes = 0; // Contador de clientes

// ==================== FUNÇÃO PRINCIPAL ====================
int main() {
    menu(); // Inicia o menu principal do sistema
    return 0;
}

// ==================== MENU PRINCIPAL ====================
void menu() {
    int opcao = 0;
    do {
        // Exibe o menu de opções
        printf("\n======================================\n");
        printf("=============== Banco Malvader ================\n");
        printf("======================================\n");
        printf("1 - Criar conta\n");
        printf("2 - Efetuar saque\n");
        printf("3 - Efetuar deposito\n");
        printf("4 - Efetuar transferencia\n");
        printf("5 - Consultar saldo e dados\n");
        printf("6 - Atualizar dados do cliente\n");
        printf("7 - Listar contas\n");
        printf("8 - Encerrar conta\n");
        printf("9 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        // Processa a opção escolhida
        switch (opcao) {
            case 1: criarConta(); break;
            case 2: efetuarSaque(); break;
            case 3: efetuarDeposito(); break;
            case 4: efetuarTransferencia(); break;
            case 5: consultarConta(); break;
            case 6: atualizarDados(); break;
            case 7: listarContas(); break;
            case 8: encerrarConta(); break;
            case 9:
                printf("Tem certeza que deseja sair? (1 = sim / 0 = nao): ");
                int sair; scanf("%d", &sair);
                if (sair == 1) {
                    printf("Encerrando o sistema...\n");
                    sleep(2);
                    exit(0);
                }
                break;
            default:
                printf("Opcao invalida.\n");
                sleep(1);
        }
    } while (1); // Mantém o menu até o usuário sair
}

// ==================== FUNÇÕES AUXILIARES ====================

// Atualiza o saldo total da conta (saldo + limite)
float atualizaSaldoTotal(Conta conta) {
    return conta.saldo + conta.limite;
}

// Busca uma conta pelo número
Conta *buscarContaPorNumero(int numero) {
    for (int i = 0; i < contador_contas; i++) {
        if (contas[i].numero == numero) return &contas[i];
    }
    return NULL; // Retorna NULL se não encontrar
}

// Busca uma conta pelo CPF do cliente
Conta *buscarContaPorCPF(char *cpf) {
    for (int i = 0; i < contador_contas; i++) {
        if (strcmp(contas[i].cliente.cpf, cpf) == 0) return &contas[i];
    }
    return NULL;
}

// Exibe informações básicas de um cliente
void infoCliente(Cliente cliente) {
    printf("Nome: %s\nCPF: %s\nTelefone: %s\nAgencia: %s\nStatus: %s\n",
           cliente.nome, cliente.cpf, cliente.telefone, cliente.agencia,
           cliente.status == ATIVA ? "ATIVA" : "ENCERRADA");
}

// Exibe informações completas de uma conta
void infoConta(Conta conta) {
    printf("\n--- Dados da Conta ---\n");
    printf("Numero da conta: %d\n", conta.numero);
    infoCliente(conta.cliente);
    printf("Saldo: %.2f\n", conta.saldo);
    printf("Saldo Total: %.2f\n", conta.saldoTotal);
    printf("------------------------\n");
}

// ==================== CRIAR CONTA ====================
void criarConta() {
    Cliente cliente;

    // Gera automaticamente a data atual do cadastro
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(cliente.dataCadastro, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Solicita os dados do cliente
    printf("Informe os dados do cliente:\n");
    cliente.codigo = contador_clientes + 1;

    printf("Nome: ");
    fgets(cliente.nome, 50, stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = 0;

    printf("E-mail: ");
    fgets(cliente.email, 50, stdin);
    cliente.email[strcspn(cliente.email, "\n")] = 0;

    printf("CPF: ");
    fgets(cliente.cpf, 20, stdin);
    cliente.cpf[strcspn(cliente.cpf, "\n")] = 0;

    // Garante unicidade do CPF (não pode ter dois iguais)
    if (buscarContaPorCPF(cliente.cpf)) {
        printf("Ja existe uma conta ativa com este CPF.\n");
        sleep(2);
        return;
    }

    printf("Telefone: ");
    fgets(cliente.telefone, 20, stdin);
    cliente.telefone[strcspn(cliente.telefone, "\n")] = 0;

    printf("Agencia: ");
    fgets(cliente.agencia, 20, stdin);
    cliente.agencia[strcspn(cliente.agencia, "\n")] = 0;

    printf("Data de nascimento: ");
    fgets(cliente.dataNascimento, 20, stdin);
    cliente.dataNascimento[strcspn(cliente.dataNascimento, "\n")] = 0;

    cliente.status = ATIVA;

    // Cria uma nova conta associada ao cliente
    Conta nova;
    nova.numero = contador_contas + 1;
    nova.cliente = cliente;
    nova.saldo = 0.0;
    nova.limite = 0.0;
    nova.saldoTotal = atualizaSaldoTotal(nova);

    // Armazena a nova conta no vetor global
    contas[contador_contas++] = nova;
    contador_clientes++;

    printf("\nConta criada com sucesso!\n");
    infoConta(nova);
    sleep(3);
}

// ==================== OPERAÇÕES BANCÁRIAS ====================

// Função de saque
void sacar(Conta *conta, float valor) {
    if (valor > 0 && conta->saldoTotal >= valor) {
        conta->saldo -= valor;
        conta->saldoTotal = atualizaSaldoTotal(*conta);
        printf("Saque realizado com sucesso!\n");
    } else {
        printf("Saldo insuficiente ou valor invalido.\n");
    }
}

// Função de depósito
void depositar(Conta *conta, float valor) {
    if (valor > 0) {
        conta->saldo += valor;
        conta->saldoTotal = atualizaSaldoTotal(*conta);
        printf("Deposito realizado com sucesso!\n");
    } else {
        printf("Valor invalido.\n");
    }
}

// Função de transferência entre contas
void transferir(Conta *origem, Conta *destino, float valor) {
    if (valor <= 0) {
        printf("Valor invalido.\n");
        return;
    }
    if (origem->saldoTotal < valor) {
        printf("Saldo insuficiente.\n");
        return;
    }

    origem->saldo -= valor;
    destino->saldo += valor;
    origem->saldoTotal = atualizaSaldoTotal(*origem);
    destino->saldoTotal = atualizaSaldoTotal(*destino);

    printf("Transferencia realizada com sucesso!\n");
}

// ==================== FUNÇÕES DO MENU ====================

// Efetua um saque chamando a função sacar()
void efetuarSaque() {
    int numero;
    printf("Numero da conta: ");
    scanf("%d", &numero);
    Conta *c = buscarContaPorNumero(numero);
    if (c && c->cliente.status == ATIVA) {
        float valor;
        printf("Valor do saque: ");
        scanf("%f", &valor);
        sacar(c, valor);
    } else {
        printf("Conta nao encontrada ou encerrada.\n");
    }
    sleep(2);
}

// Efetua um depósito chamando a função depositar()
void efetuarDeposito() {
    int numero;
    printf("Numero da conta: ");
    scanf("%d", &numero);
    Conta *c = buscarContaPorNumero(numero);
    if (c && c->cliente.status == ATIVA) {
        float valor;
        printf("Valor do deposito: ");
        scanf("%f", &valor);
        depositar(c, valor);
    } else {
        printf("Conta nao encontrada ou encerrada.\n");
    }
    sleep(2);
}

// Efetua transferência entre duas contas
void efetuarTransferencia() {
    int origem, destino;
    printf("Conta origem: ");
    scanf("%d", &origem);
    printf("Conta destino: ");
    scanf("%d", &destino);
    Conta *c1 = buscarContaPorNumero(origem);
    Conta *c2 = buscarContaPorNumero(destino);

    if (c1 && c2 && c1->cliente.status == ATIVA && c2->cliente.status == ATIVA) {
        float valor;
        printf("Valor da transferencia: ");
        scanf("%f", &valor);
        transferir(c1, c2, valor);
    } else {
        printf("Conta origem/destino invalida.\n");
    }
    sleep(2);
}

// ==================== CONSULTAR CONTA ====================
void consultarConta() {
    int tipo;
    printf("Consultar por (1) numero ou (2) CPF: ");
    scanf("%d", &tipo);
    getchar();

    Conta *c = NULL;
    if (tipo == 1) {
        int numero;
        printf("Numero da conta: ");
        scanf("%d", &numero);
        c = buscarContaPorNumero(numero);
    } else if (tipo == 2) {
        char cpf[20];
        printf("CPF: ");
        fgets(cpf, 20, stdin);
        cpf[strcspn(cpf, "\n")] = 0;
        c = buscarContaPorCPF(cpf);
    }

    if (c) infoConta(*c);
    else printf("Conta nao encontrada.\n");

    sleep(3);
}

// ==================== ATUALIZAR DADOS ====================
// Permite alterar telefone e agência de um cliente
void atualizarDados() {
    int numero;
    printf("Numero da conta: ");
    scanf("%d", &numero);
    getchar();

    Conta *c = buscarContaPorNumero(numero);
    if (c && c->cliente.status == ATIVA) {
        printf("Novo telefone: ");
        fgets(c->cliente.telefone, 20, stdin);
        c->cliente.telefone[strcspn(c->cliente.telefone, "\n")] = 0;

        printf("Nova agencia: ");
        fgets(c->cliente.agencia, 20, stdin);
        c->cliente.agencia[strcspn(c->cliente.agencia, "\n")] = 0;

        printf("Dados atualizados com sucesso!\n");
    } else {
        printf("Conta nao encontrada ou encerrada.\n");
    }
    sleep(2);
}

// ==================== LISTAR CONTAS ====================
void listarContas() {
    if (contador_contas == 0) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    int filtro;
    printf("Listar (1) ativas, (2) encerradas, (3) todas: ");
    scanf("%d", &filtro);

    // Exibe contas conforme o filtro
    for (int i = 0; i < contador_contas; i++) {
        if (filtro == 1 && contas[i].cliente.status != ATIVA) continue;
        if (filtro == 2 && contas[i].cliente.status != ENCERRADA) continue;
        infoConta(contas[i]);
    }
    sleep(2);
}

// ==================== ENCERRAR CONTA ====================
void encerrarConta() {
    int numero;
    printf("Numero da conta: ");
    scanf("%d", &numero);
    Conta *c = buscarContaPorNumero(numero);

    if (c) {
        // Só permite encerrar conta com saldo zerado
        if (c->saldo == 0) {
            c->cliente.status = ENCERRADA;
            printf("Conta encerrada com sucesso!\n");
        } else {
            printf("Conta so pode ser encerrada com saldo igual a zero.\n");
        }
    } else {
        printf("Conta nao encontrada.\n");
    }
    sleep(2);
}


