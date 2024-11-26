#pragma once

//班号
#define default_classNo 10
//班号规整位数
#define defaultlength_classNo 3
//学号最终位数
#define defaultlength_studentID 6
//随机生成的学生数
#define default_studentCount 2000

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//随机数生成
#include <cstdlib>
#include <ctime>
//统一位数
#include <iomanip>

using namespace std;

//生成学生信息,并存入文件中
void InitStudents();