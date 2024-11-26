#include"InitStudents.h"

//生成中文姓名(以张x为例)
inline string RealName(int index)
{
    return "张" + to_string(index + 1);
}

// 生成网名(网x)
inline string ScreenName(int index)
{
    return "网" + to_string(index + 1);
}

// 生成成绩60-100
inline int RandomScore()
{
    return (rand() % 41 + 60);
}

//生成学生信息
void InitStudents()
{
    srand((unsigned int)time(NULL)); //初始化随机数
    ofstream outFile("students.txt"); // 输出文件

    if (!outFile) //故障并结束程序
    {
        cerr << "无法创建文件!" << endl;
        exit(1);
    }

    for (int i = 0; i < default_studentCount; ++i) 
    {
        //将学号统一格式
        ostringstream studentIDStream;
        studentIDStream << setw(defaultlength_classNo) << setfill('0') << default_classNo
            << setw(defaultlength_studentID - defaultlength_classNo) << setfill('0') << (i + 1);
        string studentID = studentIDStream.str();  

        string name = RealName(i); //真实姓名
        string screenName = ScreenName(i); //网名
        int math = RandomScore(); // 数学成绩
        int english = RandomScore(); //英语成绩
        int dataStructure = RandomScore(); // 数据结构成绩

        // 写入文件
        outFile << studentID << " " << name << " " << screenName << " "
            << default_classNo << " " << math << " "
            << english << " " << dataStructure << endl;
    }

    //关闭文件
    outFile.close();
    cout << "学生信息已成功生成并保存到 students.txt 文件中" << endl;
}