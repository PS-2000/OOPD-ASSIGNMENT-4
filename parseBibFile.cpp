
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include "Author.h"
#include "Publication.h"

using namespace std;

// Function to trim leading and trailing spaces
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if (start == string::npos || end == string::npos) return "";
    return str.substr(start, end - start + 1);
}

// Convert string to lowercase
string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Normalize name by trimming, converting to lowercase, and reformatting (first_last_name format)
string normalizeName(const string& name) {
    string normalized = toLower(trim(name));
    size_t commaPos = normalized.find(',');
    if (commaPos != string::npos) {
        // If it's in "Last Name, First Name" format, reverse it to "first last" format
        string firstName = trim(normalized.substr(commaPos + 1));
        string lastName = trim(normalized.substr(0, commaPos));
        normalized = firstName + " " + lastName;
    }
    return normalized;
}

// Split author names by "and" and normalize each name
vector<Author> splitAuthors(const string& authorStr) {
    vector<Author> authors;
    size_t start = 0;
    size_t end = authorStr.find("and");
    while (end != string::npos) {
        authors.emplace_back(normalizeName(trim(authorStr.substr(start, end - start))), "");
        start = end + 3; // Skip past "and"
        end = authorStr.find("and", start);
    }
    authors.emplace_back(normalizeName(trim(authorStr.substr(start))), "");
    return authors;
}

// Function to load authors from the CSV file
set<string> loadAuthorsFromCSV(const string& filename) {
    set<string> authors;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open CSV file.\n";
        exit(1);
    }

    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        authors.insert(normalizeName(line)); // Normalize and add to set
    }
    return authors;
}

// Function to display publication details
void displayPublication(const Publication& pub) {
    cout << "Title: " << pub.title << "\n";
    cout << "Venue: " << pub.venue << "\n";
    cout << "Year: " << pub.year << "\n";
    cout << "Authors: ";
    for (const auto& author : pub.authors) {
        cout << author.name << " ";
    }
    cout << "\n";
    cout << "DOI: " << pub.doi << "\n";
}

// Function to display invalid entries
void displayInvalidEntries(const vector<string>& invalidEntries) {
    cout << "\nInvalid Entries:\n";
    for (const auto& entry : invalidEntries) {
        cout << entry << "\n";
    }
}

// Main function
int main() {
    // Hardcoded file paths
    string bibFile = "C:/Users/KIIT/Downloads/IIITD/MT24068_OOPD ASSIGNMENT 4/publist.bib";
    string csvFile = "C:/Users/KIIT/Downloads/IIITD/MT24068_OOPD ASSIGNMENT 4/faculty_names.csv";

    // Open .bib file
    ifstream file(bibFile);
    if (!file.is_open()) {
        cerr << "Error: Could not open bib file.\n";
        return 1;
    }

    // Load authors from the CSV file
    set<string> validAuthors = loadAuthorsFromCSV(csvFile);

    vector<Publication> publications;
    vector<string> invalidEntries;
    string line, currentEntry;
    regex fieldRegex(R"((\w+)\s*=\s*\{(.*?)\})");

    // Read .bib file line by line
    while (getline(file, line)) {
        currentEntry += line + "\n";
        if (line.find('}') != string::npos && currentEntry.find('@') != string::npos) { // End of entry
            string title, venue, doi;
            vector<Author> authors;
            int year = 0;

            // Extract fields using regex
            auto fieldsBegin = sregex_iterator(currentEntry.begin(), currentEntry.end(), fieldRegex);
            auto fieldsEnd = sregex_iterator();
            for (auto it = fieldsBegin; it != fieldsEnd; ++it) {
                string key = (*it)[1];
                string value = (*it)[2];
                if (key == "title") title = value;
                else if (key == "venue" || key == "journal") venue = value;
                else if (key == "year") year = stoi(value);
                else if (key == "author") authors = splitAuthors(value); // Split author string and normalize
                else if (key == "doi") doi = value;
            }

            // Validate entry
            if (authors.empty() || title.empty() || year == 0) {
                invalidEntries.push_back(currentEntry); // Invalid entry found
            } else {
                Publication pub(title, venue, year, doi);
                for (const auto& author : authors) {
                    pub.addAuthor(author);
                }
                publications.push_back(pub);
            }
            currentEntry.clear(); // Reset entry for next publication
        }
    }

    // Prompt user for filter option
    int option;
    cout << "Search publications by:\n";
    cout << "1. Author\n";
    cout << "2. Year\n";
    cout << "3. Venue\n";
    cout << "Enter option (1-3): ";
    cin >> option;
    cin.ignore();  // To ignore the newline after entering the option

    if (option == 1) {
        // Search by Author
        cout << "Enter author name to filter publications: ";
        string authorQuery;
        getline(cin, authorQuery);

        // Normalize the user input
        authorQuery = normalizeName(authorQuery);

        // Display matching publications
        cout << "\nPublications by " << authorQuery << ":\n";
        bool found = false;
        for (const auto& pub : publications) {
            for (const auto& author : pub.authors) {
                if (author.name == authorQuery) { // Check for exact match of normalized author
                    displayPublication(pub);
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "No publications found for the author: " << authorQuery << "\n";
        }
    } else if (option == 2) {
        // Search by Year
        int yearQuery;
        cout << "Enter year to filter publications: ";
        cin >> yearQuery;

        bool found = false;
        for (const auto& pub : publications) {
            if (pub.year == yearQuery) {
                displayPublication(pub);
                found = true;
            }
        }

        if (!found) {
            cout << "No publications found for the year: " << yearQuery << "\n";
        }
    } else if (option == 3) {
        // Search by Venue
        cout << "Enter venue to filter publications: ";
        string venueQuery;
        getline(cin, venueQuery);

        // Normalize the venue input
        venueQuery = toLower(trim(venueQuery));

        bool found = false;
        for (const auto& pub : publications) {
            if (toLower(pub.venue) == venueQuery) {
                displayPublication(pub);
                found = true;
            }
        }

        if (!found) {
            cout << "No publications found for the venue: " << venueQuery << "\n";
        }
    } else {
        cout << "Invalid option selected.\n";
    }

    // Display invalid entries
    displayInvalidEntries(invalidEntries);

    return 0;
}
