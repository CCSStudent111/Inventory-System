/***********************************************************************************
**********************
This is to certify that this project is my own work, based on my personal efforts
in studying and applying the concepts learned. I
have constructed the functions and their respective algorithms and corresponding
code by myself. The program was run, tested, and debugged
by my own efforts. I further certify that I have not copied in part or whole or
otherwise plagiarized the work of other students and/or persons.
Jandeil Dural, DLSU ID# 12201006
***********************************************************************************
**********************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100
#define inv "inventory.txt" //Any mention of inventory.txt are now refered as "inv".
#define rary "temp.txt" //Any mention of temp.txt (for the UpdateProd___() functions) are now refered as "rary".
typedef char string[101]; //For readability's sake.

//Structure for DataStorage
struct DataStorage {
    string ProductID[6];
    string ProductName[31];
    int Quantity;
    double PPU;
};

//ProdIDUnique returns false if either (1) the inv file is not available, or (2) both the IDs are identical (hence == 0), and returns true if otherwise.
bool ProdIDUnique(const char* productID){
    char line[100];

    FILE* fp = fopen(inv, "r");
    if (fp == NULL){
        printf("Error opening file.\n");
        return false;
    }
    while (fgets(line, sizeof(line), fp)){
        char ID[6];
        sscanf(line, "%[^,]", ID);
        if(strcmp(productID, ID) == 0){
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

//AddNewProdData adds a new product to the inv file with a unique Product ID.
void AddNewProdData(){
    struct DataStorage newStorage;
    FILE* fp = fopen(inv, "a");
    if (fp == NULL){
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the product ID (up to 5 characters): ");
    scanf("%5s", newStorage.ProductID);
    while (!ProdIDUnique(newStorage.ProductID)){
        printf("Product ID already exists in the inventory.\nPlease enter a unique ID: ");
        scanf("%s", newStorage.ProductID);
    }
    printf("Enter Product Name (max of 30 chars): ");
    scanf(" %31[^\n]", newStorage.ProductName);

    printf("Enter Quantity: ");
    scanf("%d", &newStorage.Quantity);

    printf("Enter Price per Unit: ");
    scanf("%lf", &newStorage.PPU);

    printf("\n--- Product added succesfully. ---\n");
    fprintf(fp, "%s,%s,%d,%lf\n", newStorage.ProductID, newStorage.ProductName, newStorage.Quantity, newStorage.PPU);

    fclose(fp);
}

//DisplayStorage displays the contents of the inv file.
void DisplayStorage(){
    FILE* fp = fopen(inv, "r");
    struct DataStorage storage;

    printf("\nInventory:\n\n");
    printf("-----------------------------------------------------------------\n");
    while(fscanf(fp, "%[^,],%[^,],%d,%lf\n", storage.ProductID,
        storage.ProductName, &storage.Quantity, &storage.PPU) == 4){
        printf("Product ID: %s\n", storage.ProductID);
        printf("Product Name: %s\n", storage.ProductName);
        printf("Quantity: %d\n", storage.Quantity);
        printf("Price Per Unit: %.2f\n", storage.PPU);
        printf("-----------------------------------------------------------------\n");
    }

    fclose(fp);
}

//SearchProd asks for either the Product ID or the Product Name and returns the structure that is tied to said Product ID or Name.
void SearchProd(){
    FILE* fp = fopen(inv, "r");
    char line[100];
    int found = 0;
    char search[31];

    if (fp == NULL){
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the Product ID or the Product Name: ");
    scanf("%s", search);

    while (fgets(line, sizeof(line), fp)){
        char ProdID[6];
        char ProdName[31];
        int Quantity;
        float PPU;

        sscanf(line, "%[^,], %31[^,],%d,%f", ProdID, ProdName, &Quantity, &PPU);

        if (strcmp(search, ProdID) == 0 || strcmp(search,ProdName) == 0){
            printf("\n\n--- Product found. ---\n");
            printf("-----------------------------------------------------------------\n");
            printf("Product ID: %s\n", ProdID);
            printf("Product Name: %s\n", ProdName);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %.2f\n", PPU);
            printf("-----------------------------------------------------------------\n");
            found = 1;
        }
        }
    fclose(fp);
    if (!found){
        printf("\n--- Product not found. ---\n");
    }
}

//UpdateProdQuan updates the DataStorage.Quantity of the specified Product ID.
void UpdateProdQuan() {
    FILE* fp = fopen(inv, "r");
    FILE* tempfp = fopen(rary, "w");
    struct DataStorage newStorage[100];
    char ProdID[6];
    int newQuantity;
    int numProds = 0;
    int found = 0;

    if (fp == NULL || tempfp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the Product ID: ");
    scanf("%5s", ProdID);

    while (fscanf(fp, "%[^,],%[^,],%d,%lf\n", newStorage[numProds].ProductID, newStorage[numProds].ProductName, &newStorage[numProds].Quantity, &newStorage[numProds].PPU) == 4) {
        if (strcmp(ProdID, newStorage[numProds].ProductID) == 0) {
            printf("Enter the new quantity: ");
            scanf("%d", &newQuantity);
            newStorage[numProds].Quantity = newQuantity;
            found = 1;
        }
        numProds++;
    }

    fclose(fp);

    if (!found) {
        printf("--- Product ID not found. ---\n");
        remove(rary);
        return;
    }

    for (int i = 0; i < numProds; i++) {
        fprintf(tempfp, "%s,%s,%d,%.2f\n", newStorage[i].ProductID, newStorage[i].ProductName, newStorage[i].Quantity, newStorage[i].PPU);
    }

    fclose(tempfp);
    remove(inv);
    rename(rary, inv);
    printf("\n--- Product quantity updated successfully. ---\n");
}

//UpdateProdPrice updates the DataStorage.PPU of the specified Product ID.
void UpdateProdPrice() {
    FILE* fp = fopen(inv, "r");
    FILE* tempfp = fopen(rary, "w");
    struct DataStorage newStorage[100];
    char ProdID[6];
    double newPrice;
    int numProds = 0;
    int found = 0;

    if (fp == NULL || tempfp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("Enter the Product ID: ");
    scanf("%5s", ProdID);

    while (fscanf(fp, "%[^,],%[^,],%d,%lf\n", newStorage[numProds].ProductID, newStorage[numProds].ProductName, &newStorage[numProds].Quantity, &newStorage[numProds].PPU) == 4) {
        if (strcmp(ProdID, newStorage[numProds].ProductID) == 0) {
        printf("Enter the new price: ");
        scanf("%lf", &newPrice);
        newStorage[numProds].PPU = newPrice;
        found = 1;
        }
        numProds++;
    }

    fclose(fp);
    if (!found) {
        printf("\n--- Product ID not found. ---\n");
        remove(rary);
        return;
    }

    for (int i = 0; i < numProds; i++) {
        fprintf(tempfp, "%s,%s,%d,%.2f\n", newStorage[i].ProductID, newStorage[i].ProductName, newStorage[i].Quantity, newStorage[i].PPU);
    }

    fclose(tempfp);

    remove(inv);
    rename(rary, inv);
    printf("\n--- Product price updated successfully. ---\n");
}
//The main body of the program.
int main(){
    int pwCounter = 0;
    int Access = 0;
    string pass1;
    string truePW = "CCPROG2";
    int choice;

    //This acts as the security of the Inventory System.
    while (pwCounter < 3 && !Access){
        printf("Please input the password: ");
        scanf("%s", pass1);

        if (strcmp(pass1, truePW) == 0){
            printf("Access granted. Welcome to the Inventory System.\n");
            Access = 1;
        }
        else {
            printf("Unauthorized access not allowed.\n");
            pwCounter++;
        }
    }
    if (pwCounter == 3){
        printf("\n!!!Access denied. Too many attempts!!!\n");
        return 1;
    }
    do {
        printf("\n--- Product Inventory System Menu ---\n");
        printf("1. View Inventory\n");
        printf("2. Add Product\n");
        printf("3. Update Product Quantity\n");
        printf("4. Update Product Price\n");
        printf("5. Search Product\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                DisplayStorage();
                break;
            case 2:
                AddNewProdData();
                break;
            case 3:
                UpdateProdQuan();
                break;
            case 4:
                UpdateProdPrice();
                break;
            case 5:
                SearchProd();
                break;
            case 6:
                printf("Exiting the program. Thank you for using the Inventory System. Goodbye!\n");
                break;
        }
    } while (choice != 6);

    return 0;
}
