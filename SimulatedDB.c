#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

typedef struct node *Data;
struct node {
    char namaQuest[1024];
    char reward[1024];
    int child_count;
    Data *children;
};

typedef struct {
    Data root;
} Tree;

Tree *forest = NULL;
int tree_count = 0;

Data createNode(char *nama, char *reward) {
    Data node = (Data)malloc(sizeof(struct node));
    strcpy(node->namaQuest, nama);
    strcpy(node->reward, reward);
    node->child_count = 0;
    node->children = NULL;
    return node;
}

Tree* createTree(char *nama) {
    tree_count++;
    forest = (Tree*)realloc(forest, tree_count * sizeof(Tree));
    char *reward = " ";
    forest[tree_count - 1].root = createNode(nama, reward);
    return &forest[tree_count - 1];
}

void insertNode(Data parent, char *nama, char *reward) {
    parent->child_count++;
    parent->children = (Data*)realloc(parent->children, parent->child_count * sizeof(Data));
    parent->children[parent->child_count - 1] = createNode(nama, reward);
}

Data findNode(Data root, char *namaQuest) {
    if (root == NULL) return NULL;
    if (strcmp(root->namaQuest, namaQuest) == 0) return root;
    Data result = NULL;
    for (int i = 0; i < root->child_count; i++) {
        result = findNode(root->children[i], namaQuest);
        if (result != NULL) return result;
    }
    return NULL;
}

void printNode(Data node) {
    if (node != NULL) {
        printf("Nama: %s, Reward: %s, Child Count: %d\n", node->namaQuest, node->reward, node->child_count);
    } else {
        printf("Node tidak ditemukan.\n");
    }
}

void printInorder(Data root, int level) {
    if (root == NULL) return;
    if (root->child_count > 0) printInorder(root->children[0], level + 1);

    printf("%*s%s (Reward: %s) (Level %d)\n", level * 2, "", root->namaQuest, root->reward, level);

    for (int i = 1; i < root->child_count; i++) {
        printInorder(root->children[i], level + 1);
    }
}

void printPreorder(Data root, int level) {
    if (root == NULL) return;

    printf("%*s%s (Reward: %s) (Level %d)\n", level * 2, "", root->namaQuest, root->reward, level);

    for (int i = 0; i < root->child_count; i++) {
        printPreorder(root->children[i], level + 1);
    }
}

void printPostorder(Data root, int level) {
    if (root == NULL) return;

    for (int i = 0; i < root->child_count; i++) {
        printPostorder(root->children[i], level + 1);
    }

    printf("%*s%s (Reward: %s) (Level %d)\n", level * 2, "", root->namaQuest, root->reward, level);
}

void freeTree(Data root) {
    if (root == NULL) return;

    for (int i = 0; i < root->child_count; i++) {
        freeTree(root->children[i]);
    }

    free(root->children);
    free(root);
}

void freeForest() {
    for (int i = 0; i < tree_count; i++) {
        freeTree(forest[i].root);
    }
    free(forest);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int pilihan; // Masukan User
    int count, treeIndex; // Awal Cabang/Cabang yang mau dihubungkan
    char nama[1024], reward[1024], parentNama[1024], searchNama[1024];
    
    while (1) {
        system("cls");
        printf("Daftar Pohon:\n");
        for (int i = 0; i < tree_count; i++) {
            printf("Pohon %d:\n", i + 1);
            printPreorder(forest[i].root, 0);
            printf("\n");
        }
        printf("1. Buat Quest Baru\n2. Masukkan Quest ke Cabang\n3. Menu Traverse\n4. Cari Node\n5. Exit\nMasukkan Pilihan: ");
        scanf("%d", &pilihan);
        clearInputBuffer(); // Bersihkan buffer setelah mengambil pilihan

        switch (pilihan) {
            case 1:
                system("cls");
                printf("Masukkan Nama: ");
                fgets(nama, sizeof(nama), stdin);
                nama[strcspn(nama, "\n")] = 0; // Hapus karakter newline jika ada
                createTree(nama);
                break;
            case 2:
                system("cls");
                if (tree_count == 0) {
                    printf("\nBelum ada tree yang dibuat.\n");
                    break;
                }

                printf("Masukkan nomor tree: ");
                scanf("%d", &treeIndex);
                clearInputBuffer(); // Bersihkan buffer setelah mengambil input
                if (treeIndex > tree_count) {
                    printf("Tree tidak ditemukan.\n");
                    break;
                }

                Tree *currentTree = &forest[treeIndex - 1];

                printf("\nJumlah Cabang: ");
                scanf("%d", &count);
                clearInputBuffer(); // Bersihkan buffer setelah mengambil input
                
                printf("Masukkan Nama Parent: ");
                fgets(parentNama, sizeof(parentNama), stdin);
                parentNama[strcspn(parentNama, "\n")] = 0; // Hapus karakter newline jika ada
                Data parent = findNode(currentTree->root, parentNama);
                if (parent != NULL) {
                    printf("Masukkan Nama dan Reward Cabang Baru (format: nama reward): ");
                    for (int i = 0; i < count; i++) {
                        fgets(nama, sizeof(nama), stdin);
                        nama[strcspn(nama, "\n")] = 0; // Hapus karakter newline jika ada
                        fgets(reward, sizeof(reward), stdin);
                        reward[strcspn(reward, "\n")] = 0; // Hapus karakter newline jika ada
                        insertNode(parent, nama, reward);
                    }
                } else {
                    printf("Parent dengan Nama %s tidak ditemukan.\n", parentNama);
                    break;
                }
                break;
            case 3:
                system("cls");
                if (tree_count == 0) {
                    printf("Belum ada tree yang dibuat.\n");
                    break;
                }

                printf("Masukkan nomor tree: ");
                scanf("%d", &treeIndex);
                clearInputBuffer(); // Bersihkan buffer setelah mengambil input
                if (treeIndex < 1 || treeIndex > tree_count) {
                    printf("Tree tidak ditemukan.\n");
                    break;
                }

                currentTree = &forest[treeIndex - 1];

                printf("\n1. InOrder\n2. PreOrder\n3. PostOrder\nPilihan: ");
                scanf("%d", &pilihan);
                clearInputBuffer(); // Bersihkan buffer setelah mengambil input

                if (pilihan == 1) printInorder(currentTree->root, 0);
                if (pilihan == 2) printPreorder(currentTree->root, 0);
                if (pilihan == 3) printPostorder(currentTree->root, 0);

                break;
            case 4:
                system("cls");
                if (tree_count == 0) {
                    printf("Belum ada tree yang dibuat.\n");
                    break;
                }

                printf("Masukkan nomor tree: ");
                scanf("%d", &treeIndex);
                clearInputBuffer(); // Bersihkan buffer setelah mengambil input
                if (treeIndex < 1 || treeIndex > tree_count) {
                    printf("Tree tidak ditemukan.\n");
                    break;
                }

                currentTree = &forest[treeIndex - 1];

                printf("Masukkan Nama Quest yang Dicari: ");
                fgets(searchNama, sizeof(searchNama), stdin);
                searchNama[strcspn(searchNama, "\n")] = 0; // Hapus karakter newline jika ada
                Data node = findNode(currentTree->root, searchNama);
                printNode(node);

                break;
            case 5:
                printf("Exit. . .\n");
                freeForest();
                system("cls");
                return 0;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
        system("pause");
        system("cls");
    }
    return 0;
}
