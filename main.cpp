
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <regex>
#include <algorithm>
#include <sstream>
#include "Author.h"
#include "Publication.h"

using namespace std;


inline std::string ltrim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : s.substr(start);
}

inline std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(" \t\n\r");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

inline std::string trim(const std::string& s) {
    return rtrim(ltrim(s));
}


string normalizeName(const string& name) {
    string normalized = trim(name);
    transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    return normalized;
}


vector<Author> parseAuthors(const string& authorStr) {
    vector<Author> authors;
    stringstream ss(authorStr);
    string author;
    while (getline(ss, author, ',')) {
        authors.emplace_back(trim(author), "");
    }
    return authors;
}


vector<Publication> parseBibFile(const string& bibFilePath) {
    ifstream file(bibFilePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open bib file.\n";
        exit(1);
    }

    vector<Publication> publications;
    string line, currentEntry;
    regex fieldRegex(R"((\w+)\s*=\s*\{(.*?)\})");

    while (getline(file, line)) {
        currentEntry += line + "\n";
        if (line.find('}') != string::npos && currentEntry.find('@') != string::npos) { // End of entry
            string title, venue, doi;
            vector<Author> authors;
            int year = 0;

            auto fieldsBegin = sregex_iterator(currentEntry.begin(), currentEntry.end(), fieldRegex);
            auto fieldsEnd = sregex_iterator();
            for (auto it = fieldsBegin; it != fieldsEnd; ++it) {
                string key = (*it)[1];
                string value = (*it)[2];
                if (key == "title") title = value;
                else if (key == "venue" || key == "journal") venue = value;
                else if (key == "year") year = stoi(value);
                else if (key == "author") authors = parseAuthors(value);
                else if (key == "doi") doi = value;
            }

            if (!title.empty() && !authors.empty() && year > 0) {
                Publication pub(title, venue, year, doi);
                for (const auto& author : authors) {
                    pub.addAuthor(author);
                }
                publications.push_back(pub);
            }
            currentEntry.clear();
        }
    }
    return publications;
}


void displayPublication(const Publication& pub) {
    cout << "Title: " << pub.title << "\n";
    cout << "Venue: " << pub.venue << "\n";
    cout << "Year: " << pub.year << "\n";
    cout << "Authors: ";
    for (const auto& author : pub.authors) {
        cout << author.name << " ";
    }
    cout << "\n";
    cout << "DOI: " << pub.doi << "\n\n";
}


double calculateAverageCoAuthors(const vector<const Publication*>& authorPublications) {
    int totalCoAuthors = 0;
    for (const auto* pub : authorPublications) {
        totalCoAuthors += pub->authors.size() - 1; 
    }
    return authorPublications.empty() ? 0.0 : static_cast<double>(totalCoAuthors) / authorPublications.size();
}


int main() {
    
    string bibFilePath = "C:/Users/KIIT/Downloads/IIITD/MT24068_OOPD ASSIGNMENT 4/publist.bib";
    vector<Publication> publications = parseBibFile(bibFilePath);

    
    cout << "Enter author name(s) (comma-separated if more than one): "; //take input Bhattacharya
    string inputAuthorsStr;
    getline(cin, inputAuthorsStr);

    
    vector<string> inputAuthors;
    stringstream ss(inputAuthorsStr);
    string author;
    while (getline(ss, author, ',')) {
        inputAuthors.push_back(normalizeName(author));
    }

    
    for (const auto& inputAuthor : inputAuthors) {
        cout << "\nPublications by " << inputAuthor << ":\n";
        vector<const Publication*> authorPublications;

        for (const auto& pub : publications) {
            for (const auto& author : pub.authors) {
                if (normalizeName(author.name) == inputAuthor) {
                    authorPublications.push_back(&pub);
                    break;
                }
            }
        }

        if (authorPublications.empty()) {
            cout << "No publications found for author: " << inputAuthor << "\n";
        } else {
            for (const auto* pub : authorPublications) {
                displayPublication(*pub);
            }

            double avgCoAuthors = calculateAverageCoAuthors(authorPublications);
            cout << "Average number of co-authors per paper for " << inputAuthor << ": "
                 << avgCoAuthors << "\n";
        }
    }

    return 0;
}
