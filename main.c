#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_MUSICAS 100
#define TAM_NOME 100

typedef struct {
    char nome[TAM_NOME];
    char artista[TAM_NOME];
    char album[TAM_NOME];
} Musica;

typedef struct {
    Musica musicas[MAX_MUSICAS];
    int total;
    int atual;
} Playlist;

void carregar_playlist(Playlist *playlist, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    playlist->total = 0;
    playlist->atual = -1;
    while (fscanf(arquivo, " %99[^\n] %99[^\n] %99[^\n]", playlist->musicas[playlist->total].nome, playlist->musicas[playlist->total].artista, playlist->musicas[playlist->total].album) == 3) {
        playlist->total++;
    }
    fclose(arquivo);
}

void salvar_playlist(Playlist *playlist, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    for (int i = 0; i < playlist->total; i++) {
        fprintf(arquivo, "%s\n%s\n%s\n", playlist->musicas[i].nome, playlist->musicas[i].artista, playlist->musicas[i].album);
    }
    fclose(arquivo);
}

void exibir_playlist(Playlist *playlist) {
    for (int i = 0; i < playlist->total; i++) {
        printf("%d. %s - %s (%s)\n", i + 1, playlist->musicas[i].nome, playlist->musicas[i].artista, playlist->musicas[i].album);
    }
}

int comparar_musicas(const void *a, const void *b) {
    return strcmp(((Musica *)a)->nome, ((Musica *)b)->nome);
}

void exibir_playlist_ordenada(Playlist *playlist) {
    qsort(playlist->musicas, playlist->total, sizeof(Musica), comparar_musicas);
    exibir_playlist(playlist);
}

void adicionar_musica(Playlist *playlist, const char *nome, const char *artista, const char *album) {
    if (playlist->total >= MAX_MUSICAS) {
        printf("Playlist cheia!\n");
        return;
    }
    strcpy(playlist->musicas[playlist->total].nome, nome);
    strcpy(playlist->musicas[playlist->total].artista, artista);
    strcpy(playlist->musicas[playlist->total].album, album);
    playlist->total++;
}

void remover_musica(Playlist *playlist, const char *nome) {
    int encontrado = 0;
    for (int i = 0; i < playlist->total; i++) {
        if (strcmp(playlist->musicas[i].nome, nome) == 0) {
            encontrado = 1;
            for (int j = i; j < playlist->total - 1; j++) {
                playlist->musicas[j] = playlist->musicas[j + 1];
            }
            playlist->total--;
            break;
        }
    }
    if (!encontrado) {
        printf("Música não encontrada!\n");
    }
}

void buscar_musica(Playlist *playlist, const char *nome) {
    for (int i = 0; i < playlist->total; i++) {
        if (strcmp(playlist->musicas[i].nome, nome) == 0) {
            printf("Música encontrada: %s - %s (%s)\n", playlist->musicas[i].nome, playlist->musicas[i].artista, playlist->musicas[i].album);
            return;
        }
    }
    printf("Música não encontrada!\n");
}

void avancar_musica(Playlist *playlist) {
    if (playlist->total > 0) {
        playlist->atual = (playlist->atual + 1) % playlist->total;
        printf("Tocando agora: %s - %s (%s)\n", playlist->musicas[playlist->atual].nome, playlist->musicas[playlist->atual].artista, playlist->musicas[playlist->atual].album);
    } else {
        printf("Nenhuma música na playlist!\n");
    }
}

void retornar_musica(Playlist *playlist) {
    if (playlist->total > 0) {
        playlist->atual = (playlist->atual - 1 + playlist->total) % playlist->total;
        printf("Tocando agora: %s - %s (%s)\n", playlist->musicas[playlist->atual].nome, playlist->musicas[playlist->atual].artista, playlist->musicas[playlist->atual].album);
    } else {
        printf("Nenhuma música na playlist!\n");
    }
}

void menu(Playlist *playlist, const char *nome_arquivo) {
    int opcao;
    char nome[TAM_NOME], artista[TAM_NOME], album[TAM_NOME];
    
    do {
        printf("\nMenu:\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenada pelo nome das músicas\n");
        printf("3. Inserir nova música\n");
        printf("4. Remover música\n");
        printf("5. Buscar música\n");
        printf("6. Avançar para próxima música\n");
        printf("7. Retornar a música anterior\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                exibir_playlist(playlist);
                break;
            case 2:
                exibir_playlist_ordenada(playlist);
                break;
            case 3:
                printf("Nome: ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("Artista: ");
                fgets(artista, TAM_NOME, stdin);
                artista[strcspn(artista, "\n")] = '\0';
                printf("Álbum: ");
                fgets(album, TAM_NOME, stdin);
                album[strcspn(album, "\n")] = '\0';
                adicionar_musica(playlist, nome, artista, album);
                salvar_playlist(playlist, nome_arquivo);
                break;
            case 4:
                printf("Nome da música a remover: ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                remover_musica(playlist, nome);
                salvar_playlist(playlist, nome_arquivo);
                break;
            case 5:
                printf("Nome da música a buscar: ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                buscar_musica(playlist, nome);
                break;
            case 6:
                avancar_musica(playlist);
                break;
            case 7:
                retornar_musica(playlist);
                break;
            case 8:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 8);
}

int main() {
    setlocale (LC_ALL, "Portuguese");
    Playlist playlist;
    const char *nome_arquivo = "./musicas.txt";

    carregar_playlist(&playlist, nome_arquivo);
    menu(&playlist, nome_arquivo);

    return 0;
}
