#pragma once

#include "macros.h"

struct OpenSSL {
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
};

void handleErrors(void);

unsigned char key[KEY_SIZE];
unsigned char iv[IV_SIZE];

void set_key(unsigned char*);
void set_iv(unsigned char*);

void init_openssl();

int encrypt(unsigned char *, int , unsigned char *,
            unsigned char *, unsigned char *);

int decrypt(unsigned char *, int , unsigned char *,
            unsigned char *, unsigned char *);

void encryptFileAndSaveToFile();