#include"InitStudents.h"

//������������(����xΪ��)
inline string RealName(int index)
{
    return "��" + to_string(index + 1);
}

// ��������(��x)
inline string ScreenName(int index)
{
    return "��" + to_string(index + 1);
}

// ���ɳɼ�60-100
inline int RandomScore()
{
    return (rand() % 41 + 60);
}

//����ѧ����Ϣ
void InitStudents()
{
    srand((unsigned int)time(NULL)); //��ʼ�������
    ofstream outFile("students.txt"); // ����ļ�

    if (!outFile) //���ϲ���������
    {
        cerr << "�޷������ļ�!" << endl;
        exit(1);
    }

    for (int i = 0; i < default_studentCount; ++i) 
    {
        //��ѧ��ͳһ��ʽ
        ostringstream studentIDStream;
        studentIDStream << setw(defaultlength_classNo) << setfill('0') << default_classNo
            << setw(defaultlength_studentID - defaultlength_classNo) << setfill('0') << (i + 1);
        string studentID = studentIDStream.str();  

        string name = RealName(i); //��ʵ����
        string screenName = ScreenName(i); //����
        int math = RandomScore(); // ��ѧ�ɼ�
        int english = RandomScore(); //Ӣ��ɼ�
        int dataStructure = RandomScore(); // ���ݽṹ�ɼ�

        // д���ļ�
        outFile << studentID << " " << name << " " << screenName << " "
            << default_classNo << " " << math << " "
            << english << " " << dataStructure << endl;
    }

    //�ر��ļ�
    outFile.close();
    cout << "ѧ����Ϣ�ѳɹ����ɲ����浽 students.txt �ļ���" << endl;
}