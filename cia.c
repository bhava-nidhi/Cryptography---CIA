#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Caesar Cipher
void caesar_encrypt(char *text, int key, char *result) {
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            result[i] = (text[i] - base + key) % 26 + base;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// Atbash Cipher
void atbash_cipher(char *text, char *result) {
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) result[i] = 'Z' - (text[i] - 'A');
        else if (islower(text[i])) result[i] = 'z' - (text[i] - 'a');
        else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// August Cipher (simple letter shift: a=1, b=2...wraps around)
void august_cipher(char *text, char *result) {
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            result[i] = (text[i] - base + (text[i] - base + 1)) % 26 + base;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// Affine Cipher
int mod_inverse(int a, int m) {
    for (int x = 1; x < m; x++) if ((a * x) % m == 1) return x;
    return -1;
}
void affine_encrypt(char *text, int a, int b, char *result) {
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            result[i] = (a * (text[i] - base) + b) % 26 + base;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// Vigenère Cipher
void vigenere_encrypt(char *text, char *key, char *result) {
    int keyLen = strlen(key);
    for (int i = 0, j = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int shift = toupper(key[j % keyLen]) - 'A';
            result[i] = (text[i] - base + shift) % 26 + base;
            j++;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// Gronsfeld Cipher
void gronsfeld_encrypt(char *text, char *key, char *result) {
    for (int i = 0, j = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            int shift = key[j % strlen(key)] - '0';
            char base = isupper(text[i]) ? 'A' : 'a';
            result[i] = (text[i] - base + shift) % 26 + base;
            j++;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// Beaufort Cipher
void beaufort_cipher(char *text, char *key, char *result) {
    int keyLen = strlen(key);
    for (int i = 0, j = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int k = toupper(key[j % keyLen]) - 'A';
            result[i] = (26 + k - (toupper(text[i]) - 'A')) % 26 + base;
            j++;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// Autoclave (Running Key)
void autokey_cipher(char *text, char *key, char *result) {
    char fullKey[2048];
    strcpy(fullKey, key);
    strcat(fullKey, text);
    int keyLen = strlen(fullKey);
    for (int i = 0, j = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int shift = toupper(fullKey[j]) - 'A';
            result[i] = (text[i] - base + shift) % 26 + base;
            j++;
        } else result[i] = text[i];
    }
    result[strlen(text)] = '\0';
}

// N-gram Operation (bi-grams shown)
void ngram_operation(char *text, int n, char *result) {
    int len = strlen(text);
    result[0] = '\0';
    for (int i = 0; i < len - n + 1; i++) {
        strncat(result, &text[i], n);
        strcat(result, " ");
    }
}

// Hill Cipher (2x2 matrix, assumes even length)
void hill_cipher_encrypt(char *text, char *key, char *result) {
    int matrix[2][2] = { {key[0]-'0', key[1]-'0'}, {key[2]-'0', key[3]-'0'} };
    int len = strlen(text);
    if (len % 2 != 0) text[len++] = 'X'; // padding
    for (int i = 0; i < len; i += 2) {
        int a = toupper(text[i]) - 'A';
        int b = toupper(text[i+1]) - 'A';
        result[i]   = ((matrix[0][0]*a + matrix[0][1]*b) % 26) + 'A';
        result[i+1] = ((matrix[1][0]*a + matrix[1][1]*b) % 26) + 'A';
    }
    result[len] = '\0';
}

// Rail Fence Cipher
void rail_fence_encrypt(char *text, int rails, char *result) {
    int len = strlen(text);
    char rail[rails][len];
    memset(rail, 0, sizeof(rail));
    int row = 0, dir = 1;

    for (int i = 0; i < len; i++) {
        rail[row][i] = text[i];
        row += dir;
        if (row == rails - 1 || row == 0) dir *= -1;
    }

    int idx = 0;
    for (int i = 0; i < rails; i++)
        for (int j = 0; j < len; j++)
            if (rail[i][j] != 0) result[idx++] = rail[i][j];

    result[idx] = '\0';
}

// Route Cipher (matrix fill and read by column)
void route_cipher_encrypt(char *text, int rows, int cols, char *result) {
    char grid[rows][cols];
    int len = strlen(text), k = 0;

    for (int i = 0; i < rows && k < len; i++)
        for (int j = 0; j < cols && k < len; j++)
            grid[i][j] = text[k++];

    int idx = 0;
    for (int j = 0; j < cols; j++)
        for (int i = 0; i < rows; i++)
            result[idx++] = grid[i][j];

    result[idx] = '\0';
}

// Myszkowski Cipher (simple simulation)
void myszkowski_encrypt(char *text, char *key, char *result) {
    int len = strlen(text), keyLen = strlen(key);
    char order[10] = "0123456789";
    int idx = 0;
    for (char ch = '0'; ch <= '9'; ch++) {
        for (int i = 0; i < keyLen; i++) {
            if (key[i] == ch) {
                for (int j = i; j < len; j += keyLen) {
                    result[idx++] = text[j];
                }
            }
        }
    }
    result[idx] = '\0';
}

// ------------------- MAIN FUNCTION ---------------------
int main() {
    int choice;
    char plaintext[1024], ciphertext[1024], key[1024];

    while (1) {
        printf("\nChoose a Cipher:\n");
        printf("1. Caesar\n2. Atbash\n3. August\n4. Affine\n5. Vigenère\n6. Gronsfeld\n7. Beaufort\n8. Autoclave/Running Key\n9. Ngram operations\n10. Hill Cipher\n11. Rail Fence\n12. Route\n13. Myszkowski\n0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        if (choice == 0) break;

        printf("Enter Plaintext: ");
        fgets(plaintext, sizeof(plaintext), stdin);
        plaintext[strcspn(plaintext, "\n")] = '\0';

        if (choice != 2 && choice != 3 && choice != 9) {
            printf("Enter Key (if applicable): ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';
        }

        switch (choice) {
            case 1: caesar_encrypt(plaintext, atoi(key), ciphertext); break;
            case 2: atbash_cipher(plaintext, ciphertext); break;
            case 3: august_cipher(plaintext, ciphertext); break;
            case 4: affine_encrypt(plaintext, atoi(key), 5, ciphertext); break;
            case 5: vigenere_encrypt(plaintext, key, ciphertext); break;
            case 6: gronsfeld_encrypt(plaintext, key, ciphertext); break;
            case 7: beaufort_cipher(plaintext, key, ciphertext); break;
            case 8: autokey_cipher(plaintext, key, ciphertext); break;
            case 9: ngram_operation(plaintext, 2, ciphertext); break;
            case 10: hill_cipher_encrypt(plaintext, key, ciphertext); break;
            case 11: rail_fence_encrypt(plaintext, atoi(key), ciphertext); break;
            case 12: route_cipher_encrypt(plaintext, 4, 4, ciphertext); break;
            case 13: myszkowski_encrypt(plaintext, key, ciphertext); break;
            default: printf("Invalid choice!\n"); continue;
        }

        printf("Ciphertext: %s\n", ciphertext);
    }

    return 0;
}
