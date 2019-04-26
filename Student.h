
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

enum CalculationType {
    MEDIAN, AVERAGE
};

class Student {
private:
    string name;
    string surname;
    vector<int> homeworkGrades;
    int exam = 0;
public:
    void InputData();

    void GenerateGrades();

    void InputGrades();

    double GetAverageOfHomeworkGrades() const;

    double GetMedianOfHomeworkGrades() const;

    double GetFinalGrade(CalculationType calcType) const;

    std::string getName() const { return name; };

    std::string getSurname() const { return surname; };

    int getExam() const { return exam; };


    vector<int> getHomeworkGrades() const { return homeworkGrades; };

    Student(string name, string surname, vector<int> grades, int exam) {
        this->name = name;
        this->surname = surname;
        this->homeworkGrades = grades;
        this->exam = exam;
    }

    Student() = default;
};

#endif //VU_OBJ_NO_2_STUDENT_H
