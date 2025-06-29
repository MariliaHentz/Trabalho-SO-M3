/*#include "filesystem.h"

int main() {
    // Criação do sistema de arquivos
    Directory* root = get_root_directory();

    // Diretórios de segundo nível
    TreeNode* dirSO = create_directory("SO");
    TreeNode* dirTEST = create_directory("TEST");
    TreeNode* dirTEST1 = create_directory("TEST1");
    TreeNode* dirTEST2 = create_directory("TEST2");
    TreeNode* dirTEST3 = create_directory("TEST3");
    TreeNode* dirTEST4 = create_directory("TEST4");
    btree_insert(root->tree, dirSO);
    btree_insert(root->tree, dirTEST);
    btree_insert(root->tree, dirTEST1);
    btree_insert(root->tree, dirTEST2);
    btree_insert(root->tree, dirTEST3);
    btree_insert(root->tree, dirTEST4);
    btree_insert(root->tree, dirTEST4);

    // Arquivo em SO
    TreeNode* file1 = create_txt_file("arquivo1.txt", "Arquivo de teste de SO.");
    btree_insert(dirSO->data.directory->tree, file1);

    // Listagem
    printf("--- listar toda a arvore ---\n");
    list_directory_contents(root);
    btree_traverse(root->tree);

    printf("\n--- Conteúdo do diretório SO ---\n");
    list_directory_contents(dirSO->data.directory);
/*
    // Simulação de persistência
    FILE* img = fopen("fs.img", "w");
    if (img) {
        fprintf(img, "ROOT\n");
        fprintf(img, "├── SO\n");
        fprintf(img, "│   └── arquivo1.txt: Arquivo de teste de SO.\n");
        fprintf(img, "└── TEST\n");
        fclose(img);
        printf("\nSistema de arquivos salvo em fs.img\n");
    } else {
        perror("Erro ao criar fs.img");
    }

    TreeNode* found = btree_search(dirSO->data.directory->tree, "arquivo1.txt");
    if (found) {
        printf("Encontrado: %s\n", found->name);
    } else {
        printf("Arquivo não encontrado.\n");
    }

     TreeNode* found1 = btree_search(dirSO->data.directory->tree, "arquivo2.txt");
    if (found1) {
        printf("Encontrado: %s\n", found1->name);
    } else {
        printf("Arquivo não encontrado.\n");
    }

    /*delete_txt_file(dirSO->data.directory->tree, "arquivo1.txt");
    printf("--- Conteúdo do diretório SO após exclusão ---\n");
    list_directory_contents(dirSO->data.directory);

    // Cria diretório vazio "TEMP" dentro do root
    TreeNode* tempDir = create_directory("TEMP");
    btree_insert(root->tree, tempDir);
     printf("--- Conteúdo do diretório ROOT ---\n");
    list_directory_contents(root);

    // Deleta diretório TEMP
    delete_directory(root->tree, "TEMP");
    printf("--- Conteúdo do diretório ROOT após exclusão ---\n");
    list_directory_contents(root);

    // Cria diretório "DOCS" e adiciona na raiz
TreeNode* dirDOCS = create_directory("DOCS");
btree_insert(root->tree, dirDOCS);

// Cria arquivo dentro de "DOCS"
TreeNode* file2 = create_txt_file("manual.txt", "Este é o manual do sistema.");
btree_insert(dirDOCS->data.directory->tree, file2);

// Lista conteúdo do diretório DOCS
printf("--- Conteúdo do diretório DOCS ---\n");
list_directory_contents(dirDOCS->data.directory);

// Exclui o arquivo de dentro de DOCS
delete_txt_file(dirDOCS->data.directory->tree, "manual.txt");

// Agora exclui o diretório DOCS
//delete_directory(root->tree, "DOCS");

printf("--- Conteúdo do diretório ROOT após exclusão ---\n");
list_directory_contents(root);
/*
// Mudar o diretório atual para "SO"
Directory* current = root;
change_directory(&current, "SO");

// Mostrar o conteúdo do diretório atual
printf("\n--- Conteúdo do diretório atual após mudança para 'SO' ---\n");
list_directory_contents(current);

printf("\n--- Teste direto de btree_traverse para diretório ROOT ---\n");
btree_traverse(root->tree);

printf("\n--- Teste DOCS ---\n");
list_directory_contents(dirSO->data.directory);

    return 0;
}*/

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
        printf("9 - Mostrar árvore B (recursiva)\n");
        printf("10 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();  // limpa o '\n'

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
                    printf("Encontrado: %s (%s)\n", result->name,
                        result->type == FILE_TYPE ? "Arquivo" : "Diretório");
                } else {
                    printf("Item '%s' não encontrado.\n", nome);
                }
                break;

            case 9: // Mostrar árvore
                printf("Árvore do diretório atual:\n");
                btree_traverse(current->tree);
                break;

            case 10:
                printf("Encerrando sistema de arquivos.\n");
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}