#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <signal.h>
#include <stdbool.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define KEY_SIZE 32   // 256 bits for AES-256
#define IV_SIZE 16    // 128 bits for AES-256 CBC

#define Levels 12
#define Classes 10
#define TelephoneLength 11
#define NumOfGrades 10
#define NameLength 100
#define InputLength 15
#define PendingToBeExpelled 65
#define BUFFER_SIZE 1024

#define FIRST_NAME 0
#define LAST_NAME 1
#define TELEPHONE 2
#define LEVEL 3
#define CLASS 4
#define GRADES 5


#define OPTION_PRINT_STUDENTS 0
#define OPTION_ADD_STUDENT 1
#define OPTION_DELETE_STUDENT 2
#define OPTION_UPDATE_STUDENT 3
#define OPTION_SEARCH_STUDENT 4
#define OPTION_PRINT_TEN_BEST_STUDENTS_FOR_COURSE 5
#define OPTION_AVG_FOR_COURSE_AND_LEVEL 6
#define OPTION_PRINT_WORST_STUDENT 7
#define OPTION_EXIT 8

#define FILE_NAME "/Users/yaakovhaimoff/Desktop/school/year_3/semester2/excellents/bootcamp/school_db/students_with_class.txt"
#define ENCRYPT_FILE_NAME "/Users/yaakovhaimoff/Desktop/school/year_3/semester2/excellents/bootcamp/school_db/encryptFile.txt"
//#define FILE_NAME "/Users/yaakovhaimoff/Desktop/school/year_3/semester2/excellents/bootcamp/school_db/students.txt"

enum Courses {
    MATH,
    ENGLISH,
    SCIENCE,
    HISTORY,
    PHYSICS,
    CHEMISTRY,
    BIOLOGY,
    COMPUTER_SCIENCE,
    LITERATURE,
    ART,
    NUM_COURSES // Add this to keep track of the number of courses
};
