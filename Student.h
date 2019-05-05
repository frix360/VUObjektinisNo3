
#ifndef VU_OBJ_NO_2_STUDENT_H
#define VU_OBJ_NO_2_STUDENT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <sstream>
#include <ctime>
#include <exception>
#include <cstdlib>
#include <chrono>
#include <list>
#include <deque>
#include "Person.h"


using std::string;
using std::vector;
using std::deque;
using std::list;
using std::cout;
using std::cin;
using std::endl;
using std::left;
using std::setw;
using std::ofstream;
using std::istream;
using std::ostream;

enum CalculationType {
    MEDIAN, AVERAGE
};

class Student : public Person {
private:
    vector<int> homeworkGrades;
    int exam = 0;
public:
    void InputData();

    void GenerateGrades();

    void InputGrades();

    double GetAverageOfHomeworkGrades() const;

    double GetMedianOfHomeworkGrades() const;

    double GetFinalGrade(CalculationType calcType) const;

    int getExam() const { return exam; };

    vector<int> getHomeworkGrades() const { return homeworkGrades; };

    Student(string name, string surname, vector<int> grades, int exam);

    Student();

    ~Student();

    Student &operator=(const Student &s);

    friend ostream &operator<<(ostream &os, const Student &student);
};

#endif //VU_OBJ_NO_2_STUDENT_H
