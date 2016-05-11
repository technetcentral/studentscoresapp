#include <iostream>
#include <iomanip> // for setw
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept> // for domain_error
#include <assert.h>
using namespace std;

// Functions from OrganizingPrograms

double median(const vector<double>& v) {
   if (v.size() == 0)
      throw domain_error("can't compute median of empty vector");
   vector<double> v2(v);
   sort(v2.begin(), v2.end());
   int mid = v2.size() / 2;
   double median;
   if (v2.size() % 2 == 0)
      median = (v2[mid - 1] + v2[mid]) / 2;
   else
      median = v2[mid];
   return median;
}

double grade(double midterm, double final, double homework) {
	return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

// Note, changed to add an "if" and clear.
istream& read_homework(istream& in, vector<double>& homework) {
   if (in) {
	   double x;
	   homework.clear();
	   while (cin >> x)
		   homework.push_back(x);
	   in.clear(); // clear the error state
   }
   return in;
}

// New structure for grouping student information.

struct student_info {
	string name;
	double midterm, final;
	vector<double> homework;
};

istream& read(istream& in, student_info& s) {
	in >> s.name >> s.midterm >> s.final;
	return read_homework(in, s.homework);
}

double grade(const student_info& s) {
	return grade(s.midterm, s.final, median(s.homework));
}

bool compare_names(const student_info& s1, const student_info& s2) {
	return s1.name < s2.name;
}

// The new stuff starts here.

bool did_all_hw(const student_info& s) {
	return find(s.homework.begin(), s.homework.end(), 0)
				== s.homework.end();
}

double grade_aux(const student_info& s) {
	try {
		return grade(s);
	} catch (const domain_error&) {
		return grade(s.midterm, s.final, 0.0);
	}
	return 0.0;
}

double median_analysis(const vector<student_info>& students) {
	vector<double> grades;
	transform(students.begin(), students.end(),
	 		  back_inserter(grades), grade_aux);
	return median(grades);
}

double average(const vector<double>& v) {
	return accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double average_grade(const student_info& s) {
	return grade(s.midterm, s.final, average(s.homework));
}

double average_analysis(const vector<student_info>& students) {
	vector<double> grades;
	transform(students.begin(), students.end(),
		      back_inserter(grades), average_grade);
	return median(grades);
}

double opti_median(const student_info& s) {
	vector<double> nonzeros;
	remove_copy(s.homework.begin(), s.homework.end(),
				back_inserter(nonzeros), 0);
	if (nonzeros.empty())
		return grade(s.midterm, s.final, 0);
	else
		return grade(s.midterm, s.final, median(nonzeros));
}

double opti_median_analysis(const vector<student_info>& students) {
	vector<double> grades;
	transform(students.begin(), students.end(),
	 		  back_inserter(grades), opti_median);
	return median(grades);
}

void write_analysis(ostream& out, const string& name,
					double analysis(const vector<student_info>&),
					const vector<student_info>& did,
					const vector<student_info>& didnt) {
	out << name << ": analysis(did) = " << analysis(did) <<
			", analysis(didnt) = " << analysis(didnt) << endl;
}

int main() {
	vector<student_info> did, didnt;
	student_info student;
	while (read(cin, student)) {
		if (did_all_hw(student))
			did.push_back(student);
		else
			didnt.push_back(student);
	}
	if (did.empty()) {
		cout << "No student did all the homework" << endl;
	}
	if (didnt.empty()) {
		cout << "Every students did all the homework" << endl;
	}

	write_analysis(cout, "median", median_analysis, did, didnt);
	write_analysis(cout, "average", average_analysis, did, didnt);
	write_analysis(cout, "median of hw turned in", opti_median_analysis, did, didnt);
	return 0;
}
