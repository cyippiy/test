#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <string>
#include <regex>
#include "course.h"
#include <unordered_map>

using json = nlohmann::json;
using std::cout;

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

//santizes input, only allowing
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
	cout << "outputting vector! \n";
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
void merge(std::vector<std::string>& s,int low, int mid, int high){
    int i = low;
    int k = low;
    int j = mid + 1;
    unsigned int count = s.size();
    std::vector<std::string> v;
    v.resize(count);
    while (i <= mid and j <= high){
        if (s[i] < s[j]){
            v[k] = s[i];
            k++;
            i++;
        }
        else{
            v[k] = s[j];
            k++;
            j++;
        }
    }
    while (i <= mid){
        v[k] = s[i];
        k++;
        i++;
    }
    while (j <= high){
        v[k] = s[j];
        k++;
        j++;
    }
    for (i = low; i < k; i++){
        s[i] = v[i];
    }
}

void mergesort(std::vector<std::string>& s, int low, int high){
    int mid;
    if (low < high){
        mid=(low+high)/2;
        mergesort(s,low,mid);
        mergesort(s,mid+1,high);
        merge(s,low,high,mid);
	}
}

void mergesort(std::vector<std::string>& s){
	int x = 0;
	int y = s.size();
	mergesort(s,0,s.size()-1);
}

void check_duplicate(std::vector<std::string> s){

}

