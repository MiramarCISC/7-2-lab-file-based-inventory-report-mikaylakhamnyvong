#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
    // Return true when quantity is 0 or greater.
    return quantity >= 0;
}

bool isValidPrice(double price) {
    // Return true when price is 0 or greater.
    return price >= 0;
}

double calculateItemValue(const InventoryItem& item) {
    // Return quantity multiplied by price.
    // If quantity or price is invalid, return 0.0.
    if(!isValidQuantity(item.quantity) || !isValidPrice(item.price)){
        return 0.0;
    }
    return item.quantity * item.price;
}

int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    // Read records in this format: sku name quantity price
    // Store valid records in the items array.
    // Stop when the file ends or maxItems is reached.
    // Return the number of records stored. pxpham: deleted unnecessary comment and deleted unncessary spaces.
    ifstream inFile(filename);
    if(!inFile.is_open()) {
        return 0;
    }
    int count = 0;
    InventoryItem temp;
    while(count < maxItems && (inFile >> temp.sku >> temp.name >> temp.quantity >> temp.price)) {
        if(isValidQuantity(temp.quantity) && isValidPrice(temp.price)) { 
            items[count] = temp;
            count++;
        }
    }
    inFile.close();
    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    // Write in output file: each item and its total value, total inventory value.
    // Return true if the report was written successfully. pxpham: rewrote comments for efficient spacings and shortening.
    ofstream outFile(filename);
    if(!outFile.is_open()) {
        return false;
    } //pxpham: deleted unnecessary spacing.
    outFile << fixed << setprecision(2);
    for(int i = 0; i < count; i++) {
        double value = calculateItemValue(items[i]);
        outFile << items[i].sku << " "
                << items[i].name << " "
                << items[i].quantity << " "
                << items[i].price << " "
                << value << "\n";
    }
    double total = calculateTotalInventoryValue(items, count);
    outFile << "Total inventory value: " << total << "\n";
    outFile.close();
    return true;
}

double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    // Return the sum of all item values.
    // Return 0.0 for null arrays or invalid counts. pxpham: deleted unncessary spacing.
    if(items == nullptr || count <= 0) {
        return 0.0;
    }
    double total = 0.0;
    for(int i = 0; i < count; i++) {
        total += calculateItemValue(items[i]);
    }
    return total;
}

int findItemBySku(const InventoryItem items[], int count, std::string sku) {
    // Search for a matching SKU.
    // Return the index if found. Return -1 if not found. pxpham: combined comments for effcient spacing. Deleted unnecessary spacing.
    if (items == nullptr || count <= 0) {
        return -1;
    }
    for(int i = 0; i < count; i++) {
        if(items[i].sku == sku) {
            return i;
        }
    }
    return -1;
}

int findHighestValueItemIndex(const InventoryItem items[], int count) {
    // Return the index of the item with the highest item value.
    // Return -1 for null arrays or invalid counts.
    if(items == nullptr || count <= 0) {
        return -1;
    }

    int highestIndex = 0;
    double highestValue = calculateItemValue(items[0]);
    for(int i = 1; i < count; i++) {
        double value = calculateItemValue(items[i]);
        if(value > highestValue) {
            highestValue = value;
            highestIndex = i;
        }
    }
    return highestIndex;
}
