#include <set>
#include <iostream>


#include "boost/multi_index_container.hpp"  
#include "boost/multi_index/member.hpp"  
#include "boost/multi_index/ordered_index.hpp"  

using boost::multi_index_container;  
using namespace boost::multi_index; 

struct stu_num{};   // 索引-学号  
struct stu_name{};  // 索引-姓名  
struct stu_age{};   // 索引-年龄 

// 课程  
struct Course  
{  
    unsigned int        course_num;     // 课程编号  
    unsigned int        course_hour;    // 课时  
    std::string     course_name;    // 课程名  

    // 课程枚举  
    enum CourseNum_Enum  
    {  
        CourseNum_CPP       =   0,  // C++  
        CourseNum_English,              // 英语  
        CourseNum_Maths,                // 数学  
        CourseNum_Computer,         // 计算机  
        CourseNum_DataStructure         // 数据结构  
    };  

    static  const Course courses[5];  

    Course( unsigned int num, unsigned int hour, std::string name );  
};  

// 学生  
struct Student  
{  
    unsigned int        stu_num;        // 学号  
    std::string     stu_name;       // 姓名  
    unsigned int        stu_age;        // 年龄  
    std::set<Course>  stu_couselist;  // 主修课程表  

    friend Student CreateStudent( const std::string& name, unsigned int age );  

private:  
    Student( unsigned int num, const std::string& name, unsigned int age );  
};  

// 预定义一些课程  
const Course Course::courses[5] = {   
    Course( CourseNum_CPP, 80, "C++程序设计" ),  
    Course( CourseNum_English, 60, "大学英语" ),  
    Course( CourseNum_Maths, 45, "离散数学" ),  
    Course( CourseNum_Computer, 50, "计算机组成原理" ),  
    Course( CourseNum_DataStructure, 60, "数据结构" )  

};  


typedef  
boost::multi_index_container<  
Student,  
    indexed_by<  
    ordered_unique<  
    // 学号是唯一值的索引  
    tag<stu_num>,  BOOST_MULTI_INDEX_MEMBER(Student,unsigned int,stu_num)>,  
    // 姓名是非唯一值的索引  
    ordered_non_unique<  
    tag<stu_name>,BOOST_MULTI_INDEX_MEMBER(Student,std::string,stu_name)>,  
    // 年龄是非唯一值的索引  
    ordered_non_unique<  
    tag<stu_age>, BOOST_MULTI_INDEX_MEMBER(Student,unsigned int,stu_age)>    
    >  
    > StudentContainer;  

    template<typename Tag,typename MultiIndexContainer>  
void print_out_by( const MultiIndexContainer& s )  
{  
    /* obtain a reference to the index tagged by Tag */  

    const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i = get<Tag>(s);  

    typedef typename MultiIndexContainer::value_type value_type;  

    /* dump the elements of the index to cout */  

    std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));  
} 


int main()
{

    StudentContainer studentsets;  

    // 插入一些数据  
    Student& stu1 = CreateStudent( "张三", 19 );  
    stu1.stu_couselist.insert( Course::courses[Course::CourseNum_CPP] );  
    stu1.stu_couselist.insert( Course::courses[Course::CourseNum_English] );  
    studentsets.insert(stu1);  

    Student& stu2 = CreateStudent("李四", 18);  
    stu2.stu_couselist.insert( Course::courses[Course::CourseNum_CPP] );  
    stu2.stu_couselist.insert( Course::courses[Course::CourseNum_DataStructure] );  
    stu2.stu_couselist.insert( Course::courses[Course::CourseNum_Computer] );  
    studentsets.insert(stu2);  

    Student& stu3 = CreateStudent("王五", 21);  
    stu3.stu_couselist.insert( Course::courses[Course::CourseNum_English] );  
    stu3.stu_couselist.insert( Course::courses[Course::CourseNum_Maths] );  
    studentsets.insert(stu3);  

    Student& stu4 = CreateStudent("张三", 18);  
    stu4.stu_couselist.insert( Course::courses[Course::CourseNum_Computer] );  
    stu4.stu_couselist.insert( Course::courses[Course::CourseNum_Maths] );  
    studentsets.insert(stu4);  

    // 按学号排序输出序列  
    print_out_by<stu_num>(studentsets);
