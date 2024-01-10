#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Task
{
    string description;
    bool completed;
};

void showTodoList(const vector<Task> &todoList)
{
}
void addTodoItem(vector<Task> &todoList)
{
}
void completeTodoItem(vector<Task> &todoList)
{
}
void deleteTodoItem(vector<Task> &todoList)
{
}
void clearTodoList(vector<Task> &todoList)
{
}
int main()
{
    vector<Task> todoList;

    int choice;

    do
    {

        cout << "\n\t\tToDo List Management\n";
        cout << "\t     ---------------------------\n";
        cout << "1. Show Todo List\n";
        cout << "2. Add Todo Item\n";
        cout << "3. Complete Todo Item\n";
        cout << "4. Delete Todo Item\n";
        cout << "5. Clear Todo List\n";
        cout << "6. Exit\n";
        cout << "7. Enter your choice: \n";
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

    return 0;
}
