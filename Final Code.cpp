#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

using namespace std;

class Task {
public:
    string description;
    bool completed;
    int priority; // 1: High, 2: Medium, 3: Low
    string dueDate; // Format: YYYY-MM-DD

    Task(const string &desc, int prio, const string &due) 
        : description(desc), completed(false), priority(prio), dueDate(due) {}

    string toString() const {
        stringstream ss;
        ss << setw(30) << left << description 
           << setw(10) << (completed ? "Yes" : "No") 
           << setw(10) << getPriorityString() 
           << setw(15) << dueDate;
        return ss.str();
    }

private:
    string getPriorityString() const {
        switch (priority) {
            case 1: return "High";
            case 2: return "Medium";
            case 3: return "Low";
            default: return "Unknown";
        }
    }
};

class TaskManager {
private:
    vector<Task> tasks;

    void saveToFile() {
        ofstream file("tasks.txt");
        for (const auto &task : tasks) {
            file << task.description << "|"
                 << task.completed << "|"
                 << task.priority << "|"
                 << task.dueDate << "\n";
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("tasks.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string desc, completedStr, prioStr, dueDate;
            getline(ss, desc, '|');
            getline(ss, completedStr, '|');
            getline(ss, prioStr, '|');
            getline(ss, dueDate, '|');

            Task task(desc, stoi(prioStr), dueDate);
            task.completed = (completedStr == "1");
            tasks.push_back(task);
        }
        file.close();
    }

public:
    TaskManager() {
        loadFromFile();
    }

    ~TaskManager() {
        saveToFile();
    }

    void addTask(const string &taskDescription, int priority, const string &dueDate) {
        tasks.emplace_back(taskDescription, priority, dueDate);
        cout << "Task added: " << taskDescription << endl;
    }

    void completeTask(int taskIndex) {
        if (taskIndex >= 0 && taskIndex < tasks.size()) {
            tasks[taskIndex].completed = true;
            cout << "Task completed: " << tasks[taskIndex].description << endl;
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void completeTask(const string &taskDescription) {
        auto it = find_if(tasks.begin(), tasks.end(), [&](const Task &task) {
            return task.description == taskDescription;
        });
        if (it != tasks.end()) {
            it->completed = true;
            cout << "Task completed: " << it->description << endl;
        } else {
            cout << "Task not found." << endl;
        }
    }

    void listTasks() const {
        cout << setw(5) << "No." 
             << setw(30) << "Description" 
             << setw(10) << "Completed" 
             << setw(10) << "Priority" 
             << setw(15) << "Due Date" << endl;
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << setw(5) << i + 1 
                 << tasks[i].toString() << endl;
        }
    }

    void searchTasks(const string &keyword) const {
        cout << setw(5) << "No." 
             << setw(30) << "Description" 
             << setw(10) << "Completed" 
             << setw(10) << "Priority" 
             << setw(15) << "Due Date" << endl;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (tasks[i].description.find(keyword) != string::npos) {
                cout << setw(5) << i + 1 
                     << tasks[i].toString() << endl;
            }
        }
    }

    void sortTasksByDueDate() {
        sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
            return a.dueDate < b.dueDate;
        });
        cout << "Tasks sorted by due date." << endl;
    }
};

void displayMenu() {
    cout << "\n1. Add Task\n"
         << "2. Complete Task by Number\n"
         << "3. Complete Task by Name\n"
         << "4. List Tasks\n"
         << "5. Search Tasks\n"
         << "6. Sort Tasks by Due Date\n"
         << "7. Exit\n"
         << "Choice: ";
}

int main() {
    TaskManager taskManager;
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        if (choice == 7) break;

        switch (choice) {
            case 1: {
                cin.ignore();
                string taskDescription;
                int priority;
                string dueDate;
                cout << "Enter task description: ";
                getline(cin, taskDescription);
                cout << "Enter priority (1: High, 2: Medium, 3: Low): ";
                cin >> priority;
                cin.ignore();
                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDate);
                taskManager.addTask(taskDescription, priority, dueDate);
                break;
            }
            case 2: {
                int taskIndex;
                cout << "Enter task number to complete: ";
                cin >> taskIndex;
                taskManager.completeTask(taskIndex - 1);
                break;
            }
            case 3: {
                cin.ignore();
                string taskDescription;
                cout << "Enter task name to complete: ";
                getline(cin, taskDescription);
                taskManager.completeTask(taskDescription);
                break;
            }
            case 4:
                taskManager.listTasks();
                break;
            case 5: {
                cin.ignore();
                string keyword;
                cout << "Enter keyword to search: ";
                getline(cin, keyword);
                taskManager.searchTasks(keyword);
                break;
            }
            case 6:
                taskManager.sortTasksByDueDate();
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }

    cout << "Exiting. Goodbye!" << endl;
    return 0;
}
