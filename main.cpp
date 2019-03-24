#include "header.h"

int main() {

    vector<Student> students;
    vector<string> headers;

    cout << "Ar norite ivesti duomenis patys (1) ar nuskaityti juos is failo? "
            "(2), o gal sugeneruoti failus ir testuoti programos greiti? (3) ";

    int choice;

    cin >> choice;

    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (!((choice == 1 || choice == 2 || choice == 3) && cin.good())) {
        cout << "Tokio pasirinkimo nera. Veskite is naujo:  ";
        cin >> choice;

        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (choice == 1) {
        InputData(students, headers);
        PrintStudentsData(cout, students, headers);
    } else if (choice == 2) {
        try {
            std::ifstream fd("kursiokai.txt");
            ReadFile(fd, students, headers);
            PrintStudentsData(cout, students, headers);
        } catch (const std::exception &ex) {
            cout << ex.what();
            cin.get();
            return 1;
        }
    } else if (choice == 3) {

        cout << "Kokia strategija naudoti: pirma-paprastoji (1), antra (2) , trecia-resize (3) ? ";
        cin >> choice;
        while (!((choice == 1 || choice == 2 || choice == 3) && cin.good())) {
            cout << "Tokio pasirinkimo nera. Veskite is naujo:  ";
            cin >> choice;

            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        try {
            bool useDifferentStrategy = choice == 2;
            bool useResizeMethod = choice == 3;
            GenerateFilesAndTest(5, useDifferentStrategy, useResizeMethod);
        } catch (const std::exception &ex) {
            cout << ex.what();
            cin.get();
            return  1;
        }

    }

    cin.get();
    return 0;
}

