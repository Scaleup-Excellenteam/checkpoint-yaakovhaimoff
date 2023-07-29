#include "School.h"
#include "Course.h"

struct StudentNode *mallocStudentNode() {
    struct StudentNode *studentNode = malloc(sizeof(struct StudentNode));
    if (studentNode == NULL) {
        printf("Error: malloc failed in mallocStudentNode\n");
        exit(1);
    }
    studentNode->next = NULL;
    return studentNode;
}

struct StudentsPendingToBeExpelledNode *mallocStudentsPendingToBeExpelledNode() {
    struct StudentsPendingToBeExpelledNode *mallocStudentsPendingToBeExpelledNode =
            malloc(sizeof(struct StudentsPendingToBeExpelledNode));
    if (mallocStudentsPendingToBeExpelledNode == NULL) {
        printf("Error: malloc failed in mallocStudentsPendingToBeExpelled\n");
        exit(1);
    }
    return mallocStudentsPendingToBeExpelledNode;
}

void trimWhiteSpace(char *str) {
    char *end;

    // Trim leading white spaces
    while (isspace((unsigned char) *str)) {
        str++;
    }

    if (*str == '\0') { // All spaces, no need to trim
        return;
    }

    // Trim trailing white spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';
}

void getStudentName(char *firstName, char *lastName) {
    printf("Enter the student's first name: ");
    scanf("%s", firstName);
    trimWhiteSpace(firstName);
    printf("Enter the student's last name: ");
    scanf("%s", lastName);
    trimWhiteSpace(firstName);
}

void getStudentLevelAndClass(int *level, int *class) {
    printf("Enter the student's level: ");
    scanf("%d", &*level);
    *level -= 1;
    printf("Enter the student's class: ");
    scanf("%d", &*class);
    *class -= 1;
}

void getStudentTelephone(char *telephone) {
    printf("Enter the student's telephone: ");
    scanf("%s", telephone);
}

void assignStudentData(struct Student *student,
                       char *firstName, char *lastName, char *telephone) {
    student->first_name = strdup(firstName);
    student->last_name = strdup(lastName);
    strcpy(student->telephone, telephone);
}

struct StudentNode *
addStudent(struct School *school,
           char *firstName, char *lastName, char *telephone,
           int level, int class) {
    if (school->students[level][class] == NULL) {
        // If there are no students for the given level and class, add the new student as the first student.
        school->students[level][class] = mallocStudentNode();
        assignStudentData(&school->students[level][class]->student, firstName, lastName, telephone);
        school->students[level][class]->next = NULL; // Set next to NULL as this is the only student for now.
    } else {
        // If there are existing students, create a new student node and link it to the current first student.
        struct StudentNode *newStudent = mallocStudentNode();
        assignStudentData(&newStudent->student, firstName, lastName, telephone);
        newStudent->next = school->students[level][class];
        school->students[level][class] = newStudent; // Update the first student to be the new student.
    }
    return school->students[level][class];
}

void addStudentToTenBestStudentsForCourse(struct TenBestStudentsForCourse *tenBestStudentsForCourse,
                                          struct StudentNode *studentForGrade, int level) {
    for (int class = 0; class < Classes; class++) {
        for (int grade = 0; grade < NumOfGrades; grade++) {
            if (tenBestStudentsForCourse->students[level][class][grade] == NULL ||
                tenBestStudentsForCourse->students[level][class][grade]->student.grades[grade]->grade.grade <
                studentForGrade->student.grades[grade]->grade.grade) {
                tenBestStudentsForCourse->students[level][class][grade] = studentForGrade;
                break;
            }
        }
    }
}

void addStudentToStudentsPendingToBeExpelled(struct StudentsPendingToBeExpelled *studentsPendingToBeExpelled,
                                             struct StudentNode *studentForGrade, int level) {
    int sum = 0;
    for (int grade = 0; grade < NumOfGrades; grade++) {
        sum += studentForGrade->student.grades[grade]->grade.grade;
    }
    if (sum / NumOfGrades < PendingToBeExpelled) {
        if (studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level] == NULL) {
            studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level] = mallocStudentsPendingToBeExpelledNode();
            studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level]->student = studentForGrade;
            studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level]->next = NULL;
        } else {
            struct StudentsPendingToBeExpelledNode *newStudentPendingToBeExpelled = mallocStudentsPendingToBeExpelledNode();
            newStudentPendingToBeExpelled->next = studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level];
            newStudentPendingToBeExpelled->student = studentForGrade;
            studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level] = newStudentPendingToBeExpelled;
        }
    }
}

void freeStudentList(struct StudentNode *node) {
    while (node != NULL) {
        struct StudentNode *temp = node;
        node = node->next;
        free(temp->student.first_name);
        free(temp->student.last_name);
        free(temp);
    }
}

void freeStudents(struct School *school) {
    for (int i = 0; i < Levels; i++) {
        for (int j = 0; j < Classes; j++) {
            struct StudentNode *node = school->students[i][j];
            freeStudentList(node);
            school->students[i][j] = NULL;
        }
    }
}

void printStudents(struct School school) {
    for (int level = 0; level < Levels; level++) {
        for (int class = 0; class < Classes; class++) {
            struct StudentNode *node = school.students[level][class];
            if (node == NULL) {
                continue;
            }
            while (node != NULL) {
                printf("Name: %s %s\n", node->student.first_name, node->student.last_name);
                printf("Telephone: %s\n", node->student.telephone);
                printf("Level: %d, Class: %d\n", level + 1, class + 1);
                printf("Grades: ");
                for (int i = 0; i < NumOfGrades; i++) {
                    printf("%d ", node->student.grades[i]->grade.grade);
                }
                printf("\n\n");
                node = node->next;
            }
        }
    }
}

struct StudentNode *searchStudentInLevelClass(struct StudentNode *node, char *firstName, char *lastName) {
    while (node != NULL) {
        if (strcmp(node->student.first_name, firstName) == 0
            && strcmp(node->student.last_name, lastName) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL; // Student not found in this level and class
}

struct StudentNode *
searchStudentByName(struct School *school, char *firstName, char *lastName, int *levelToSave, int *classToSave) {
    for (int level = 0; level < Levels; level++) {
        for (int class = 0; class < Classes; class++) {
            struct StudentNode *foundStudent =
                    searchStudentInLevelClass(school->students[level][class], firstName, lastName);
            if (foundStudent != NULL) {
                *levelToSave = level;
                *classToSave = class;
                return foundStudent;
            }
        }
    }
    return NULL; // Student not found
}

void deleteStudent(struct School *school, struct Course *courses, int level, int class, char *telephone) {
    struct StudentNode *current = school->students[level][class];
    struct StudentNode *prev = NULL;

    // Find the student in the student list
    while (current != NULL) {
        if (strcmp(current->student.telephone, telephone) == 0) {
            // Remove the student from the student list
            if (prev == NULL) {
                // If the student is the first one in the list
                school->students[level][class] = current->next;
            } else {
                prev->next = current->next;
            }

            // Free the memory for the removed student's first_name and last_name
            free(current->student.first_name);
            free(current->student.last_name);

            // Free the memory for the student node
            free(current);

            // Remove the student's grades from the courses
            deleteStudentFromGrades(courses, current, level, class);

            return; // Exit the function after deleting the student
        }

        prev = current;
        current = current->next;
    }

    printf("Student not found.\n");
}

void deleteStudentFromUser(struct School *school, struct Course *courses) {
    int level, class;
    char telephone[TelephoneLength];
    getStudentLevelAndClass(&level, &class);
    getStudentTelephone(telephone);

    deleteStudent(school, courses, level, class, telephone);
}

void search(struct School *school) {
    char firstName[NameLength], lastName[NameLength];
    getStudentName(firstName, lastName);

    int level = 0, class = 0;
    struct StudentNode *student = searchStudentByName(school, firstName, lastName, &level, &class);
    if (student == NULL) {
        printf("Student not found.\n");
        return;
    }

    printf("For %s %s:\n", student->student.first_name, student->student.last_name);
    printf("Level: %d\n", level + 1);
    printf("Class: %d\n", class + 1);
    printf("Telephone: %s\n", student->student.telephone);
    printf("Grades: ");
    for (int i = 0; i < NumOfGrades; i++) {
        printf("%s: %d ", courseStrings[i], student->student.grades[i]->grade.grade);
    }
    printf("\n\n");
}

void updateStudent(struct School *school, struct Course *courses) {
    char firstName[NameLength], lastName[NameLength];
    getStudentName(firstName, lastName);
    int level = 0, class = 0, course = 0, grade = 0;
    getStudentLevelAndClass(&level, &class);
    struct StudentNode *student = searchStudentInLevelClass(school->students[level][class], firstName, lastName);
    if (student == NULL) {
        printf("Student not found.\n");
        return;
    }
    printf("Enter course number to update from:\n");
    printf("Math: 0, English: 1, Science: 2, History: 3, Physics: 4, Chemistry: 5, Biology: 6, Computer Science: 7, Literature: 8, Art: 9\n");
    scanf("%d", &course);
    printf("Enter grade to update: ");
    scanf("%d", &grade);
    school->students[level][class]->student.grades[course]->grade.grade = grade;
}

void addNewStudentFromUser(struct School *school, struct Course *courses,
                           struct TenBestStudentsForCourse *tenBestStudentsForCourse) {
    char firstName[NameLength], lastName[NameLength], telephone[TelephoneLength];
    int level, class;
    getStudentName(firstName, lastName);
    getStudentTelephone(telephone);
    getStudentLevelAndClass(&level, &class);

    printf("Enter the student's grades:\n");
    int grades[NumOfGrades];
    for (int i = 0; i < NumOfGrades; i++) {
        scanf("%d", &grades[i]);
    }
    struct StudentNode *studentForGrade;
    studentForGrade = addStudent(school, firstName, lastName, telephone, level, class);
    addStudentGrades(courses, school, studentForGrade, level, class, grades);
    addStudentToTenBestStudentsForCourse(tenBestStudentsForCourse, studentForGrade, level);
//    addStudentToStudentsPendingToBeExpelled(studentsPendingToBeExpelled, studentForGrade, level - 1);
}

void freeWorstStudents(struct StudentsPendingToBeExpelled *studentsPendingToBeExpelled) {
    for (int level = 0; level < Levels; level++) {
        struct StudentsPendingToBeExpelledNode *node = studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level];
        while (node != NULL) {
            struct StudentsPendingToBeExpelledNode *temp = node;
            node = node->next;
            free(temp);
        }
    }
}

void printTenBestStudentsForCourse(struct TenBestStudentsForCourse *tenBestStudentsForCourse) {
    for (int level = 0; level < Levels; level++) {
        for (int class = 0; class < Classes; class++) {
            printf("Level: %d, Class: %s\n", level + 1, courseStrings[class]);
            for (int grade = 0; grade < NumOfGrades; grade++) {
                if (tenBestStudentsForCourse->students[level][class][grade] == NULL) {
                    continue;
                }
                printf("%s %s %d\n", tenBestStudentsForCourse->students[level][class][grade]->student.first_name,
                       tenBestStudentsForCourse->students[level][class][grade]->student.last_name,
                       tenBestStudentsForCourse->students[level][class][grade]->student.grades[grade]->grade.grade);
            }
            printf("\n");
        }
    }
}

void printWorstStudent(struct StudentsPendingToBeExpelled *studentsPendingToBeExpelled) {
    for (int level = 0; level < Levels; level++) {
        if (studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level] == NULL) {
            continue;
        }
        printf("Level: %d\n", level + 1);
        while (studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level] != NULL) {
            printf("%s %s\n",
                   studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level]->student->student.first_name,
                   studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level]->student->student.last_name);
            studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level] = studentsPendingToBeExpelled->studentsPendingToBeExpelledNode[level]->next;
        }
        printf("\n");
    }
}