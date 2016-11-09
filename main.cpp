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
			int count;
			course course_var;

			try{
				j = json::parse(json_content);	
			}
			catch (const std::invalid_argument& ia){
				std::cerr << "Error: Argument is invalid JSON file\n";
				return 1;
			}

			cout << "Size of Json array: " << j.size() << "\n";
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

			for (int i = 0; i < course_list.size(); i++){
				if (course_list[i].has_prereq() == false){
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

			int action = 0;
			count = 0;
			int current_size = pending.size();
			if (output.empty() == true){
				std::cerr << "Error: no classes without preque were found\n";
				return 1;
			}
			std::vector<int> pop_holder;
		//	cout << "\n\n\n\n";


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

			//cycles through the pending course list
			while(pending.empty() == false){
				// iterates through each pending
				while (count < current_size){
					cout << "Current check: " << pending[count].output_name() << "\n\n";
					pending[count].output_prereq();
					//checks current count's prereq
					//cout << "How many loops in this check? " << pending[count].prereq_size() << "\n";

					//cycles through each prereq currently not found in output vector
					for (int x = 0; x < pending[count].prereq_size(); x++){
						
						//searches for output.
						//if found, store in a vector position, and removes from prereq
						for (int y = 0; y < output.size(); y++){
							if (pending[count].top_prereq() == output[y]){
								pending[count].remove_prereq(output[y]);
								action=1;
								cout << "Found " << output[y] << " ! Removed it\n\n";
								break;
							}
						}
					}
					//checks if pending has no prereq
					//stores into a vector of ints to be removed later
					if (pending[count].has_prereq() == false){
						cout << "Pushing " << pending[count].output_name() << " into output! \n\n";
						output.push_back(pending[count].output_name());
						pop_holder.push_back(count);
					}
					//}*/
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
					std::cerr << "Error, an ineligible course found\n";
					output_vector(output);
					return 1;
				}
			}


			cout << "Course to take: \n";
		//	output_vector(output);

			
			cout << "\n\n";
			std::vector<std::string> test=output;
			/*if (test[1] < test[0])
			{
				cout << "test " << test[1] << " wins\n";
			}
			else{
				cout << "test "<< test[0] <<" wins\n";
			*/
			//mergesort(test);
			delete_duplicate(test,output);
			cout << "Course to take: \n";
			output_vector(test);
			

		}	

		//reads out classes

		cout << "\nProgram ended\n";
	}
	return 0;
}
