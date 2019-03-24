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

enum CalculationType {
    MEDIAN, AVERAGE
};

const int MAX_RAND_HOMEWORK_GRADES = 20;
const int MIN_RAND_HOMEWORK_GRADES = 10;

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

struct Student {
    string name;
    string surname;
    vector<int> homeworkGrades;
    int exam = 0;

    void InputData() {
        cout << "Iveskite varda ir pavarde: ";
        cin >> name >> surname;
        cout << endl;

        cout << "Ar sugeneruoti pazymius ir egzamino rezultata atsitiktinai (Y/N)? ";
        char c;
        cin >> c;

        while (!((c == 'y' || c == 'Y' || c == 'n' || c == 'N') && cin.good())) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << "Tokio pasirinkimo nera, iveskite teisinga pasirinkima: ";
            cin >> c;
        }

        if (toupper(c) == 'Y') {
            GenerateGrades();
        } else if (toupper(c) == 'N') {
            InputGrades();
            cout << "Koks egzamino rezultatas? ";
            cin >> exam;
        }
    }

    void GenerateGrades() {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);  // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> gradesCountRand(MIN_RAND_HOMEWORK_GRADES,
                                                        MAX_RAND_HOMEWORK_GRADES);
        std::uniform_int_distribution<> gradesRand(1, 10);

        int gradesCount = gradesCountRand(gen);

        for (int i = 0; i < gradesCount; i++) {
            homeworkGrades.push_back(gradesRand(gen));
        }

        exam = gradesRand(gen);
    }

    void InputGrades() {
        cout << "Iveskite pazymius (kad baigti ivedima, iveskite ne skaiciu)."
             << endl;

        int grade;

        while (cin >> grade) {
            if (!(grade >= 1 && grade <= 10)) {
                cout << "Pazymys turi buti nuo 1 iki 10, veskite toliau. " << endl;
                continue;
            }
            homeworkGrades.push_back(grade);
        }

        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    double GetAverageOfHomeworkGrades() const {
        if (homeworkGrades.empty())
            throw std::runtime_error(
                    "Mokinys " + name + " " + surname + "  neturi pazymiu, tad negalime isvesti vidurkio");

        int sum = 0;

        for (auto &n : homeworkGrades) {
            sum += n;
        }

        return (double) sum / homeworkGrades.size();
    }

    double GetMedianOfHomeworkGrades() const {
        int gradesCount = homeworkGrades.size();

        if (gradesCount == 0)
            throw std::runtime_error(
                    "Mokinys " + name + " " + surname + "  neturi pazymiu, tad negalime isvesti vidurkio");

        vector<int> sortedGrades(homeworkGrades);

        sort(sortedGrades.begin(), sortedGrades.end());

        if (gradesCount % 2 == 0) {
            return (double) (sortedGrades[gradesCount / 2] +
                             sortedGrades[gradesCount / 2 - 1]) / 2;
        } else {
            return sortedGrades[gradesCount / 2];
        }
    }

    double GetFinalGrade(CalculationType calcType) const {
        switch (calcType) {
            case AVERAGE:
                return 0.4 * GetAverageOfHomeworkGrades() + 0.6 * exam;
            case MEDIAN:
                return 0.4 * GetMedianOfHomeworkGrades() + 0.6 * exam;
        }
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
