#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "crypto.h"

int main() {
    char password[128];
    char plaintext[128];

    printf("Enter a 16-character plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0;  // remove newline

    if (strlen(plaintext) != 16) {
        printf("Error: Plaintext must be exactly 16 characters.\n");
        return 1;
    }
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;  // remove newline

    uint8_t key[16];
    uint8_t encrypted[16];
    uint8_t decrypted[16];

    key_from_password(password, key);

    aes_encrypt((uint8_t *)plaintext, key, encrypted);

    printf("\nEncrypted (hex): ");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", encrypted[i]);
    }
    printf("\n");

    aes_decrypt(encrypted, key, decrypted);
    printf("Decrypted text: %.*s\n", 16, decrypted);

    return 0;
}

