#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <string>
#include <vector>
#include "Author.h"

class Publication {
public:
    std::string title;
    std::string venue;
    int year;
    std::string doi;
    std::vector<Author> authors;

    Publication(const std::string& title, const std::string& venue, int year, const std::string& doi)
        : title(title), venue(venue), year(year), doi(doi) {}

    void addAuthor(const Author& author) {
        authors.push_back(author);
    }
};

#endif // PUBLICATION_H