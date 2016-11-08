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

void remove_from_pending(std::vector<course>& v, std::vector<int> x){


}

//takes current element, which is a course object, swaps to back
//if only 3 or more, temp to 2nd last vector
//flag is how many to remove
void swap_to_back(std::vector<course>& v,const int &x, const int &flag){
	course temp(v[x].output_name(),v[x].link_prereq());
	
	//if flag


	/*
	if (v.size() >= 3){
		v[x].output_name();
		v[x] = v[v.size()-2];
		v[v.size()-2] = v[v.size()-1];
		v[v.size()-1] = temp;
	}
	else if (v.size() == 2){
		if (x == 0){
			temp = v[1];
			v[1] = v[0];
			v[0] = temp;
		}
	}
	else{

	}
	*/
}/*
	course temp = v[x];
	v[x] = v[v.size()-1];
	v[v.size()-1] = temp;
*/

//deletes
	/*
void erase_prereq(std::vector<course>& v,const std::vector<int>& p){
	
	std::vector<std::string> temp;

	for (int i = 0; i < p.size(); i++){
		temp.push_back(v[p[i]].ouput_name());
	}
	for (int i = 0; i < temp.size(); i++){
		if()
			v.erase()
	}
}
*/

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
					pending.push_back(course_list[i]);
				}
			}
			if (course_list.empty()){
				std::cerr << "Error: no course list found\n";
				return 1;
			}

			//checks if all courses required a prereq
			if (pending.size() == course_list.size()){
				std::cerr << "Error: invalid class list (all prereq courses found)\n";
				return 1;
			}
			cout << "pending: \n";
			output_vector(pending);

			//need to check for vector size
			//std::vector<int> x;
			//x.clear();
			//cout << "Vector empty?: " << x.empty() << "\n";

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
			count = 0;
			int current_size = pending.size();
			if (output.empty() == true){
				std::cerr << "Error: no classes without preque were found\n";
				return 1;
			}
			int pop_flag = 0;
			std::vector<int> pop_holder;
			while(pending.empty() == false){
				cout << "Pending is not empty \n";

				// iterates through each pending
				while (count < current_size){
					cout << "Current check: " << pending[count].output_name() << "\n";
					pending[count].output_prereq();
					//checks current count's prereq
					cout << "How many loops in this check? " << pending[count].prereq_size() << "\n";
					for (int x = 0; x < pending[count].prereq_size(); x++){
						
						//searches for output.
						//if found, store in a vector position
						for (int y = 0; y < output.size(); y++){
							if (pending[count].top_prereq() == output[y]){
								pending[count].remove_prereq(output[y]);
								action=1;
								cout << "Found " << output[y] << " ! Removed it\n";
								break;
							}
						}
					}
					//checks if pending has no prereq
					//stores into a vector of ints to be removed later
					if (pending[count].has_prereq() == false){
						cout << "Pushing " << pending[count].output_name() << " into output! \n";
						output.push_back(pending[count].output_name());
						pop_holder.push_back(count);
					}
					//}*/
					count++;
				}

				//removes the class from the pending vector if  
				//pops pending
				if (pop_holder.empty() == false){
					for (int i = 0; i < pop_holder.size(); i++){
						swap_to_back(pending,pop_holder[i]);
					//	pending.pop_back();
					}
				}
				if (pending.empty() == true and output.size() == course_list.size()){
					break;
				}
				else if (action == 1 && pending.empty() == false){
					current_size = current_size - pop_flag;
					pop_flag = 0;
					count=0;
					action=0;

				}
				else{
					std::cerr << "Error, an ineligible course found\n";
					output_vector(output);
					return 1;
				}
			}
			cout << "Course to take: \n";
			output_vector(output);
			
		}	

		//reads out classes

		cout << "\nProgram ended\n";
	}
	return 0;
}
