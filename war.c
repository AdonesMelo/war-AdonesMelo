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

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função de ataque
// Simula uma batalha entre dois territórios usando dados (1 a 6).
// Atualiza o número de tropas e pode transferir posse do território.
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoA = rand() % 6 + 1; // rolagem do atacante
    int dadoD = rand() % 6 + 1; // rolagem do defensor

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoA);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoD);

    if (dadoA > dadoD) {
        defensor->tropas--; // defensor perde tropa
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");

        // Se o defensor ficar sem tropas, território é conquistado
        if (defensor->tropas <= 0) {
            printf("%s foi conquistado pelo exército %s!\n", defensor->nome, atacante->cor);
            // Transferir posse: copiar cor do atacante para o defensor
            strcpy(defensor->cor, atacante->cor);
            // Opcional: mover 1 tropa para o território conquistado
            defensor->tropas = 1; // mínimo de tropas no território conquistado
            atacante->tropas--; // atacante move uma tropa para ocupar
        }
    } else {
        atacante->tropas--; // atacante perde tropa
        printf("DEFESA RESISTIU! O atacante perdeu 1 tropa.\n");
    }

    printf("\nPressione Enter para continuar para o próximo turno...");
    getchar();
}

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

    // 1. ALOCAÇÃO DINÂMICA NA MEMÓRIA
    // Agora, em vez array esático, usar ponteiro
    struct Territorio *territorios; 

    // Usar malloc para o array de emprestimos. malloc(tamanho_total_em_bytes).
    territorios = (struct Territorio *) malloc(MAX_TER * sizeof(struct Territorio));

    // Verificação: é crucial verificar se alocação de memória deu certo.
    if (territorios == NULL) {
        printf("Erro: falha ao locar memória.\n");
        return 1; // retornar 1 para indicar um erro.
    }

    // Exibir o menu de opções
    printf("\n###########################################################\n");
    printf("    WAR ESTRUTURADO - CADASTRO INICIAL");
    printf("\n###########################################################\n");
    
    // --- Cadastro dos territórios ---
    for (int i = 0; i < MAX_TER; i++) {
        printf("\n--- Cadastro Território - %d ---\n", i + 1);

        printf("Nome do Território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // Remove o '\n'

        printf("Cor do Exército (ex: Azul, Verde): ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0'; // Remove o '\n'

        printf("Número de Tropas: ");
        scanf("%d", &territorios[i].tropas);

        limparBufferEntrada(); // limpar o '\n' deixado pelo scanf.
    }

    int opcao;
    // --- Loop principal da fase de ataque ---
    do {
        // Exibe o mapa atualizado
        printf("\n###########################################################\n");
        printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
        printf("###########################################################\n");

        for (int i = 0; i < MAX_TER; i++) {
            printf("%d. %s (Exército %s, Tropas: %d)\n",
                i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        }

        // Escolha do atacante
        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", MAX_TER);
        scanf("%d", &opcao);
        limparBufferEntrada();

        if (opcao == 0) {
            printf("\nSaindo do Sistema...\n");
            break;
        }

        if (opcao < 1 || opcao > MAX_TER) {
            printf("\nOpção inválida!\n");
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

        // Executa a batalha
        atacar(&territorios[atacante], &territorios[defensor]);

    } while (opcao != 0);

    // Liberação da memória
    free(territorios);
    printf("\nMemória liberada com sucesso!\n");
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
