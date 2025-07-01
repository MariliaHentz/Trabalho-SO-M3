#include "filesystem.h"
#include <stdio.h>
#include <string.h>

#define MAX_NAME 100

int main() {
    Directory* root = get_root_directory();
    Directory* current = root;
    int opcao;
    char nome[MAX_NAME];
    char current_dir_name[MAX_NAME] = "root";

    while (1) {
        
        printf("\nDiretório atual: %s\n", current_dir_name);
        printf("1 - Inserir diretório\n");
        printf("2 - Inserir arquivo .txt\n");
        printf("3 - Listar conteúdo do diretório\n");
        printf("4 - Mudar diretório (cd)\n");
        printf("5 - Voltar ao diretório pai\n");
        printf("6 - Deletar arquivo .txt\n");
        printf("7 - Deletar diretório\n");
        printf("8 - Buscar item por nome\n");
        printf("9 - Mostrar árvore B \n");
        printf("10 - Salvar sistema de arquivos em fs.img\n");///////////
        printf("11 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1: // Inserir diretório
                printf("Nome do diretório: ");
                fgets(nome, MAX_NAME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                TreeNode* newDir = create_directory(nome);
                btree_insert(current->tree, newDir);
                break;

            case 2: // Inserir arquivo
                printf("Nome do arquivo .txt: ");
                fgets(nome, MAX_NAME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                TreeNode* newFile = create_txt_file(nome, "Arquivo criado pelo usuário.");
                btree_insert(current->tree, newFile);
                break;

            case 3: // Listar conteúdo
                list_directory_contents(current);
                break;

            case 4: // Mudar diretório
                printf("Nome do diretório: ");
                fgets(nome, MAX_NAME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                change_directory(&current, nome);
                strncpy(current_dir_name, nome, MAX_NAME);   
                break;

            case 5: // Voltar ao diretório pai
                if (current != root) {
                        current = root;
                        strncpy(current_dir_name, "root", MAX_NAME);
                        printf("Retornou ao diretório raiz.\n");
                        
                    } else {
                        printf("Já está no diretório raiz.\n");
                    }
                break;

            case 6: // Deletar arquivo
                printf("Nome do arquivo .txt: ");
                fgets(nome, MAX_NAME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                delete_txt_file(current->tree, nome);
                break;

            case 7: // Deletar diretório
                printf("Nome do diretório: ");
                fgets(nome, MAX_NAME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                delete_directory(current->tree, nome);
                break;

            case 8: // Buscar item
                printf("Nome do item a buscar: ");
                fgets(nome, MAX_NAME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                TreeNode* result = btree_search(current->tree, nome);
                if (result) {
                    printf("Encontrado: %s (", result->name);
                    if (result->type == FILE_TYPE) {
                        printf("Arquivo");
                    } else {
                        printf("Diretório");
                    }
                    printf(")\n");
                }else {
                    printf("Item '%s' não encontrado.\n", nome);
                }
                break;

            case 9: // Mostrar árvore
                printf("Árvore do diretório atual:\n");
                btree_traverse(current->tree);
                break;

            case 10:
                save_filesystem_image(root, "fs.img");
            break;

            case 11:
                printf("Encerrando sistema de arquivos.\n");
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}