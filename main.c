#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

#define LIMIT 5000

struct Details{           //details structure for customer information
    int accountno;
    char username[100];
    int password;
    float balance;
    char baseCountries[5];
};

struct TransactionLog{
    int accountno;
    float amount;
    char type[10];
    long long timestamp;
};

void recordTransaction(int accountno,float amount,const char* type){
    FILE* lptr = fopen("transaction.log","a");

    if(lptr==NULL){
        printf("\n Transaction file is Missing\n");
        return;
    }

    long currTime = (long)time(NULL);

    fprintf(lptr,"%d,%f,%s, %ld.\n",
        accountno,
        amount,
        type,
        currTime
    );

    fclose(lptr);
}

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

void genReportHrly(){
    FILE* lptr = fopen("transaction.log","r");

    if(lptr==NULL){
        printf("\n Transaction file is Missing\n");
        return;
    }

    long lastHour = time(NULL) - 3600;

    char line[200];
    struct TransactionLog lentry;
    int count=0;

    printf("\t TRANSACTIONS LAST HOUR \t\n");
    printf("Time\t\t\tAccount ID\tType\t\tAmount\n");

    while(fgets(line,sizeof(line),lptr)){

        int result = sscanf(line,"%d,%f,%[^,],%lld",
            &lentry.accountno,
            &lentry.amount,
            lentry.type,
            &lentry.timestamp
        );

        if(result!=4) continue;

        if(lentry.timestamp>=lastHour){
            struct tm *localTime = localtime(&lentry.timestamp);
            char timeStr[20];
            strftime(timeStr,sizeof(timeStr),"%H : %M : %S",localTime);

            printf("%s\t\t%d\t\t%s\t%.2f\n",
                timeStr,
                lentry.accountno,
                lentry.type,
                lentry.amount
            );
            count++;
        }
    }

    fclose(lptr);
    printf("\n========================\n");
    printf("Transactions in the Last Hour: %d\n",count);

    return;
}

void withdraw(int acc){
    
    FILE* fptr = fopen("details.csv","r");
    FILE* temp = fopen("temp.csv","w");

    if(fptr==NULL){
        printf("\n Transaction file is Missing\n");
        return;
    }
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

    //float newBalance;

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

                    recordTransaction(search.accountno,amount,"WITHDRAW");

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
}

void deposit(int acc){

    FILE* fptr = fopen("details.csv","r");
    FILE* temp = fopen("temp.csv","w");

    if(fptr==NULL){
        printf("\n Transaction file is Missing\n");
        return;
    }
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

    //float newBalance;

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

                recordTransaction(search.accountno,amount,"DEPOSIT");

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
}

void userlogin(){
    struct Details acc;
    char username[100];
    char line[200];
    int password,choice=0,userFound=0;
    float amount;

    printf("\nEnter your Username: ");
    // getchar();
    fgets(username,sizeof(username),stdin);
    username[strcspn(username,"\n")]='\0';
    
    printf("\nEnter your password: ");
    scanf(" %d",&password);

    FILE* fptr = fopen("details.csv","r");

    if(fptr==NULL){
        printf("\n Transaction file is Missing\n");
        return;
    }

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
    
    fclose(fptr);
    
    do{
        printf("\n1.Deposit.\n2.Withdraw.\n3.Apply for loan.\n0.Exit.\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                deposit(acc.accountno);
                break;
            case 2:
                withdraw(acc.accountno);
                break;
            case 3:{
            	float loan;
            	printf("\nEnter loan amount: ");
            	scanf("%f",&loan);
            	printf("\nYour loan request has been recieved successfully!");
				break;
			}
            case 0:
                printf("\nLogging out as %s\n",acc.username);
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
    fgets(temp.username, sizeof(temp.username), stdin);
    temp.username[strcspn(temp.username,"\n")] = '\0';
    printf("\nEnter the PIN:");
    scanf("%d",&temp.password);
    printf("\nEnter balance: ");
    scanf("%f",&temp.balance);
    getchar();
    printf("\nEnter Country of Origin:");
    fgets(temp.baseCountries, sizeof(temp.baseCountries), stdin);
    temp.baseCountries[strcspn(temp.baseCountries, "\n")] = '\0';

    FILE* fptr = fopen("details.csv","a");  //we used append because we want previous data to exist

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

    // printf("Acc. No. || Holdername || Password || Balance || Country\n");
    printf("%-12s %-20s %-12s %-12s %-15s\n",
        "Acc. No.", "Holdername", "Password", "Balance", "Country");
    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%d,%f,%s",
            &account.accountno,
            account.username,
            &account.password,
            &account.balance,
            account.baseCountries
        );
        printf("%-12d %-20s %-12d %-12.3f %-15s\n",
            account.accountno,
            account.username,
            account.password,
            account.balance,
            account.baseCountries
        );
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
    printf("\n\t=====Logging in as ADMIN=====\t\nEnter ADMIN Username: ");
    scanf("%s",username);
    //getchar();
    printf("Enter ADMIN password: ");
    scanf("%d",&password);

    if(!(strcmp(username,"ADMIN"))){
        if(password==3060){
            printf("\nLogged in As Admin!\n");
        }
        else{
            printf("Wrong Admin Password,\nLogin (As ADMIN) Failed");
            return;
        }
    }else{
        printf("Wrong Admin Username.,\nLogin (As ADMIN) Failed");
        return ;
    }
    
    int choice=0, accountno;

    do{
        printf("\n1.Create Account.\n2.Delete Account.\n3.List All Accounts\n4.List Account Details.\n5.Generate Hourly Report.\n0.Exit\nEnter your choice: ");
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
            case 5:
                genReportHrly();
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

void login(){
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

    srand(time(NULL));
    
    login();

    return 0;
}
