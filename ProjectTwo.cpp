#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

struct Course
{
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

bool loadCourses(const string &filename, unordered_map<string, Course> &courses)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Unable to open course file." << endl;
        return false;
    }

    // Read the course file one line at a time
    string line;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        // Separate the course number and name
        stringstream ss(line);
        string courseNumber;
        string courseName;

        getline(ss, courseNumber, ',');
        getline(ss, courseName, ',');

        // Make sure the course number and name are provided
        if (courseNumber.empty() || courseName.empty())
        {
            cout << "Error: Invalid course data." << endl;
            return false;
        }

        // Create a course using the information from this line
        Course course;
        course.courseNumber = courseNumber;
        course.courseName = courseName;

        // Add any prerequisites listed after the course name
        string prerequisite;

        while (getline(ss, prerequisite, ','))
        {
            if (!prerequisite.empty())
            {
                course.prerequisites.push_back(prerequisite);
            }
        }

        // Store the course using its course number as the key
        courses[course.courseNumber] = course;
    }

    // Check that each prerequisite exists in the course data
    for (const auto &entry : courses)
    {
        const Course &course = entry.second;

        for (const string &prerequisite : course.prerequisites)
        {
            if (courses.find(prerequisite) == courses.end())
            {
                cout << "Error: Prerequisite " << prerequisite
                     << " does not exist." << endl;
                return false;
            }
        }
    }

    // Confirm that the course data loaded successfully
    cout << "Course data loaded successfully." << endl;
    return true;
}

void printCourseList(const unordered_map<string, Course> &courses)
{
    // Collect the course numbers so they can be sorted
    vector<string> courseNumbers;

    for (const auto &entry : courses)
    {
        courseNumbers.push_back(entry.first);
    }

    // Sort course numbers from lowest to highest
    sort(courseNumbers.begin(), courseNumbers.end());

    // Print each course in alphanumeric order
    cout << "\nHere is a sample schedule:\n"
         << endl;

    for (const string &number : courseNumbers)
    {
        const Course &course = courses.at(number);
        cout << course.courseNumber << ", "
             << course.courseName << endl;
    }
}

void searchCourse(const unordered_map<string, Course> &courses, string courseNumber)
{
    // Allow course numbers to be entered in lowercase
    transform(courseNumber.begin(), courseNumber.end(),
              courseNumber.begin(), [](unsigned char c)
              { return toupper(c); });

    // Find the requested course in the hash table
    auto it = courses.find(courseNumber);

    if (it == courses.end())
    {
        cout << "Course not found." << endl;
        return;
    }

    const Course &course = it->second;

    // Display the course number and title
    cout << "\n"
         << course.courseNumber << ", "
         << course.courseName << endl;

    // Display prerequisite course numbers and titles
    if (course.prerequisites.empty())
    {
        cout << "Prerequisites: None" << endl;
    }
    else
    {
        cout << "Prerequisites: ";

        for (size_t i = 0; i < course.prerequisites.size(); ++i)
        {
            const string &prerequisite = course.prerequisites[i];
            auto prereqIt = courses.find(prerequisite);

            if (prereqIt != courses.end())
            {
                if (i > 0)
                {
                    cout << ", ";
                }

                cout << prereqIt->second.courseNumber << ", "
                     << prereqIt->second.courseName;
            }
        }

        cout << endl;
    }
}

int main()
{
    // Store course information in hash table
    unordered_map<string, Course> courses;

    int choice = 0;
    bool dataLoaded = false;

    // Keep showing the menu until the user chooses to exit
    while (choice != 9)
    {
        // Display the advising program menu
        cout << "\nWelcome to the course planner." << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "\nWhat would you like to do? ";

        // Check that the user entered a valid number
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option. Please enter a number." << endl;
            continue;
        }

        // Remove any remaining characters from the input line
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            // Ask for the file containing the course information
            string filename;
            cout << "Enter the file name: ";
            getline(cin, filename);

            // Load the courses and track whether loading succeeded
            courses.clear();
            dataLoaded = loadCourses(filename, courses);

            if (!dataLoaded)
            {
                courses.clear();
            }
        }

        else if (choice == 2)
        {
            // Make sure the course data has been loaded first
            if (!dataLoaded)
            {
                cout << "Please load the course data first." << endl;
            }
            else
            {
                printCourseList(courses);
            }
        }

        else if (choice == 3)
        {
            // Make sure the course data is available before searching
            if (!dataLoaded)
            {
                cout << "Please load the course data first." << endl;
            }
            else
            {
                string courseNumber;

                cout << "Enter the course number: ";
                cin >> courseNumber;

                // Find and display the requested course
                searchCourse(courses, courseNumber);
            }
        }

        else if (choice == 9)
        {
            // Exit the advising program
            cout << "Thank you for using the course planner!" << endl;
        }
        else
        {
            // Handle menu choices that are not available
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
