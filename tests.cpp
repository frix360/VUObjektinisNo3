#include "catch.hpp"
#include "header.h"
#include "Student.h"

TEST_CASE( "Student grades is calculated", "[student]" ) {
vector<int> grades {4, 5, 2, 7, 7};
Student student("Testas", "Testauskas", grades, 10 );

REQUIRE(student.GetAverageOfHomeworkGrades() == 5);
REQUIRE(student.GetMedianOfHomeworkGrades() == 5);

REQUIRE(student.GetFinalGrade(CalculationType::AVERAGE) == 8);

REQUIRE(student.GetFinalGrade(CalculationType::MEDIAN) == 8);
}