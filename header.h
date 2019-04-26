#ifndef VU_OBJ_NO_2_HEADER_H
#define VU_OBJ_NO_2_HEADER_H

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
#include "Student.h"

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

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
    Timer() : start{std::chrono::high_resolution_clock::now()} {}

    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsed() const {
        return std::chrono::duration<double>
                (std::chrono::high_resolution_clock::now() - start).count();
    }
};



void SortCollection(vector<Student> students);

void SortCollection(std::deque<Student> students);

void SortCollection(std::list<Student> students);

template<typename Container>
size_t FindLongestStringLength(const Container &container);

template<typename Container>
void PrintStudentsData(std::ostream &stream, const Container &students, const std::vector<std::string> &headers);

template<typename Container>
void PrintStudentsData(std::ostream &stream, Container students);

void InputData(vector<Student> &students, vector<string> &headers);

template<typename Container>
void ReadFile(std::istream &stream, Container &students, vector<string> &headers);

template<typename Container>
void ReadFile(std::istream &stream, Container &students);

string GenerateFile(int studentsCount, int gradesCount);

void GenerateFilesAndTest(int gradesCount = 5, bool useDifferentStrategy = false, bool useResizeMethod = false);

template<typename Container>
void SeparateStudents(Container students, Container &smartless, Container &smartfull);

vector<Student> SeparateStudents(vector<Student> &students, bool optimized = true);

list<Student> SeparateStudents(list<Student> &students);

deque<Student> SeparateStudents(deque<Student> &students);

// PAPILDOMA UZD
vector<Student> SeparateStudentsWithResize(vector<Student> &students);

deque<Student> SeparateStudentsWithResize(deque<Student> &students);

template<typename Container>


void TestSpeedOfSeparation(int studentsToGenerate, int gradesCount = 5, bool useDifferentStrategy = false,
                           bool useResizeMethod = false);

#endif //VU_OBJ_NO_2_HEADER_H
