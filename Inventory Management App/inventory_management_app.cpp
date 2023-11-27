/********GROUP MEMBERS**********
Dabillah Benjamin  26102827

John Bannerman - 26102839

Mikhail Mubarak - 26102989

Samuel Sarpong Kobby - 26102797

Fawzan Awuni - 26102796 */


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Item structure to represent each inventory item
struct InventoryItem {
    string name;
    int quantity;
    double price;
};

// Function to display the inventory
void displayInventory(const vector<InventoryItem>& inventory) {
    cout << "Inventory:\n";
    cout << "---------------------------------\n";
    cout << "Name\t\tQuantity\tPrice\n";
    cout << "---------------------------------\n";

    for (const auto& item : inventory) {
        cout << item.name << "\t\t" << item.quantity << "\t\t$" << item.price << "\n";
    }

    cout << "---------------------------------\n";
}

// Function to add an item to the inventory
void addItem(vector<InventoryItem>& inventory) {
    InventoryItem newItem;
    cout << "Enter item name: ";
    cin >> newItem.name;
    cout << "Enter quantity: ";
    cin >> newItem.quantity;
    cout << "Enter price: $";
    cin >> newItem.price;

    inventory.push_back(newItem);

    cout << "Item added to inventory.\n";
}

// Function to remove an item from the inventory
void removeItem(vector<InventoryItem>& inventory) {
    string itemName;
    cout << "Enter the name of the item to remove: ";
    cin >> itemName;

    // Find the item in the inventory
    auto it =  find_if(inventory.begin(), inventory.end(),[itemName](const InventoryItem& item) {return item.name == itemName;});

    // Check if the item was found
    if (it != inventory.end()) {
        // Remove the item
        inventory.erase(it);
        cout << "Item removed from inventory.\n";
    } else {
        cout << "Item not found in inventory.\n";
    }
}

int main() {
    vector<InventoryItem> inventory; // Vector to store inventory items

    int choice;

    do {
        // Display menu
        cout << "\nInventory Management System\n";
        cout << "1. Display Inventory\n";
        cout << "2. Add Item\n";
        cout << "3. Remove Item\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Process user choice
        switch (choice) {
            case 1:
                displayInventory(inventory);
                break;
            case 2:
                addItem(inventory);
                break;
            case 3:
                removeItem(inventory);
                break;
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }

    } while (choice != 4);

    return 0;
}
