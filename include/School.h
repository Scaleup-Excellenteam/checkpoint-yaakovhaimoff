#pragma once

#include "macros.h"

struct Course;

struct Student {
    char *first_name;
    char *last_name;
    char telephone[TelephoneLength];
    struct GradeNode *grades[Classes];
};

struct StudentNode {
    struct Student student;
    struct StudentNode *next;
};

struct School {
    struct StudentNode *students[Levels][Classes];
};

struct TenBestStudentsForCourse {
    struct StudentNode *students[Levels][Classes][NumOfGrades];
};

struct StudentsPendingToBeExpelledNode {
    struct StudentNode *student;
    struct StudentsPendingToBeExpelledNode *next;
};

struct StudentsPendingToBeExpelled {
    struct StudentsPendingToBeExpelledNode *studentsPendingToBeExpelledNode[Levels];
};

struct StudentNode *mallocStudentNode();

struct StudentsPendingToBeExpelledNode *mallocStudentsPendingToBeExpelledNode();

void trimWhiteSpace(char *);

void getStudentName(char *, char *);

void getStudentLevelAndClass(int *, int *);

void getStudentTelephone(char *);

void assignStudentData(struct Student *,
                       char *, char *, char *);

struct StudentNode *
addStudent(struct School *,
           char *, char *, char *,
           int, int);

void addStudentToTenBestStudentsForCourse(struct TenBestStudentsForCourse *,
                                          struct StudentNode *, int);

void addStudentToStudentsPendingToBeExpelled(struct StudentsPendingToBeExpelled *,
                                             struct StudentNode *, int);

void freeStudentList(struct StudentNode *);

void freeStudents(struct School *);

void printStudents(struct School);

void printCountStudents(struct School);

struct StudentNode *searchStudentInLevelClass(struct StudentNode *,
                                              char *, char *);

struct StudentNode *
searchStudentByName(struct School *, char *, char *, int *, int *);

void deleteStudent(struct School *,
                   struct Course *,
                   int, int, char *);

void deleteStudentFromUser(struct School *,
                           struct Course *);

void search(struct School *);

void updateStudent(struct School *,
                   struct Course *);

void addNewStudentFromUser(struct School *,
                           struct Course *,
                           struct TenBestStudentsForCourse *);

void freeWorstStudents(struct StudentsPendingToBeExpelled *);

void printTenBestStudentsForCourse(struct TenBestStudentsForCourse *);

void printWorstStudent(struct StudentsPendingToBeExpelled *);