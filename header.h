#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <sstream>
#include <time.h>
#include <exception>
#include <stdlib.h>
#include <chrono>
#include <list>
#include <deque>

using std::string;
using std::vector;
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
        std::mt19937 gen(
                time(NULL));  // Standard mersenne_twister_engine seeded with rd()
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

    double GetAverageOfHomeworkGrades() {
        if (homeworkGrades.size() == 0)  // todo throw exceptions to handle this error
            throw std::runtime_error(
                    "Mokinys " + name + " " + surname + "  neturi pazymiu, tad negalime isvesti vidurkio");

        int sum = 0;

        for (auto &n : homeworkGrades) {
            sum += n;
        }

        return (double) sum / homeworkGrades.size();
    }

    double GetMedianOfHomeworkGrades() {
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

    double GetFinalGrade(CalculationType calcType) {
        switch (calcType) {
            case AVERAGE:
                return 0.4 * GetAverageOfHomeworkGrades() + 0.6 * exam;
            case MEDIAN:
                return 0.4 * GetMedianOfHomeworkGrades() + 0.6 * exam;
        }
    }
};

template<typename Container>
void PrintStudentsData(std::ostream &stream, Container students, std::vector<std::string> headers);

template<typename Container>
void PrintStudentsData(std::ostream &stream, Container students);

template<typename Container>
void ReadFile(std::istream &stream, Container &students, vector<string> &headers);

template<typename Container>
void ReadFile(std::istream &stream, Container &students);

template<typename Container>
void InputData(Container &students, std::vector<std::string> &headers);

string GenerateFile(int studentsCount, int gradesCount);

void GenerateFilesAndTest(int gradesCount);

template<typename Container>
void SeperateStudents(Container students, Container &smartless, Container &smartfull);

void TestSpeedOfSeperation(int studentsToGenerate, int gradesCount);

template<typename Container>
void PrintStudentsData(std::ostream &stream, Container students, std::vector<std::string> headers) {
//  sort(students.begin(), students.end(),
//       [](Student s1, Student s2) { return s1.name < s2.name; });

    stream << left << setw(12) << headers[0] << setw(12) << headers[1] << setw(20)
           << "Galutinis (Vid.)"
           << "Galutinis (Med)" << endl;
    stream << string(55, '-') << endl;

    vector<string> errors;
    for (auto &student : students) {
        try {
            stream << std::fixed << std::setprecision(2) << left << setw(12)
                   << student.name << setw(12) << student.surname << setw(20)
                   << student.GetFinalGrade(AVERAGE) << student.GetFinalGrade(MEDIAN)
                   << endl;
        } catch (std::exception &ex) {
            errors.push_back(ex.what());
        }
    }

    if (errors.size() > 0) {
        cout << endl << "Isvedant duomenis buvo klaidu, del kuriu negalejome isvesti rezultatu: " << endl << endl;
        for (auto &error : errors) {
            cout << error << endl;
        }
    }
}

template<typename Container>
void PrintStudentsData(std::ostream &stream, Container students) {
    vector<string> headers;
    headers.push_back("Vardas");
    headers.push_back("Pavarde");

    PrintStudentsData(stream, students, headers);
}

template<typename Container>
void InputData(Container &students, vector<string> &headers) {
    int studentsCount;

    students.clear();
    headers.clear();

    cout << "Kiek studentu ivesite? ";
    cin >> studentsCount;

    while (cin.fail() || studentsCount < 1) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cout << "Ivesta neteisinga reiksme, veskite is naujo: ";
        cin >> studentsCount;
    }

    students.reserve(studentsCount);

    for (int i = 0; i < studentsCount; i++) {
        Student student;
        student.InputData();
        students.push_back(student);
    }

    headers.push_back("Vardas");
    headers.push_back("Pavarde");
}

template<typename Container>
void ReadFile(std::istream &stream, Container &students, vector<string> &headers) {
    stream.clear();
    stream.seekg(0, std::ios::beg);
    students.clear();
    //students.reserve(50);

    if (stream.fail()) {
        throw new std::runtime_error("Failas neegzistuoja");
    }

    string line;
    getline(stream, line);  // Nuskaitome pirma eilute

    if (line.empty()) {
        throw new std::runtime_error("Pirma eilute tuscia, nutraukiame");
    }

    std::istringstream iss(line);

    string s;
    while (iss >> s) {
        headers.push_back(s);
    }

    if (headers.size() < 4) {
        throw new std::runtime_error("Failas neturi pakankamai stulpeliu, ( min 4 stulpeliai ).");
    }

    const int homeworkGradesCount = headers.size() - 3;

    while (getline(stream, line)) {
        std::istringstream iss(line);

        Student student;
        iss >> student.name >> student.surname;

        for (int i = 0; i < homeworkGradesCount; i++) {
            int grade;
            iss >> grade;
            student.homeworkGrades.push_back(grade);
        }

        iss >> student.exam;
        students.push_back(student);
    }
}

template<typename Container>
void ReadFile(std::istream &stream, Container &students) {
    vector<string> headers;
    ReadFile(stream, students, headers);
}

string GenerateFile(int studentsCount, int gradesCount) {
    string fileName = "kursiokai" + std::to_string(studentsCount) + ".txt";
    ofstream stream(fileName);
    std::mt19937 gen(time(NULL));
    std::uniform_int_distribution<> gradesRand(1, 10);

    stream << "Vardas " << "Pavarde ";

    for (int i = 1; i < gradesCount; i++) {
        stream << "ND" << i << " ";
    }

    stream << "Egzaminas" << "\n";


    for (int i = 0; i < studentsCount; i++) {
        stream << "Vardas" << i << " Pavarde" << i << " ";
        for (int j = 0; j < gradesCount + 1; j++) {
            stream << gradesRand(gen) << " ";
        }
        stream << "\n";
    }

    stream.close();

    return fileName;
}

template<typename Container>
void SeperateStudents(Container students, Container &smartless, Container &smartfull) {
    for (Student student : students) {
        if (student.GetFinalGrade(AVERAGE) < 5.0) {
            smartless.push_back(student);
        } else {
            smartfull.push_back(student);
        }
    }
}

void TestSpeedOfSeperation(int studentsToGenerate, int gradesCount) {
    cout << "Testuojame " << studentsToGenerate << " studentu  atskyrima kai kiekvienas studentas turi po "
         << gradesCount << " pazymius:" << endl;


    auto fileName = GenerateFile(studentsToGenerate, gradesCount);

    std::ifstream stream(fileName);

    if (stream.fail()) {
        throw new std::runtime_error("Failas " + fileName + " neegzistuoja");
    }

    Timer t;
    std::vector<Student> studentsVec;

    ReadFile(stream, studentsVec);

    std::vector<Student> smartfulVec;
    std::vector<Student> smartlessVec;
    SeperateStudents(studentsVec, smartlessVec, smartfulVec);
    cout << "Operacija su Vector ivykdyta per " << t.elapsed() << endl;
    t.reset();

    std::ofstream smartfulOut("kietiakai1" + std::to_string(studentsToGenerate) + ".txt");
    std::ofstream smartlessOut("nuskriaustukai1" + std::to_string(studentsToGenerate) + ".txt");
    PrintStudentsData(smartfulOut, smartfulVec);
    PrintStudentsData(smartlessOut, smartlessVec);
    smartfulOut.close();
    smartlessOut.close();
    cout << "Irasymas ivykdytas per " << t.elapsed() << endl;
    t.reset();

    std::list<Student> studentsList;

    ReadFile(stream, studentsList);

    std::list<Student> smartfulList;
    std::list<Student> smartlessList;

    SeperateStudents(studentsList, smartlessList, smartfulList);
    cout << "Operacija su List ivykdyta per " << t.elapsed() << endl;
    t.reset();

    std::ofstream smartfulOutList("kietiakai2" + std::to_string(studentsToGenerate) + ".txt");
    std::ofstream smartlessOutList("nuskriaustukai2" + std::to_string(studentsToGenerate) + ".txt");
    PrintStudentsData(smartfulOutList, smartfulList);
    PrintStudentsData(smartlessOutList, smartlessList);
    smartfulOutList.close();
    smartlessOutList.close();
    cout << "Irasymas ivykdytas per " << t.elapsed() << endl;
    t.reset();

    std::deque<Student> studentsDeq;

    ReadFile(stream, studentsDeq);

    std::deque<Student> smartfulDeq;
    std::deque<Student> smartlessDeq;

    SeperateStudents(studentsDeq, smartlessDeq, smartfulDeq);
    cout << "Operacija su Deque ivykdyta per " << t.elapsed() << endl;
    t.reset();

    std::ofstream smartfulOutDeq("kietiakai3" + std::to_string(studentsToGenerate) + ".txt");
    std::ofstream smartlessOutDeq("nuskriaustukai3" + std::to_string(studentsToGenerate) + ".txt");
    PrintStudentsData(smartfulOutDeq, smartfulDeq);
    PrintStudentsData(smartlessOutDeq, smartlessDeq);
    smartfulOutDeq.close();
    smartlessOutDeq.close();
    cout << "Irasymas ivykdytas per " << t.elapsed() << endl;
    t.reset();


}

void GenerateFilesAndTest(int gradesCount = 10) {
    TestSpeedOfSeperation(10, gradesCount);
    TestSpeedOfSeperation(100, gradesCount);
    TestSpeedOfSeperation(1000, gradesCount);
    TestSpeedOfSeperation(10000, gradesCount);
    TestSpeedOfSeperation(100000, gradesCount);

    cout << "Testavimas baigtas" << endl;
}


#endif // HEADER_H_INCLUDED
