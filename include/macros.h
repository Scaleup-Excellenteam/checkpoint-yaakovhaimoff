#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Levels 12
#define Classes 10
#define TelephoneLength 11
#define NumOfGrades 10
#define NameLength 100

#define OPTION_PRINT_STUDENTS 0
#define OPTION_ADD_STUDENT 1
#define OPTION_DELETE_STUDENT 2
#define OPTION_UPDATE_STUDENT 3
#define OPTION_SEARCH_STUDENT 4
#define OPTION_EXIT 5

#define FILE_NAME "/Users/yaakovhaimoff/Desktop/school/year_3/semester2/excellents/bootcamp/school_db/students.txt"

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

extern const char* courseStrings[] = {
        "Math",
        "English",
        "Science",
        "History",
        "Physics",
        "Chemistry",
        "Biology",
        "Computer Science",
        "Literature",
        "Art"
};
