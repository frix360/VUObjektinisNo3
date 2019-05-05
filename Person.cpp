#include <utility>

//
// Created by Ignas on 5/5/2019.
//

#include "Person.h"

Person::Person(string name, string surname) {
    this->name = std::move(name);
    this->surname = std::move(surname);
}

std::string Person::getName() const {
    return this->name;
}

std::string Person::getSurname() const {
    return this->surname;
}

Person::Person() = default;

Person::~Person() = default;
