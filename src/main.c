#include "macros.h"

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

struct Grade {
    int grade;
    struct StudentNode *student;
};

struct GradeNode {
    struct Grade grade;
    struct GradeNode *next;
};

struct Course {
    char *name;
    struct GradeNode *grades[Levels][Classes][NumOfGrades];
};

struct StudentNode *addStudent(struct School *school, char *firstName, char *lastName, char *telephone, int level, int class, int *grades) {
    struct StudentNode *studentForGrade;
    if (school->students[level][class] == NULL) {
        // If there are no students for the given level and class, add the new student as the first student.
        school->students[level][class] = malloc(sizeof(struct StudentNode));
        school->students[level][class]->student.first_name = strdup(firstName);
        school->students[level][class]->student.last_name = strdup(lastName);
        strcpy(school->students[level][class]->student.telephone, telephone);
        school->students[level][class]->next = NULL; // Set next to NULL as this is the only student for now.
        studentForGrade = school->students[level][class]; // Set the studentForGrade to the first student.
    } else {
        // If there are existing students, create a new student node and link it to the current first student.
        struct StudentNode *newStudent = malloc(sizeof(struct StudentNode));
        newStudent->student.first_name = strdup(firstName);
        newStudent->student.last_name = strdup(lastName);
        strcpy(newStudent->student.telephone, telephone);
        newStudent->next = school->students[level][class];
        school->students[level][class] = newStudent; // Update the first student to be the new student.
        studentForGrade = newStudent; // Set the studentForGrade to the new student.
    }
    return studentForGrade;
}

void addStudentGrades(struct Course *courses, struct School *school, struct StudentNode *studentForGrade, int level,
                      int class, const int *grades) {
    for (int i = 0; i < NumOfGrades; i++) {
        if (courses->grades[level][class][i] == NULL) {
            courses->grades[level][class][i] = malloc(sizeof(struct GradeNode));
            courses->grades[level][class][i]->grade.grade = grades[i];
            courses->grades[level][class][i]->grade.student = studentForGrade;
            courses->grades[level][class][i]->next = NULL;
            school->students[level][class]->student.grades[i] = courses->grades[level][class][i];
        } else {
            struct GradeNode *newGrade = malloc(sizeof(struct GradeNode));
            newGrade->grade.grade = grades[i];
            newGrade->grade.student = studentForGrade;
            newGrade->next = courses->grades[level][class][i];
            courses->grades[level][class][i] = newGrade;
            school->students[level][class]->student.grades[i] = newGrade;
        }
    }
}

void readData(struct School *school, struct Course *courses) {
    memset(school->students, 0, sizeof(school->students));
    memset(courses->grades, 0, sizeof(courses->grades));

    FILE *filePointer;
    filePointer = fopen(FILE_NAME, "r");

    if (filePointer == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char firstName[100], lastName[100], telephone[11];
    int level, class, grades[NumOfGrades];

    while (fscanf(filePointer, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d", firstName, lastName, telephone, &level,
                  &class,
                  &grades[0], &grades[1], &grades[2], &grades[3], &grades[4], &grades[5], &grades[6], &grades[7],
                  &grades[8], &grades[9]) == 15) {

        struct StudentNode *studentForGrade;
        studentForGrade = addStudent(school, firstName, lastName, telephone, level, class, grades);
        addStudentGrades(courses, school, studentForGrade, level, class, grades);
    }
    fclose(filePointer);
}


// Function to free memory for the linked list of students
void freeStudentList(struct StudentNode *node) {
    while (node != NULL) {
        struct StudentNode *temp = node;
        node = node->next;

        // Free first_name and last_name strings
        free(temp->student.first_name);
        free(temp->student.last_name);

        // Free the node itself
        free(temp);
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
                printf("Level: %d\n", level);
                printf("Class: %d\n", class);
                for (int i = 0; i < NumOfGrades; i++) {
                    printf("%d ", node->student.grades[i]->grade.grade);
                }
                printf("\n\n");
                node = node->next;
            }
        }
    }
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
            for (int i = 0; i < NumOfGrades; i++) {
                struct GradeNode *gradeNode = courses->grades[level][class][i];
                struct GradeNode *prevGrade = NULL;

                while (gradeNode != NULL) {
                    if (gradeNode->grade.student == current) {
                        if (prevGrade == NULL) {
                            // If the grade node is the first one in the list
                            courses->grades[level][class][i] = gradeNode->next;
                        } else {
                            prevGrade->next = gradeNode->next;
                        }
                        free(gradeNode);
                        break; // No need to continue searching for the same student's grades
                    }

                    prevGrade = gradeNode;
                    gradeNode = gradeNode->next;
                }
            }

            return; // Exit the function after deleting the student
        }

        prev = current;
        current = current->next;
    }

    printf("Student not found.\n");
}

void deleteStudentFromUser(struct School *school, struct Course *courses) {
    printf("Enter the student's level: ");
    int level;
    scanf("%d", &level);
    printf("Enter the student's class: ");
    int class;
    scanf("%d", &class);
    printf("Enter the student's telephone: ");
    char telephone[TelephoneLength];
    scanf("%s", telephone);

    deleteStudent(school, courses, level, class, telephone);
}

void printMenu() {
    printf("\nMenu:\n");
    printf("0: Print all students\n");
    printf("1: Add student\n");
    printf("2: Delete student\n");
    printf("3: Update student\n");
    printf("4: Search student\n");
    printf("5: Exit\n");
    printf("Enter your choice: ");
}


void addNewStudentFromUser(struct School *school, struct Course *courses) {
    printf("Enter the student's first name: ");
    char firstName[100];
    scanf("%s", firstName);
    printf("Enter the student's last name: ");
    char lastName[100];
    scanf("%s", lastName);
    printf("Enter the student's telephone: ");
    char telephone[11];
    scanf("%s", telephone);
    printf("Enter the student's level: ");
    int level;
    scanf("%d", &level);
    printf("Enter the student's class: ");
    int class;
    scanf("%d", &class);
    printf("Enter the student's grades: ");
    int grades[NumOfGrades];
    for (int i = 0; i < NumOfGrades; i++) {
        scanf("%d", &grades[i]);
    }
    struct StudentNode *studentForGrade;
    studentForGrade = addStudent(school, firstName, lastName, telephone, level, class, grades);
    addStudentGrades(courses, school, studentForGrade, level, class, grades);
}


int main() {
    struct School school;
    struct Course courses;
    readData(&school, &courses);

    int choice;
    while (1) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case OPTION_PRINT_STUDENTS:
                printStudents(school);
                break;
            case OPTION_ADD_STUDENT:
                addNewStudentFromUser(&school, &courses);
                break;
            case OPTION_DELETE_STUDENT:
                deleteStudentFromUser(&school, &courses);
                break;
            case OPTION_UPDATE_STUDENT:
                // Implement update student function
                printf("Update student function not implemented yet.\n");
                break;
            case OPTION_SEARCH_STUDENT:
                // Implement search student function
                printf("Search student function not implemented yet.\n");
                break;
            case OPTION_EXIT:
                // Free memory and exit
                for (int i = 0; i < Levels; i++) {
                    for (int j = 0; j < Classes; j++) {
                        struct StudentNode *node = school.students[i][j];
                        freeStudentList(node);
                        school.students[i][j] = NULL;
                    }
                }

                for (int i = 0; i < Levels; i++) {
                    for (int j = 0; j < Classes; j++) {
                        for (int k = 0; k < NumOfGrades; k++) {
                            struct GradeNode *node = courses.grades[i][j][k];
                            while (node != NULL) {
                                struct GradeNode *temp = node;
                                node = node->next;
                                free(temp);
                            }
                        }
                    }
                }
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
