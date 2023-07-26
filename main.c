#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Levels 12
#define Classes 10
#define TelephoneLength 11
#define NumOfGrades 10

#define FILE_NAME "/Users/yaakovhaimoff/Desktop/school/year_3/semester2/excellents/bootcamp/school_db/students.txt"

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

    while (fscanf(filePointer, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d", firstName, lastName, telephone, &level, &class,
    &grades[0], &grades[1], &grades[2], &grades[3], &grades[4], &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]) == 15) {

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

        for(int i = 0; i < NumOfGrades; i++) {
            if(courses->grades[level][class][i] == NULL) {
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
            if(node == NULL) {
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



int main() {
    struct School school;
    struct Course courses;

    // Read the data from the file
    readData(&school, &courses);

    // Print all students
    printStudents(school);

    // Free the memory for all students
    for (int i = 0; i < Levels; i++) {
        for (int j = 0; j < Classes; j++) {
            struct StudentNode *node = school.students[i][j];
            freeStudentList(node);
            school.students[i][j] = NULL; // Reset the pointer to NULL
        }
    }

    for(int i = 0; i < Levels; i++) {
        for(int j = 0; j < Classes; j++) {
            for(int k = 0; k < NumOfGrades; k++) {
                struct GradeNode *node = courses.grades[i][j][k];
                while(node != NULL) {
                    struct GradeNode *temp = node;
                    node = node->next;
                    free(temp);
                }
            }
        }
    }

    return 0;
}
