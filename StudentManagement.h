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

//ѧ����Ϣ
struct Student 
{
    string _id;            // ѧ��
    string _realName;      // ��ʵ����
    string _screenName;    // ����
    string _classNo;       // �༶��
    int _math;             // ��ѧ�ɼ�
    int _english;          // Ӣ��ɼ�
    int _dataStructure;    // ���ݽṹ�ɼ�
};

//�˵�ѡ��
enum Option 
{
    EXIT,      //�˳�
    ADD,       //���ѧ��
    SHOW,      //��ʾ����ѧ��
    SEARCH_RN, //����ʵ������ѯ
    SEARCH_SN, //��������ѯ
    EDIT_RN,   //����ʵ�����༭
    EDIT_SN,   //�������༭
    DELETE_RN, //����ʵ����ɾ��
    DELETE_SN,  //������ɾ��
    TEST_HASH   //���Թ�ϣ����
};

//��ϣ����
//���ַ�ֵ���
size_t SimpleHash(const string& key, size_t tableSize);
//ASCiiֵƽ����
size_t SquareHash(const string& key, size_t tableSize);
//DJB�㷨
size_t DJBHash(const string& key, size_t tableSize);
// BKDR�㷨
size_t BKDRHash(const string& key, size_t tableSize);

//ѧ������ StudentManagement
class SM 
{
private:
    vector<list<Student>> _RealNameHash;   //����ʵ���������Ĺ�ϣ��
    vector<list<Student>> _ScreenNameHash; //�����������Ĺ�ϣ��
    string _Filename = "students.txt";   //ѧ�������ļ� �����е���(ע�����ݵĸ�ʽ)

    //��ʼ������
    void InitFromFile();
    //��������
    void SaveToFile();

public:
    SM(size_t tableSize = 10)
        : _RealNameHash(tableSize)
        , _ScreenNameHash(tableSize)
    {
        if (tableSize == 0) 
        {
            cout << "��ϣ��Ĵ�С����Ϊ 0��" << endl;
            return;
        }
        InitFromFile();  //���ļ�����ѧ����Ϣ
    }

    ~SM()
    {
        SaveToFile();
    }
    //��ȡĿǰ��������
    size_t Count() const;
    // ���ݲ������������
    void Rehash();

    // ��
    void AddStudent(const Student& s);
    // ɾ     ����Ϊ����Ϊ��ʵ�������ң�Ϊ��������������
    void DeleteStudent(bool byRealName);
    // ��ʾ
    void ShowStudents();
    // ��
    void SearchStudent(bool byRealName);
    // ��
    void EditStudent(bool byRealName);
    // ���Ե�ǰ��ϣ������ƽ�����ҳ���
    void TestHash();
};