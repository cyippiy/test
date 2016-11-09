#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <string>
#include <regex>
#include "course.h"
#include <unordered_map>
#include <algorithm> 

using json = nlohmann::json;
using std::cout;

//didn't use this. tried using this to parse regex
//however C++ 11 doesn't like long regex strings
bool check_regex(const std::string & x){
	std::string format = "\\[(,*\\s\\s*\\{\\s+\"name\":\\s*\"(\\w+\\s*\\w)+\",\\s*\"prerequisites\":\\s*\\[(\"(\\w+\\s*\\w)+\",*\\s*)*\\]\\s*\\}\\s*)+\\]";
	std::regex test(format);
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

//santizes input. removed symbols that seem unecessary for course names
void clean_string(std::string & x){
	char chars[] = "()<>/\\[]\"";
	for (unsigned int i = 0; i < strlen(chars); i++){
		x.erase (std::remove(x.begin(),x.end(),chars[i]), x.end());
	}
}

bool isempty(const std::vector<std::string> & v){
	return std::all_of(v[0].begin(),v[0].end(),isspace);
}

//checks if vector is empty for course
bool isempty(const std::vector<course>& v){
	if (v.size() == 0){
		return true;
	}
	else{
		return false;
	}
}

//takes the prereq courses and loads them into course vector
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
	//cout << "outputting vector! \n";
	for (int i = 0; i < v.size(); i++){
		cout << v[i] << "\n";
	}
}

//outputs coursename of the vector of courses
//for testing purposes
void output_vector(std::vector<course> & v){
	for (int i = 0; i < v.size(); i++){
			cout << v[i].output_name() << "\n";
	}
}

//takes current element, which is a course object, swaps to back
//if only 3 or more, temp to 2nd last vector
void swap_to_back(std::vector<course>& v,const int &x){
	course temp = v[x];
	v[x] = v[v.size()-1];
	v[v.size()-1] = temp;
}


//deletes the pending vector with a vector of addresses
void pop_pending(std::vector<course>& v, std::vector<int>& x){
	std::vector<course> temp;
	for (int i = 0; i < x.size(); i++){
		temp.push_back(v[x[i]]);
	}
	for (int i = 0; i < temp.size(); i++){
		for (int j = 0; j < v.size(); j++ )
			if (temp[i] == v[j]){
				v.erase(v.begin()+j);
				break;
			}
	}
}


//converts the JSON into string
//takes in json
//converts the json into a vector of courses
void json_to_string(const json & j, std::vector<course>& course_list){
	std::string name;
	std::string prereq;
	std::vector<std::string> v_temp;
	int count;
	course course_temp;
	for (int i = 0; i < j.size(); i++){
		auto json_var = j[i].get<std::unordered_map<std::string, json>>();
		name = json_var["name"];
		clean_string(name);
		prereq = json_var["prerequisites"].dump();
		clean_string(prereq);
		load_prereq(prereq,v_temp);
		if (isempty(v_temp)){
			count = 0;
			course_temp.copy_prereq(v_temp,0);
		}
		else{
			count = v_temp.size();
			course_temp.copy_prereq(v_temp,count);
		}
		course_temp.set_name(name);
		course_list.push_back(course_temp);
		v_temp.clear();
	}
}

//takes a temp vector list of courses, the vector of no prereq course, and vector of pending course
//will sort the temp vector... O(nlogn)
//checks if it detects a duplicate entry
//removes it from each respective vector
void delete_duplicate(std::vector<std::string>& s,std::vector<std::string>& x, std::vector<course>& c){
	std::sort(s.begin(),s.end());
	int counter = 0;
	int size = s.size();
	int max_size = s.size();
	bool flag_delete= 0;
	bool complete = 0;
	std::vector<std::string> store;
	while(complete == 0){
		if (counter < size){
			for(int i = counter; i < size-1; i++){
				if (s[i] == s[i+1]){
					//cout << "found dupe: " << s[i] << "\n";
					store.push_back(s[i]);
					s.erase(s.begin()+i);
					flag_delete = 1;
					size--;
					counter = i;
					break;
				}
			}
			if (flag_delete == 1){
				complete = 0;
				flag_delete = 0;
			}
			else{
				complete = 1;
			}
		}
		else{
			complete = 1;
		}
	}
	for (int i = 0; i < store.size(); i++){
		for (int j = 0; j < x.size(); j++){
			if (store[i] == x[j]){
				x.erase(x.begin()+j);
				break;
			}
		}
	}

	for (int i = 0; i < store.size(); i++){
		for (int j = 0; j < c.size(); j++){
			if (store[i] == c[j].output_name()){
				c.erase(c.begin()+j);
				break;
			}
		}
	}
}

//takes in the course list, output list, pending list, and temp (for sorting later)
//divides the courses to whether it can be taken immediately or needs prereq
void separate_course(std::vector<course>& c, std::vector<std::string>& o, std::vector<course>& p, std::vector<std::string>& temp){			
	for (int i = 0; i < c.size(); i++){
		if (c[i].has_prereq() == false){
			o.push_back(c[i].output_name());
		}
		else{
			p.push_back(c[i]);
		}
		temp.push_back(c[i].output_name());
	}
}

//takes in the pending course vector, vector of output, and a flag for indefinite loop
//will loop until either pending vector is empty or sees indefinite loop
//stores into output vector in order with respect of prereq
void check_pending(std::vector<course>& pending,std::vector<std::string>& output, bool& flag){
	std::vector<int> pop_holder;
	int action = 0;
	int count = 0;
	int current_size = pending.size();

	while(pending.empty() == false){
		// iterates through each pending
		while (count < current_size){
			//checks current count's prereq
		
			//cycles through each prereq currently not found in output vector
			for (int x = 0; x < pending[count].prereq_size(); x++){
					
				//searches for output.
				//if found, store in a vector position, and removes from prereq
				for (int y = 0; y < output.size(); y++){
					if (pending[count].top_prereq() == output[y]){							
						pending[count].remove_prereq(output[y]);
						action=1;
						break;
					}
				}
			}
			//checks if pending has no prereq
			//stores into a vector of ints to be removed later
			if (pending[count].has_prereq() == false){
				output.push_back(pending[count].output_name());
				pop_holder.push_back(count);
			}
			count++;
		}

		//removes the class from the pending vector if  
		//pops pending
		if (pop_holder.empty() == false){
			pop_pending(pending,pop_holder);
		}
		//there has been an action and pending isn't empty yet. reset values
		if (action == 1 && pending.empty() == false){
			current_size = pending.size();
			count=0;
			action=0;
		}
		//no action was done, infinite loop detected
		else if (action == 0 && pending.empty() == false){
			flag=1;
			break;
		}
	}
}

//my original delete_duplicate function. was meant to do after pending was checked
//sorts the list into a temporary so it's easier to find the duplicate
//modifies output to remove any duplicate course
void delete_duplicate(std::vector<std::string>& s, std::vector<std::string>& output){
	std::sort(s.begin(),s.end());
	int counter = 0;
	int size = s.size();
	int max_size = s.size();
	bool flag_delete= 0;
	bool complete = 0;
	std::vector<std::string> store;
	while(complete == 0){
		if (counter < size){
			for(int i = counter; i < size-1; i++){
				if (s[i] == s[i+1]){
					cout << "found dupe: " << s[i] << "\n";
					store.push_back(s[i]);
					s.erase(s.begin()+i);
					flag_delete = 1;
					size--;
					counter = i;
					break;
				}
			}
			if (flag_delete == 1){
				complete = 0;
				flag_delete = 0;
			}
			else{
				complete = 1;
			}
		}
		else{
			complete = 1;
		}
	}
	for (int i = 0; i < store.size(); i++){
		for (int j = 0; j < output.size(); j++){
			if (store[i] == output[j]){
				output.erase(output.begin()+j);
				break;
			}
		}
	}
}

