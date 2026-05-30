// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h> // Para strcspn(), uma biblioteca para manipular strings
#include <stdlib.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TER 5 // Número máximo de territórios a cadastrar
#define TOTAL_MISSOES 5

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Vetor de missões ---
char *missoes[TOTAL_MISSOES] = {
    "Conquistar todos os territórios.",
    "Eliminar todas as tropas da cor Vermelha.",
    "Manter pelo menos 5 tropas em cada território.",
    "Conquistar 3 territórios seguidos.",
    "Defender com sucesso 2 ataques consecutivos."
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Protótipos ---
void limparBufferEntrada();
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void cadastrarExercito(struct Territorio *territorios);
void atualizarMapa(struct Territorio *territorios);
void menuAtacar(int *opcao);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho);
void exibirMissao(char* missao);
void liberarMemoria(struct Territorio *territorios, char *missaoJogador);


// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    srand(time(NULL)); // inicializa gerador de números aleatórios

    // Alocação dinâmica dos territórios
    struct Territorio *territorios = (struct Territorio*) calloc(MAX_TER, sizeof(struct Territorio));
    if (territorios == NULL) {
        printf("Erro: falha ao alocar memória.\n");
        return 1;
    }

    // Alocação dinâmica da missão
    char *missaoJogador = (char*) malloc(100 * sizeof(char));
    atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES);
    exibirMissao(missaoJogador); // missão exibida apenas uma vez

    cadastrarExercito(territorios);

    int opcao;
    do {
        atualizarMapa(territorios);
        menuAtacar(&opcao);

        if (opcao == 0) {
            printf("\nSaindo do Sistema...\n");
            break;
        }

        if (opcao < 1 || opcao > MAX_TER) {
            printf("\nOpção inválida! Digite entre 1 e %d ou 0 para sair.\n", MAX_TER);
            continue;
        }

        int atacante = opcao - 1;

        // Validação: atacante precisa ter pelo menos 2 tropas
        if (territorios[atacante].tropas < 2) {
            printf("\nO atacante precisa ter pelo menos 2 tropas para atacar!\n");
            continue;
        }

        // Escolha do defensor
        printf("Escolha o território defensor (1 a %d): ", MAX_TER);
        int defensor;
        scanf("%d", &defensor);
        limparBufferEntrada();

        if (defensor < 1 || defensor > MAX_TER || defensor == opcao) {
            printf("\nDefensor inválido!\n");
            continue;
        }

        defensor = defensor - 1;

        // Validação: só pode atacar inimigos
        if (strcmp(territorios[atacante].cor, territorios[defensor].cor) == 0) {
            printf("\nVocê não pode atacar um território aliado!\n");
            continue;
        }

        // Executa a batalha
        atacar(&territorios[atacante], &territorios[defensor]);

        // Verificação silenciosa da missão
        if (verificarMissao(missaoJogador, territorios, MAX_TER)) {
            printf("\nO jogador do exército %s cumpriu sua missão e venceu o jogo!\n", territorios[atacante].cor);
            break;
        }

    } while (opcao != 0);

    liberarMemoria(territorios, missaoJogador);
    printf("\nFim do programa.\n");

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

// --- Implementações ---

// Limpa o buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Cadastro inicial dos territórios
void cadastrarExercito(struct Territorio *territorios) {
    printf("\n###########################################################\n");
    printf("    WAR ESTRUTURADO - CADASTRO INICIAL");
    printf("\n###########################################################\n");

    for (int i = 0; i < MAX_TER; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do Exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBufferEntrada();
    }
}

// Função de ataque
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Atacante %s tirou: %d\n", atacante->nome, dadoA);
    printf("Defensor %s tirou: %d\n", defensor->nome, dadoD);

    if (dadoA > dadoD) {
        defensor->tropas--;
        printf("Vitória do ataque! O defensor perdeu 1 tropa.\n");

        if (defensor->tropas <= 0) {
            printf("%s foi conquistado pelo exército %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);

            int tropasTransferidas = atacante->tropas / 2;
            defensor->tropas = tropasTransferidas;
            atacante->tropas -= tropasTransferidas;
        }
    } else {
        atacante->tropas--;
        printf("Defesa resistiu! O atacante perdeu 1 tropa.\n");
    }

    printf("\nPressione Enter para continuar...");
    getchar();
}

// Exibe o mapa atual
void atualizarMapa(struct Territorio *territorios) {
    // Exibe o mapa atualizado
    printf("\n###########################################################\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("###########################################################\n");
    
    for (int i = 0; i < MAX_TER; i++) {
        printf("%d. %s (Exército %s, Tropas: %d)\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// Menu de ataque
void menuAtacar(int *opcao) {
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", MAX_TER);
    scanf("%d", opcao);
    limparBufferEntrada();
}

// Sorteia uma missão
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Verifica se a missão foi cumprida
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho) {
    if (strcmp(missao, "Conquistar todos os territórios.") == 0) {
        for (int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[0].cor) != 0) return 0;
        }
        return 1;
    }
    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelha.") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1;
    }
    if (strcmp(missao, "Manter pelo menos 5 tropas em cada território.") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas < 5) return 0;
        }
        return 1;
    }
    return 0;
}

// Exibe missão ao jogador
void exibirMissao(char* missao) {
    printf("\n>>> Sua missão secreta: %s\n", missao);
}

// Libera memória
void liberarMemoria(struct Territorio *territorios, char *missaoJogador) {
    free(territorios);
    free(missaoJogador);
    printf("Memória liberada com sucesso!\n");
}