#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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



int updateUserInfoFile(struct userInfoBlock *_currUserList, unsigned int index, unsigned int column){
    
}

int main() {
    
    struct userInfoBlock userInfoList[MAX_NO_OF_USERS];
    loadUserInfoData(userInfoList, MAX_NO_OF_USERS);

    struct mobileNoInfoBlock mobileNoList[MAX_NO_OF_USERS];
    loadMobileNoInfoData(mobileNoList, MAX_NO_OF_USERS);

    struct packageInfoBlock packageList[NO_OF_PACKAGES];
    loadPackageInfoData(packageList, NO_OF_PACKAGES);

    for(int i=0; i < NO_OF_PACKAGES; i++){
        printf("%d\n", packageList[i].value);
    }

    return 0;
}
