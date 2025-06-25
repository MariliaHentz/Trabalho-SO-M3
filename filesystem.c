#include "filesystem.h"

BTree* btree_create() {
    BTree* tree = malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

TreeNode* create_txt_file(const char* name, const char* content) {
    File* file = malloc(sizeof(File));
    file->name = strdup(name);
    file->content = strdup(content);
    file->size = strlen(content);

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = FILE_TYPE;
    node->data.file = file;
    return node;
}

TreeNode* create_directory(const char* name) {
    Directory* dir = malloc(sizeof(Directory));
    dir->tree = btree_create();

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = DIRECTORY_TYPE;
    node->data.directory = dir;
    return node;
}

Directory* get_root_directory() {
    Directory* root = malloc(sizeof(Directory));
    root->tree = btree_create();
    return root;
}


/*
void btree_insert(BTree* tree, TreeNode* node) {
    printf("Inserindo: %s (simulado)\n", node->name);
}


void btree_traverse(BTree* tree) {
    printf("[Exemplo] arquivo.txt\n");

}
*/
void btree_insert(BTree* tree, TreeNode* node) {
    if (!tree->root) {
        tree->root = malloc(sizeof(BTreeNode));
        tree->root->num_keys = 0;
        tree->root->leaf = 1;
    }

    BTreeNode* root = tree->root;

    // Simples: sem divisão de nós (para grau pequeno e teste inicial)
    if (root->num_keys < (2 * BTREE_ORDER - 1)) {
        int i = root->num_keys - 1;
        while (i >= 0 && strcmp(node->name, root->keys[i]->name) < 0) {
            root->keys[i + 1] = root->keys[i];
            i--;
        }
        root->keys[i + 1] = node;
        root->num_keys++;
    } else {
        printf("Inserção não implementada para nós cheios (overflow).\n");
    }
}


void btree_traverse(BTree* tree) {
    if (!tree || !tree->root) return;

    BTreeNode* node = tree->root;

    // Itera sobre todas as chaves (arquivos e diretórios) do nó raiz
    for (int i = 0; i < node->num_keys; i++) {
        printf("- %s\n", node->keys[i]->name); // Exibe o nome do arquivo ou diretório

        // Se for um diretório, chama recursivamente para listar o conteúdo do subdiretório
        if (node->keys[i]->type == DIRECTORY_TYPE) {
            btree_traverse(node->keys[i]->data.directory->tree);  // chamada recursiva
        }
    }
}


void delete_txt_file(BTree* tree, const char* name) {
    TreeNode* found = btree_search(tree, name);
    if (found && found->type == FILE_TYPE) {
        btree_delete(tree, name);
    } else {
        printf("Arquivo '%s' não encontrado ou não é um arquivo .txt.\n", name);
    }
}

void list_directory_contents(Directory* dir) {
    if (!dir || !dir->tree || !dir->tree->root) {
        printf("Diretório vazio.\n");
        return;
    }

    BTreeNode* root_node = dir->tree->root;
    if (root_node->num_keys == 0) {
        printf("Diretório vazio.\n");
        return;
    }

    printf("Conteúdo do diretório:\n");

    // Itera sobre todas as chaves do nó raiz (diretórios e arquivos)
    for (int i = 0; i < root_node->num_keys; i++) {
        // Exibe o nome do arquivo ou diretório
        printf("- %s\n", root_node->keys[i]->name);

        // Se for um diretório, chama recursivamente para listar o conteúdo do subdiretório
        if (root_node->keys[i]->type == DIRECTORY_TYPE) {
            // Aqui estamos chamando list_directory_contents de forma recursiva
            list_directory_contents(root_node->keys[i]->data.directory);
        }
    }
}

void change_directory(Directory** current, const char* path) {
    TreeNode* found = btree_search((*current)->tree, path);
    if (found && found->type == DIRECTORY_TYPE) {
        *current = found->data.directory;
        printf("Diretório alterado para: %s\n", path);
    } else {
        printf("Diretório '%s' não encontrado.\n", path);
    }
}

void delete_directory(BTree* tree, const char* name) {
    TreeNode* found = btree_search(tree, name);
    if (found && found->type == DIRECTORY_TYPE) {
        // Verifica se o diretório está vazio
        if (found->data.directory->tree->root == NULL || found->data.directory->tree->root->num_keys == 0) {
            btree_delete(tree, name);
        } else {
            printf("Diretório '%s' não está vazio.\n", name);
        }
    } else {
        printf("Diretório '%s' não encontrado ou não é um diretório.\n", name);
    }
}



void btree_delete(BTree* tree, const char* name) {
    // OBS: Versão simplificada que apenas marca como excluído na árvore
    // (não faz rebalanceamento ainda)

    if (!tree || !tree->root) return;

    BTreeNode* node = tree->root;
    for (int i = 0; i < node->num_keys; i++) {
        if (strcmp(node->keys[i]->name, name) == 0) {
            // Libera a memória do TreeNode
            if (node->keys[i]->type == FILE_TYPE) {
                free(node->keys[i]->data.file->content);
                free(node->keys[i]->data.file->name);
                free(node->keys[i]->data.file);
            } else if (node->keys[i]->type == DIRECTORY_TYPE) {
                // Não implementado: liberar recursivamente os filhos
            }
            free(node->keys[i]->name);
            free(node->keys[i]);

            // Move as outras chaves para preencher o espaço
            for (int j = i; j < node->num_keys - 1; j++) {
                node->keys[j] = node->keys[j + 1];
            }
            node->num_keys--;
            printf("Item '%s' deletado.\n", name);
            return;
        }
    }

    printf("Item '%s' não encontrado para deletar.\n", name);
}

TreeNode* btree_search(BTree* tree, const char* name) {
    if (!tree || !tree->root) return NULL;

    BTreeNode* node = tree->root;

    while (node != NULL) {
        int i = 0;
        while (i < node->num_keys && strcmp(name, node->keys[i]->name) > 0) {
            i++;
        }

        if (i < node->num_keys && strcmp(name, node->keys[i]->name) == 0) {
            return node->keys[i];
        }

        if (node->leaf) {
            return NULL;
        } else {
            node = node->children[i];
        }
    }

    return NULL;
}