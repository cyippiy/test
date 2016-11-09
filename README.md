----------------------
Command Line Arguments
----------------------
./scheduler filename
example: ./scheduler math.json

compiled with OSX clang-703.0.31
use either the following commands to compile:
make
make all
------------------------
Design Decision & Issues
------------------------
I ended up choosing C++ mainly because I'm most familiar with the language. Unfortunately, C++ does not have a native json parser so I had to be creative with an open source library.

The program takes 1 argument, a JSON file, and checks for proper format. Originally I tried using regex to help for syntax, but std::regex doesn't like such a long regex. Instead, I had to rely on catching errors while converting the json to string. I also sanitized the course names of characters that might be harmful. I created the class course to help represent the important information from the json, mainly the course name and it's prereqisites.

The program will first parse the json, and then split the array. I then split the course list into two lists: output (those without prereq) and course that are pending prereq. I then removed any duplicate entries. Finally, I checked each pending course and checked whether or not the prereq exists in the output list. The pending check does detect if there are courses in the list that are impossible to take because the prereq was never listed. After this step, the final ouput is a correct order of courses to take with respect to prereq.
----------------------
Performance Analysis
----------------------
I will say that this program is not the most efficient program. I was unable to find an efficient way to iterate through all the courses because the prereq must be fulfilled first. It's not like a a list of number where you can find an algorithm to sort the vectors. I did use the std::sort to help find duplicates, since it was much easier to check for sorted names rather than do another for loop. I looked up that the time complexity is O(nlogn) for std::sort.

So a lot of the functions, such as deleting duplicates and check_pending, required a double for loop, giving a O(n^2) for worst case scenarios. 
