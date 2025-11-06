🏦 BANCO MALVADER

Um sistema bancário simples em linguagem C, desenvolvido para fins educacionais.
O programa permite cadastro e manutenção de contas correntes, simula operações bancárias básicas e mantém todos os dados em memória (sem uso de arquivos ou banco de dados externo).

📋 Funcionalidades

O sistema oferece as seguintes operações:

Nº	Função	Descrição

1️⃣	Criar conta	Cadastra um novo cliente e cria uma conta bancária associada.

2️⃣	Efetuar saque	Realiza um saque da conta, atualizando o saldo.

3️⃣	Efetuar depósito	Permite depositar um valor em uma conta existente.

4️⃣	Efetuar transferência	Transfere valores entre duas contas válidas e ativas.

5️⃣	Consultar conta	Permite consultar informações de uma conta por número ou CPF.

6️⃣	Atualizar dados	Atualiza o telefone e a agência do cliente.

7️⃣	Listar contas	Exibe todas as contas cadastradas, com opção de filtrar por status.

8️⃣	Encerrar conta	Encerra contas com saldo igual a zero.

9️⃣	Sair	Finaliza o sistema de forma segura.

🧠 Estrutura do Código

O programa é dividido em módulos lógicos, organizados por função:

🧩 Structs
Cliente

Representa os dados pessoais e bancários do cliente:

typedef struct {
    int codigo;
    char nome[50];
    char email[50];
    char cpf[20];
    char telefone[20];
    char dataNascimento[20];
    char dataCadastro[20];
    char agencia[20];
    int status; // 1 ativa, 0 encerrada
} Cliente;

Conta

Representa uma conta corrente, vinculada a um cliente:

typedef struct {
    int numero;
    Cliente cliente;
    float saldo;
    float limite;
    float saldoTotal;
} Conta;

⚙️ Como Executar
🔧 Pré-requisitos

Você precisa ter um compilador C instalado.
Recomenda-se o uso do GCC (disponível no Linux, macOS e Windows via MinGW).

▶️ Compilação

No terminal, execute:

gcc banco_malvader.c -o banco

▶️ Execução

Depois, execute o programa:

./banco

🧾 Funcionamento Básico

Ao iniciar o programa, o menu principal será exibido.

Selecione a opção desejada digitando o número correspondente.

Siga as instruções mostradas na tela.

O sistema mantém os dados somente em memória, ou seja, ao fechar o programa, todas as contas serão perdidas.

🧮 Estrutura em Memória

O sistema usa vetores estáticos para armazenar as contas:

static Conta contas[50];
static int contador_contas = 0;


Isso significa que o sistema pode gerenciar até 50 contas simultâneas.

🚫 Limitações

Não há persistência de dados (as informações são apagadas ao encerrar o programa).

Não há controle de limite de crédito funcional (campo limite é apenas simbólico).

As validações de entrada são simples (sem tratamento de caracteres inválidos).

👨‍💻 Autora:
Bianca Aiala
@aialamoretti

Projeto: Banco Malvader
Desenvolvido em linguagem C como exemplo de manipulação de estruturas e funções.
Ideal para uso em disciplinas introdutórias de programação estruturada.
