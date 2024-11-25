#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>

class Author {
public:
    std::string name;
    std::string affiliation;

    Author(const std::string& name, const std::string& affiliation)
        : name(name), affiliation(affiliation) {}
};

#endif // AUTHOR_H
