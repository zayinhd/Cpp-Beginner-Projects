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

void start(int choice, vector<Task> &todoList){
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
        cout << "7. Enter your choice: ";
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
	cout<<"\n\t\tTodo List\n";
	cout << "------------------------\n";
    cout << "Status\tDescription\n";
    cout << "------------------------\n";
    
    for(const auto& task : todoList){
    	cout<<(task.completed ? "Done\t" : "Pending\t")<<task.description<<endl;
	}
	cout << "------------------------\n";
}
void addTodoItem(vector<Task> &todoList)
{
	Task newTask;
	cout<<"\nWhat are you planning to do?\n";
	cin.ignore();
	getline(cin, newTask.description);
	
	newTask.completed = false;
	
	todoList.push_back(newTask);
	
	cout<<"New Task added to Todo List.\n";
	
}
void completeTodoItem(vector<Task> &todoList)
{
	int index;
	cout<<"Enter the index of the task to mark as Completed : ";
	cin>>index;
	
	if(index >= 0 && index < todoList.size()){
		todoList[index].completed = true;
		cout<<"Task marked as completed.\n";
	}
	else{
		cout << "Invalid index.\n";
	}
}
void deleteTodoItem(vector<Task> &todoList)
{
	int index;
	cout<<"Enter the index of the task to Delete : ";
	cin>>index;
	
	if(index >= 0 && index < todoList.size()){
		todoList.erase(todoList.begin() + index);
		cout<<"Task removed from Todo List.\n";
	}
	else{
		cout<<"Task not found in Todo List.\n";
	}
}
void clearTodoList(vector<Task> &todoList)
{
	// Use the clear function to remove all task
	todoList.clear();
	cout<<"Todo List Cleared.\n";
}
int main()
{
    vector<Task> todoList;

    int choice;
	
	//Create User credentials DB
	/*sqlite3* db;
	
	int rc = sqlite3_open("user_credentials.db", &db);
	if (rc != SQLITE_OK){
		cerr<"Cannot open the database: "<<sqlite3_errmsg(db)<<endl;
		return rc;
	}
	
	else{
	const char* insertDataQuery = "INSERT INTO UserCredentials (Username, Password) VALUES (?,?);";
	
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, insertDataQuery, -1, &stmt, 0);
	
	sqlite3_bind_text(stmt, 1, "use1", -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, "password1", -1, SQLITE_STATIC);
	
	rc = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);
	const char* createTableQuery = "CREATE TABLE IF NOT EXISTS UserCredentials (ID INTEGER PRIMARY KEY AUTOINCREMENT, Username TEXT NOT NULL, Password TEXT NOT NULL;";
	
	}*/
	
    start(choice, todoList);

    return 0;
}
