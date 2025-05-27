#include <stdint.h>
#include <string.h>
#include "crypto.h"

// Multiplicative Folding Hash
void key_from_password(const char *password, uint8_t *key) {
    memset(key, 0, 16); // start with all 0s

    for (int i = 0; password[i] != '\0'; i++) {
        key[i % 16] = (key[i % 16] * 31 + password[i]) % 256;
    }
}

