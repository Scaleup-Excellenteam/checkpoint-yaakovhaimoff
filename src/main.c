#include "School.h"
#include "Course.h"

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
