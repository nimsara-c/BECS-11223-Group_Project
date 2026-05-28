#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NO_OF_USERS 50
#define NO_OF_PACKAGES 15

enum errorTypes{
    SUCCESS,
    ERROR
};

enum connectionTypes{
    PREPAID,
    POSTPAID
};

struct userInfoBlock{
    char mobNum[11];
    char nicNum[15];
    char custName[50];
    enum connectionTypes conType;
};

struct mobileNoInfoBlock{
    char mobNum[11];
    float amount;
    unsigned int remData;   // in MB
    unsigned int remVoice;  // in mins
    unsigned int billDate;
    int state;
};

struct packageInfoBlock{
    unsigned int index;
    int value;
    unsigned int data;   // in MB
    unsigned int voice;  // in mins
};

// Global Variables
unsigned int g_usrCount = 0;

int loadUserInfoData(struct userInfoBlock *_userList, unsigned int _maxSize){

    FILE *fptr;
    char buffer[200];
    unsigned int count = 0;

    fptr = fopen("user_info.txt", "r");

    if (fptr == NULL) {
        printf("Error: Could not open user_info file.\n");
        return ERROR;
    }

    while (fgets(buffer, 200, fptr) && count <= _maxSize) {

        sscanf(buffer, "%s %s %s %d", _userList[count].mobNum, _userList[count].nicNum, _userList[count].custName, &_userList[count].conType);
        //printf("%s %s %s %d\n", _userList[count].mobNum, _userList[count].nicNum, _userList[count].custName, _userList[count].conType);
        count++;

    }

    g_usrCount = count;

    fclose(fptr);

    return SUCCESS;
}

int loadMobileNoInfoData(struct mobileNoInfoBlock *_mobileNoInfoList, unsigned int _maxSize){
    
    FILE *fptr;
    char buffer[200];
    unsigned int count = 0;

    fptr = fopen("mobile_num_info.txt", "r");

    if (fptr == NULL) {
        printf("Error: Could not open mobile_num_info file.\n");
        return ERROR;
    }

    while (fgets(buffer, 200, fptr) && count <= _maxSize) {

        sscanf(buffer, "%s %f %u %u %u %d", _mobileNoInfoList[count].mobNum, &_mobileNoInfoList[count].amount, &_mobileNoInfoList[count].remData, &_mobileNoInfoList[count].remVoice, &_mobileNoInfoList[count].billDate, &_mobileNoInfoList[count].state);
        //printf("%s %f %u %u %u %d\n", _mobileNoInfoList[count].mobNum, _mobileNoInfoList[count].amount, _mobileNoInfoList[count].remData, _mobileNoInfoList[count].remVoice, _mobileNoInfoList[count].billDate, _mobileNoInfoList[count].state);
        count++;

    }

    fclose(fptr);

    return SUCCESS;
}

int loadPackageInfoData(struct packageInfoBlock *_pkgList, unsigned int _maxSize){

    FILE *fptr;
    char buffer[200];
    unsigned int count = 0;

    fptr = fopen("package_info.txt", "r");

    if (fptr == NULL) {
        printf("Error: Could not open package_info file.\n");
        return ERROR;
    }

    while (fgets(buffer, 200, fptr) && count <= _maxSize) {

        sscanf(buffer, "%u %d %u %u", &_pkgList[count].index, &_pkgList[count].value, &_pkgList[count].data, &_pkgList[count].voice);
        //printf("%u %d %u %u", _pkgList[count].index, _pkgList[count].value, _pkgList[count].data, _pkgList[count].voice);
        count++;

    }

    fclose(fptr);

    return SUCCESS;
}

int printUserInfoBlock(struct userInfoBlock _userBlock){
    
    printf("%s %s %s %d\n", _userBlock.mobNum, _userBlock.nicNum, _userBlock.custName, _userBlock.conType);
    
}

int updateUserInfoFile(struct userInfoBlock *_updatedUserList){

    FILE *fptr;

    fptr = fopen("user_info.txt", "w");

    if (fptr == NULL) {
        printf("Error: Could not open user_info file.\n");
        return ERROR;
    }

    for (unsigned int j = 0; j < g_usrCount; j++) {
        //_updatedUserList[j].custName[sizeof(_updatedUserList[j].custName) - 1] = '\0';
        fprintf(fptr, "%s %s %s %d\n", _updatedUserList[j].mobNum, _updatedUserList[j].nicNum, _updatedUserList[j].custName, (int)_updatedUserList[j].conType);
        //printf("%s %s %s %d\n", _updatedUserList[j].mobNum, _updatedUserList[j].nicNum, _updatedUserList[j].custName, _updatedUserList[j].conType);
    }

    fclose(fptr);

    return SUCCESS;
}

int updateMobileNoInfoFile(struct mobileNoInfoBlock *_updatedMobileNoList){

    FILE *fptr;

    fptr = fopen("user_info.txt", "w");

    if (fptr == NULL) {
        printf("Error: Could not open mobile_num_info file.\n");
        return ERROR;
    }

    for (unsigned int i = 0; i < g_usrCount; i++) {
        fprintf(fptr, "%s %f %u %u %u %d\n", _updatedMobileNoList[i].mobNum, _updatedMobileNoList[i].amount, _updatedMobileNoList[i].remData, _updatedMobileNoList[i].remVoice, _updatedMobileNoList[i].billDate, _updatedMobileNoList[i].state);
    }

    fclose(fptr);

    return SUCCESS;
}

unsigned int getTodaysDate(){

    time_t raw_time = time(NULL);
    struct tm *info = localtime(&raw_time);

    int day = info->tm_mday;

    return (unsigned int)day;
}

int getYN(){
    char c;
    printf("Y(Yes) / No(N)? : ");
    scanf("%c", &c);
    if(c == 'Y' || c == 'y' || c == '1'){
        return 1;
    }
    else{
        return 0;
    }
}

unsigned int isMobileNoExists(char *mobNum, struct userInfoBlock *_userList){  // #index (starting from 1) - Yes    0 - No
    for(unsigned int i=0; i < g_usrCount; i++){
        if(strcmp(_userList[i].mobNum, mobNum) == 0){
            return (i + 1); // So that we can use 0 for the state of "Not exist"
        }
    }
    return 0;
}

int runActivateOrDeactivateNums(struct mobileNoInfoBlock *_mobileNoList){

    unsigned int dueUsersIndices[g_usrCount];

    int count = 0;

    printf("\n\t|---------- DUE USERS ----------|\n\n");
    printf("\tNO\tMOBILE_NUMBER\tDATE\tAMOUNT\n");
    printf("-----\t-------------\t----\t------\n");
    for(int index=0; index < g_usrCount; index++){

        if(_mobileNoList[index].billDate > getTodaysDate() && _mobileNoList[index].amount < 0){
            dueUsersIndices[count] = index;
            count++;
            printf("%d\t%s\t%u\t%.2f\n", count, _mobileNoList[index].mobNum, _mobileNoList[index].billDate, _mobileNoList[index].amount);
        }
    }
    printf("\n");

    while(1){
        int choice;
        printf("\nActivate/Deactivate a Number?\n  0-No\n  1-Activate\n  2-Deactivate\n:");
        scanf("%d", &choice);

        if(choice == 0) {  break;  }

        if(choice == 1){
            int inputIndex;
            printf("Enter the index to Activate: ");
            scanf("%d", &inputIndex);
            _mobileNoList[dueUsersIndices[inputIndex-1]].state = 1;
        }
        else{
            int inputIndex;
            printf("Enter the index to Deactivate: ");
            scanf("%d", &inputIndex);
            _mobileNoList[dueUsersIndices[inputIndex-1]].state = 0;
        }
    }

    updateMobileNoInfoFile(_mobileNoList);
    return 0;

}

int createNewUser(struct userInfoBlock *_userInfoList, struct mobileNoInfoBlock *_mobileNoList, const char *_inputMobNum){

    struct userInfoBlock tempUsrData;
    printf("----------  Enter New User Info  ----------\n");
    printf("NIC No: ");
    scanf("%s", tempUsrData.nicNum);
    printf("Customer Name: ");
    scanf("%s", tempUsrData.custName);
    strcpy(tempUsrData.mobNum, _inputMobNum);

    // Connection type error handling loop
    while(1){
        printf("Connection Type(0 - PREPAID / 1 - POSTPAID): ");

        int tempConnType;
        scanf("%d", &tempConnType);

        if(tempConnType == 0 || tempConnType == 1){
            tempUsrData.conType = tempConnType;
            break;
        }
        else{
            printf("Invalid INPUT!");
        }
    }

    if(g_usrCount < MAX_NO_OF_USERS){

        _userInfoList[g_usrCount] = tempUsrData;
        printUserInfoBlock(_userInfoList[g_usrCount]);

        struct mobileNoInfoBlock tempMobInfo;
        strcpy(tempMobInfo.mobNum, tempUsrData.mobNum);
        tempMobInfo.amount = 0.0;
        tempMobInfo.remData = 0;
        tempMobInfo.remVoice = 0;
        tempMobInfo.state = 1;
        if(tempUsrData.conType == POSTPAID){
            tempMobInfo.billDate = getTodaysDate();
        }
        else{
            tempMobInfo.billDate = 0;
        }

        _mobileNoList[g_usrCount] = tempMobInfo;

        printf("usrCount: %u", g_usrCount);
        g_usrCount++;
        printf("usrCount: %u", g_usrCount);

        updateUserInfoFile(_userInfoList);
        updateMobileNoInfoFile(_mobileNoList);
    }
    else{
        printf("Cannot Input New User! No. of users are at maximum!");
    }
}

int run(){

    struct userInfoBlock userInfoList[MAX_NO_OF_USERS];
    loadUserInfoData(userInfoList, MAX_NO_OF_USERS);

    struct mobileNoInfoBlock mobileNoList[MAX_NO_OF_USERS];
    loadMobileNoInfoData(mobileNoList, MAX_NO_OF_USERS);

    struct packageInfoBlock packageList[NO_OF_PACKAGES];
    loadPackageInfoData(packageList, NO_OF_PACKAGES);

    while(1){

        printf("Display payment due users?");
        if(getYN()){
            runActivateOrDeactivateNums(mobileNoList);
        }

        while(1){
            printf("Enter Mobile Number: ");
            char inputMobNum[11];
            scanf("%s", inputMobNum);

            unsigned int index =  isMobileNoExists(inputMobNum, userInfoList);

            // Mobile No does not exist. So create a new user.
            if(index == 0){
                printf("\n* Mobile No doesn't exist! Creating a new user *!\n\n");
                createNewUser(userInfoList, mobileNoList, inputMobNum);
                //updateUserInfoFile(userInfoList);
                continue;
            }
            else{
                printf("* User exists with index = %u\n *", index);
            }
        }
        

    }
}

int main() {
    
    

    //strcpy(userInfoList[41].nicNum, "2004562378");   // 195564215778
    //updateUserInfoFile(userInfoList);

    //runActivateOrDeactivateNums(mobileNoList);
    run();

    printf("User Count: %u\nDate: %u\n", g_usrCount, getTodaysDate());

    return 0;
}
