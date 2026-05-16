#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Account {
    int accNo;
    char name[50];
    float balance;
};
void insertRecord() {
    FILE *fp = fopen("bank.dat", "ab");
    struct Account a;
    if (fp == NULL) {
        printf("File open failed!\n");
        return;
    }
    printf("Enter Account No: ");
    scanf("%d", &a.accNo);
    printf("Enter Name: ");
    scanf(" %[^\n]", a.name);  // allows spaces
    printf("Enter Balance: ");
    scanf("%f", &a.balance);
    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);
    printf("Record inserted successfully!\n");
}
void displayRecords() {
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }
    printf("\n--- Bank Records ---\n");
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo != -1) {
            printf("AccNo: %d | Name: %s | Balance: %.2f\n",
                   a.accNo, a.name, a.balance);
        }
    }
    fclose(fp);
}
void updateRecord() {
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int searchAcc, found = 0, choice;
    float amount;
    if (fp == NULL) {
        printf("File open failed!\n");
        return;
    }
    printf("Enter Account No: ");
    scanf("%d", &searchAcc);
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == searchAcc) {
            found = 1;
            printf("Account Found!\n");
            printf("Current Balance: %.2f\n", a.balance);
            printf("1. Deposit\n2. Withdraw\nChoose option: ");
            scanf("%d", &choice);
            printf("Enter Amount: ");
            scanf("%f", &amount);
            if (choice == 1) {
                a.balance += amount;
            } 
            else if (choice == 2) {
                if (amount > a.balance) {
                    printf("Insufficient balance!\n");
                    fclose(fp);
                    return;
                }
                a.balance -= amount;
            } 
            else {
                printf("Invalid choice!\n");
                fclose(fp);
                return;
            }
            fseek(fp, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);
            printf("Transaction successful! New Balance: %.2f\n", a.balance);
            break;
        }
    }
    if (!found) {
        printf("Account not found!\n");
    }
    fclose(fp);
}
void deleteRecord() {
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int searchAcc, found = 0;
    if (fp == NULL) {
        printf("File open failed!\n");
        return;
    }
    printf("Enter Account No to Delete: ");
    scanf("%d", &searchAcc);
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == searchAcc) {
            found = 1;
            a.accNo = -1;  // logical delete
            fseek(fp, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);
            printf("Record deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Account not found!\n");
    }
    fclose(fp);
}
void searchRecord() {
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;
    int searchAcc, found = 0;
    if (fp == NULL) {
        printf("File open failed!\n");
        return;
    }
    printf("Enter Account No to Search: ");
    scanf("%d", &searchAcc);
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == searchAcc) {
            found = 1;
            printf("\n--- Account Found ---\n");
            printf("AccNo: %d\nName: %s\nBalance: %.2f\n",
                   a.accNo, a.name, a.balance);
            break;
        }
    }
    if (!found) {
        printf("Account not found!\n");
    }
    fclose(fp);
}
int main() {
    int choice;

    while (1) {
        printf("\n===== BANK MENU =====\n");
        printf("1. Insert\n");
        printf("2. Display\n");
        printf("3. Deposit/Withdraw\n");
        printf("4. Delete\n");
        printf("5. Search\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: insertRecord(); break;
            case 2: displayRecords(); break;
            case 3: updateRecord(); break;
            case 4: deleteRecord(); break;
            case 5: searchRecord(); break;
            case 6:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}

