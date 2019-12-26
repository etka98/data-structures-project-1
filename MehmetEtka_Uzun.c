#include <stdio.h> 
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//STRUCTS
struct tag{
    char tagName[50];
    int countForZero;
    int countForOne;
    int countForTwo;
    int countForZeroTxt;
    int countForOneTxt;
    int countForTwoTxt;
    float idfForZero;
    float idfForOne;
    float idfForTwo;
    struct tag *nextTag;
    struct folder *headFolderList;
    struct tagList *headFirstOrder;
    struct tagList *headSecondOrder;
    struct tagList *headThirdOrder;
};
struct folder{
    char folderName[50];
    char typeOfFolder[50];
    struct folder *next;
};
struct tagList{
    struct tag *node;
    struct tagList *nxtPtr;
};
//Global variables 
DIR *direction;
struct dirent *dir;
char folderNames[3][50];
int file[3];
int numOfZero = 0;
int numOfOne = 0;
int numOfTwo = 0;
int numZero = 0;
int numOne = 0;
int numTwo = 0;
//METHODS
void determineFolderNames(char *string);
int determineFilesNumbers(char* folderName);
void read(char* folderName, int i, struct tag **head);
void addFolder(struct folder **head, struct folder *newFolder);
void addTag(struct tag **head, struct tag *newPtr, char* fileName, char* folderName);
int compareFolderName(struct folder *headFirst, struct folder *headSecond);
void addFirstOrder(struct tagList **head, struct tag *tagPtr);
void createFirsOrderList(struct tag **head);
void addTagSecondOrderAndThirdOrder(struct tagList **head, struct tag *tagPtr);
void compareFirstOrder(struct tagList **head, struct tagList *headFirst, struct tagList *headSecond, char *tagName);
void createSecondOrderList(struct tag **head);
int isThere(char *tagName, struct tagList *head);
void createThirdOrderList(struct tag **head);
void compareForThirdOrder(struct tagList **head, 
                            struct tagList *headFirst,
                            struct tagList *headFirstSecondOrder,
                            struct tagList *headSecond,
                            char *tagName); 
void fillArrays(struct tag **head, struct tag zero[], struct tag one[], struct tag two[]);
void selectionSortForZero(struct tag array[], int num);
void selectionSortForOne(struct tag array[], int num);
void selectionSortForTwo(struct tag array[], int num);
void calculateTxtNumber(struct tag **head);
void calculateIdfForZero(struct tag ** head, struct tag array[]);
void calculateIdfForOne(struct tag ** head, struct tag array[]);
void calculateIdfForTwo(struct tag ** head, struct tag array[]);
void selectionSortForIdfZero(struct tag array[], int num);
void selectionSortForIdfOne(struct tag array[], int num);
void selectionSortForIdfTwo(struct tag array[], int num);
//main method
int main(int argc, char const *argv[]){
    struct tag *headPtr = NULL;
    determineFolderNames("dataset");
    file[0] = determineFilesNumbers(folderNames[0]);
    file[1] = determineFilesNumbers(folderNames[1]);
    file[2] = determineFilesNumbers(folderNames[2]);
    for(int i = 1; i <= file[0]; i++){
         read(folderNames[0], i, &headPtr);
    }
    for(int i = 1; i <= file[1]; i++){
         read(folderNames[1], i, &headPtr);
    }
    for(int i = 1; i <= file[2]; i++){ 
         read(folderNames[2], i, &headPtr);
    }
    struct tag *tempPtr = headPtr;
    headPtr->headFirstOrder = NULL;
    headPtr->headSecondOrder = NULL;
    headPtr->headThirdOrder= NULL;
    struct tag zeroArray[numOfZero];
    struct tag oneArray[numOfOne];
    struct tag twoArray[numOfTwo];
    struct tag zeroArrayForIdf[numOfZero];
    struct tag oneArrayForIdf[numOfOne];
    struct tag twoArrayForIdf[numOfTwo];
    calculateTxtNumber(&headPtr);
    fillArrays(&headPtr, zeroArray, oneArray, twoArray);
    selectionSortForZero(zeroArray, numOfZero);
    selectionSortForOne(oneArray, numOfOne);
    selectionSortForTwo(twoArray, numOfTwo);
    calculateIdfForZero(&headPtr, zeroArrayForIdf);
    calculateIdfForOne(&headPtr, oneArrayForIdf);
    calculateIdfForTwo(&headPtr, twoArrayForIdf);
    selectionSortForIdfZero(zeroArrayForIdf, numOfZero);
    selectionSortForIdfOne(oneArrayForIdf, numOfOne);
    selectionSortForIdfTwo(twoArrayForIdf, numOfTwo);
    createFirsOrderList(&headPtr);
    createSecondOrderList(&headPtr);
    createThirdOrderList(&headPtr);
    printf("First Order \n-----------------------------------------------\n");
    for(struct tag *temp = headPtr; temp != NULL; temp = temp->nextTag){
        for(struct tagList *tempFirstOrder = temp->headFirstOrder;
                tempFirstOrder != NULL;
                    tempFirstOrder = tempFirstOrder->nxtPtr
                    ){
            printf("[%s, %s]\n", temp->tagName, tempFirstOrder->node->tagName);   
        }
    }
    printf("Second order \n-----------------------------------------------\n");
    for(struct tag *temp = headPtr; temp != NULL; temp = temp->nextTag){
        for(struct tagList *tempSecondOrder = temp->headSecondOrder;
                tempSecondOrder != NULL;
                    tempSecondOrder = tempSecondOrder->nxtPtr
                    ){
            printf("[%s, %s]\n", temp->tagName, tempSecondOrder->node->tagName);   
        }
    }
    printf("Third order \n-----------------------------------------------\n");
    for(struct tag *temp = headPtr; temp != NULL; temp = temp->nextTag){
        for(struct tagList *tempThirdOrder = temp->headThirdOrder;
                tempThirdOrder != NULL;
                    tempThirdOrder = tempThirdOrder->nxtPtr
                    ){
            printf("[%s, %s]\n", temp->tagName, tempThirdOrder->node->tagName);   
        }
    }
    printf("%s\n--------------------------\n", folderNames[0]);
    for(int i = 0; i < 5; i++){
        printf("%s : %d\n", zeroArray[i].tagName, zeroArray[i].countForZero);
    }
    printf("%s\n--------------------------\n", folderNames[1]);
    for(int i = 0; i < 5; i++){
        printf("%s : %d\n", oneArray[i].tagName, oneArray[i].countForOne);
    }
    printf("%s\n--------------------------\n", folderNames[2]);
    for(int i = 0; i < 5; i++){
        printf("%s : %d\n", twoArray[i].tagName, twoArray[i].countForTwo);
    }
    printf("%s\n-------------------------\n", folderNames[0]);
    for(int i = 0; i < 5; i++){
        printf("%s : %f\n", zeroArrayForIdf[i].tagName, zeroArrayForIdf[i].idfForZero);
    }
    printf("%s\n-------------------------\n", folderNames[1]);
    for(int i = 0; i < 5; i++){
        printf("%s : %f\n", oneArrayForIdf[i].tagName, oneArrayForIdf[i].idfForOne);
    }
    printf("%s\n-------------------------\n", folderNames[2]);
    for(int i = 0; i < 5; i++){
        printf("%s : %f\n", twoArrayForIdf[i].tagName, twoArrayForIdf[i].idfForTwo);
    }
}
//find the folder names
void determineFolderNames(char *string){
    direction = opendir(string);
    int count = 0;
    if(direction){
        while((dir = readdir(direction)) != NULL){
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".DS_Store") == 0){
			    continue;
		}
            strcpy(folderNames[count], dir->d_name);
            count++;
        }  
        closedir(direction);
    }   
}
//find and return txt's numbers
int determineFilesNumbers(char* folderName){
	char fileNames[50];
	strcpy(fileNames,"dataset//");
	strcat(fileNames, folderName);
    direction = opendir (fileNames);
	int numOfFiles = 0;
	if (direction != NULL) {
  	    while ((dir = readdir (direction)) != NULL){
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".DS_Store") == 0){
			    continue;
		}
    	    numOfFiles++;
  	    }
  	    closedir (direction);
	} 
    return numOfFiles;
}
//reads input files
void read(char* folderName, int i, struct tag **head){
    char result[50]; 
    sprintf(result, "%d", i); 
	char fileNames[100];
	strcpy(fileNames,"dataset//");
	strcat(fileNames, folderName);
	strcat(fileNames, "//"); 
	strcat(fileNames, result);
	strcat(fileNames, ".txt");	
	FILE *fp = fopen(fileNames, "r");
    char buf[50];
        while(fscanf(fp, "%s", buf) != EOF){
            struct tag *newTag = malloc(sizeof(struct tag));
            strcpy(newTag->tagName, buf);
            addTag(head, newTag, fileNames, folderName);
        }	
}
//add and set folderNames
void addFolder(struct folder **head, struct folder *newFolder){
    if(!(*head)){
        *head = newFolder;
    }
    else{
        struct folder *prev;
        for(struct folder *temp = *head; temp != NULL; temp = temp->next){
            if(strcmp(temp->folderName, newFolder->folderName) == 0){   
                return;
            }
            prev = temp; 
        }
        prev->next = newFolder;
    } 
}
//add and set tagNames
void addTag(struct tag **head,struct tag *newTag, char* fileName, char* folderName){
    struct folder *newFolder = malloc(sizeof(struct folder));
    strcpy(newFolder->folderName, fileName);
    strcpy(newFolder->typeOfFolder, folderName);
    if(!(*head)){
        *head = newTag;
        addFolder(&((*head)->headFolderList), newFolder);
        if(strcmp(folderNames[0], folderName) == 0){
            (*head)->countForZero++;
            numOfZero++;
        }
        else if(strcmp(folderNames[1], folderName) == 0){
            (*head)->countForOne++;
            numOfOne++;
        }
        else {
            (*head)->countForTwo++; 
            numOfTwo++; 
        } 
    }
    else
    { 
        struct tag *prev;
        for(struct tag *temp = *head; temp != NULL; temp = temp->nextTag){
            if(strcmp(temp->tagName, newTag->tagName) == 0) {   
                addFolder(&(temp->headFolderList), newFolder);
                if(strcmp(folderNames[0], folderName) == 0){
                    temp->countForZero++;
                    if(temp->countForZero < 2){
                        numOfZero++;
                    }
                }
                else if(strcmp(folderNames[1], folderName) == 0){
                    temp->countForOne++;
                    if(temp->countForOne < 2){
                        numOfOne++;
                    }
                }
                else {
                    temp->countForTwo++;
                    if(temp->countForTwo < 2){
                        numOfTwo++;
                    }    
                }
                return;
            }
            prev = temp;  
        }
        addFolder(&(newTag->headFolderList), newFolder);
        if(strcmp(folderNames[0], folderName) == 0){
            newTag->countForZero++;
            numOfZero++;
        }
        else if(strcmp(folderNames[1], folderName) == 0){
            newTag->countForOne++;
            numOfOne++;
        }
        else {
            newTag->countForTwo++;  
            numOfTwo++;
        }
        prev->nextTag = newTag;
    }    
}
//compare the folder names
int compareFolderName(struct folder *headFirst, struct folder *headSecond){
    for(struct folder *tempFirst = headFirst; tempFirst != NULL; tempFirst = tempFirst->next){
        for(struct folder *tempSecond = headSecond; tempSecond != NULL; tempSecond = tempSecond->next){
            if(strcmp(tempFirst->folderName, tempSecond->folderName) == 0){
                return 1;
            }   
        }  
    }
    return 0;
}
//add and set first order
void addFirstOrder(struct tagList **head, struct tag *tagPtr){
    struct tagList *newOrder = malloc(sizeof(struct tagList));
    newOrder->node = tagPtr;
    struct tagList *temp = *head;
    *head = newOrder; 
    (*head)->nxtPtr = temp;
}
//create first order list
void createFirsOrderList(struct tag **head){
    struct tag *tempFirst = *head;
    while(tempFirst){
        struct tag *tempSecond = *head;
            while(tempSecond){
               if(strcmp(tempFirst->tagName, tempSecond->tagName) == 0){
                    tempSecond = tempSecond->nextTag;
                    continue;
                }
                if(compareFolderName(tempFirst->headFolderList, tempSecond->headFolderList) == 1){
                    addFirstOrder(&(tempFirst->headFirstOrder), tempSecond);
                }
                tempSecond = tempSecond->nextTag; 
            }      
       tempFirst = tempFirst->nextTag; 
    }    
}
//add and set second order
void addTagSecondOrderAndThirdOrder(struct tagList **head, struct tag *tagPtr){
    if(isThere(tagPtr->tagName, *head) == 0){
        struct tagList *newOrder = malloc(sizeof(struct tagList));
        newOrder->node = tagPtr;
        struct tagList *temp = *head;
        *head = newOrder; 
        (*head)->nxtPtr = temp;
    }
}
//compare first order and add second order
void compareFirstOrder(struct tagList **head, 
                        struct tagList *headFirst,
                        struct tagList *headSecond,
                        char *tagName
                        ){
    for(struct tagList *tempFirst = headSecond;
                tempFirst != NULL;
                    tempFirst = tempFirst->nxtPtr
                    ){
        if(strcmp(tempFirst->node->tagName, tagName) != 0 && 
                isThere(tempFirst->node->tagName, headFirst) == 0
                ){
            addTagSecondOrderAndThirdOrder(head, tempFirst->node);
            
        }
    }
}
int isThere(char *tagName, struct tagList *head){
    for(struct tagList *temp = head; temp != NULL; temp = temp->nxtPtr ){
        if(strcmp(temp->node->tagName, tagName) == 0)
            return 1;
    }
    return 0;
}
//create second order list
void createSecondOrderList(struct tag **head){
    struct tag *tempFirst = *head;
    while(tempFirst){
        struct tagList *tempSecond = tempFirst->headFirstOrder;
        if(strcmp(tempFirst->tagName, tempSecond->node->tagName) == 0){
            tempSecond = tempSecond->nxtPtr;
        }
            while(tempSecond){
                compareFirstOrder(&(tempFirst->headSecondOrder), 
                                    tempFirst->headFirstOrder, 
                                    tempSecond->node->headFirstOrder, 
                                    tempFirst->tagName
                                    );
                tempSecond = tempSecond->nxtPtr; 
            }      
       tempFirst = tempFirst->nextTag; 
    }
}
//create third order list
void createThirdOrderList(struct tag **head){
    for( struct tag *tempFirst = *head; tempFirst != NULL; tempFirst = tempFirst->nextTag){
        for(struct tagList *tempSecond = tempFirst->headFirstOrder;
                tempSecond != NULL;
                    tempSecond = tempSecond->nxtPtr
                    ){
           if(strcmp(tempFirst->tagName, tempSecond->node->tagName) != 0){
                compareForThirdOrder(&(tempFirst->headThirdOrder), 
                                    tempFirst->headFirstOrder,
                                    tempFirst->headSecondOrder, 
                                    tempSecond->node->headSecondOrder, 
                                    tempFirst->tagName
                                    );
            }
        } 
        for(struct tagList *tempThird = tempFirst->headSecondOrder;
                tempThird != NULL;
                    tempThird = tempThird->nxtPtr
                    ){
           if(strcmp(tempFirst->tagName, tempThird->node->tagName) != 0){
                compareForThirdOrder(&(tempFirst->headThirdOrder), 
                                    tempFirst->headSecondOrder, 
                                    tempFirst->headFirstOrder,
                                    tempThird->node->headFirstOrder, 
                                    tempFirst->tagName
                                    );
            }
        } 
        for(struct tagList *tempFourth = tempFirst->headThirdOrder;
                tempFourth != NULL;
                    tempFourth = tempFourth->nxtPtr
                    ){
           if(strcmp(tempFirst->tagName, tempFourth->node->tagName) != 0){
                compareForThirdOrder(&(tempFirst->headThirdOrder), 
                                    tempFirst->headThirdOrder, 
                                    tempFirst->headSecondOrder,
                                    tempFourth->node->headFirstOrder, 
                                    tempFirst->tagName
                                    );
            }
        }     
    }
}
//compare second and first order and add third order
void compareForThirdOrder(struct tagList **head, 
                        struct tagList *headFirst,
                        struct tagList *headFirstSecondOrder,
                        struct tagList *headSecond,
                        char *tagName
                        ){
    for(struct tagList *tempFirst = headSecond;
                tempFirst != NULL;
                    tempFirst = tempFirst->nxtPtr
                    ){
        if(strcmp(tempFirst->node->tagName, tagName) != 0 
            && isThere(tempFirst->node->tagName, headFirst) == 0
                && isThere(tempFirst->node->tagName, headFirstSecondOrder) == 0
                    ){
            addTagSecondOrderAndThirdOrder(head, tempFirst->node);  
        }
    }
}
void fillArrays(struct tag **head, struct tag zero[], struct tag one[], struct tag two[]){
    for(struct tag *temp = *head; temp != NULL; temp = temp->nextTag){
        if(temp->countForZero != 0){
            zero[numZero++] = *temp; 
        }
        if(temp->countForOne != 0){
            one[numOne++] = *temp;
        }
        if(temp->countForTwo != 0){
            two[numTwo++] = *temp;
        }
    }
}
void selectionSortForZero(struct tag array[], int num){
    int biggest;
    struct tag temp;
    for (int i = 0; i < num - 1; i++){
        biggest = i;
        for (int j = i + 1; j < num; j++){
            if (array[j].countForZero > array[biggest].countForZero){
                biggest = j;
            }  
        }
        temp = array[i];
        array[i] = array[biggest];
        array[biggest] = temp;
    }
}
void selectionSortForOne(struct tag array[], int num){
    int biggest;
    struct tag temp;
    for (int i = 0; i < num - 1; i++){
        biggest = i;
        for (int j = i + 1; j < num; j++){
            if (array[j].countForOne > array[biggest].countForOne){
                biggest = j;
            }  
        }
        temp = array[i];
        array[i] = array[biggest];
        array[biggest] = temp;
    }
}
void selectionSortForTwo(struct tag array[], int num){
    int biggest;
    struct tag temp;
    for (int i = 0; i < num - 1; i++){
        biggest = i;
        for (int j = i + 1; j < num; j++){
            if (array[j].countForTwo > array[biggest].countForTwo){
                biggest = j;
            }  
        }
        temp = array[i];
        array[i] = array[biggest];
        array[biggest] = temp;
    }
}
void calculateTxtNumber(struct tag **head){
    for(struct tag *temp = *head; temp != NULL; temp = temp->nextTag){
        for(struct folder *tempFolder = temp->headFolderList; tempFolder != NULL;  tempFolder = tempFolder->next){
            if(strcmp(folderNames[0], tempFolder->typeOfFolder) == 0){
                temp->countForZeroTxt++;
            }
            if(strcmp(folderNames[1], tempFolder->typeOfFolder) == 0){
                temp->countForOneTxt++;
            }
            if(strcmp(folderNames[2], tempFolder->typeOfFolder) == 0){
                temp->countForTwoTxt++;
            }
        }
    }
}
void calculateIdfForZero(struct tag ** head, struct tag array[]){
    int i = 0;
    for(struct tag *temp = *head; temp != NULL; temp = temp->nextTag){
        if(temp->countForZeroTxt != 0){
            temp->idfForZero = (temp->countForZero)*(log(file[0]/temp->countForZeroTxt));
            array[i++] = *temp;
        }
    }
}
void calculateIdfForOne(struct tag ** head, struct tag array[]){
    int i = 0;
    for(struct tag *temp = *head; temp != NULL; temp = temp->nextTag){
        if(temp->countForOneTxt != 0){
            temp->idfForOne = (temp->countForOne)*(log(file[1]/temp->countForOneTxt));
            array[i++] = *temp;
        }
    }
}
void calculateIdfForTwo(struct tag ** head, struct tag array[]){
    int i = 0;
    for(struct tag *temp = *head; temp != NULL; temp = temp->nextTag){
        if(temp->countForTwoTxt != 0){
            temp->idfForTwo = (temp->countForTwo)*(log(file[2]/temp->countForTwoTxt));
            array[i++] = *temp;
        }
    }
}
void selectionSortForIdfZero(struct tag array[], int num){
    int biggest;
    struct tag temp;
    for (int i = 0; i < num - 1; i++){
        biggest = i;
        for (int j = i + 1; j < num; j++){
            if (array[j].idfForZero > array[biggest].idfForZero){
                biggest = j;
            }  
        }
        temp = array[i];
        array[i] = array[biggest];
        array[biggest] = temp;
    }
}
void selectionSortForIdfOne(struct tag array[], int num){
    int biggest;
    struct tag temp;
    for (int i = 0; i < num - 1; i++){
        biggest = i;
        for (int j = i + 1; j < num; j++){
            if (array[j].idfForOne > array[biggest].idfForOne){
                biggest = j;
            }  
        }
        temp = array[i];
        array[i] = array[biggest];
        array[biggest] = temp;
    }
}
void selectionSortForIdfTwo(struct tag array[], int num){
    int biggest;
    struct tag temp;
    for (int i = 0; i < num - 1; i++){
        biggest = i;
        for (int j = i + 1; j < num; j++){
            if (array[j].idfForTwo > array[biggest].idfForTwo){
                biggest = j;
            }  
        }
        temp = array[i];
        array[i] = array[biggest];
        array[biggest] = temp;
    }
}