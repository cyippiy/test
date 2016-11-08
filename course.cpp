#include "course.h"
#include <string>
#include <iostream>
#include <vector>

course::course(){
	name = "";
	prereq.clear();
	prereq_count = 0;
}

course::course(const std::string & n){
	name = n;
	prereq.clear();
	prereq_count = 0;
}

course::course(const std::string & n,const std::vector<std::string> & p){
	name = n;
	prereq = p;
	prereq_count = prereq.size();
}

course& course::operator=(const course& c){
	name = c.name;
	prereq = c.prereq;
	prereq_count = c.prereq_count;
	return *this;
}


void course::set_prereq(const std::vector<std::string>& n){
	prereq = n;
	prereq_count = n.size();
}

std::string course::get_name(){
	return name;
}

void course::set_name(const std::string & n){
	name = n;
}

void course::add_prereq(const std::string & n){
	prereq.push_back(n);
	prereq_count++;
}

void course::remove_prereq(const std::string & n){
	for (int i = 0; i <= prereq_count; i++){
		if (prereq[i] == n){
			prereq.erase(prereq.begin()+i);
			prereq_count--;
			break;
		}
	}
}

void course::copy_prereq(const std::vector<std::string> & v, int i){
	prereq = v;
	prereq_count = i;
}

bool course::has_prereq() const{
	if (prereq_count > 0){
		return true;
	}
	else{
		return false;
	}
}

int course::prereq_size() const{
	return prereq_count;
}

void course::output_prereq() const{
	std::cout << "Prereq list for " << name << " to take:\n";
	for (int i = 0; i < prereq_count; i++){
		std::cout << prereq[i] << "\n";
	}
}

std::string course::output_name() const{
	return name;
}

std::vector<std::string> course::link_prereq() const{
	return prereq;
}
void course::set_prereq_count(int i){
	prereq_count = i;
}

std::string course::top_prereq() const{
	return prereq[0];
}