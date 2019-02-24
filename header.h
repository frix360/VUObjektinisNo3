#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <random>
#include <sstream>
#include <time.h>
#include <exception>
#include <stdlib.h>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::left;
using std::setw;

enum CalculationType { MEDIAN, AVERAGE };

const int MAX_RAND_HOMEWORK_GRADES = 20;
const int MIN_RAND_HOMEWORK_GRADES = 10;

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
      throw std::runtime_error("Mokinys " + name + " " + surname + "  neturi pazymiu, tad negalime isvesti vidurkio");

    int sum = 0;

    for (auto& n : homeworkGrades) {
      sum += n;
    }

    return (double)sum / homeworkGrades.size();
  }

  double GetMedianOfHomeworkGrades() {
    int gradesCount = homeworkGrades.size();

    if (gradesCount == 0)
      throw std::runtime_error("Mokinys " + name + " " + surname + "  neturi pazymiu, tad negalime isvesti vidurkio");

    vector<int> sortedGrades(homeworkGrades);

    sort(sortedGrades.begin(), sortedGrades.end());

    if (gradesCount % 2 == 0) {
      return (double)(sortedGrades[gradesCount / 2] +
                      sortedGrades[gradesCount / 2 - 1]) /2;
    } else {
      return sortedGrades[gradesCount / 2];
    }
  }

  double GetFinalGrade(CalculationType calcType) {
    switch (calcType) {
      case AVERAGE:
        return 0.4 * GetAverageOfHomeworkGrades() + 0.6 * exam;
        break;
      case MEDIAN:
        return 0.4 * GetMedianOfHomeworkGrades() + 0.6 * exam;
    }
  }
};

void PrintStudentsData(std::vector<Student> students, std::vector<std::string> headers);
void ReadFile(std::vector<Student>& students, std::vector<std::string>& headers);
void InputData(std::vector<Student>& students, std::vector<std::string>& headers);

void PrintStudentsData(std::vector<Student> students, std::vector<std::string> headers) {
  sort(students.begin(), students.end(),
       [](Student s1, Student s2) { return s1.name < s2.name; });

  cout << left << setw(12) << headers[0] << setw(12) << headers[1] << setw(20)
       << "Galutinis (Vid.)"
       << "Galutinis (Med)" << endl;
  cout << string(55, '-') << endl;

  vector<string> errors;
  for (auto& student : students) {
    try {
    cout << std::fixed << std::setprecision(2) << left << setw(12)
         << student.name << setw(12) << student.surname << setw(20)
         << student.GetFinalGrade(AVERAGE) << student.GetFinalGrade(MEDIAN)
         << endl;
    } catch (std::exception &ex) {
        errors.push_back(ex.what());
    }
  }

  if (errors.size() > 0) {
    cout << endl << "Isvedant duomenis buvo klaidu, del kuriu negalejome isvesti rezultatu: " << endl << endl;
    for (auto& error : errors) {
        cout << error << endl;
    }
  }
}

void InputData(vector<Student>& students, vector<string>& headers) {
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

void ReadFile(vector<Student>& students, vector<string>& headers) {
  students.clear();
  headers.clear();

  students.reserve(50);

  std::ifstream fd("kursiokai.txt");

  if (fd.fail()) {
    throw new std::runtime_error("Failas \"kursiokai.txt\" neegzistuoja");
  }

  string line;
  getline(fd, line);  // Nuskaitome pirma eilute

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

  while (getline(fd, line)) {
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
  fd.close();
}


#endif // HEADER_H_INCLUDED
