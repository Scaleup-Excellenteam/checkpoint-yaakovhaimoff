#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Levels 12
#define Classes 10
#define TelephoneLength 10
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
    struct GradeNode *grades[Levels][Classes];
};

int main() {
    struct School school;

    FILE *filePointer;
    filePointer = fopen(FILE_NAME, "r");

    if (filePointer == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    char firstName[100], lastName[100], telephone[11];
    int level, class, grades[NumOfGrades];

    while (fscanf(filePointer, "%s %s %s %d %d", firstName, lastName, telephone, &level, &class) == 5) {
        // Combine first name and last name to form the full name
        char *fullName = malloc(strlen(firstName) + strlen(lastName) + 2);
        strcpy(fullName, firstName);
        strcat(fullName, " ");
        strcat(fullName, lastName);

        // Read the 10 grades and store them in the student's grades array
        for (int i = 0; i < NumOfGrades; i++) {
            fscanf(filePointer, "%d", &grades[i]);
        }

        printf("Name: %s\n", fullName);
        printf("Telephone: %s\n", telephone);
        printf("Level: %d\n", level);
        printf("Grades: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", grades[i]);
        }
        printf("\n\n");

        free(fullName); // Don't forget to free dynamically allocated memory


//        // Create a new student node and populate the student information
//        struct StudentNode *newNode = malloc(sizeof(struct StudentNode));
//        newNode->student.first_name = fullName;
//        newNode->student.last_name = lastName;
//        strcpy(newNode->student.telephone, telephone);
//
//        // Read the 10 grades and store them in the student's grades array
//        for (int i = 0; i < 10; i++) {
//            fscanf(filePointer, "%d", &grades[i]);
//
//            // Create a new grade node for each grade and link it to the student's grades array
//            struct GradeNode *newGradeNode = malloc(sizeof(struct GradeNode));
//            newGradeNode->grade.grade = grades[i];
//            newGradeNode->grade.student = newNode;
//            newGradeNode->next = NULL;
//
//            // Link the grade node to the student's grades array
//            newNode->student.grades[i] = newGradeNode;
//        }
//
//        newNode->next = school.students[level][0]; // For this example, we assume class is always 0
//        school.students[level][0] = newNode; // For this example, we assume class is always 0
    }

    fclose(filePointer);

//    // Print the data to verify it's correctly stored
//    for (int level = 0; level < Levels; level++) {
//        for (int class = 0; class < Classes; class++) {
//            struct StudentNode *currentStudent = school.students[level][class];
//            while (currentStudent != NULL) {
//                printf("Name: %s\n", currentStudent->student.first_name);
//                printf("Telephone: %s\n", currentStudent->student.telephone);
//                printf("Grades: ");
//                for (int i = 0; i < 10; i++) {
//                    printf("%d ", currentStudent->student.grades[i]->grade.grade);
//                }
//                printf("\n\n");
//                currentStudent = currentStudent->next;
//            }
//        }
//    }

    // Don't forget to free the allocated memory when you're done using it.
    // In a real program, make sure to deallocate the memory properly.

    return 0;
}
