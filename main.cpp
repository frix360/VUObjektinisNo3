#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <random>
#include <sstream>
#include <time.h>

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

    cout
        << "Ar sugeneruoti pazymius ir egzamino rezultata atsitiktinai (Y/N)? ";
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
    if (homeworkGrades.size() ==
        0)  // todo throw exceptions to handle this error
      return 0;

    int sum = 0;

    for (auto& n : homeworkGrades) {
      sum += n;
    }

    return (double)sum / homeworkGrades.size();
  }

  double GetMedianOfHomeworkGrades() {
    int gradesCount = homeworkGrades.size();

    if (gradesCount == 0)  // todo throw exceptions to handle this error
      return 0;

    vector<int> sortedGrades(homeworkGrades);

    sort(sortedGrades.begin(), sortedGrades.end());

    if (gradesCount % 2 == 0) {
      return (double)(sortedGrades[gradesCount / 2] +
                      sortedGrades[gradesCount / 2 - 1]) /
             2;
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

void PrintStudentsData(vector<Student> students, vector<string> headers);
void ReadFile(vector<Student>& students, vector<string>& headers);
void InputData(vector<Student>& students, vector<string>& headers);

int main() {
  vector<Student> students;
  vector<string> headers;

  cout << "Ar norite ivesti duomenis patys (1) ar nuskaityti juos is failo? "
          "(2): ";

  int choice;

  cin >> choice;

  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  while (!((choice == 1 || choice == 2) && cin.good())) {
    cout << "Tokio pasirinkimo nera. Veskite is naujo:  ";
    cin >> choice;

    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  if (choice == 1) {
    InputData(students, headers);
  } else if (choice == 2) {
    ReadFile(students, headers);
  }

  PrintStudentsData(students, headers);

  cin.get();
  return 0;
}

void PrintStudentsData(vector<Student> students, vector<string> headers) {
  sort(students.begin(), students.end(),
       [](Student s1, Student s2) { return s1.name < s2.name; });

  cout << left << setw(12) << headers[0] << setw(12) << headers[1] << setw(20)
       << "Galutinis (Vid.)"
       << "Galutinis (Med)" << endl;
  cout << string(55, '-') << endl;

  for (auto& student : students) {
    cout << std::fixed << std::setprecision(2) << left << setw(12)
         << student.name << setw(12) << student.surname << setw(20)
         << student.GetFinalGrade(AVERAGE) << student.GetFinalGrade(MEDIAN)
         << endl;
  }
}

void InputData(vector<Student>& students, vector<string>& headers) {
  int studentsCount;

  students.clear();
  headers.clear();

  cout << "Kiek studentu ivesite? ";
  cin >> studentsCount;

  while (cin.fail()) {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "Ivesta neteisinga reiksme, veskite is naujo: ";
    cin >> studentsCount;
  }

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

  std::ifstream fd("kursiokai.txt");

  if (fd.fail()) {
    cout << "Failas \"kursiokai.txt\" neegzistuoja";
    return;
  }

  string line;
  getline(fd, line);  // Nuskaitome pirma eilute

  if (line.empty()) {
    cout << "There is nothing on the first line of the file. Aborting.";
    return;
  }

  std::istringstream iss(line);

  string s;
  while (iss >> s) {
    headers.push_back(s);
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
