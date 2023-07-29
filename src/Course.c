#include "Course.h"
#include "School.h"

const char* courseStrings[] = {
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

struct GradeNode *mallocGradeNode() {
    struct GradeNode *gradeNode = malloc(sizeof(struct GradeNode));
    if (gradeNode == NULL) {
        printf("Error: malloc failed in mallocGradeNode\n");
        exit(1);
    }
    gradeNode->next = NULL;
    return gradeNode;
}

void assignGradeData(struct Grade *grade, int gradeValue, struct StudentNode *student) {
    grade->grade = gradeValue;
    grade->student = student;
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
