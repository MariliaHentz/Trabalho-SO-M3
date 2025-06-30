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
/////////////////////////////
void btree_split_child(BTreeNode* parent, int i, BTreeNode* full_child) {
    int t = BTREE_ORDER;

    BTreeNode* new_child = malloc(sizeof(BTreeNode));
    new_child->leaf = full_child->leaf;
    new_child->num_keys = t - 1;

    // Copia as últimas t-1 chaves para o novo filho
    for (int j = 0; j < t - 1; j++) {
        new_child->keys[j] = full_child->keys[j + t];
    }

    // Se não for folha, copia os filhos também
    if (!full_child->leaf) {
        for (int j = 0; j < t; j++) {
            new_child->children[j] = full_child->children[j + t];
        }
    }

    full_child->num_keys = t - 1;

    // Move os filhos do pai para abrir espaço
    for (int j = parent->num_keys; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = new_child;

    // Move as chaves do pai para abrir espaço para a chave promovida
    for (int j = parent->num_keys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[i] = full_child->keys[t - 1]; // chave do meio sobe
    parent->num_keys++;
}

void btree_insert_nonfull(BTreeNode* node, TreeNode* item) {
    int i = node->num_keys - 1;

    if (node->leaf) {
        // Move as chaves para abrir espaço
        while (i >= 0 && strcmp(item->name, node->keys[i]->name) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        // Verifica duplicação
        if (i >= 0 && strcmp(item->name, node->keys[i]->name) == 0) {
            printf("Erro: item com nome '%s' já existe.\n", item->name);
            return;
        }

        node->keys[i + 1] = item;
        node->num_keys++;
    } else {
        // Encontra o filho correto
        while (i >= 0 && strcmp(item->name, node->keys[i]->name) < 0) {
            i--;
        }
        i++;

        // Verifica duplicação
        if (i < node->num_keys && strcmp(item->name, node->keys[i]->name) == 0) {
            printf("Erro: item com nome '%s' já existe.\n", item->name);
            return;
        }

        // Divide se necessário
        if (node->children[i]->num_keys == 2 * BTREE_ORDER - 1) {
            btree_split_child(node, i, node->children[i]);
            if (strcmp(item->name, node->keys[i]->name) > 0) {
                i++;
            }
        }

        btree_insert_nonfull(node->children[i], item);
    }
}

void btree_insert(BTree* tree, TreeNode* item) {
    if (!tree->root) {
        tree->root = malloc(sizeof(BTreeNode));
        tree->root->leaf = 1;
        tree->root->num_keys = 0;
    }

    BTreeNode* root = tree->root;

    // Verifica se a chave já existe
    if (btree_search(tree, item->name) != NULL) {
        printf("Erro: item com nome '%s' já existe.\n", item->name);
        return;
    }

    // Se a raiz está cheia, cria nova raiz e divide
    if (root->num_keys == 2 * BTREE_ORDER - 1) {
        BTreeNode* new_root = malloc(sizeof(BTreeNode));
        new_root->leaf = 0;
        new_root->num_keys = 0;
        new_root->children[0] = root;

        btree_split_child(new_root, 0, root);
        btree_insert_nonfull(new_root, item);
        tree->root = new_root;
    } else {
        btree_insert_nonfull(root, item);
    }
}


void btree_traverse_node(BTreeNode* node) {
    static int indent = 0;
    if (!node) return;

    int i;
    for (i = 0; i < node->num_keys; i++) {
        // Visita o filho antes da chave, se não for folha
        if (!node->leaf) {
            indent++;
            btree_traverse_node(node->children[i]);
            indent--;
        }

        for (int j = 0; j < indent; j++) {
            printf("  ");  // 2 espaços por nível
        }

        // Imprime o nome do arquivo ou diretório
        printf("- %s\n", node->keys[i]->name);

        // Se for diretório, mostra conteúdo recursivamente
        if (node->keys[i]->type == DIRECTORY_TYPE) {
            for (int j = 0; j < indent; j++) {
                printf("  ");
            }

            printf("  (Conteúdo de %s):\n", node->keys[i]->name);
            indent++;
            btree_traverse(node->keys[i]->data.directory->tree);
            indent--;
        }
    }

    // Visita o último filho
    if (!node->leaf) {
        indent++;
        btree_traverse_node(node->children[i]);
        indent--;
    }
}

void btree_traverse(BTree* tree) {
    if (!tree || !tree->root) return;
    btree_traverse_node(tree->root);
}


void delete_txt_file(BTree* tree, const char* name) {
    TreeNode* aux = btree_search(tree, name);
    if (aux && aux->type == FILE_TYPE) {
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

    for (int i = 0; i < root_node->num_keys; i++) {
        printf("- %s\n", root_node->keys[i]->name);
    }
}

void change_directory(Directory** current, const char* path) {
    TreeNode* aux = btree_search((*current)->tree, path);
    if (aux && aux->type == DIRECTORY_TYPE) {
        *current = aux->data.directory;
        printf("Diretório alterado para: %s\n", path);
    } else {
        printf("Diretório '%s' não encontrado.\n", path);
    }
    
}

void delete_directory(BTree* tree, const char* name) {
    TreeNode* aux = btree_search(tree, name);
    if (aux && aux->type == DIRECTORY_TYPE) {
        
        if (aux->data.directory->tree->root == NULL || aux->data.directory->tree->root->num_keys == 0) {
            btree_delete(tree, name);
        } else {
            printf("Diretório '%s' não está vazio.\n", name);
        }
    } else {
        printf("Diretório '%s' não encontrado ou não é um diretório.\n", name);
    }
}



void btree_delete(BTree* tree, const char* name) {

    if (!tree || !tree->root) return;

    BTreeNode* node = tree->root;
    for (int i = 0; i < node->num_keys; i++) {
        if (strcmp(node->keys[i]->name, name) == 0) {
            // Libera a memória do TreeNode
            if (node->keys[i]->type == FILE_TYPE) {
                free(node->keys[i]->data.file->content);
                free(node->keys[i]->data.file->name);
                free(node->keys[i]->data.file);
            } 

            // Remove o ponteiro
            free(node->keys[i]->name);
            free(node->keys[i]);

            // Move chaves para preencher o espaço
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

////////////
void save_tree_to_file(FILE* file, BTreeNode* node, int depth);

void save_directory_image(FILE* file, Directory* dir, int depth) {
    if (!dir || !dir->tree || !dir->tree->root) return;
    save_tree_to_file(file, dir->tree->root, depth);
}

void save_tree_to_file(FILE* file, BTreeNode* node, int depth) {
    if (!node) return;

    int i;
    for (i = 0; i < node->num_keys; i++) {
        for (int d = 0; d < depth; d++) fprintf(file, "  "); 
        fprintf(file, "- %s (%s)\n", node->keys[i]->name,
                node->keys[i]->type == FILE_TYPE ? "Arquivo" : "Diretório");

        // Se for diretório, salvar conteúdo recursivo
        if (node->keys[i]->type == DIRECTORY_TYPE) {
            save_directory_image(file, node->keys[i]->data.directory, depth + 1);
        }

        if (!node->leaf) {
            save_tree_to_file(file, node->children[i], depth);
        }
    }

    if (!node->leaf) {
        save_tree_to_file(file, node->children[i], depth);
    }
}

void save_filesystem_image(const Directory* root, const char* path) {
    FILE* file = fopen(path, "w");
    if (!file) {
        perror("Erro ao criar fs.img");
        return;
    }

    fprintf(file, "Estrutura do Sistema de Arquivos:\n");
    save_directory_image(file, (Directory*)root, 0);
    fclose(file);

    printf("Sistema de arquivos salvo em '%s'\n", path);
}
