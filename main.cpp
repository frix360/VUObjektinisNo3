#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <random>
#include <sstream>
#include <time.h>

using namespace std;

enum CalculationType { MEDIAN, AVERAGE };

const int MAX_RAND_HOMEWORK_GRADES = 20;
const int MIN_RAND_HOMEWORK_GRADES = 10;


struct Student {
    string name;
    string surname;
    vector<int> homeworkGrades;
    int gradesCount = 0;
    int exam = 0;

    void InputData() {
        cout << "Iveskite varda ir pavarde: ";
        cin >> name >> surname;
        cout << endl;

        cout << "Ar sugeneruoti pazymius ir egzamino rezultata atsitiktinai (Y/N)? ";
        char c;
        cin >> c;

        if (toupper(c) == 'Y') {
            GenerateGrades();
        } else {
            InputGrades();
            cout << "Koks egzamino rezultatas? ";
            cin >> exam;
        }



    }

    void GenerateGrades() {
        mt19937 gen(time(NULL)); //Standard mersenne_twister_engine seeded with rd()
        uniform_int_distribution<> gradesCountRand(MIN_RAND_HOMEWORK_GRADES, MAX_RAND_HOMEWORK_GRADES);
        uniform_int_distribution<> gradesRand(1, 10);


        gradesCount = gradesCountRand(gen);

        for (int i = 0; i < gradesCount; i++) {
            homeworkGrades.push_back(gradesRand(gen));
        }

        exam = gradesRand(gen);
    }

    void InputGrades() {

        cout << "Iveskite pazymius (kad baigti ivedima, iveskite ne skaiciu)." << endl;

        int grade;

        while(cin >> grade)
        {
              if (!(grade >= 1 && grade <= 10)) {
                cout << "Pazymys turi buti nuo 1 iki 10, veskite toliau. " << endl;
                continue;
            }
            homeworkGrades.push_back(grade);
        }

        gradesCount = homeworkGrades.size();

        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

    }

    float GetAverageOfHomeworkGrades() {
        int sum = 0;
        for (int i = 0; i < gradesCount; i++) {
            sum += homeworkGrades[i];
        }

        return (float)sum / gradesCount;
    }

    float GetMedianOfHomeworkGrades() {

        int sortedGrades[gradesCount];

        for (int i = 0; i < gradesCount; i++) {
            sortedGrades[i] = homeworkGrades[i];
        }

        sort(sortedGrades, sortedGrades + gradesCount);

        if (gradesCount % 2 == 0) {
            return (sortedGrades[gradesCount/2] + sortedGrades[gradesCount/2 - 1]) / (float)2;
        }
        else {
            return sortedGrades[gradesCount/2];
        }
    }

    float GetFinalGrade(CalculationType calcType) {
        switch (calcType) {
        case AVERAGE:
            return 0.4f  * GetAverageOfHomeworkGrades() + 0.6f * exam;
            break;
        case MEDIAN:
             return 0.4f  * GetMedianOfHomeworkGrades() + 0.6f * exam;
        }
    }
};

void printStudentsData(vector<Student> students);

int main()
{
    ifstream fd ("data.txt");
    vector<string> headers;

    string line;
    getline(fd, line); // Nuskaitome pirma eilute
    istringstream iss(line);

    string s;
    while (iss >> s) {
        headers.push_back(s);
    }

    vector<Student> students;

    const int homeworkGradesCount = headers.size() - 3;

    while (getline(fd, line)) {
        istringstream iss(line);

        Student student;
        iss >> student.name >> student.surname;

        for (int i = 0; i < homeworkGradesCount; i++) {
            int grade;
            iss >> grade;
            student.homeworkGrades.push_back(grade);
            student.gradesCount++;
        }

        iss >> student.exam;
        students.push_back(student);
    }



    printStudentsData(students);

    cin.get();
    return 0;
}


void printStudentsData(vector<Student> students) {
    sort(students.begin(), students.end(),
         [](Student s1, Student s2) {
            return s1.name < s2.name;
         });

    cout << left << setw(10) << "Vardas" << setw(10) << "Pavarde" << setw(20) << "Galutinis (Vid.)" << "Galutinis (Med)" << endl;
    cout << string (55 , '-') << endl;



    for (int i = 0; i < students.size(); i++) {
        cout << fixed << setprecision(2) << left << setw(10) << students[i].name <<  setw(10) <<students[i].surname <<  setw(20) << students[i].GetFinalGrade(AVERAGE) << students[i].GetFinalGrade(MEDIAN) << endl;
    }
}
