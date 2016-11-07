#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <string>
#include <regex>
#include "course.h"
#include "course.cpp"
#include <unordered_map>

using json = nlohmann::json;
using std::cout;


// checks regex if file is an array of JSON
// regex: \\[(,*\\s\\s*\\{\\s+\"name\":\\s*\"(\\w+\\s*\\w)+\",\\s*\"prerequisites\":\\s*\\[(\"(\\w+\\s*\\w)+\",*\\s*)*\\]\\s*\\}\\s*)+\\]
// input: in string of file
// output: returns true or false
bool check_regex(const std::string & x){
	std::regex test("\\[(,*\\s\\s*\\{\\s+\"name\":\\s*\"(\\w+\\s*\\w)+\",\\s*\"prerequisites\":\\s*\\[(\"(\\w+\\s*\\w)+\",*\\s*)*\\]\\s*\\}\\s*)+\\]");
	//cout << x << "\n";
	std::smatch match;
	if (std::regex_match(x,match,test) && match.size() > 0){
		cout << "this passed check_regex!\n";
		return true;
	}
	else{
		cout << "failed check_regex!\n";
		return false;
	}
}

//santitizes input to only characters and whitespace
void clean_string(std::string & x){
	char chars[] = "()<>[]\"";
	for (unsigned int i = 0; i < strlen(chars); i++){
		x.erase (std::remove(x.begin(),x.end(),chars[i]), x.end());
	}
}

//checks first index to see if it's just whitespace
bool isempty(const std::vector<std::string> & v){
	return std::all_of(v[0].begin(),v[0].end(),isspace);
}

bool isempty(const std::vector<course>& v){
	if (v.size() == 0){
		return true;
	}
	else{
		return false;
	}
}

void load_prereq(const std::string & s, std::vector<std::string> & v){
	std::string delimiter = ",";
	size_t pos = 0;
	std::string test = s;
	std::string token;
	while ((pos = test.find(delimiter)) != std::string::npos){
		token = test.substr(0, pos);
		v.push_back(token);
		test.erase(0, pos + delimiter.length());
	}
	v.push_back(test);
}

//outputs the vector
void output_vector(std::vector<std::string> & v){
	for (int i = 0; i < v.size(); i++){
		cout << v[i] << "\n";
	}
}

void output_vector(std::vector<course> & v){
		for (int i = 0; i < v.size(); i++){
			cout << v[i].output_name() << "\n";
		}
}

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
			try{
				j = json::parse(json_content);	
			}
			catch (const std::invalid_argument& ia){
				std::cerr << "Error: Invalid JSON file\n";
				return 1;
			}
		
			//checks if json is valid
			if (!check_regex(json_content)){
				std::cerr << "Error: Bad class file\n";
				return 1;
			}
			cout << "Size of Json array: " << j.size() << "\n";
			std::vector<course> course_list;
			std::string name_var;
			std::string p;
			std::vector<std::string> v;
			int count;

			course course_var;
			//takes in a json object
			//parses through each array element
			//strips the name and preque
			for (int i = 0; i < j.size(); i++){
				auto json_var = j[i].get<std::unordered_map<std::string, json>>();
				name_var = json_var["name"];
				clean_string(name_var);
				p = json_var["prerequisites"].dump();
				clean_string(p);
				cout << "name: " << name_var << "\n" << "prerequisites: "<< p << "\n";
				load_prereq(p,v);
				if (isempty(v)){
					count = 0;
					course_var.copy_prereq(v,0);
				}
				else{
					count = v.size();
					course_var.copy_prereq(v,count);
				}
				cout << count << "\n";
				course_var.set_name(name_var);
				course_list.push_back(course_var);
				v.clear();
			}
			/*
			for (unsigned int i = 0; i < course_list.size(); i++){
				cout << "Name: " << course_list[i].output_name() << "\n";
				course_list[i].output_prereq();
			}*/
			std::vector<std::string> output;
			std::vector<course> pending;
			output.clear();
			//cout << course_list[1].get << "\n";
			//is json array empty?
			for (int i = 0; i < course_list.size(); i++){
				if (course_list[i].has_prereq() == false){
					//cout << i << "\n";
					output.push_back(course_list[i].output_name());
				}
				else{
					//check top prereq
					//

					//for (int j = 0; j < output.size(); j++){
					//	if (output[j] == course_list[i].top_prereq()){
							
					//		output.push_back(course_list[i].output_name());
					//	}
					//}

					pending.push_back(course_list[i]);
				}
			}
			//checks if all courses required a prereq
			if (pending.size() == course_list.size()){
				std::cerr << "Error: invalid class list (all prereq courses)";
				return 1;
			}
			cout << "pending: \n";
			output_vector(pending);
			std::vector<int> x;
			x.clear();
			cout << "vector size test: " << x.size();

			//flag infinite
			//checks to see if no action occurred

			//is pending empty?
			//no
			//starts with top element
			//is prereq empty? if yes, add to output, pop. action++
			//else. 
			//scans output
			//if matches,remove prereq,
			//action++
			//checks top prereq, is empty?
			//if yes, add to output
			//action++
			//if not, scans output
			//if no match, check for next prereq
			//if final prereq, check for next pending
			//if next pending is final course, check to see if pending size >= 1 and action != 0;
			int action = 0;
			while (isempty(pending) == false){

		//		if (action)
			}
			cout << "Course to take: \n";
			output_vector(output);
			
		}	

		//reads out classes

		cout << "\nProgram ended\n";
	}
	return 0;
}
