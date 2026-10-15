#include <stdio.h>
#include <string.h>

#define SALDO_INICIAL 7000.00

void deposito(double *saldo);
void saque(double *saldo);
void transferencia(double *saldo);
void visualizarSaldo(double saldo);
void registrar_transacao(const char *tipo, double valor, double saldo);
int ler_opcao(void);
double ler_valor(const char *mensagem);

int main(void) {
    double saldo = SALDO_INICIAL;
    int escolha;

    printf("=== SIMULADOR BANCARIO ===\n");

    do {
        printf("\nEscolha uma operacao:\n");
        printf("1 - Deposito\n");
        printf("2 - Saque\n");
        printf("3 - Transferencia\n");
        printf("4 - Consultar saldo\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        escolha = ler_opcao();

        switch (escolha) {
            case 1:
                deposito(&saldo);
                break;

            case 2:
                saque(&saldo);
                break;

            case 3:
                transferencia(&saldo);
                break;

            case 4:
                visualizarSaldo(saldo);
                break;

            case 0:
                printf("Encerrando o simulador.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (escolha != 0);

    return 0;
}

int ler_opcao(void) {
    int opcao;
    char entrada[100];
    char extra;

    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        return 0;
    }

    if (sscanf(entrada, " %d %c", &opcao, &extra) != 1) {
        return -1;
    }

    return opcao;
}

double ler_valor(const char *mensagem) {
    double valor;
    char entrada[100];
    char extra;

    printf("%s", mensagem);

    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        return -1;
    }

    if (sscanf(entrada, " %lf %c", &valor, &extra) != 1) {
        return -1;
    }

    return valor;
}

void deposito(double *saldo) {
    double valor = ler_valor("Valor do deposito: R$ ");

    if (valor <= 0) {
        printf("Valor invalido.\n");
        return;
    }

    *saldo += valor;

    registrar_transacao("Deposito", valor, *saldo);

    printf("Deposito realizado com sucesso!\n");
    printf("Saldo atual: R$ %.2f\n", *saldo);
}

void saque(double *saldo) {
    double valor = ler_valor("Valor do saque: R$ ");

    if (valor <= 0) {
        printf("Valor invalido.\n");
        return;
    }

    if (valor > *saldo) {
        printf("Saldo insuficiente.\n");
        return;
    }

    *saldo -= valor;

    registrar_transacao("Saque", valor, *saldo);

    printf("Saque realizado com sucesso!\n");
    printf("Saldo atual: R$ %.2f\n", *saldo);
}

void transferencia(double *saldo) {
    char destinatario[100];
    int modalidade;
    double valor;
    double taxa;

    printf("Chave do destinatario: ");

    if (fgets(destinatario, sizeof(destinatario), stdin) == NULL) {
        printf("Erro ao ler destinatario.\n");
        return;
    }

    destinatario[strcspn(destinatario, "\n")] = '\0';

    if (destinatario[0] == '\0') {
        printf("Destinatario invalido.\n");
        return;
    }

    printf("\nModalidade:\n");
    printf("1 - PIX (sem taxa, limite de R$ 500,00)\n");
    printf("2 - DOC (taxa de R$ 5,00, limite de R$ 4.999,99)\n");
    printf("3 - TED (taxa de R$ 10,50)\n");
    printf("Opcao: ");

    modalidade = ler_opcao();

    switch (modalidade) {
        case 1:
            taxa = 0;
            break;

        case 2:
            taxa = 5.00;
            break;

        case 3:
            taxa = 10.50;
            break;

        default:
            printf("Modalidade invalida.\n");
            return;
    }

    valor = ler_valor("Valor da transferencia: R$ ");

    if (valor <= 0) {
        printf("Valor invalido.\n");
        return;
    }

    if (modalidade == 1 && valor > 500.00) {
        printf("O PIX possui limite de R$ 500,00 neste simulador.\n");
        return;
    }

    if (modalidade == 2 && valor > 4999.99) {
        printf("O DOC possui limite de R$ 4.999,99 neste simulador.\n");
        return;
    }

    if (valor + taxa > *saldo) {
        printf("Saldo insuficiente para realizar a transferencia.\n");
        return;
    }

    *saldo -= valor + taxa;

    if (modalidade == 1) {
        registrar_transacao("Transferencia PIX", valor, *saldo);
    } else if (modalidade == 2) {
        registrar_transacao("Transferencia DOC", valor, *saldo);
    } else {
        registrar_transacao("Transferencia TED", valor, *saldo);
    }

    printf("\nTransferencia realizada com sucesso!\n");
    printf("Destinatario: %s\n", destinatario);
    printf("Valor transferido: R$ %.2f\n", valor);
    printf("Taxa: R$ %.2f\n", taxa);
    printf("Saldo atual: R$ %.2f\n", *saldo);
}

void visualizarSaldo(double saldo) {
    printf("Saldo atual: R$ %.2f\n", saldo);
}

void registrar_transacao(
    const char *tipo,
    double valor,
    double saldo
) {
    FILE *arquivo = fopen("extrato.txt", "a");

    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel registrar a transacao.\n");
        return;
    }

    fprintf(
        arquivo,
        "%s | Valor: R$ %.2f | Saldo: R$ %.2f\n",
        tipo,
        valor,
        saldo
    );

    fclose(arquivo);
}
