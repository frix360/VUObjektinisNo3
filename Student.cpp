#include <algorithm>
#include "Student.h"
#include "catch.hpp"
const int MAX_RAND_HOMEWORK_GRADES = 20;
const int MIN_RAND_HOMEWORK_GRADES = 10;

void Student::InputData() {
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

void Student::GenerateGrades() {
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

void Student::InputGrades() {
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

double Student::GetAverageOfHomeworkGrades() const {
    if (homeworkGrades.empty())
        throw std::runtime_error(
                "Mokinys " + name + " " + surname + "  neturi pazymiu, tad negalime isvesti vidurkio");

    int sum = 0;

    for (auto &n : homeworkGrades) {
        sum += n;
    }

    return (double) sum / homeworkGrades.size();
}

double Student::GetMedianOfHomeworkGrades() const {
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

double Student::GetFinalGrade(CalculationType calcType) const {
    switch (calcType) {
        case AVERAGE:
            return 0.4 * GetAverageOfHomeworkGrades() + 0.6 * exam;
        case MEDIAN:
            return 0.4 * GetMedianOfHomeworkGrades() + 0.6 * exam;
    }
}

Student::~Student() = default;

Student::Student() = default;

Student::Student(string name, string surname, vector<int> grades, int exam) : Person(name, surname) {
    this->name = name;
    this->surname = surname;
    this->homeworkGrades = grades;
    this->exam = exam;
}

Student &Student::operator=(const Student &other) {
    this->name = other.name;
    this->surname = other.surname;
    this->exam = other.exam;
    this->homeworkGrades = other.homeworkGrades;

    return *this;
}

ostream &operator<<(ostream &os, const Student &student) {
    int maxLength = std::max(student.getName().length(), student.getSurname().length());

    try {
        os << std::fixed << std::setprecision(2) << left << setw(maxLength)
           << student.getName() << setw(maxLength) << student.getSurname() << setw(maxLength)
           << student.GetFinalGrade(AVERAGE) << setw(maxLength) << student.GetFinalGrade(MEDIAN)
           << endl;
    } catch (std::exception &ex) {
        os << "There was an error while outputting a student";
    }

    return os;
}



