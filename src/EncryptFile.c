#include "EncryptFile.h"

//void set_key(unsigned char *key) {
//    struct OpenSSL openSSL;
//
//    // Generate random key
//    if (RAND_bytes(key, sizeof(key)) != 1) {
//        printf("Error generating random key.\n");
//        exit(1);
//    }
//}
//
//void set_iv(unsigned char *iv) {
//    // Generate random IV
//    if (RAND_bytes(iv, sizeof(iv)) != 1) {
//        printf("Error generating random iv.\n");
//        exit(1);
//    }
//}
void set_key(unsigned char *key) {
    // Hardcoded 256-bit key (32 bytes)
    unsigned char hardcoded_key[] = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
            0x01, 0x23, 0x45, 0x67, 0x89, 0x9A, 0xBC, 0xDE,
            0xF0, 0xE1, 0xC2, 0xB3, 0xA4, 0x95, 0x86, 0x77
    };

    memcpy(key, hardcoded_key, 32); // Copy the hardcoded key to the provided buffer
}

void set_iv(unsigned char *iv) {
    // Hardcoded 128-bit IV (16 bytes)
    unsigned char hardcoded_iv[] = {
            0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
            0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
    };

    memcpy(iv, hardcoded_iv, 16); // Copy the hardcoded IV to the provided buffer
}


void init_openssl() {
    set_key(key);
    set_iv(iv);
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;
    ciphertext[ciphertext_len] = '\0';

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;
//    ciphertext[ciphertext_len] = '\0';

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void encryptFileAndSaveToFile() {
    FILE *filePointer;
    filePointer = fopen(FILE_NAME, "r");

    FILE *encryptFilePointer;
    encryptFilePointer = fopen(ENCRYPT_FILE_NAME, "w");

    if (!filePointer || !encryptFilePointer) {
        perror("Error opening file");
        exit(1);
    }

    // Initialize OpenSSL library
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    unsigned char ciphertext[BUFFER_SIZE];
    // Buffer to hold each line from the input file
    char line[BUFFER_SIZE];
    int ciphertext_len;

// Read data from the input file line by line and encrypt it
    while (fgets(line, sizeof(line), filePointer)) {
        // Encrypt the line
        ciphertext_len = encrypt((unsigned char *) line, strlen(line), key, iv, ciphertext);

        // Write the size of the encrypted data at the beginning of the file
        fwrite(&ciphertext_len, sizeof(int), 1, encryptFilePointer);

        // Write the encrypted data to the output file
        if (fwrite(ciphertext, 1, ciphertext_len, encryptFilePointer) != ciphertext_len) {
            perror("Error writing to output file");
            fclose(filePointer);
            fclose(encryptFilePointer);
            exit(1);
        }
    }
    // Close the files
    fclose(filePointer);
    fclose(encryptFilePointer);
}