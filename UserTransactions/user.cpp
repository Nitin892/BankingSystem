// Include necessary header files
#include <string>
#include <fstream>
#include <iostream>

// Use the standard namespace
using namespace std;

// Declare a global variable to keep track of the logged-in user
string loggedIN;

// Define a struct to store the details of the transactions received by a user
struct Received
{
    string name;
    double amount;
    Received *next;
    Received(string name, double amount)
    {
        this->name = name;
        this->amount = amount;
        this->next = nullptr;
    }
};

// Define a struct to store the details of the transactions sent by a user
struct Sent
{
    string name;
    double amount;
    Sent *next;
    Sent(string name, double amount)
    {
        this->name = name;
        this->amount = amount;
        next = nullptr;
    }
};

// Define a struct to store the details of a user
struct User
{
    string name;
    Sent *sent;         // Head pointer for the linked list of sent transactions
    Received *received; // Head pointer for the linked list of received transactions
    User *next;         // Pointer to the next user in the linked list

    // Constructors

    User() {}
    User(string name)
    {
        this->name = name;
        sent = nullptr;
        received = nullptr;
        next = nullptr;
    }
};

// This function checks if a user with the given name already exists in the "user.txt" file or not.
// The function takes the name of the user as input and returns a boolean value.
int isUserExist(const string &name)
{
    // Open the "user.txt" file in read mode.
    ifstream file("user.txt");

    // Search for the given name in the file by reading each line.
    string line;
    int lineNum = 1;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            string dup = line;
            if (line.find(name) != std::string::npos)
            {
                string storedname = dup.substr(0,line.find('-'));
                if (storedname == name)
                {
                    file.close();
                    return true;
                }
            }
        }
    }

    // The user does not exist in the file. Return 0.
    file.close();
    return 0;
}

bool SignUp(const string &name, const string &password)
{
    // Check if user already exists
    if (isUserExist(name))
        return false;

    // Open file in append mode to add new user
    ofstream file;
    file.open("user.txt", ios::app);

    // Check if file opened successfully
    if (!file.is_open())
    {
        file.close();
        return false;
    }

    // Write the new user's name and password to the file
    file << name << "-" << password << endl;

    // Close the file and return true to indicate success
    file.close();
    return true;
}

bool login(const string &name, const string &password)
{

    // Opening file
    fstream file("user.txt");

    // Searching for name and password combination
    string line;
    while (getline(file, line))
    {

        // Splitting the line into name and password
        int pos = line.find("-");
        string storedName = line.substr(0, pos);
        string storedPassword = line.substr(pos + 1);

        // Checking if name and password match
        if (name == storedName && password == storedPassword)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool newUser(string name, string password)
{
    // Call the SignUp function to create a new user with the given name and password
    if (SignUp(name, password))
        return true;

    // If SignUp returns false, the user creation failed, so return false
    return false;
}

// Check if the provided credentials are valid
bool credentials(string name, string password)
{

    // Call the login() function to check if the name and password match
    if (login(name, password))
        return true;

    // If the name and password do not match, return false
    return false;
}
