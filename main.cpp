#include "header.h"

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
      try {
        ReadFile(students, headers);
      } catch (const std::exception & ex) {
        cout << ex.what();
        cin.get();
        return 1;
      }
  }

  PrintStudentsData(students, headers);

  cin.get();
  return 0;
}

