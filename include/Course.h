#pragma once

#include "macros.h"

struct School;

struct Grade {
    int grade;
    struct StudentNode *student;
};

struct GradeNode {
    struct Grade grade;
    struct GradeNode *next;
};

struct Course {
    struct GradeNode *grades[Levels][Classes][NumOfGrades];
};

extern const char* courseStrings[NumOfGrades];

struct GradeNode *mallocGradeNode();

void assignGradeData(struct Grade *, int,
                     struct StudentNode *);

void addStudentGrades(struct Course *,
                      struct School *,
                      struct StudentNode *,
                      int, int, const int *);

void printGrades(struct Course *);

void freeCourses(struct Course *);

void deleteStudentFromGrades(struct Course *,
                             struct StudentNode *,
                             int, int);

void avgForCourseAndLevel(struct Course *);


