#include "macros.h"

//#include "School.h"
//#include "Course.h"

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

struct StudentNode *mallocStudentNode() {
    struct StudentNode *studentNode = malloc(sizeof(struct StudentNode));
    if (studentNode == NULL) {
        printf("Error: malloc failed in mallocStudentNode\n");
        exit(1);
    }
    studentNode->next = NULL;
    return studentNode;
}

struct GradeNode *mallocGradeNode() {
    struct GradeNode *gradeNode = malloc(sizeof(struct GradeNode));
    if (gradeNode == NULL) {
        printf("Error: malloc failed in mallocGradeNode\n");
        exit(1);
    }
    gradeNode->next = NULL;
    return gradeNode;
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
    level -= 1;
    printf("Enter the student's class: ");
    scanf("%d", &*class);
    class -= 1;
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

void assignGradeData(struct Grade *grade, int gradeValue, struct StudentNode *student) {
    grade->grade = gradeValue;
    grade->student = student;
}

struct StudentNode *
addStudent(struct School *school,
           char *firstName, char *lastName, char *telephone,
           int level, int class, int *grades) {
    struct StudentNode *studentForGrade;
    if (school->students[level][class] == NULL) {
        // If there are no students for the given level and class, add the new student as the first student.
        school->students[level][class] = mallocStudentNode();
        assignStudentData(&school->students[level][class]->student, firstName, lastName, telephone);
        school->students[level][class]->next = NULL; // Set next to NULL as this is the only student for now.
        studentForGrade = school->students[level][class]; // Set the studentForGrade to the first student.
    } else {
        // If there are existing students, create a new student node and link it to the current first student.
        struct StudentNode *newStudent = mallocStudentNode();
        assignStudentData(&newStudent->student, firstName, lastName, telephone);
        newStudent->next = school->students[level][class];
        school->students[level][class] = newStudent; // Update the first student to be the new student.
        studentForGrade = newStudent; // Set the studentForGrade to the new student.
    }
    return studentForGrade;
}

void addStudentGrades(struct Course *courses,
                      struct School *school,
                      struct StudentNode *studentForGrade,
                      int level, int class, const int *grades) {
    for (int i = 0; i < NumOfGrades; i++) {
        if (courses->grades[level][class][i] == NULL) {
            courses->grades[level][class][i] = mallocGradeNode();
            assignGradeData(&courses->grades[level][class][i]->grade, grades[i], studentForGrade);
            courses->grades[level][class][i]->next = NULL;
            school->students[level][class]->student.grades[i] = courses->grades[level][class][i];
        } else {
            struct GradeNode *newGrade = mallocGradeNode();
            assignGradeData(&newGrade->grade, grades[i], studentForGrade);
            newGrade->next = courses->grades[level][class][i];
            courses->grades[level][class][i] = newGrade;
            school->students[level][class]->student.grades[i] = newGrade;
        }
    }
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

void
readData(struct School *school,
         struct Course *courses,
         struct TenBestStudentsForCourse *tenBestStudentsForCourse) {
    memset(school->students, 0, sizeof(school->students));
    memset(courses->grades, 0, sizeof(courses->grades));
    memset(tenBestStudentsForCourse->students, 0, sizeof(tenBestStudentsForCourse->students));

    FILE *filePointer;
    filePointer = fopen(FILE_NAME, "r");

    if (filePointer == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char firstName[NameLength], lastName[NameLength], telephone[TelephoneLength];
    int level, class, grades[NumOfGrades];

    while (fscanf(filePointer, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d", firstName, lastName, telephone, &level,
                  &class,
                  &grades[MATH], &grades[ENGLISH], &grades[SCIENCE], &grades[HISTORY], &grades[PHYSICS],
                  &grades[CHEMISTRY], &grades[BIOLOGY], &grades[COMPUTER_SCIENCE],
                  &grades[LITERATURE], &grades[ART]) == InputLength) {

        struct StudentNode *studentForGrade;
        studentForGrade = addStudent(school, firstName, lastName, telephone, level - 1, class - 1, grades);
        addStudentGrades(courses, school, studentForGrade, level - 1, class - 1, grades);
        addStudentToTenBestStudentsForCourse(tenBestStudentsForCourse, studentForGrade, level - 1);
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

void freeStudents(struct School *school) {
    for (int i = 0; i < Levels; i++) {
        for (int j = 0; j < Classes; j++) {
            struct StudentNode *node = school->students[i][j];
            freeStudentList(node);
            school->students[i][j] = NULL;
        }
    }
}

void freeCourses(struct Course *courses) {
    for (int i = 0; i < Levels; i++) {
        for (int j = 0; j < Classes; j++) {
            for (int k = 0; k < NumOfGrades; k++) {
                struct GradeNode *node = courses->grades[i][j][k];
                while (node != NULL) {
                    struct GradeNode *temp = node;
                    node = node->next;
                    free(temp);
                }
            }
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

struct StudentNode *
searchStudentByName(struct School *school, char *firstName, char *lastName, int *levelToSave, int *classToSave) {
    for (int level = 0; level < Levels; level++) {
        for (int class = 0; class < Classes; class++) {
            struct StudentNode *node = school->students[level][class];
            while (node != NULL) {
                if (strcmp(node->student.first_name, firstName) == 0 &&
                    strcmp(node->student.last_name, lastName) == 0) {
                    *levelToSave = level;
                    *classToSave = class;
                    return node;
                }
                node = node->next;
            }
        }
    }
    return NULL; // Student not found
}

void deleteStudentFromGrades(struct Course *courses, struct StudentNode *current, int level, int class) {
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
                break;
            }

            prevGrade = gradeNode;
            gradeNode = gradeNode->next;
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
    printf("Level: %d\n", level);
    printf("Class: %d\n", class);
    printf("Telephone: %s\n", student->student.telephone);
    printf("Grades: ");
    for (int i = 0; i < NumOfGrades; i++) {
        printf("%s: %d ", courseStrings[i], student->student.grades[i]->grade.grade);
    }
    printf("\n\n");
}

void avgForCourseAndLevel(struct Course *course) {
    for (int level = 0; level < Levels; level++) {
        printf("Level: %d, ", level+1);
        for (int class = 0; class < Classes; class++) {
            int sum = 0;
            int count = 0;
            for (int grade = 0; grade < NumOfGrades; grade++) {
                struct GradeNode *gradeNode = course->grades[level][class][grade];
                while (gradeNode != NULL) {
                    sum += gradeNode->grade.grade;
                    count++;
                    gradeNode = gradeNode->next;
                }
            }
            if(class%2==0)
            {
                printf("\n");
            } else
            {
                printf("\t\t");
            }
            if (count == 0) {
                printf("Class: %s, Average: 0 ", courseStrings[class]);
            } else {
                printf("Class: %s, Average: %f ", courseStrings[class], (float) sum / count);
            }
        }
        printf("\n");
    }
}

void addNewStudentFromUser(struct School *school, struct Course *courses) {
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
    studentForGrade = addStudent(school, firstName, lastName, telephone, level, class, grades);
    addStudentGrades(courses, school, studentForGrade, level, class, grades);
}

void printMenu() {
    printf("\nMenu:\n");
    printf("0: Print all students\n");
    printf("1: Add student\n");
    printf("2: Delete student\n");
    printf("3: Update student\n");
    printf("4: Search student\n");
    printf("5: Print ten best students for course\n");
    printf("6: Print average for every course and level\n");
    printf("7: Exit\n");
    printf("Enter your choice: ");
}


int main() {
    struct School school;
    struct Course courses;
    struct TenBestStudentsForCourse tenBestStudentsForCourse;
    readData(&school, &courses, &tenBestStudentsForCourse);

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
                search(&school);
                break;
            case OPTION_PRINT_TEN_BEST_STUDENTS_FOR_COURSE:
                printTenBestStudentsForCourse(&tenBestStudentsForCourse);
                break;
            case OPTION_AVG_FOR_COURSE_AND_LEVEL:
                avgForCourseAndLevel(&courses);
                break;
            case OPTION_EXIT:

                freeStudents(&school);

                freeCourses(&courses);

                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
