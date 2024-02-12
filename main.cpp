#include <bits/stdc++.h>
using namespace std;

struct Contact
{
    string firstName;
    string lastName;
    string phoneNumber;
};

class ContactManager
{
private:
    vector<Contact> contacts;
    const string filename = "contacts.csv";

public:
    ContactManager()
    {
        loadContacts();
    }

    // Load contacts from CSV file
    void loadContacts()
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error opening file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string firstName, lastName, phoneNumber;
            getline(ss, firstName, ',');
            getline(ss, lastName, ',');
            getline(ss, phoneNumber, ',');
            contacts.push_back({firstName, lastName, phoneNumber});
        }
        file.close();
    }

    // Save contacts to CSV file
    void saveContacts()
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error opening file " << filename << endl;
            return;
        }

        for (const auto &contact : contacts)
        {
            file << contact.firstName << "," << contact.lastName << "," << contact.phoneNumber << "\n";
        }
        file.close();
    }

    // Add a new contact
    void addContact(const string &firstName, const string &lastName, const string &phoneNumber)
    {
        if (validatePhoneNumber(phoneNumber))
        {
            if (isPhoneNumberUnique(phoneNumber))
            {
                contacts.push_back({firstName, lastName, phoneNumber});
                saveContacts();
                cout << "Contact added successfully." << endl;
            }
            else
            {
                cerr << "Phone number already exists for another contact." << endl;
            }
        }
        else
        {
            cerr << "Invalid phone number format. Phone number must contain exact 10 digits." << endl;
        }
    }

    // Search contacts by first name
    vector<Contact> searchByFirstName(const string &firstName)
    {
        return searchHelper(firstName, true, false, false);
    }

    // Search contacts by first name prefix
    vector<Contact> searchByFirstNamePrefix(const string &prefix)
    {
        return searchHelper(prefix, false, true, false);
    }

    // Search contacts by last name
    vector<Contact> searchByLastName(const string &lastName)
    {
        return searchHelper(lastName, false, false, true);
    }

    // Search contacts by last name prefix
    vector<Contact> searchByLastNamePrefix(const string &prefix)
    {
        return searchHelper(prefix, false, true, false);
    }

    // Search contacts by phone number
    vector<Contact> searchByPhoneNumber(const string &phoneNumber)
    {
        return searchHelper(phoneNumber, false, false, false);
    }

    // Search contacts by phone number prefix
    vector<Contact> searchByPhoneNumberPrefix(const string &prefix)
    {
        return searchHelper(prefix, false, true, false);
    }

    // Delete a contact
    void deleteContact(const string &firstName, const string &lastName)
    {
        vector<Contact> matchingContacts;
        for (const auto &contact : contacts)
        {
            if (contact.firstName == firstName && contact.lastName == lastName)
            {
                matchingContacts.push_back(contact);
            }
        }

        if (!matchingContacts.empty())
        {
            if (matchingContacts.size() > 1)
            {
                cout << "Multiple contacts found with the same first name and last name:" << endl;
                displayContacts(matchingContacts);
                int choice = 0;
                cout << "Enter the number of the contact to delete: ";
                cin >> choice;
                if (choice > 0 && choice <= static_cast<int>(matchingContacts.size()))
                {
                    contacts.erase(remove_if(contacts.begin(), contacts.end(),
                                             [&](const Contact &c)
                                             { return c.firstName == firstName && c.lastName == lastName && --choice == 0; }),
                                   contacts.end());
                    saveContacts();
                    cout << "Contact deleted successfully." << endl;
                }
                else
                {
                    cerr << "Invalid choice." << endl;
                }
            }
            else
            {
                contacts.erase(remove_if(contacts.begin(), contacts.end(),
                                         [&](const Contact &c)
                                         { return c.firstName == firstName && c.lastName == lastName; }),
                               contacts.end());
                saveContacts();
                cout << "Contact deleted successfully." << endl;
            }
        }
        else
        {
            cerr << "Contact not found." << endl;
        }
    }

    // Update a contact
    void updateContact(const string &firstName, const string &lastName, const string &newPhoneNumber)
    {
        vector<Contact> matchingContacts;
        for (const auto &contact : contacts)
        {
            if (contact.firstName == firstName && contact.lastName == lastName)
            {
                matchingContacts.push_back(contact);
            }
        }

        if (!matchingContacts.empty())
        {
            if (matchingContacts.size() > 1)
            {
                cout << "Multiple contacts found with the same first name and last name:" << endl;
                displayContacts(matchingContacts);
                int choice = 0;
                cout << "Enter the number of the contact to update: ";
                cin >> choice;
                if (choice > 0 && choice <= static_cast<int>(matchingContacts.size()))
                {
                    auto it = find_if(contacts.begin(), contacts.end(),
                                      [&](const Contact &c)
                                      { return c.firstName == firstName && c.lastName == lastName && --choice == 0; });
                    if (it != contacts.end())
                    {
                        it->phoneNumber = newPhoneNumber;
                        saveContacts();
                        cout << "Contact updated successfully." << endl;
                    }
                    else
                    {
                        cerr << "Contact not found." << endl;
                    }
                }
                else
                {
                    cerr << "Invalid choice." << endl;
                }
            }
            else
            {
                auto it = find_if(contacts.begin(), contacts.end(),
                                  [&](const Contact &c)
                                  { return c.firstName == firstName && c.lastName == lastName; });
                if (it != contacts.end())
                    it->phoneNumber = newPhoneNumber;
                saveContacts();
                cout << "Contact updated successfully." << endl;
            }
        }
        else
        {
            cerr << "Contact not found." << endl;
        }
    }

    // Display all contacts
    void displayContacts()
    {
        cout << "All Contacts:" << endl;
        for (const auto &contact : contacts)
        {
            cout << contact.firstName << " " << contact.lastName << " (" << contact.phoneNumber << ")" << endl;
        }
    }

private:
    // Helper function for search
    vector<Contact> searchHelper(const string &key, bool firstNameSearch, bool prefixSearch, bool lastNameSearch)
    {
        vector<Contact> result;
        for (const auto &contact : contacts)
        {
            if ((firstNameSearch && contact.firstName == key) ||
                (prefixSearch && contact.firstName.find(key) == 0) ||
                (lastNameSearch && contact.lastName == key) ||
                (prefixSearch && contact.lastName.find(key) == 0) ||
                (!prefixSearch && !firstNameSearch && !lastNameSearch && contact.phoneNumber == key) ||
                (prefixSearch && contact.phoneNumber.find(key) == 0))
            {
                result.push_back(contact);
            }
        }
        return result;
    }

    // Validate phone number format
    bool validatePhoneNumber(const string &phoneNumber)
    {
        return phoneNumber.length() == 10 && all_of(phoneNumber.begin(), phoneNumber.end(), ::isdigit);
    }

    // Check if phone number is unique
    bool isPhoneNumberUnique(const string &phoneNumber)
    {
        return find_if(contacts.begin(), contacts.end(), [&](const Contact &c)
                       { return c.phoneNumber == phoneNumber; }) == contacts.end();
    }

    // Function to display matching contacts
    void displayContacts(const vector<Contact> &matchingContacts)
    {
        for (size_t i = 0; i < matchingContacts.size(); ++i)
        {
            cout << i + 1 << ". " << matchingContacts[i].firstName << " " << matchingContacts[i].lastName << " (" << matchingContacts[i].phoneNumber << ")" << endl;
        }
    }
};

// Function to display search results
void displaySearchResult(const vector<Contact> &searchResult)
{
    if (!searchResult.empty())
    {
        cout << "Search result:" << endl;
        for (size_t i = 0; i < searchResult.size(); ++i)
        {
            cout << i + 1 << ". " << searchResult[i].firstName << " " << searchResult[i].lastName << " (" << searchResult[i].phoneNumber << ")" << endl;
        }
    }
    else
    {
        cout << "Contact not found." << endl;
    }
}

// Search sub-menu
void searchMenu(ContactManager &contactManager)
{
    int searchChoice;
    string searchKey;

    cout << "\nSearch By:\n"
         << "1. First Name\n"
         << "2. First Name Prefix\n"
         << "3. Last Name\n"
         << "4. Last Name Prefix\n"
         << "5. Phone Number\n"
         << "6. Phone Number Prefix\n"
         << "7. Back to Main Menu\n\n"
         << "Enter your choice: ";
    cin >> searchChoice;

    switch (searchChoice)
    {
    case 1:
        cout << "Enter first name to search: ";
        cin >> searchKey;
        displaySearchResult(contactManager.searchByFirstName(searchKey));
        break;

    case 2:
        cout << "Enter first name prefix to search: ";
        cin >> searchKey;
        displaySearchResult(contactManager.searchByFirstNamePrefix(searchKey));
        break;

    case 3:
        cout << "Enter last name to search: ";
        cin >> searchKey;
        displaySearchResult(contactManager.searchByLastName(searchKey));
        break;

    case 4:
        cout << "Enter last name prefix to search: ";
        cin >> searchKey;
        displaySearchResult(contactManager.searchByLastNamePrefix(searchKey));
        break;

    case 5:
        cout << "Enter phone number to search: ";
        cin >> searchKey;
        displaySearchResult(contactManager.searchByPhoneNumber(searchKey));
        break;

    case 6:
        cout << "Enter phone number prefix to search: ";
        cin >> searchKey;
        displaySearchResult(contactManager.searchByPhoneNumberPrefix(searchKey));
        break;

    case 7:
        break;

    default:
        cout << "Invalid choice!" << endl;
        break;
    }
}

int main()
{
    ContactManager contactManager;
    int choice;
    string searchKey;

    do
    {
        cout << "\n1. Add Contact\n"
             << "2. Search Contacts\n"
             << "3. Update Contact\n"
             << "4. Delete Contact\n"
             << "5. Display All Contacts\n"
             << "6. Exit\n\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string firstName, lastName, phoneNumber;
            cout << "Enter first name: ";
            cin >> firstName;
            cout << "Enter last name: ";
            cin >> lastName;
            cout << "Enter phone number: ";
            cin >> phoneNumber;
            contactManager.addContact(firstName, lastName, phoneNumber);
        }
        break;

        case 2:
            searchMenu(contactManager);
            break;

        case 3:
        {
            string firstName, lastName, newPhoneNumber;
            cout << "Enter first name of contact to update: ";
            cin >> firstName;
            cout << "Enter last name of contact to update: ";
            cin >> lastName;
            cout << "Enter new phone number: ";
            cin >> newPhoneNumber;
            contactManager.updateContact(firstName, lastName, newPhoneNumber);
        }
        break;

        case 4:
        {
            string firstName, lastName;
            cout << "Enter first name of contact to delete: ";
            cin >> firstName;
            cout << "Enter last name of contact to delete: ";
            cin >> lastName;
            contactManager.deleteContact(firstName, lastName);
        }
        break;

        case 5:
            contactManager.displayContacts();
            break;

        case 6:
            cout << "Exiting...";
            break;

        default:
            cout << "Invalid choice!";
            break;
        }

    } while (choice != 6);

    return 0;
}
