#ifndef __COURSE_H__
#define __COURSE_H__

#include <string>
#include <vector>

class course{
private:
	std::string name;
	std::vector<std::string> prereq;
	int prereq_count;
public:
	course();
	course(const std::string & n);
	course(const std::string & n, const std::vector<std::string> & prereq );
	course& operator=(const course & c);
	std::string get_name();
	void set_name(const std::string & n);
	void add_prereq(const std::string & n);
	void copy_prereq(const std::vector<std::string> & v, int i);
	void remove_prereq(const std::string & n);
	void set_prereq(const std::vector<std::string>& n);
	bool has_prereq() const;
	int prereq_size() const;
	void output_prereq() const;
	std::string output_name() const;
	std::vector<std::string> link_prereq() const;
	void set_prereq_count(int i);
	std::string top_prereq() const;
};

#endif