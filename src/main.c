#include "School.h"
#include "Course.h"
#include "EncryptFile.h"

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

    // Open the encrypted file for reading
    FILE *decryptFilePointer = fopen(ENCRYPT_FILE_NAME, "r");
    if (decryptFilePointer == NULL) {
        perror("Error opening encrypted file");
        exit(1);
    }

    unsigned char decrypted_line[KEY_SIZE],
            ciphertext[BUFFER_SIZE];
    char data[InputLength][BUFFER_SIZE] = {0}, *f;
    int grades[NumOfGrades], ciphertext_len, decryptedtext_len;

    // Read data from the encrypted file in binary mode and decrypt it
    while (fread(&ciphertext_len, sizeof(int), 1, decryptFilePointer) == 1) {
        // Read the encrypted data based on the size read
        if (fread(ciphertext, 1, ciphertext_len, decryptFilePointer) != ciphertext_len) {
            perror("Error reading encrypted data");
            fclose(decryptFilePointer);
            exit(1);
        }

        // Decrypt the line
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decrypted_line);

        // Print the decrypted line
        char decrypted_line_copy[BUFFER_SIZE];

        sprintf(decrypted_line_copy, "%.*s ", decryptedtext_len, decrypted_line);
        trimWhiteSpace(decrypted_line_copy);

        f = strtok((char *)decrypted_line_copy, " ");

        for (int i = 0; f != NULL; i++) {
            strcpy(data[i], f);
            f = strtok(NULL, " ");
//            printf("data: %s, ", data[i]);
        }
        printf("\n");

        int level = atoi(data[LEVEL]);
        int class = atoi(data[CLASS]);

        for (int j = 0; j < NumOfGrades; ++j) {
            grades[j] = atoi(data[GRADES + j]);
        }

        struct StudentNode *studentForGrade;
//        printf("here1\n");
        studentForGrade = addStudent(school, data[FIRST_NAME], data[LAST_NAME], data[TELEPHONE],
                                     level - 1, class - 1);
//        printf("here2\n");
        addStudentGrades(courses, school, studentForGrade, level - 1, class - 1, grades);
//        printf("here3\n");
        addStudentToTenBestStudentsForCourse(tenBestStudentsForCourse, studentForGrade, level - 1);
//        printf("here4\n");
        addStudentToStudentsPendingToBeExpelled(studentsPendingToBeExpelled, studentForGrade, level - 1);
//        printf("here5\n");
    }
    int i =0;
    // Close the decrypted file
    fclose(decryptFilePointer);
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
    init_openssl();
    encryptFileAndSaveToFile();
    readData(&school, &courses, &tenBestStudentsForCourse, &studentsPendingToBeExpelled);

//    int choice;
//    while (1) {
//        printMenu();
//        scanf("%d", &choice);
//
//        switch (choice) {
//            case OPTION_PRINT_STUDENTS:
////                printGrades(&courses);
//                printStudents(school);
//                break;
//            case OPTION_ADD_STUDENT:
//                addNewStudentFromUser(&school, &courses, &tenBestStudentsForCourse);
//                break;
//            case OPTION_DELETE_STUDENT:
//                deleteStudentFromUser(&school, &courses);
//                break;
//            case OPTION_UPDATE_STUDENT:
//                updateStudent(&school, &courses);
//                break;
//            case OPTION_SEARCH_STUDENT:
//                search(&school);
//                break;
//            case OPTION_PRINT_TEN_BEST_STUDENTS_FOR_COURSE:
//                printTenBestStudentsForCourse(&tenBestStudentsForCourse);
//                break;
//            case OPTION_AVG_FOR_COURSE_AND_LEVEL:
//                avgForCourseAndLevel(&courses);
//                break;
//            case OPTION_PRINT_WORST_STUDENT:
//                printWorstStudent(&studentsPendingToBeExpelled);
//                break;
//            case OPTION_EXIT:
//                freeStudents(&school);
//                freeCourses(&courses);
//                freeWorstStudents(&studentsPendingToBeExpelled);
//                return 0;
//            default:
//                printf("Invalid choice. Please try again.\n");
//        }
//    }
}
