#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <string>
#include <regex>
#include "course.h"
#include <unordered_map>
#include "functions.cpp"
#include <algorithm>

using json = nlohmann::json;
using std::cout;




int main(int argc, char *argv[]){
	if (argc != 2){
		std::cerr << "Error: No argument provided.\n";
		return 1;
	}
	else{
		std::ifstream json_file (argv[1]);
		//checks for valid file
		if ( !json_file.is_open() ){
			std::cerr << "Error: could not open file\n";
			return 1;
		}
		else{
			//reads file and stores into a string
			std::string json_content {std::istreambuf_iterator<char>(json_file),std::istreambuf_iterator<char>()};

			json j;
			std::vector<course> course_list;
			std::vector<std::string> temp;
			try{
				j = json::parse(json_content);	
			}
			catch (const std::invalid_argument& ia){
				std::cerr << "Error: Argument is invalid JSON file\n";
				return 1;
			}

			try{		
				json_to_string(j,course_list);
			}
			catch ( ... ){
				std::cerr << "Error: converting JSON to string (bad syntax for file) \n\n";
				return 1;
			}
			std::vector<std::string> output;
			std::vector<course> pending;
			output.clear();

			//separate output/pending
			separate_course(course_list,output,pending,temp);
			delete_duplicate(temp,output,pending);

			if (course_list.empty()){
				std::cerr << "Error: no course list found\n";
				return 1;
			}
			//checks if all courses required a prereq
			if (pending.size() == course_list.size()){
				std::cerr << "Error: invalid class list (all prereq courses found)\n";
				return 1;
			}
			if (output.empty() == true){
				std::cerr << "Error: no classes without preque were found\n";
				return 1;
			}
			bool flag = 0;
			check_pending(pending, output, flag);
			if (flag == 1){
				std::cerr << "Error, an ineligible course found\n";
				return 1;
			}
			cout << "\nCourses to take: \n";
			output_vector(output);
		}	
	}
	return 0;
}
