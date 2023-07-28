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

struct StudentsPendingToBeExpelledNode {
    struct StudentNode *student;
    struct StudentsPendingToBeExpelledNode *next;
};

struct StudentsPendingToBeExpelled {
    struct StudentsPendingToBeExpelledNode *studentsPendingToBeExpelledNode[Levels];
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

void assignGradeData(struct Grade *grade, int gradeValue, struct StudentNode *student) {
    grade->grade = gradeValue;
    grade->student = student;
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

void addStudentGrades(struct Course *courses,
                      struct School *school,
                      struct StudentNode *studentForGrade,
                      int level, int class, const int *grades) {
    for (int course = 0; course < NumOfGrades; course++) {
        if (courses->grades[level][class][course] == NULL) {
            courses->grades[level][class][course] = mallocGradeNode();
            assignGradeData(&courses->grades[level][class][course]->grade, grades[course], studentForGrade);
            courses->grades[level][class][course]->next = NULL;
            school->students[level][class]->student.grades[course] = courses->grades[level][class][course];
        } else {
            struct GradeNode *newGrade = mallocGradeNode();
            assignGradeData(&newGrade->grade, grades[course], studentForGrade);
            newGrade->next = courses->grades[level][class][course];
            courses->grades[level][class][course] = newGrade;
            school->students[level][class]->student.grades[course] = newGrade;
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

void
readData(struct School *school,
         struct Course *courses,
         struct TenBestStudentsForCourse *tenBestStudentsForCourse,
         struct StudentsPendingToBeExpelled *studentsPendingToBeExpelled) {
    memset(school->students, 0, sizeof(school->students));
    memset(courses->grades, 0, sizeof(courses->grades));
    memset(tenBestStudentsForCourse->students, 0, sizeof(tenBestStudentsForCourse->students));
    memset(studentsPendingToBeExpelled->studentsPendingToBeExpelledNode, 0,
           sizeof(studentsPendingToBeExpelled->studentsPendingToBeExpelledNode));

    FILE *filePointer;
    filePointer = fopen(FILE_NAME, "r");

    if (filePointer == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char firstName[NameLength], lastName[NameLength], telephone[TelephoneLength];
    int level, class, grades[NumOfGrades];
    while (fscanf(filePointer, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
                  firstName, lastName, telephone, &level, &class,
                  &grades[MATH], &grades[ENGLISH], &grades[SCIENCE], &grades[HISTORY], &grades[PHYSICS],
                  &grades[CHEMISTRY], &grades[BIOLOGY], &grades[COMPUTER_SCIENCE],
                  &grades[LITERATURE], &grades[ART]) == InputLength) {

        struct StudentNode *studentForGrade;
        studentForGrade = addStudent(school, firstName, lastName, telephone, level - 1, class - 1);
        addStudentGrades(courses, school, studentForGrade, level - 1, class - 1, grades);
        addStudentToTenBestStudentsForCourse(tenBestStudentsForCourse, studentForGrade, level - 1);
        addStudentToStudentsPendingToBeExpelled(studentsPendingToBeExpelled, studentForGrade, level - 1);
    }
    fclose(filePointer);
}

// Function to free memory for the linked list of students
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

void printGrades(struct Course *courses) {
    for (int i = 0; i < Levels; i++) {
        for (int j = 0; j < Classes; j++) {
            printf("Level %d, class %d\n", i + 1, j + 1);
            for (int k = 0; k < NumOfGrades; k++) {
                struct GradeNode *node = courses->grades[i][j][k];
                while (node != NULL) {
                    printf("%d ", node->grade.grade);
                    node = node->next;
                }
                printf("\n");
            }
            printf("\n");
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
    printf("Enter course number to update from:\n");
    printf("Math: 0, English: 1, Science: 2, History: 3, Physics: 4, Chemistry: 5, Biology: 6, Computer Science: 7, Literature: 8, Art: 9\n");
    scanf("%d", &course);
    printf("Enter grade to update: ");
    scanf("%d", &grade);
    printf("course %d", courses->grades[level - 1][class - 1][course]->grade.grade);
    school->students[level - 1][class - 1]->student.grades[course]->grade.grade = grade;
//    Kirby
//    Lomago
//    10
//    4
//    1
//    80
}

void avgForCourseAndLevel(struct Course *course) {
    for (int level = 0; level < Levels; level++) {
        printf("Level: %d, ", level + 1);
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
            if (class % 2 == 0) {
                printf("\n");
            } else {
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
    printf("7: Print students pending to be expelled\n");
    printf("8: Exit\n");
    printf("Enter your choice: ");
}


int main() {
    struct School school;
    struct Course courses;
    struct TenBestStudentsForCourse tenBestStudentsForCourse;
    struct StudentsPendingToBeExpelled studentsPendingToBeExpelled;
    readData(&school, &courses, &tenBestStudentsForCourse, &studentsPendingToBeExpelled);

    int choice;
    while (1) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case OPTION_PRINT_STUDENTS:
//                printGrades(&courses);
                printStudents(school);
                break;
            case OPTION_ADD_STUDENT:
                addNewStudentFromUser(&school, &courses, &tenBestStudentsForCourse);
                break;
            case OPTION_DELETE_STUDENT:
                deleteStudentFromUser(&school, &courses);
                break;
            case OPTION_UPDATE_STUDENT:
                updateStudent(&school, &courses);
                break;
            case OPTION_SEARCH_STUDENT:
                search(&school);
                break;
            case OPTION_PRINT_TEN_BEST_STUDENTS_FOR_COURSE:
                printTenBestStudentsForCourse(&tenBestStudentsForCourse);
                break;
            case OPTION_AVG_FOR_COURSE_AND_LEVEL:
                avgForCourseAndLevel(&courses);
                break;
            case OPTION_PRINT_WORST_STUDENT:
                printWorstStudent(&studentsPendingToBeExpelled);
                break;
            case OPTION_EXIT:
                freeStudents(&school);
                freeCourses(&courses);
                freeWorstStudents(&studentsPendingToBeExpelled);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
