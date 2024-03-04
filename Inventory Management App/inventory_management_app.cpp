#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

// Item structure to represent each inventory item
struct InventoryItem
{
    string name;
    int quantity;
    double price;
};

// Functions declarations
void createTables(sqlite3 *db);
bool authenticateUser(sqlite3 *db, const string &username, const string &password);
void loginUser(sqlite3 *db, string &username, string &password);
void registerUser(sqlite3 *db, const string &username, const string &password);

void displayInventory(sqlite3 *db);
void addItem(sqlite3 *db);
void removeItem(sqlite3 *db);
void clearInventory(sqlite3 *db);

// SQLite callback function for queries
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

int main()
{
    int choice;
    string username, password, user_ans = " ";
    sqlite3 *db;
    char *zErrMsg = 0;

    // Open DB and Create Tables
    if (sqlite3_open("inventory.db", &db) == SQLITE_OK)
    {

        createTables(db);

        cout << "Are you a user?y/n";
        cin >> user_ans;

        if (user_ans == "y" || user_ans == "Y")
        {
            loginUser(db, username, password);
        }
        else if (user_ans == "n" || user_ans == "n")
        {
            registerUser(db, username, password);
        }
        else
        {
            cout << "You entered an Invalid answer!";
        }

        // Program Menu
        do
        {
            // Display menu
            cout << "\nInventory Management System\n";
            cout << "1. Display Inventory\n";
            cout << "2. Add Item\n";
            cout << "3. Remove Item\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            // Process user choice
            switch (choice)
            {
            case 1:
                displayInventory(db);
                break;
            case 2:
                addItem(db);
                break;
            case 3:
                removeItem(db);
                break;
            case 4:
                clearInventory(db);
                break;
            case 5:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
            }
        } while (choice != 5);
    }

    // Close Db
    sqlite3_close(db);

    return 0;
}

//********FUNCTIONS

// Create Tables for User and Task
void createTables(sqlite3 *db)
{
    // Create the users table
    string createUserTableSQL = "CREATE TABLE IF NOT EXISTS users ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                " username TEXT NOT NULL,"
                                " password TEXT NOT NULL);";

    if (sqlite3_exec(db, createUserTableSQL.c_str(), callback, 0, 0) != SQLITE_OK)
    {
        cerr << "Error creating users table." << endl;
    }

    // Create the tasks table
    string createInventoryTableSQL = "CREATE TABLE IF NOT EXISTS inventory("
                                     " id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "name TEXT NOT NULL, "
                                     "quantity INTEGER NOT NULL, "
                                     "price REAL NOT NULL);";

    if (sqlite3_exec(db, createInventoryTableSQL.c_str(), callback, 0, 0) != SQLITE_OK)
    {
        cerr << "Error creating Inventory table." << endl;
    }
}

// Authenticate user
bool authenticateUser(sqlite3 *db, const string &username, const string &password)
{
    string query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "';";
    char *errorMsg = nullptr;

    if (sqlite3_exec(db, query.c_str(), callback, 0, &errorMsg) == SQLITE_OK)
    {
        return true; // Authentication successful
    }
    else
    {
        cerr << "SQL error: " << errorMsg << endl;
        sqlite3_free(errorMsg);
        return false; // Authentication failed
    }
}

// Register New User
void registerUser(sqlite3 *db, const string &username, const string &password)
{

    string insertUserSQL = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "');";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, insertUserSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_STATIC);

    // Execute Statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Error Registering user. SQL ERROR: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Registration successful!" << endl;
    }

    sqlite3_finalize(stmt);
}

// Function to display the inventory
void displayInventory(sqlite3 *db)
{
    cout << "Inventory:\n";
    cout << "---------------------------------\n";
    cout << "Name\t\tQuantity\tPrice\n";
    cout << "---------------------------------\n";

    string showInventorySQL = "SELECT * FROM inventory";
    if (sqlite3_exec(db, showInventorySQL.c_str(), callback, 0, 0) != SQLITE_OK)
    {
        cerr << "Error retrieving tasks." << endl;
    }

    cout
        << "---------------------------------\n";
}

// Function to add an item to the inventory
void addItem(sqlite3 *db)
{
    InventoryItem newItem;
    cout << "Enter item name: ";
    cin >> newItem.name;
    cout << "Enter item quantity: ";
    cin >> newItem.quantity;
    cout << "Enter item price: ";
    cin >> newItem.price;

    string insertItemSQL = "INSERT INTO inventory(name, quantity, price) VALUES (?,?,?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, insertItemSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, newItem.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, newItem.quantity);
    sqlite3_bind_double(stmt, 3, newItem.price);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Error adding task. SQL ERROR: " << sqlite3_errmsg(db) << endl;
    }
    cout << "Item added to inventory.\n";
    sqlite3_finalize(stmt);
}

// Function to remove an item from the inventory
void removeItem(sqlite3 *db)
{
    int itemId;
    cout << "Enter the Id of the item to remove from Inventory: ";
    cin >> itemId;

    string removeInventorySQL = "DELETE FROM inventory WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, removeInventorySQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, itemId);

    // Execute Statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Error deleting item. SQL ERROR: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Item removed from Inventory.\n";
    }

    sqlite3_finalize(stmt);
}

void clearInventory(sqlite3 *db)
{
    string clearInventorySQL = "DELETE FROM inventory WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, clearInventorySQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }
}