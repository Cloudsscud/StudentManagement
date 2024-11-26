#include "StudentManagement.h"
#include "InitStudents.h"

void menu() 
{
    cout << "*************************" << endl;
    cout << "1. ���ѧ��" << endl;
    cout << "2. ��ʾ����ѧ��" << endl;
    cout << "3. ����ʵ������ѯ" << endl;
    cout << "4. ��������ѯ" << endl;
    cout << "5. ����ʵ�����༭" << endl;
    cout << "6. �������༭" << endl;
    cout << "7. ����ʵ����ɾ��" << endl;
    cout << "8. ������ɾ��" << endl;
    cout << "9. ���Ե�ǰʹ�õĹ�ϣ����" << endl;
    cout << "0. �˳�" << endl;
    cout << "*************************" << endl;
    cout << "��ѡ��: ";
}

int main() 
{
    // ������Ϣ��
    //InitStudents(); //�Զ�����ѧ����Ϣ

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
            cout << "����ѧ����Ϣ��" << endl;
            cout << "ѧ��: "; cin >> s._id;
            cout << "��ʵ����: ";cin >> s._realName;
            cout << "����: "; cin >> s._screenName;
            cout << "�༶��: "; cin >> s._classNo;
            cout << "��ѧ�ɼ�: "; cin >> s._math;
            cout << "Ӣ��ɼ�: ";cin >> s._english;
            cout << "���ݽṹ�ɼ�: "; cin >> s._dataStructure;
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
            cout << "��Ч���룡" << endl;
        }
    }
}
