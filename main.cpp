#include "StudentManagement.h"
#include "InitStudents.h"

void menu() 
{
    cout << "*************************" << endl;
    cout << "1. 添加学生" << endl;
    cout << "2. 显示所有学生" << endl;
    cout << "3. 按真实姓名查询" << endl;
    cout << "4. 按网名查询" << endl;
    cout << "5. 按真实姓名编辑" << endl;
    cout << "6. 按网名编辑" << endl;
    cout << "7. 按真实姓名删除" << endl;
    cout << "8. 按网名删除" << endl;
    cout << "9. 测试当前使用的哈希函数" << endl;
    cout << "0. 退出" << endl;
    cout << "*************************" << endl;
    cout << "请选择: ";
}

int main() 
{
    // 重置信息用
    //InitStudents(); //自动生成学生信息

    SM sm;
    system("cls");
    while (true) 
    {
        menu();

        int choice;
        cin >> choice;

        switch (choice) 
        {
        case ADD: 
        {
            Student s;
            cout << "输入学生信息：" << endl;
            cout << "学号: "; cin >> s._id;
            cout << "真实姓名: ";cin >> s._realName;
            cout << "网名: "; cin >> s._screenName;
            cout << "班级号: "; cin >> s._classNo;
            cout << "数学成绩: "; cin >> s._math;
            cout << "英语成绩: ";cin >> s._english;
            cout << "数据结构成绩: "; cin >> s._dataStructure;
            sm.AddStudent(s);
            break;
        }
        case SHOW: 
            sm.ShowStudents(); 
            break;
        case SEARCH_RN: 
            sm.SearchStudent(true); 
            break;
        case SEARCH_SN: 
            sm.SearchStudent(false); 
            break;
        case EDIT_RN: 
            sm.EditStudent(true); 
            break;
        case EDIT_SN: 
            sm.EditStudent(false); 
            break;
        case DELETE_RN: 
            sm.DeleteStudent(true); 
            break;
        case DELETE_SN: 
            sm.DeleteStudent(false); 
            break;
        case TEST_HASH: 
            sm.TestHash(); 
            break;
        case EXIT: 
            return 0;
        default: 
            cout << "无效输入！" << endl;
        }
    }
}
