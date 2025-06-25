#include "filesystem.h"

int main() {
    // Criação do sistema de arquivos
    Directory* root = get_root_directory();

    // Diretórios de segundo nível
    TreeNode* dirSO = create_directory("SO");
    TreeNode* dirTEST = create_directory("TEST");
    btree_insert(root->tree, dirSO);
    btree_insert(root->tree, dirTEST);

    // Arquivo em SO
    TreeNode* file1 = create_txt_file("arquivo1.txt", "Arquivo de teste de SO.");
    btree_insert(dirSO->data.directory->tree, file1);

    // Listagem
    printf("--- Conteúdo do diretório ROOT ---\n");
    list_directory_contents(root);

    printf("\n--- Conteúdo do diretório SO ---\n");
    list_directory_contents(dirSO->data.directory);

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
*/
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
*/
printf("\n--- Teste direto de btree_traverse para diretório ROOT ---\n");
btree_traverse(root->tree);

printf("\n--- Teste DOCS ---\n");
list_directory_contents(dirSO->data.directory);

    return 0;
}
