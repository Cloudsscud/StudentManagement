#include "StudentManagement.h"

// ��ϣ����ʵ��
size_t SimpleHash(const string& key, size_t tableSize)
{
    size_t sum = 0;
    for (char c : key) 
        sum += c;
    return sum % tableSize;
}

size_t SquareHash(const string& key, size_t tableSize)
{
    size_t sum = 0;
    for (char c : key) 
        sum += c * c;
    return sum % tableSize;
}

size_t DJBHash(const string& key, size_t tableSize)
{
    size_t hash = 5381;
    for (char c : key) 
        hash = ((hash << 5) + hash) + c;
    return hash % tableSize;
}

size_t BKDRHash(const string& key, size_t tableSize)
{
    size_t seed = 131;
    size_t hash = 0;

    for (char c : key)
    {
        hash = hash * seed + c;
    }

    return hash % tableSize;
}

void SM::InitFromFile()
{
    ifstream file(_Filename);
    if (!file.is_open())
    {
        cout << "δ�ҵ��ļ����������ء�" << endl;
        return;
    }

    Student s;
    while (file >> s._id >> s._realName >> s._screenName >> s._classNo
        >> s._math >> s._english >> s._dataStructure)
    {
        //size_t indexRN = BKDRHash(s._realName, _RealNameHash.size());
        //size_t indexSN = BKDRHash(s._screenName, _ScreenNameHash.size());

        //size_t indexRN = SimpleHash(s._realName, _RealNameHash.size());
        //size_t indexSN = SimpleHash(s._screenName, _ScreenNameHash.size());

        //size_t indexRN = SquareHash(s._realName, _RealNameHash.size());
        //size_t indexSN = SquareHash(s._screenName, _ScreenNameHash.size());

        size_t indexRN = DJBHash(s._realName, _RealNameHash.size());
        size_t indexSN = DJBHash(s._screenName, _ScreenNameHash.size());

        _RealNameHash[indexRN].push_back(s);
        _ScreenNameHash[indexSN].push_back(s);

        // ��鸺�������Ƿ���ڵ���0.75(��������
        double loadfactor = static_cast<double>(Count()) / _RealNameHash.size();
        if (loadfactor >= 0.75)
        {
            Rehash(); // ����
        }
    }

    file.close();
    cout << "ѧ����Ϣ�ѳɹ����ļ����ء�" << endl;
}



void SM::SaveToFile()
{
    ofstream file(_Filename);
    if (!file.is_open())
    {
        cerr << "�޷����ļ����б��棡" << endl;
        return;
    }

    // ������ϣ���е�ÿ������
    for (const auto& chain : _RealNameHash)
    {
        for (const auto& student : chain)
        {
            // ���ļ���ʽ��������
            file << student._id << " "           // ѧ��
                << student._realName << " "    // ��ʵ����
                << student._screenName << " " // ����
                << student._classNo << " "     // �༶��
                << student._math << " "       // ��ѧ�ɼ�
                << student._english << " "    // Ӣ��ɼ�
                << student._dataStructure     // ���ݽṹ�ɼ�
                << endl;                      // ����
        }
    }

    file.close(); // �ر��ļ�
    cout << "ѧ����Ϣ�ѳɹ����浽�ļ� " << _Filename << endl;
}

// ���ѧ��
void SM::AddStudent(const Student& s)
{
    // ������ʵ�����������Ĺ�ϣ����
    size_t indexRN = DJBHash(s._realName, _RealNameHash.size());
    size_t indexSN = DJBHash(s._screenName, _ScreenNameHash.size());

    // ��ѧ����Ϣ��ӵ���Ӧ��ϣ��
    _RealNameHash[indexRN].push_back(s);
    _ScreenNameHash[indexSN].push_back(s);

    // ��鸺�������Ƿ���ڵ���0.75(��������
    double loadfactor = static_cast<double>(Count()) / _RealNameHash.size();
    if (loadfactor >= 0.75)
    {
        Rehash(); // ����
    }

    // �Զ���������
    cout << "ѧ����Ϣ�����" << endl;
    SaveToFile();
}

//��ȡĿǰ��������
size_t SM::Count() const
{
    size_t count = 0;

    for (const auto& chain : _RealNameHash)
    {
        count += chain.size();
    }

    return count;
}

void SM::Rehash()
{
    // �±���ԭ�������
    size_t newsize = _RealNameHash.size() * 2;

    // �±�
    vector<list<Student>> newRealNameHash(newsize);
    vector<list<Student>> newScreenNameHash(newsize);

    // ���²�������
    for (const auto& chain : _RealNameHash)
    {
        for (const auto& student : chain)
        {
            size_t newIndexRN = DJBHash(student._realName, newsize);
            newRealNameHash[newIndexRN].push_back(student);
        }
    }

    for (const auto& chain : _ScreenNameHash)
    {
        for (const auto& student : chain)
        {
            size_t newIndexSN = DJBHash(student._screenName, newsize);
            newScreenNameHash[newIndexSN].push_back(student);
        }
    }

    // ��������
    _RealNameHash = move(newRealNameHash);
    _ScreenNameHash = move(newScreenNameHash);

    cout << "���ݳɹ�" << endl;
}

// ֻ��ɾ��һ�ű����Ϣ
//// ɾ��ѧ��
//void SM::DeleteStudent(bool byRealName)
//{
//    string key;
//    cout << "����Ҫɾ��ѧ����" << (byRealName ? "��ʵ����: " : "����: ");
//    cin >> key;
//
//    //Ҫ�����Ĺ�ϣ��
//    auto& table = byRealName ? _RealNameHash : _ScreenNameHash;
//    size_t index = DJBHash(key, table.size());
//    auto& chain = table[index];
//
//    //ɾ��Ŀ��ѧ��
//    chain.remove_if([&](const Student& s)
//        {
//            return byRealName ? s._realName == key : s._screenName == key;
//        });
//
//    //��������
//    cout << "ѧ����Ϣ��ɾ��" << endl;
//    SaveToFile();
//}

// �����ű�ɾ����Ϣ
void SM::DeleteStudent(bool byRealName)
{
    string key;
    cout << "����Ҫɾ��ѧ����" << (byRealName ? "��ʵ����: " : "����: ");
    cin >> key;

    // Ҫ���ҵ������͸���
    string fkey = key;
    string skey;

    // ȷ������͸���
    auto& ftable = byRealName ? _RealNameHash : _ScreenNameHash;
    auto& stable = byRealName ? _ScreenNameHash : _RealNameHash;

    // ����Ŀ��ѧ��
    size_t findex = DJBHash(fkey, ftable.size());
    auto& fchain = ftable[findex];
    bool found = false;

    // ����������������Ŀ��ѧ����Ϣ����¼����
    for (const auto& student : fchain)
    {
        // �ҵ�
        if ((byRealName && student._realName == fkey) || (!byRealName && student._screenName == fkey))
        {
            skey = byRealName ? student._screenName : student._realName;
            found = true;
            break;
        }
    }
    // δ�ҵ�
    if (!found)
    {
        cout << "δ�ҵ���ѧ����Ϣ��ɾ��ʧ�ܡ�" << endl;
        return;
    }

    // �����ű���ɾ��Ŀ��ѧ��
    //����
    fchain.remove_if([&](const Student& student) 
        {
        return byRealName ? student._realName == fkey : student._screenName == fkey;
        });

    //����
    size_t sindex = DJBHash(skey, stable.size());
    auto& schain = stable[sindex];
    schain.remove_if([&](const Student& student) 
        {
        return byRealName ? student._screenName == skey : student._realName == skey;
        });

    cout << "�ɹ�ɾ��" << endl;

    // ��������
    SaveToFile();
}


// ��ʾѧ��
void SM::ShowStudents()
{
    for (const auto& chain : _RealNameHash)
    {
        for (const auto& student : chain)
        {
            cout << "ѧ��: " << student._id << ", ����: " << student._realName
                << ", ����: " << student._screenName << endl;
        }
    }
}

//����ѧ��
void SM::SearchStudent(bool byRealName)
{
    string key;
    cout << "����Ҫ��ѯ��" << (byRealName ? "��ʵ����: " : "����: ");
    cin >> key;

    // ѡ���Ӧ�Ĺ�ϣ��
    auto& table = byRealName ? _RealNameHash : _ScreenNameHash;
    size_t index = DJBHash(key, table.size());  // ��������ֵ
    auto& chain = table[index];

    bool found = false;
    // ��������Ѱ��ƥ���ѧ��
    for (const auto& student : chain)
    {
        if ((byRealName && student._realName == key) || (!byRealName && student._screenName == key))
        {
            // �ҵ�
            found = true;

            // ��ʾ�ҵ���ѧ����ϸ��Ϣ
            cout << "\n�ҵ�ѧ����" << endl;
            cout << "ѧ��: " << student._id << endl;
            cout << "��ʵ����: " << student._realName << endl;
            cout << "����: " << student._screenName << endl;
            cout << "�༶��: " << student._classNo << endl;
            cout << "��ѧ�ɼ�: " << student._math << endl;
            cout << "Ӣ��ɼ�: " << student._english << endl;
            cout << "���ݽṹ�ɼ�: " << student._dataStructure << endl;
            cout << "��������������������������������������������������������" << endl;
            break;
        }
    }

    if (!found)
    {
        cout << "δ�ҵ���ѧ����" << endl;
    }
}

//// �༭ѧ��
//void SM::EditStudent(bool byRealName)
//{
//    string key;
//    cout << "����Ҫ�༭��" << (byRealName ? "��ʵ����: " : "����: ");
//    cin >> key;
//
//    auto& table = byRealName ? _RealNameHash : _ScreenNameHash;
//    size_t index = DJBHash(key, table.size());
//    auto& chain = table[index];
//
//    for (auto& s : chain)
//    {
//        if ((byRealName && s._realName == key) || (!byRealName && s._screenName == key))
//        {
//            cout << "�����µ�ѧ����Ϣ��" << endl;
//            cout << "ѧ��: "; cin >> s._id;
//            cout << "��ʵ����: "; cin >> s._realName;
//            cout << "����: "; cin >> s._screenName;
//            cout << "�༶��: "; cin >> s._classNo;
//            cout << "��ѧ�ɼ�: "; cin >> s._math;
//            cout << "Ӣ��ɼ�: "; cin >> s._english;
//            cout << "���ݽṹ�ɼ�: "; cin >> s._dataStructure;
//
//            SaveToFile();
//            cout << "ѧ����Ϣ�Ѹ��²����浽�ļ���" << endl;
//            return;
//        }
//    }
//
//    cout << "δ�ҵ�ѧ����" << endl;
//}

void SM::EditStudent(bool byRealName)
{
    string key;
    cout << "����Ҫ�༭��" << (byRealName ? "��ʵ����: " : "����: ");
    cin >> key;

    // ȷ������͸���
    auto& ftable = byRealName ? _RealNameHash : _ScreenNameHash;
    auto& stable = byRealName ? _ScreenNameHash : _RealNameHash;

    size_t findex = DJBHash(key, ftable.size());
    auto& fchain = ftable[findex];

    // ����Ŀ��ѧ����¼
    Student* ps = nullptr;
    for (auto& s : fchain)
    {
        // �ҵ�
        if ((byRealName && s._realName == key) || (!byRealName && s._screenName == key))
        {
            ps = &s;
            break;
        }
    }
    //δ�ҵ�
    if (!ps)
    {
        cout << "δ�ҵ�ѧ����Ϣ" << endl;
        return;
    }

    // ��ȡ����
    string skey = byRealName ? ps->_screenName : ps->_realName;
    size_t sindex = DJBHash(skey, stable.size());
    auto& schain = stable[sindex];

    // ����ɾ��ԭ����Ϣ
    schain.remove_if([&](const Student& student) 
        {
        return student._realName == ps->_realName && student._screenName == ps->_screenName;
        });

    fchain.remove_if([&](const Student& student) 
        {
        return student._realName == ps->_realName && student._screenName == ps->_screenName;
        });

    // ������ѧ����Ϣ
    cout << "�����µ�ѧ����Ϣ��" << endl;
    Student news;
    cout << "ѧ��: "; cin >> news._id;
    cout << "��ʵ����: "; cin >> news._realName;
    cout << "����: "; cin >> news._screenName;
    cout << "�༶��: "; cin >> news._classNo;
    cout << "��ѧ�ɼ�: "; cin >> news._math;
    cout << "Ӣ��ɼ�: "; cin >> news._english;
    cout << "���ݽṹ�ɼ�: "; cin >> news._dataStructure;

    // ���²��뵽���ű���
    size_t new_findex = DJBHash(news._realName, _RealNameHash.size());
    size_t new_sindex = DJBHash(news._screenName, _ScreenNameHash.size());

    _RealNameHash[new_findex].push_back(news);
    _ScreenNameHash[new_sindex].push_back(news);

    cout << "ѧ����Ϣ�Ѹ���" << endl;

    // ��������
    SaveToFile();
}


//// ���Ե�ǰ��ϣ������ƽ�����ҳ���
//void SM::TestHash()
//{
//
//    double success = 0.0; // �ɹ�����
//    double failue = 0.0;     // ʧ�ܲ���
//    size_t count = 0;    // Ԫ������
//
//    // ������ͳ�Ƴɹ����ҳ���
//    for (const auto& chain : _ScreenNameHash)
//    {
//        size_t chainLength = chain.size();
//        if (chainLength > 0)
//        {
//            for (size_t i = 1; i <= chainLength; ++i)
//            {
//                success += i;
//            }
//            count += chainLength;
//        }
//    }
//
//    if (count > 0)
//        success /= _ScreenNameHash.size();
//
//    // ͳ��ʧ�ܲ��ҳ���
    //for (const auto& chain : _ScreenNameHash)
    //{
    //    if (chain.size())
    //    {
    //        failue += chain.size();
    //    }
    //    else
    //    {
    //        failue += 1;
    //    }
    //}
//
//    failue /= _ScreenNameHash.size();
//
//    cout << "�ɹ����ҵ�ƽ�����ҳ��� (ASL): " << success << endl;
//    cout << "ʧ�ܲ��ҵ�ƽ�����ҳ��� (ASL): " << failue << endl;
//}

void SM::TestHash()
{
    // �����ĸ���ϣ����
    vector<pair<string, size_t(*)(const string&, size_t)>> HashFunc = {
        {"SimpleHash", SimpleHash},
        {"SquareHash", SquareHash},
        {"DJBHash", DJBHash},
        {"BKDRHash", BKDRHash}
    };

    // ����ÿ����ϣ����
    for (const auto& [name, hashFunc] : HashFunc)
    {
        // ������Ϊ������ÿ����ϣ�����������µĹ�ϣ��
        vector<list<Student>> testHashTable(_ScreenNameHash.size());

        //�±��������
        for (const auto& chain : _ScreenNameHash)
        {
            for (const auto& student : chain)
            {
                size_t index = hashFunc(student._screenName, testHashTable.size());
                testHashTable[index].push_back(student);
            }
        }

        double success = 0.0;
        double failue = 0.0;
        size_t count = 0;

        // ������ͳ�Ƴɹ����ҳ���
        for (const auto& chain : testHashTable)
        {
            size_t chainLength = chain.size();
            if (chainLength > 0)
            {
                for (size_t i = 1; i <= chainLength; ++i)
                {
                    success += i;
                }
                count += chainLength;
            }
        }

        if (count > 0)
        {
            success /= testHashTable.size();
        }

        // ͳ��ʧ�ܲ��ҳ���
        for (const auto& chain : _ScreenNameHash)
        {
            if (chain.size())
            {
                failue += chain.size();
            }
            else
            {
                failue += 1;
            }
        }
        failue /= testHashTable.size();

        // �����ǰ��ϣ�����Ĳ��Խ��
        cout << name << " �Ĳ��Խ����" << endl;
        cout << "�ɹ����ҵ�ƽ�����ҳ��� (ASL): " << success << endl;
        cout << "ʧ�ܲ��ҵ�ƽ�����ҳ��� (ASL): " << failue << endl;
        cout << "��������������������������������������������������������" << endl;
    }
}
