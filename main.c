#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

#define LIMIT 5000

struct Details{
    char username[100];
    char baseCountries[5];
    int password;
    float balance;
    int accountno;
};

int checkValidity(float amount,char transactionCountry[],char baseCountry[],int transactionsPerHour){
    if(!(strcmp(transactionCountry,baseCountry))){
        if(amount<LIMIT){
            if(transactionsPerHour<3){
                return 1;
            }else{
                printf("\nTransactions per hour exceed 3.\n");
                return 0;
            }
        }else{
            printf("\nAmount is more than the daily limit.\n");
            return 0;
        }
    }else{
        printf("\nTransaction country is not the same as Base country.\n");
        return 0;
    }
}

void withdraw(int acc){
    
    FILE* fptr = fopen("details.csv","r");
    FILE* temp = fopen("temp.csv","w");

    float amount;
    char transactionCountry[10];
    int transactionsPerHour=1;
    printf("\nEnter the amount you want to withdraw: ");
    scanf("%f",&amount);
    getchar();
    printf("\nEnter the transaction Country code: ");
    fgets(transactionCountry,sizeof(transactionCountry),stdin);
    transactionCountry[strcspn(transactionCountry,"\n")]='\0';
    char line[200];

    struct Details search;
    
    int transactionApproved = 0; // Flag for successful deposit
    int accountFound = 0;

    float newBalance;

    while(fgets(line,sizeof(line),fptr)){
        int result = sscanf(line,"%d,%[^,],%d,%f,%s",
            &search.accountno,
            search.username,
            &search.password,
            &search.balance,
            search.baseCountries
        );

        if (result != 5) {
            fprintf(temp, "%s", line); 
            continue; // Skip processing and go to the next line
        }

        if(acc == search.accountno){
            accountFound = 1;
            int check = checkValidity(amount,transactionCountry,search.baseCountries,transactionsPerHour);
            if(check==1){
                if(amount<=search.balance){
                    search.balance -= amount;
                    transactionApproved = 1; // Set flag
                    // newBalance = search.balance + amount;
                    fprintf(temp,"%d,%s,%d,%f,%s\n",
                        search.accountno,
                        search.username,
                        search.password,
                        search.balance,
                        // newBalance,
                        search.baseCountries
                    );
                }else{
                    printf("\nAmount to be withdrawn is greater than Balance\n");
                    fprintf(temp,"%s",line);
                }
            }else{
                printf("\nTransaction Blocked.\n");
                fprintf(temp,"%s",line);
            }
        }else{
            fprintf(temp,"%s",line);
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("details.csv");
    rename("temp.csv","details.csv");

    // Sleep(1000);


    // if(remove("details.csv")!=0){
    //     printf("Failed to delete details.csv.\n");
    // }

    // if(rename("temp.csv","details.csv")!=0){
    //     printf("Failed to rename temp.csv to details.csv.\n");
    // }

    // // remove("details.csv");
    // // rename("temp.csv","details.csv");

    // // if (transactionApproved) {
    // //     printf("\nDeposit Successful! New Balance: %.2f\n",search.balance);
    // //     // printf("\nDeposit Successful! New Balance: %.2f\n",newBalance);

    // // } else if (accountFound) {
    // //     // If account found but not approved, the block message was printed inside the loop.
    // // } else {
    // //     printf("\nError: Account with ID %d not found in file.\n", acc);
    // // }
}

void deposit(int acc){

    FILE* fptr = fopen("details.csv","r");
    FILE* temp = fopen("temp.csv","w");

    float amount;
    char transactionCountry[10];
    int transactionsPerHour=1;
    printf("\nEnter the amount you want to deposit: ");
    scanf("%f",&amount);
    getchar();
    printf("\nEnter the transaction Country code: ");
    fgets(transactionCountry,sizeof(transactionCountry),stdin);
    transactionCountry[strcspn(transactionCountry,"\n")]='\0';
    char line[200];

    struct Details search;
    
    int transactionApproved = 0; // Flag for successful deposit
    int accountFound = 0;

    float newBalance;

    while(fgets(line,sizeof(line),fptr)){
        int result = sscanf(line,"%d,%[^,],%d,%f,%s",
            &search.accountno,
            search.username,
            &search.password,
            &search.balance,
            search.baseCountries
        );

        if (result != 5) {
            // Write the possibly malformed line back to preserve the file contents
            fprintf(temp, "%s", line); 
            continue; // Skip processing and go to the next line
        }

        if(acc == search.accountno){
            accountFound = 1;
            int check = checkValidity(amount,transactionCountry,search.baseCountries,transactionsPerHour);
            if(check==1){
                search.balance += amount;
                // newBalance = search.balance + amount;
                transactionApproved = 1; // Set flag
                fprintf(temp,"%d,%s,%d,%f,%s\n",
                    search.accountno,
                    search.username,
                    search.password,
                    search.balance,
                    // newBalance,
                    search.baseCountries
                );
            }else{
                printf("\nTransaction Blocked.\n");
                fprintf(temp,"%s",line);
            }
        }else{
            fprintf(temp,"%s",line);
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("details.csv");
    rename("temp.csv","details.csv");
    // Sleep(1000);


    // if(remove("details.csv")!=0){
    //     printf("Failed to delete details.csv.\n");
    // }

    // if(rename("temp.csv","details.csv")!=0){
    //     printf("Failed to rename temp.csv to details.csv.\n");
    // }

    // // remove("details.csv");
    // // rename("temp.csv","details.csv");

    // // if (transactionApproved) {
    // //     printf("\nDeposit Successful! New Balance: %.2f\n",search.balance);
    // //     // printf("\nDeposit Successful! New Balance: %.2f\n",newBalance);

    // // } else if (accountFound) {
    // //     // If account found but not approved, the block message was printed inside the loop.
    // // } else {
    // //     printf("\nError: Account with ID %d not found in file.\n", acc);
    // // }
}

void userlogin(){
    struct Details acc;
    char username[100];
    char line[200];
    int password,choice=0,userFound=0;
    float amount;

    printf("\nEnter your Username: ");
    fgets(username,sizeof(username),stdin);
    username[strcspn(username,"\n")]='\0';
    printf("\nEnter your password: ");
    scanf(" %d",&password);

    FILE* fptr = fopen("details.csv","r");

    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%d,%.2f,%s",
            &acc.accountno,
            acc.username,
            &acc.password,
            &acc.balance,
            acc.baseCountries
        );

        if(!(strcmp(username,acc.username))){
            if(password==acc.password){
                printf("\nLogged in As User: %s\n",acc.username);
                userFound = 1;
                break;
            }
        }
    }

    if(!userFound){
        printf("\nUsername not found\n");
        return;
    }

    do{
        printf("\n1.Deposit.\n2.Withdraw.\n3.Apply for loan.\n0.Exit.\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                fclose(fptr);
                deposit(acc.accountno);
                break;
            case 2:
                fclose(fptr);
                withdraw(acc.accountno);
                break;
            case 0:
                printf("Logging out as %s.",acc.username);
                break;
            default:
                printf("Please enter a valid number: ");
                break;
        }
    }while(choice!=0);

    fclose(fptr);
    return;
}

void createAccount(){

    struct Details temp;
    temp.accountno = (rand()%99999)+9999;

    printf("\nAccount number: %d\n",temp.accountno);
    printf("\nEnter Account username: ");
    getchar();
    fgets(temp.username,100,stdin);
    temp.username[strcspn(temp.username,"\n")]='\0';
    printf("\nEnter the PIN:");
    scanf("%d",&temp.password);
    printf("\nEnter balance: ");
    scanf("%f",&temp.balance);
    printf("\nEnter Country of Origin:");
    scanf(" %s",temp.baseCountries);

    FILE* fptr = fopen("details.csv","a");

    fprintf(fptr,"%d,%s,%d,%f,%s\n",
        temp.accountno,
        temp.username,
        temp.password,
        temp.balance,
        temp.baseCountries
    );

    fclose(fptr);
}

void deleteAccount(int accountno){

    FILE* temp = fopen("temp.csv","w");
    FILE* fptr = fopen("details.csv","r");

    char line[200];

    struct Details acc;


    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%d,%f,%s",
            &acc.accountno,
            acc.username,
            &acc.password,
            &acc.balance,
            acc.baseCountries
        );
        if(accountno!=acc.accountno){
            fprintf(temp,"%s",line);
        }else{
            printf("\nDeleted Account Number %d and all related records.\n",accountno);
        }
    }

    fclose(temp);
    fclose(fptr);

    remove("details.csv");
    rename("temp.csv","details.csv");
}

void listAll_Accounts(){
    FILE* fptr = fopen("details.csv","r");

    struct Details account;

    char line[200];

    printf("Acc. No.\tHoldername\tPassword\tBalance\tCountry\n");
    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%d,%f,%s",
            &account.accountno,
            account.username,
            &account.password,
            &account.balance,
            account.baseCountries
        );
        // printf("Acc. No: %d\nAcc. Holder: %s\nBalance: %.2f\nCountry: %s\n",account.accountno,account.username,account.balance,account.password,account.baseCountries);
        printf("%d\t\t%s\t%d\t\t%.3f\t\t%s\n",account.accountno,account.username,account.password,account.balance,account.baseCountries);
    }
    fclose(fptr);
}

void listAccountDetails(int accountno){
    FILE* fptr = fopen("details.csv","r");

    char line[200];

    struct Details acc;

    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%d,%f,%s",
            &acc.accountno,
            acc.username,
            &acc.password,
            &acc.balance,
            acc.baseCountries
        );

        if(accountno == acc.accountno){
            printf("\n====ACCOUNT INFO====\nAccount Number: %d\nHolder Name: %s\nPIN :%d\nBalance: %.2f\nBase Country: %s\n",
            acc.accountno,
            acc.username,
            acc.password,
            acc.balance,
            acc.baseCountries);

            fclose(fptr);
            return;
        }
    }

    printf("Failed to retrive Account no: %d",accountno);
    fclose(fptr);
}

void adminlogin(){
    char username[50];
    int password;
    printf("\t=====Logging in as ADMIN=====\t\nEnter ADMIN Username: ");
    // getchar();
    scanf("%s",username);
    // getchar();
    printf("Enter ADMIN password: ");
    scanf("%d",&password);

    if(!(strcmp(username,"ADMIN"))){
        if(password==3060){
            printf("\nLogged in As Admin\n");
        }
        else{
            printf("Wrong Admin Password,\nLogin (As ADMIN) Failed");
            return ;
        }
    }else{
        printf("Wrong Admin Username.,\nLogin (As ADMIN) Failed");
        return ;
    }
    
    int choice = 0,accountno;

    do{
        printf("\n1.Create Account.\n2.Delete Account.\n3.List All Accounts\n4.List Account Details.\n0.Exit\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                createAccount();
                break;
            case 2:
                printf("Enter Account No: ");
                scanf("%d",&accountno);
                deleteAccount(accountno);
                break;
            case 3:
                listAll_Accounts();
                break;
            case 4:
                printf("Enter Account No: ");
                scanf("%d",&accountno);
                listAccountDetails(accountno);
                break;
            case 0:
                printf("Logged out as ADMIN\n");
                break;
            default:
                printf("Please enter a valid value: ");
                break;
        }
    }while(choice!=0);
}

char login(){
    int choice=0;

    do{
        printf("\n1.Admin login.\n2.User login\n0.Exit\nMake your choice: ");
        scanf("%d",&choice);
        getchar();
        switch(choice){
            case 0:
                printf("Exiting Login Interface.");
                break;
            case 1:
                adminlogin();
                break;
            case 2:
                userlogin();
                break;
            default:
                printf("Please enter a value number");
                break;
        }

    }while(choice!=0);
}

int main(){
    // srand(time(NULL));
    login();
    return 0;
}
/*
    char name[30];

    printf("Enter your name: ");
    fgets(name,sizeof(name),stdin);

    name[strcspn(name,"\n")]='\0';

    printf(" Hello %s\n..",name);
*/