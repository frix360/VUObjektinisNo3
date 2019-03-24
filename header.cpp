#include "header.h"

void SortCollection(vector<Student> students) {
    sort(students.begin(), students.end(),
         [](Student s1, Student s2) { return s1.name < s2.name; });
}

void SortCollection(std::deque<Student> students) {
    sort(students.begin(), students.end(),
         [](Student s1, Student s2) { return s1.name < s2.name; });
}

void SortCollection(std::list<Student> students) {
    students.sort([](Student s1, Student s2) { return s1.name < s2.name; });
}

template<typename Container>
size_t FindLongestStringLength(const Container &container) {
    size_t max = 0;
    for (const Student &stud : container) {
        if (stud.name.length() > max) {
            max = stud.name.length();
        }

        if (stud.surname.length() > max) {
            max = stud.surname.length();
        }
    }

    return max;
}

template<typename Container>
void PrintStudentsData(std::ostream &stream, const Container &students, const std::vector<std::string> &headers) {
    SortCollection(students);

    size_t maxLength = FindLongestStringLength(students);

    stream << left << setw(maxLength + 10) << headers[0] << setw(maxLength + 10) << headers[1] << setw(maxLength + 10)
           << "Galutinis (Vid.)"
           << setw(maxLength + 10)
           << "Galutinis (Med)" << endl;
    stream << string(4 * (maxLength + 10), '-') << endl;

    vector<string> errors;
    for (auto &student : students) {
        try {
            stream << std::fixed << std::setprecision(2) << left << setw(maxLength + 10)
                   << student.name << setw(maxLength + 10) << student.surname << setw(maxLength + 10)
                   << student.GetFinalGrade(AVERAGE) << setw(maxLength + 10) << student.GetFinalGrade(MEDIAN)
                   << endl;
        } catch (std::exception &ex) {
            errors.emplace_back(ex.what());
        }
    }

    if (!errors.empty()) {
        cout << endl << "Isvedant duomenis buvo klaidu, del kuriu negalejome isvesti rezultatu: " << endl << endl;
        for (auto &error : errors) {
            cout << error << endl;
        }
    }
}

template<typename Container>
void PrintStudentsData(std::ostream &stream, Container students) {
    vector<string> headers;
    headers.emplace_back("Vardas");
    headers.emplace_back("Pavarde");

    PrintStudentsData(stream, students, headers);
}

void InputData(vector<Student> &students, vector<string> &headers) {
    unsigned int studentsCount;

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

    headers.emplace_back("Vardas");
    headers.emplace_back("Pavarde");
}

template<typename Container>
void ReadFile(std::istream &stream, Container &students, vector<string> &headers) {
    stream.clear();
    stream.seekg(0, std::ios::beg);

    if (stream.fail()) {
        throw std::runtime_error("Failas neegzistuoja");
    }

    string line;
    getline(stream, line);  // Nuskaitome pirma eilute

    if (line.empty()) {
        throw std::runtime_error("Pirma eilute tuscia, nutraukiame");
    }

    std::istringstream issh(line);

    string s;
    while (issh >> s) {
        headers.push_back(s);
    }

    if (headers.size() < 4) {
        throw std::runtime_error("Failas neturi pakankamai stulpeliu, ( min 4 stulpeliai ).");
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
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    string fileName = "kursiokai" + std::to_string(studentsCount) + ".txt";
    ofstream stream(fileName);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> gradesRand(1, 10);

    stream << left << setw(15) << "Vardas" << setw(15) << "Pavarde";

    for (int i = 0; i < gradesCount; i++) {
        stream << setw(10) << "ND" + std::to_string(i + 1);
    }

    stream << setw(10) << "Egzaminas" << "\n";


    for (int i = 0; i < studentsCount; i++) {
        stream << setw(15) << "Vardas" + std::to_string(i) << setw(15) << "Pavarde" + std::to_string(i);
        for (int j = 0; j < gradesCount + 1; j++) {
            stream << setw(10) << gradesRand(gen);
        }
        stream << "\n";
    }

    stream.close();

    return fileName;
}

template<typename Container>
void SeparateStudents(Container students, Container &smartless, Container &smartfull) {
    for (const Student &student : students) {
        if (student.GetFinalGrade(AVERAGE) < 5.0) {
            smartless.push_back(student);
        } else {
            smartfull.push_back(student);
        }
    }
}

// PAPILDOMA UZD
vector<Student> SeparateStudentsWithResize(vector<Student> &students) {
    vector<Student> smartless;

    vector<Student>::size_type i = 0;
    unsigned int resizeCount = 0;

    while (i != students.size()) {
        if (students[i].GetFinalGrade(AVERAGE) < 5.0) {
            smartless.push_back(students[i]);
        } else {
            students.insert(students.begin(), students[i]);
            resizeCount++;
            i++;
        }
        i++;
    }

    students.resize(resizeCount);
    students.shrink_to_fit();

    return smartless;

}

deque<Student> SeparateStudentsWithResize(deque<Student> &students) {
    deque<Student> smartless;
    Student stud;

    deque<Student>::size_type i = 0;

    unsigned int resizeCount = 0;

    while (i != students.size()) {
        stud = students[i];
        if (stud.GetFinalGrade(AVERAGE) < 5.0) {
            smartless.push_front(stud);
        } else {
            students.insert(students.begin(), stud);
            resizeCount++;
            i++;  // pereiti prie kito studento
        }
        i++;
    }

    students.resize(resizeCount);
    students.shrink_to_fit();

    return smartless;

}

vector<Student> SeparateStudents(vector<Student> &students, bool optimized) {


    if (optimized) {
        auto it = std::stable_partition(students.begin(), students.end(),
                                        [](Student st) {
                                            return st.GetFinalGrade(AVERAGE) >= 5.0;
                                        });
        vector<Student> smartless(it, students.end());

        students.erase(it, students.end());

        return smartless;
    } else {
        vector<Student> smartless;
        for (auto i = students.begin(); i != students.end(); i++) {
            if (i->GetFinalGrade(AVERAGE) < 5.0) {
                smartless.push_back((*i));
                students.erase(i--);
            }
        }
        return smartless;
    }

}

list<Student> SeparateStudents(list<Student> &students) {
    list<Student> smartless;

    for (auto i = students.begin(); i != students.end(); i++) {
        if (i->GetFinalGrade(AVERAGE) < 5.0) {
            smartless.push_back((*i));
            students.erase(i--);
        }
    }
    return smartless;
}

deque<Student> SeparateStudents(deque<Student> &students) {
    deque<Student> smartless;
    deque<Student>::size_type i = 0;

    while (i != students.size()) {
        if (students[i].GetFinalGrade(AVERAGE) < 5.0) {
            smartless.push_back(students[i]);
            students.erase(students.begin() + i);
        } else
            ++i;
    }
    return smartless;
}

void TestSpeedOfSeparation(int studentsToGenerate, int gradesCount, bool useDifferentStrategy,
                           bool useResizeMethod) {
    cout << "Testuojame " << studentsToGenerate << " studentu  atskyrima kai kiekvienas studentas turi po "
         << gradesCount << " pazymius:" << endl;


    auto fileName = GenerateFile(studentsToGenerate, gradesCount);

    std::ifstream stream(fileName);

    if (stream.fail()) {
        throw std::runtime_error("Failas " + fileName + " neegzistuoja");
    }

    Timer t;
    std::vector<Student> studentsVec;

    ReadFile(stream, studentsVec);

    std::vector<Student> smartfulVec;
    std::vector<Student> smartlessVec;

    if (useResizeMethod)
        smartlessVec = SeparateStudentsWithResize(studentsVec);
    else if (useDifferentStrategy)
        smartlessVec = SeparateStudents(studentsVec);
    else
        SeparateStudents(studentsVec, smartlessVec, smartfulVec);

    cout << "Operacija su Vector ivykdyta per " << t.elapsed() << endl;
    t.reset();

    std::ofstream smartfulOut("kietiakai1-" + std::to_string(studentsToGenerate) + ".txt");
    std::ofstream smartlessOut("nuskriaustukai1-" + std::to_string(studentsToGenerate) + ".txt");
    if (useDifferentStrategy || useResizeMethod)
        PrintStudentsData(smartfulOut, studentsVec);
    else
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

    if (useDifferentStrategy || useResizeMethod)
        smartlessList = SeparateStudents(studentsList);
    else
        SeparateStudents(studentsList, smartlessList, smartfulList);
    cout << "Operacija su List ivykdyta per " << t.elapsed() << endl;
    t.reset();

    std::ofstream smartfulOutList("kietiakai2-" + std::to_string(studentsToGenerate) + ".txt");
    std::ofstream smartlessOutList("nuskriaustukai2-" + std::to_string(studentsToGenerate) + ".txt");

    if (useDifferentStrategy || useResizeMethod)
        PrintStudentsData(smartfulOutList, studentsList);
    else
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

    if (useResizeMethod)
        smartlessDeq = SeparateStudentsWithResize(studentsDeq);
    else if (useDifferentStrategy)
        smartlessDeq = SeparateStudents(studentsDeq);
    else
        SeparateStudents(studentsDeq, smartlessDeq, smartfulDeq);
    cout << "Operacija su Deque ivykdyta per " << t.elapsed() << endl;
    t.reset();

    std::ofstream smartfulOutDeq("kietiakai3-" + std::to_string(studentsToGenerate) + ".txt");
    std::ofstream smartlessOutDeq("nuskriaustukai3-" + std::to_string(studentsToGenerate) + ".txt");
    if (useDifferentStrategy || useResizeMethod)
        PrintStudentsData(smartfulOutDeq, studentsDeq);
    else
        PrintStudentsData(smartfulOutDeq, smartfulDeq);
    PrintStudentsData(smartlessOutDeq, smartlessDeq);
    smartfulOutDeq.close();
    smartlessOutDeq.close();
    cout << "Irasymas ivykdytas per " << t.elapsed() << endl;
    t.reset();
}

void GenerateFilesAndTest(int gradesCount, bool useDifferentStrategy, bool useResizeMethod) {
    TestSpeedOfSeparation(10, gradesCount, useDifferentStrategy, useResizeMethod);
    TestSpeedOfSeparation(100, gradesCount, useDifferentStrategy, useResizeMethod);
    TestSpeedOfSeparation(1000, gradesCount, useDifferentStrategy, useResizeMethod);
    TestSpeedOfSeparation(10000, gradesCount, useDifferentStrategy, useResizeMethod);
    TestSpeedOfSeparation(100000, gradesCount, useDifferentStrategy, useResizeMethod);

    cout << "Testavimas baigtas" << endl;
}
