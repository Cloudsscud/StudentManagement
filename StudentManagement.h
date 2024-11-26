#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include<list>
#include<vector>
#include<string>
using namespace std;

//学生信息
struct Student 
{
    string _id;            // 学号
    string _realName;      // 真实姓名
    string _screenName;    // 网名
    string _classNo;       // 班级号
    int _math;             // 数学成绩
    int _english;          // 英语成绩
    int _dataStructure;    // 数据结构成绩
};

//菜单选项
enum Option 
{
    EXIT,      //退出
    ADD,       //添加学生
    SHOW,      //显示所有学生
    SEARCH_RN, //按真实姓名查询
    SEARCH_SN, //按网名查询
    EDIT_RN,   //按真实姓名编辑
    EDIT_SN,   //按网名编辑
    DELETE_RN, //按真实姓名删除
    DELETE_SN,  //按网名删除
    TEST_HASH   //测试哈希函数
};

//哈希函数
//简单字符值相加
size_t SimpleHash(const string& key, size_t tableSize);
//ASCii值平方和
size_t SquareHash(const string& key, size_t tableSize);
//DJB算法
size_t DJBHash(const string& key, size_t tableSize);
// BKDR算法
size_t BKDRHash(const string& key, size_t tableSize);

//学生管理 StudentManagement
class SM 
{
private:
    vector<list<Student>> _RealNameHash;   //按真实姓名索引的哈希表
    vector<list<Student>> _ScreenNameHash; //按网名索引的哈希表
    string _Filename = "students.txt";   //学生数据文件 可自行导入(注意数据的格式)

    //初始化数据
    void InitFromFile();
    //保存数据
    void SaveToFile();

public:
    SM(size_t tableSize = 10)
        : _RealNameHash(tableSize)
        , _ScreenNameHash(tableSize)
    {
        if (tableSize == 0) 
        {
            cout << "哈希表的大小不能为 0！" << endl;
            return;
        }
        InitFromFile();  //从文件加载学生信息
    }

    ~SM()
    {
        SaveToFile();
    }
    //获取目前数据数量
    size_t Count() const;
    // 扩容并重新填充数据
    void Rehash();

    // 增
    void AddStudent(const Student& s);
    // 删     参数为真则为真实姓名查找，为假则是网名查找
    void DeleteStudent(bool byRealName);
    // 显示
    void ShowStudents();
    // 查
    void SearchStudent(bool byRealName);
    // 改
    void EditStudent(bool byRealName);
    // 测试当前哈希函数的平均查找长度
    void TestHash();
};