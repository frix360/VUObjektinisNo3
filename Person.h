#ifndef VU_OBJ_NO_2_PERSON_H
#define VU_OBJ_NO_2_PERSON_H

#include <iostream>

using std::string;

class Person {
protected:
    string name;
    string surname;
public:
    std::string getName() const;

    std::string getSurname() const;

    virtual ~Person() = 0;

    Person();

    Person(string, string);
};


#endif //VU_OBJ_NO_2_PERSON_H
