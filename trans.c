#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ======================================
// STRUCTURES
// ======================================

struct clientData
{
    unsigned int acctNum;
    char firstName[30];
    char lastName[30];
    char gender[10];
    char phone[15];
    char aadhar[20];
    char email[50];
    char address[100];
    char password[20];
    double balance;
};

struct transactionData
{
    unsigned int acctNum;
    char type[30];
    double amount;
    double currentBalance;
    char dateTime[30];
};

// ======================================
// FUNCTION PROTOTYPES
// ======================================

unsigned int menu();

void createAccount(FILE *fPtr);
void deposit(FILE *fPtr);
void withdraw(FILE *fPtr);
void transferMoney(FILE *fPtr);
void checkBalance(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void searchAccount(FILE *fPtr);
void showAllAccounts(FILE *fPtr);
void miniStatement();
void transactionHistory();
void applyInterest(FILE *fPtr);
void updateDetails(FILE *fPtr);
void loginSystem(FILE *fPtr);
void accountStatistics(FILE *fPtr);
void exportAccounts(FILE *fPtr);

void saveTransaction(unsigned int acc,
                     char type[],
                     double amount,
                     double balance);

void getDateTime(char *buffer);

// ======================================
// MAIN FUNCTION
// ======================================

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("bank.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("bank.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created\n");
            exit(1);
        }

        struct clientData blank = {0};

        for (int i = 0; i < 1000; i++)
        {
            fwrite(&blank,
                   sizeof(struct clientData),
                   1,
                   cfPtr);
        }
    }

    while ((choice = menu()) != 16)
    {
        switch (choice)
        {
        case 1:
            createAccount(cfPtr);
            break;

        case 2:
            deposit(cfPtr);
            break;

        case 3:
            withdraw(cfPtr);
            break;

        case 4:
            transferMoney(cfPtr);
            break;

        case 5:
            checkBalance(cfPtr);
            break;

        case 6:
            deleteAccount(cfPtr);
            break;

        case 7:
            searchAccount(cfPtr);
            break;

        case 8:
            showAllAccounts(cfPtr);
            break;

        case 9:
            miniStatement();
            break;

        case 10:
            transactionHistory();
            break;

        case 11:
            applyInterest(cfPtr);
            break;

        case 12:
            updateDetails(cfPtr);
            break;

        case 13:
            loginSystem(cfPtr);
            break;

        case 14:
            accountStatistics(cfPtr);
            break;

        case 15:
            exportAccounts(cfPtr);
            break;

        default:
            printf("Invalid Choice\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

// ======================================
// DATE TIME
// ======================================

void getDateTime(char *buffer)
{
    time_t t = time(NULL);

    struct tm *tm_info = localtime(&t);

    sprintf(buffer,
            "%02d/%02d/%04d %02d:%02d:%02d",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec);
}

// ======================================
// SAVE TRANSACTION
// ======================================

void saveTransaction(unsigned int acc,
                     char type[],
                     double amount,
                     double balance)
{
    FILE *tPtr;

    tPtr = fopen("transactions.dat", "ab");

    if (tPtr == NULL)
    {
        return;
    }

    struct transactionData t;

    t.acctNum = acc;

    strcpy(t.type, type);

    t.amount = amount;

    t.currentBalance = balance;

    getDateTime(t.dateTime);

    fwrite(&t,
           sizeof(struct transactionData),
           1,
           tPtr);

    fclose(tPtr);
}

// ======================================
// CREATE ACCOUNT
// ======================================

void createAccount(FILE *fPtr)
{
    struct clientData client = {0};

    unsigned int account;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists\n");
        return;
    }

    client.acctNum = account;

    printf("Enter First Name: ");
    scanf("%s", client.firstName);

    printf("Enter Last Name: ");
    scanf("%s", client.lastName);

    printf("Enter Gender: ");
    scanf("%s", client.gender);

    printf("Enter Phone Number: ");
    scanf("%s", client.phone);

    printf("Enter Aadhaar Number: ");
    scanf("%s", client.aadhar);

    printf("Enter Email: ");
    scanf("%s", client.email);

    printf("Enter Address: ");
    getchar();
    fgets(client.address,
          sizeof(client.address),
          stdin);

    printf("Create Password: ");
    scanf("%s", client.password);

    printf("Enter Initial Balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    saveTransaction(account,
                    "ACCOUNT_CREATED",
                    client.balance,
                    client.balance);

    printf("\nAccount Created Successfully\n");
}

// ======================================
// DEPOSIT
// ======================================

void deposit(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    saveTransaction(account,
                    "DEPOSIT",
                    amount,
                    client.balance);

    printf("Deposit Successful\n");

    printf("Current Balance: %.2lf\n",
           client.balance);
}

// ======================================
// WITHDRAW
// ======================================

void withdraw(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount\n");
        return;
    }

    if (amount > client.balance)
    {
        printf("Insufficient Balance\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    saveTransaction(account,
                    "WITHDRAW",
                    amount,
                    client.balance);

    printf("Withdraw Successful\n");

    printf("Current Balance: %.2lf\n",
           client.balance);
}

// ======================================
// CHECK BALANCE
// ======================================

void checkBalance(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");

    printf("Account Number : %u\n",
           client.acctNum);

    printf("Name : %s %s\n",
           client.firstName,
           client.lastName);

    printf("Phone : %s\n",
           client.phone);

    printf("Email : %s\n",
           client.email);

    printf("Balance : %.2lf\n",
           client.balance);
}

// ======================================
// DELETE ACCOUNT
// ======================================

void deleteAccount(FILE *fPtr)
{
    struct clientData blank = {0};

    unsigned int account;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blank,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Account Deleted Successfully\n");
}

// ======================================
// SEARCH ACCOUNT
// ======================================

void searchAccount(FILE *fPtr)
{
    struct clientData client;

    char name[30];

    int found = 0;

    printf("Enter First Name: ");
    scanf("%s", name);

    rewind(fPtr);

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (strcmp(client.firstName, name) == 0
            && client.acctNum != 0)
        {
            printf("%u %s %s %.2lf\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);

            found = 1;
        }
    }

    if (!found)
    {
        printf("No account found\n");
    }
}

// ======================================
// SHOW ALL ACCOUNTS
// ======================================

void showAllAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n===== ALL ACCOUNTS =====\n");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%u %s %s %.2lf\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);
        }
    }
}

// ======================================
// MINI STATEMENT
// ======================================

void miniStatement()
{
    FILE *tPtr;

    struct transactionData t;

    unsigned int account;

    tPtr = fopen("transactions.dat", "rb");

    if (tPtr == NULL)
    {
        printf("No transactions found\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%u", &account);

    printf("\n===== MINI STATEMENT =====\n");

    while (fread(&t,
                  sizeof(struct transactionData),
                  1,
                  tPtr))
    {
        if (t.acctNum == account)
        {
            printf("%s | %.2lf | %.2lf | %s\n",
                   t.type,
                   t.amount,
                   t.currentBalance,
                   t.dateTime);
        }
    }

    fclose(tPtr);
}

// ======================================
// TRANSACTION HISTORY
// ======================================

void transactionHistory()
{
    FILE *tPtr;

    struct transactionData t;

    tPtr = fopen("transactions.dat", "rb");

    if (tPtr == NULL)
    {
        printf("No transactions found\n");
        return;
    }

    printf("\n===== TRANSACTION HISTORY =====\n");

    while (fread(&t,
                  sizeof(struct transactionData),
                  1,
                  tPtr))
    {
        printf("Acc:%u | %s | %.2lf | %.2lf | %s\n",
               t.acctNum,
               t.type,
               t.amount,
               t.currentBalance,
               t.dateTime);
    }

    fclose(tPtr);
}

// ======================================
// APPLY INTEREST
// ======================================

void applyInterest(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            client.balance += client.balance * 0.05;

            fseek(fPtr,
                  -(long)sizeof(struct clientData),
                  SEEK_CUR);

            fwrite(&client,
                   sizeof(struct clientData),
                   1,
                   fPtr);
        }
    }

    printf("Interest Applied Successfully\n");
}

// ======================================
// MENU
// ======================================

unsigned int menu()
{
    unsigned int choice;

    printf("\n=========== ADVANCED BANK SYSTEM ===========\n");

    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Transfer Money\n");
    printf("5. Check Balance\n");
    printf("6. Delete Account\n");
    printf("7. Search Account\n");
    printf("8. Show All Accounts\n");
    printf("9. Mini Statement\n");
    printf("10. Transaction History\n");
    printf("11. Apply Interest\n");
    printf("12. Update Details\n");
    printf("13. Login System\n");
    printf("14. Account Statistics\n");
    printf("15. Export Report\n");
    printf("16. Exit\n");

    printf("Enter Choice: ");

    scanf("%u", &choice);

    return choice;
}