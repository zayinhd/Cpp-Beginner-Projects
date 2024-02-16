#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

struct Task
{
    string description;
    bool completed;
};

// Function declarations
void start(int choice, sqlite3 *db);
void showTodoList(sqlite3 *db);
void addTodoItem(sqlite3 *db);
void completeTodoItem(sqlite3 *db);
void deleteTodoItem(sqlite3 *db);
void clearTodoList(sqlite3 *db);
void createTables(sqlite3 *db);
bool authenticateUser(sqlite3 *db, const string &username, const string &password);
void loginUser(sqlite3 *db, string &username, string &password);
void registerUser(sqlite3 *db, const string &username, const string &password);

// SQLite callback function for queries
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

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
    string createTaskTableSQL = "CREATE TABLE IF NOT EXISTS tasks("
                                " id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "description TEXT NOT NULL, "
                                " completed INTEGER NOT NULL);";

    if (sqlite3_exec(db, createTaskTableSQL.c_str(), callback, 0, 0) != SQLITE_OK)
    {
        cerr << "Error creating tasks table." << endl;
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

// Login user with authentication function
void loginUser(sqlite3 *db, string &username, string &password)
{
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    while (!authenticateUser(db, username, password))
    {
        cout << "Login failed. Please try again." << endl;
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;
    }

    cout << "Login successful!" << endl;
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

void start(int choice, sqlite3 *db)
{
    do
    {

        cout << "\n\t\tToDo List Management App\n";
        cout << "\t     ---------------------------\n";
        cout << "1. Show Todo List\n";
        cout << "2. Add Todo Item\n";
        cout << "3. Complete Todo Item\n";
        cout << "4. Delete Todo Item\n";
        cout << "5. Clear Todo List\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            showTodoList(db);
            break;
        case 2:
            addTodoItem(db);
            break;
        case 3:
            completeTodoItem(db);
            break;
        case 4:
            deleteTodoItem(db);
            break;
        case 5:
            clearTodoList(db);
            break;
        case 6:
            cout << "Exiting program...";
            break;
        default:
            cout << "Invalid choice. Please select a valid choice.";
            break;
        }
    } while (choice != 6);
}

void showTodoList(sqlite3 *db)
{
    cout << "\n\t\tTodo List\n";
    cout << "------------------------\n";
    cout << "Status\tDescription\n";
    cout << "------------------------\n";

    string query = "SELECT * FROM tasks;";
    if (sqlite3_exec(db, query.c_str(), callback, 0, 0) != SQLITE_OK)
    {
        cerr << "Error retrieving tasks." << endl;
    }
    // for (const auto &task : todoList)
    // {
    //     cout << (task.completed ? "Done\t" : "Pending\t") << task.description << endl;
    // }
    // cout << "------------------------\n";
}
void addTodoItem(sqlite3 *db)
{
    Task newTask;
    cout << "\nWhat are you planning to do?\n";
    cin.ignore();
    getline(cin, newTask.description);

    newTask.completed = false;

    string insertTaskSQL = "INSERT INTO tasks (description, completed) VALUES (?,?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, insertTaskSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, newTask.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, newTask.completed);

    // Execute Statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Error adding task. SQL ERROR: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "New Task added to Todo List.\n";
    }

    sqlite3_finalize(stmt);
}
void completeTodoItem(sqlite3 *db)
{
    int taskId;
    cout << "Enter the index of the task to mark as Completed : ";
    cin >> taskId;

    string updateTaskSQL = "UPDATE tasks SET completed = 1 WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, updateTaskSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }
    // Bind parameters
    sqlite3_bind_int(stmt, 1, taskId);

    // Execute Statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Error completing task. SQL ERROR: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Task marked as completed.\n";
    }

    sqlite3_finalize(stmt);
}
void deleteTodoItem(sqlite3 *db)
{
    int taskId;
    cout << "Enter the index of the task to Delete : ";
    cin >> taskId;

    string deleteTaskSQL = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, deleteTaskSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing SQL statement" << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, taskId);

    // Execute Statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Error deleting task. SQL ERROR: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Task removed from Todo List.\n";
    }

    sqlite3_finalize(stmt);
}
void clearTodoList(sqlite3 *db)
{
    string clearTasksSQL = "DELETE FROM tasks;";
    char *errorMsg = nullptr;

    if (sqlite3_exec(db, clearTasksSQL.c_str(), callback, 0, &errorMsg) != SQLITE_OK)
    {
        cerr << "Error clearing tasks. SQL error: " << errorMsg << endl;
        sqlite3_free(errorMsg);
    }
    else
    {
        cout << "Todo List Cleared.\n";
    }
}

// Main function
int main()
{
    sqlite3 *db;
    int choice;
    string user;

    // Open SQLITE DB
    if (sqlite3_open("todo.db", &db) == SQLITE_OK)
    {

        createTables(db);

        cout << "Are you a user?y/n";
        cin >> user;
        string username, password;

        if (user == "y" || user == "Y")
        {
            loginUser(db, username, password);
        }
        else if (user == "n" || user == "N")
        {
            registerUser(db, username, password);
        }

        start(choice, db);
    }

    sqlite3_close(db);

    return 0;
}
