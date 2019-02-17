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


        int gradesCount = gradesCountRand(gen);

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

        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

    }

    double GetAverageOfHomeworkGrades() {
        if (homeworkGrades.size() == 0) // todo throw exceptions to handle this error
            return 0;

        int sum = 0;

        for (auto& n : homeworkGrades) {
            sum += n;
        }

        return (double)sum / homeworkGrades.size();
    }

    double GetMedianOfHomeworkGrades() {
        int gradesCount = homeworkGrades.size();

        if (gradesCount == 0) // todo throw exceptions to handle this error
            return 0;

        vector<int> sortedGrades(homeworkGrades);

        sort(sortedGrades.begin(), sortedGrades.end());

        if (gradesCount % 2 == 0) {
            return (double)(sortedGrades[gradesCount/2] + sortedGrades[gradesCount/2 - 1]) / 2;
        }
        else {
            return sortedGrades[gradesCount/2];
        }
    }

    double GetFinalGrade(CalculationType calcType) {
        switch (calcType) {
        case AVERAGE:
            return 0.4f  * GetAverageOfHomeworkGrades() + 0.6f * exam;
            break;
        case MEDIAN:
             return 0.4f  * GetMedianOfHomeworkGrades() + 0.6f * exam;
        }
    }
};

void printStudentsData(vector<Student> students, vector <string> headers);

int main()
{
    ifstream fd ("kursiokai.txt");

    if (fd.fail()) {
        cout << "Failas \"kursiokai.txt\" neegzistuoja";
        return 1;
    }
    vector<string> headers;

    string line;
    getline(fd, line); // Nuskaitome pirma eilute

    if (line.empty()) {
        cout << "There is nothing on the first line of the file. Aborting.";
        return 2;
    }
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
        }

        iss >> student.exam;
        students.push_back(student);
    }

    printStudentsData(students, headers);

    cin.get();
    return 0;
}

void printStudentsData(vector<Student> students, vector <string> headers) {
    sort(students.begin(), students.end(),
         [](Student s1, Student s2) {
            return s1.name < s2.name;
         });

    cout << left << setw(10) << headers[0] << setw(10) << headers[1] << setw(20) << "Galutinis (Vid.)" << "Galutinis (Med)" << endl;
    cout << string (55 , '-') << endl;

    for (auto &student : students) {
        cout << fixed << setprecision(2) << left << setw(10) << student.name <<  setw(10) <<student.surname <<  setw(20) << student.GetFinalGrade(AVERAGE) << student.GetFinalGrade(MEDIAN) << endl;
    }
}
