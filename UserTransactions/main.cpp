#include <iostream>
#include <vector>
#include <fstream>
#include "user.cpp"
#include <thread>
#include <chrono>
#include <typeinfo>
using namespace std;
string loggedIN_user = loggedIN;
// setting users in vector
void setUser(vector<User *> &user, const string &name)
{
    if (name.empty())
    {
        return; // Exit the function early if the name is empty
    }
    // Get the first character of the name and convert it to lowercase
    char firstchar = name.at(0);
    if (firstchar >= 65 && firstchar <= 90) // Check if the first character is uppercase
        firstchar += 32;                    // Convert it to lowercase by adding 32 to its ASCII value

    // Calculate the index in the vector based on the first character of the name
    int index = firstchar - 97; // 'a' has ASCII value 97, so subtracting it gives the index of 'a' in the vector

    // Create a new User object with the given name
    User *newUser = new User(name);

    // Check if there is no User object at the calculated index
    if (user.at(index) == nullptr)
    {
        // Insert the new User object at the calculated index
        user[index] = newUser;
    }
    else
    {
        // Insert the new User object at the front of the linked list at the calculated index
        User *temp = user.at(index); // Get the first User object in the linked list
        newUser->next = temp;        // Set the next pointer of the new User object to point to the first User object
        user[index] = newUser;       // Set the vector element at the calculated index to point to the new User object
    }
}

// This function loads User objects from a file and adds them to the vector
// The file must have one name per line, in the format "name-password"
void load(vector<User *> &user)
{
    // Opening file
    fstream file("user.txt");

    // Searching for name.
    string line;
    while (getline(file, line))
    {

        // Splitting the line into name.
        int pos = line.find("-");
        string storedName = line.substr(0, pos);

        setUser(user, storedName); // Add the name to the vector
    }
    file.close();
}

void options()
{
    cout << "Select Options " << endl;
    cout << "1) Transactions " << endl;
    cout << "2) Send Money " << endl;
    cout << "3) Received Money " << endl;
}

User *searchUser(vector<User *> &user, const string &name)
{
    // Find the index of the first letter of the name in the alphabet
    // and use it as the index for the vector of users.
    int index = name.at(0) - 97;

    // If there are no users at this index, the name cannot be found,
    // so return a null pointer.
    if (user[index] == nullptr)
        return nullptr;

    // If there are users at this index, start searching through them
    // until the user with the given name is found or the end of the
    // list is reached.
    User *userFound = user.at(index);
    while (userFound)
    {
        if (userFound->name == name)
            break;                   // The user has been found, so exit the loop.
        userFound = userFound->next; // Move on to the next user.
    }

    // Return the user with the given name, or a null pointer if the
    // name was not found in the vector.
    return userFound;
}

// This function displays the transaction history for the specified user.
void userTransactionHistory(vector<User *> &user, const string &name)
{
    // Find the user in the vector of users
    User *userFound = searchUser(user, name);

    // Display the list of transactions that the user has initiated (i.e., sent)
    Sent *sent = userFound->sent;
    if (sent == nullptr)
        cout << "Sent - You haven't sent any money" << endl
             << endl;
    else
    {
        cout << "Money you have sent--" << endl
             << endl;
        while (sent)
        {
            cout << "$ " << sent->amount << " has been debited from your account to " << sent->name << endl;
            sent = sent->next;
        }
    }

    // Display the list of transactions that the user has received (i.e., credited)
    Received *received = userFound->received;
    if (received == nullptr)
        cout << endl
             << endl
             << "Received - No money credited to our account " << endl;
    else
    {
        cout << "Money you have received--" << endl
             << endl;
        while (received)
        {
            cout << "$ " << received->amount << " has been credited to your account by " << received->name << endl;
            received = received->next;
        }
    }
}

bool sendMoney(vector<User *> &user, const string &name, const double &amount)
{
    // Check if the recipient exists
    if (!isUserExist(name))
        return false;

    // Check if the logged in user is not sending money to themselves
    if (loggedIN == name)
        return false;

    // Create a new object for the sent transaction
    Sent *newuser = new Sent(name, amount);

    // Find the logged in user
    User *searcheduser = searchUser(user, loggedIN);

    // Assign the sent transaction to the logged in user's sent list
    if (searcheduser->sent == nullptr)
        searcheduser->sent = newuser;
    else
    {
        Sent *temp = searcheduser->sent;

        newuser->next = temp;
        searcheduser->sent = newuser;
    }

    // Find the recipient user
    User *receiveduser = searchUser(user, name);

    // Create a new object for the received transaction
    Received *receiver = new Received(loggedIN, amount);

    // Assign the received transaction to the recipient's received list
    if (receiveduser->received == nullptr)
        receiveduser->received = receiver;
    else
    {
        Received *temp = receiveduser->received;
        receiver->next = temp;
        receiveduser->received = receiver;
    }
    // Return true if the transaction was successful
    return true;
}

void received(vector<User *> &user, string name)
{
    // searching user.
    User *userFound = searchUser(user, name);

    // get the linked list of received transactions for the user
    Received *received = userFound->received;

    // check if there are any transactions in the list
    if (received == nullptr)
        cout << "Received - No money credited to our account " << endl;
    else
    {
        // iterate through the list and print each transaction
        while (received)
        {
            cout << "$" << received->amount << " has been credited to your account by " << received->name << endl;
            received = received->next;
        }
    }
}

void displayMenu()
{
    cout << "                            Transaction History " << endl;
    cout << "select option" << endl
         << endl;
    cout << "1) login " << endl;
    cout << "2) SignUp " << endl;
    cout << "3) Exit " << endl;
}

// This function takes a reference to a vector of pointers to User objects.
void destroyNodes(vector<User *> &user)
{
    int size = user.size();
    // Iterate through each User in the vector.
    for (int i = 0; i < size; i++)
    {
        // If the User pointer is not null.
        if (user.at(i) != nullptr)
        {
            // Set startUser to the current User pointer.
            User *startUser = user[i];
            // While there are still Sent nodes attached to the User.
            while (startUser)
            {
                // Set startSent to the first Sent node attached to the User.
                Sent *startSent = startUser->sent;
                // While there are still Sent nodes attached to the User.
                while (startSent)
                {
                    // Set temp to the current Sent node.
                    Sent *temp = startSent;
                    // Move startSent to the next Sent node.
                    startSent = startSent->next;
                    // Delete the current Sent node.
                    delete temp;
                }

                // Set startReceived to the first Received node attached to the User.
                Received *startReceived = startUser->received;
                // While there are still Received nodes attached to the User.
                while (startReceived)
                {
                    // Set temp to the current Received node.
                    Received *temp = startReceived;
                    // Move startReceived to the next Received node.
                    startReceived = startReceived->next;
                    // Delete the current Received node.
                    delete temp;
                }

                // Set temp to the current User.
                User *temp = user[i];
                // Move the User pointer to the next User node.
                user[i] = user[i]->next;
                // Delete the current User.
                delete temp;
            }
        }
    }
}

// start of function
int main()
{
    vector<User *> user(26, nullptr);

    string name;
    load(user);
    while (1)
    {

        system("cls");
        string name;
        displayMenu();

        int choice;
        cin >> choice;
        if (choice == 1)
        {
            name;
            string password;
            cout << "Enter name:";
            cin >> name;
            cout << "Enter password:";
            cin >> password;
            bool value = credentials(name, password);
            if (value)
            {
                system("cls");
                cout << "                            transaction history" << endl;
                cout << "Welcome " << name;
                loggedIN = name;
            }
            else
            {
                cout << "Invalid credential" << endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }
        }
        else if (choice == 2)
        {
            string name;
            string password;
            string confirm_password;
            cout << "Enter name:";
            cin >> name;
            cout << "Enter password:";
            cin >> password;
            cout << "Re-enter password:";
            cin >> confirm_password;
            if (password == confirm_password)
            {

                if (newUser(name, password))
                {
                    cout << "Successfully created" << endl;
                    setUser(user, name);
                }
                else
                {
                    cout << "User name is already exist" << endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    continue;
                }
            }

            else
                cout << "password is not matching" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        else
            break;
        cout << endl
             << endl;
        while (1)
        {
            cout << "Select options:" << endl
                 << endl;
            cout << "1) View History " << endl;
            cout << "2) Sent Money" << endl;
            cout << "3) Received Money" << endl;
            cout << "4) Logout " << endl;

            cin >> choice;
            if (choice == 1)
            {
                userTransactionHistory(user, loggedIN);
                short int select;
                cout << endl
                     << endl;
                cout << "press 1 to get back " << endl;
                cin >> select;
                system("cls");
            }
            else if (choice == 2)
            {
                string send_to;
                double amount;
                cout << endl;
                cout << "Enter name:";
                cin >> send_to;
                cout << "Enter amount:";
                cin >> amount;
                if (sendMoney(user, send_to, amount))
                    cout << "Your transaction is successful of amount " << amount << " is sent to " << send_to << endl;
                else
                    cout << "There is no user named or you are trying to debit money from your account " << send_to << endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
                system("cls");
            }
            else if (choice == 3)
            {
                received(user, loggedIN);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                system("cls");
            }
            else
                break;
        }
    }
    destroyNodes(user);
    return 0;
}