#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

int checkValidy(char Tinitial[],char Tpresent[],char basecountry[]){
    
}

void deposit(float amount,char username[],int password){
    FILE* fptr = fopen("accounts.csv","r");
    FILE* temp = fopen("temp.csv","w");

    char line[200];
    char usernametoCmp[100];
    char baseCountries[10];
    int passwordtoCmp;
    float balance;
    int accountNum;

    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%f,%d,%s",
            &accountNum,
            usernametoCmp,
            &balance,
            &passwordtoCmp
            // baseCountries
        );
        if(!(strcmp(usernametoCmp,username))){
            if(password==passwordtoCmp){
                // int check = checkValidity();
                balance+=amount;
                fprintf(temp,"%d,%[^,],%f,%d",
                    accountNum,
                    usernametoCmp,
                    balance,
                    passwordtoCmp
                    // baseCountries
                );
            }else{
                fprintf(temp,"%s",line);
            }
        }else{
            fprintf(temp,"%s",line);
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("accounts.csv");
    rename("temp.csv","accounts.csv");
}

void userlogin(){
    char username[100];
    char line[200];
    int password,choice=0;
    float amount;
    char usernametoCmp[100];
    int passwordtoCmp;
    float balance;
    int accountNum;
    printf("Enter your Username: ");
    scanf(" %s",username);
    printf("Enter your password: ");
    scanf("%d",&password);

    FILE* fptr = fopen("accounts.csv","r");


    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%f,%d",
            &accountNum,
            usernametoCmp,
            &balance,
            &passwordtoCmp
        );

        if(!(strcmp(username,usernametoCmp))){
            if(password==passwordtoCmp){
                printf("\nLogged in As User: %s\n",usernametoCmp);
                break;
            }
            else{
                printf("Wrong User Password");
                return ;
            }
        }
    }

    // if(!(strcmp(username,"ADMIN"))){
    //     if(password==3060){
    //         printf("\nLogged in As Admin\n");
    //     }
    //     else{
    //         printf("Wrong Admin Password");
    //         return ;
    //     }
    // }else{
    //     printf("Wrong Admin Username.");
    //     return ;
    // }

    do{
        printf("\n1.Deposit.\n2.Withdraw.\n3.Apply for loan.\n0.Exit.\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                printf("Enter the amount you want to deposit: ");
                scanf("%f",&amount);
                deposit(amount,username,password);
                break;
            case 2:
                // withdraw();
                break;
            case 0:
                printf("Logging out as USER.");
                break;
            default:
                printf("Please enter a valid number: ");
                break;
        }
    }while(choice!=0);

    // FILE* fptr = fopen("accounts.csv","r");

}

void createAccount(){
    char username[100];
    char countries[100];
    int password;
    float balance;
    int accountNo = (rand() % 99999) + 9999;

    printf("Account number: %d",accountNo);
    printf("Enter Account username: ");
    getchar();
    fgets(username,100,stdin);
    username[strcspn(username,"\n")]='\0';
    printf("Enter the PIN:");
    scanf("%d",&password);
    printf("Enter balance: ");
    scanf("%f",&balance);
    printf("Enter Countries of Origin.");

    FILE* fptr = fopen("accounts.csv","a");

    fprintf(fptr,"%d,%s,%f,%d,%s\n",accountNo,username,balance,password,countries);

    fclose(fptr);
}
void deleteAccount(int accountno){

    FILE* temp = fopen("temp.csv","w");
    FILE* fptr = fopen("accounts.csv","r");

    char line[200];
    char username[100];
    int password;
    float balance;
    int accountNum;

    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%f,%d",
            &accountNum,
            username,
            &balance,
            &password
        );
        if(accountno!=accountNum){
            fprintf(temp,"%s",line);
        }else{
            printf("\nDeleted Account Number %d and all related records.\n",accountno);
        }
    }

    fclose(temp);
    fclose(fptr);

    remove("accounts.csv");
    rename("temp.csv","accounts.csv");
}
void listAll_Accounts(){
    FILE* fptr = fopen("accounts.csv","r");

    char line[200];
    char username[100];
    int password;
    float balance;
    int accountNum;

    printf("Account Number\tHoldername\tBalance\n");
    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%f,%d",
            &accountNum,
            username,
            &balance,
            &password
        );
        printf("%d\t\t%s\t%.2f\n",accountNum,username,balance);
    }
    fclose(fptr);
}
void listAccountDetails(int accountno){
    FILE* fptr = fopen("accounts.csv","r");

    char line[200];
    char username[100];
    int password;
    float balance;
    int accountNum;

    while(fgets(line,sizeof(line),fptr)){
        sscanf(line,"%d,%[^,],%f,%d",
            &accountNum,
            username,
            &balance,
            &password
        );
        if(accountno == accountNum){
            printf("Account Number: %d\nHolder Name: %s\nBalance :%.2f\nPIN: %d\n",accountNum,username,balance,password);
            return;
        }
    }

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
        printf("\n1.Admin login.\n2.User login\n0.Exit\n Make your choice: ");
        scanf("%d",&choice);
        // getchar();
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
/*
    char name[30];

    printf("Enter your name: ");
    fgets(name,sizeof(name),stdin);

    name[strcspn(name,"\n")]='\0';

    printf(" Hello %s\n..",name);
*/