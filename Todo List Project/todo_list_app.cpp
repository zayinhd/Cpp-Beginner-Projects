#include <iostream>
#include <vector>
#include <string>
#include "sqlite3.h"

using namespace std;

struct Task
{
    string description;
    bool completed;
};

// Function declarations
void start(int choice, vector<Task> &todoList);
void showTodoList(const vector<Task> &todoList);
void addTodoItem(vector<Task> &todoList);
void completeTodoItem(vector<Task> &todoList);
void deleteTodoItem(vector<Task> &todoList);
void clearTodoList(vector<Task> &todoList);
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
    char *errorMsg = nullptr;

    if (sqlite3_exec(db, insertUserSQL.c_str(), callback, 0, &errorMsg) != SQLITE_OK)
    {
        cerr << "Error registering user. SQL error: " << errorMsg << endl;
        sqlite3_free(errorMsg);
    }
    else
    {
        cout << "Registration successful!" << endl;
    }
}

void start(int choice, vector<Task> &todoList, sqlite3 *db)
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
            showTodoList(todoList);
            break;
        case 2:
            addTodoItem(todoList);
            break;
        case 3:
            completeTodoItem(todoList);
            break;
        case 4:
            deleteTodoItem(todoList);
            break;
        case 5:
            clearTodoList(todoList);
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

void showTodoList(const vector<Task> &todoList)
{
    cout << "\n\t\tTodo List\n";
    cout << "------------------------\n";
    cout << "Status\tDescription\n";
    cout << "------------------------\n";

    for (const auto &task : todoList)
    {
        cout << (task.completed ? "Done\t" : "Pending\t") << task.description << endl;
    }
    cout << "------------------------\n";
}
void addTodoItem(vector<Task> &todoList)
{
    Task newTask;
    cout << "\nWhat are you planning to do?\n";
    cin.ignore();
    getline(cin, newTask.description);

    newTask.completed = false;

    todoList.push_back(newTask);

    cout << "New Task added to Todo List.\n";
}
void completeTodoItem(vector<Task> &todoList)
{
    int index;
    cout << "Enter the index of the task to mark as Completed : ";
    cin >> index;

    if (index >= 0 && index < todoList.size())
    {
        todoList[index].completed = true;
        cout << "Task marked as completed.\n";
    }
    else
    {
        cout << "Invalid index.\n";
    }
}
void deleteTodoItem(vector<Task> &todoList)
{
    int index;
    cout << "Enter the index of the task to Delete : ";
    cin >> index;

    if (index >= 0 && index < todoList.size())
    {
        todoList.erase(todoList.begin() + index);
        cout << "Task removed from Todo List.\n";
    }
    else
    {
        cout << "Task not found in Todo List.\n";
    }
}
void clearTodoList(vector<Task> &todoList)
{
    // Use the clear function to remove all task
    todoList.clear();
    cout << "Todo List Cleared.\n";
}
int main()
{
    sqlite3 *db;
    vector<Task> todoList;
    int choice;

    string username = " ";
    string password = " ";

    // Open SQLITE DB
    if (sqlite3_open("todo.db", &db) == SQLITE_OK)
    {

        createTables(db);
        loginUser(db, username, password);

        start(choice, todoList, db);
    }

    sqlite3_close(db);
    return 0;
}
