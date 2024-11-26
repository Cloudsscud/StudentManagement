#include "StudentManagement.h"

// 哈希函数实现
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
        cout << "未找到文件，跳过加载。" << endl;
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

        // 检查负载因子是否大于等于0.75(链处理法）
        double loadfactor = static_cast<double>(Count()) / _RealNameHash.size();
        if (loadfactor >= 0.75)
        {
            Rehash(); // 扩容
        }
    }

    file.close();
    cout << "学生信息已成功从文件加载。" << endl;
}



void SM::SaveToFile()
{
    ofstream file(_Filename);
    if (!file.is_open())
    {
        cerr << "无法打开文件进行保存！" << endl;
        return;
    }

    // 遍历哈希表中的每个链表
    for (const auto& chain : _RealNameHash)
    {
        for (const auto& student : chain)
        {
            // 按文件格式保存数据
            file << student._id << " "           // 学号
                << student._realName << " "    // 真实姓名
                << student._screenName << " " // 网名
                << student._classNo << " "     // 班级号
                << student._math << " "       // 数学成绩
                << student._english << " "    // 英语成绩
                << student._dataStructure     // 数据结构成绩
                << endl;                      // 换行
        }
    }

    file.close(); // 关闭文件
    cout << "学生信息已成功保存到文件 " << _Filename << endl;
}

// 添加学生
void SM::AddStudent(const Student& s)
{
    // 计算真实姓名和网名的哈希索引
    size_t indexRN = DJBHash(s._realName, _RealNameHash.size());
    size_t indexSN = DJBHash(s._screenName, _ScreenNameHash.size());

    // 将学生信息添加到对应哈希表
    _RealNameHash[indexRN].push_back(s);
    _ScreenNameHash[indexSN].push_back(s);

    // 检查负载因子是否大于等于0.75(链处理法）
    double loadfactor = static_cast<double>(Count()) / _RealNameHash.size();
    if (loadfactor >= 0.75)
    {
        Rehash(); // 扩容
    }

    // 自动保存数据
    cout << "学生信息已添加" << endl;
    SaveToFile();
}

//获取目前总数据量
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
    // 新表是原表的两倍
    size_t newsize = _RealNameHash.size() * 2;

    // 新表
    vector<list<Student>> newRealNameHash(newsize);
    vector<list<Student>> newScreenNameHash(newsize);

    // 重新插入数据
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

    // 交换两表
    _RealNameHash = move(newRealNameHash);
    _ScreenNameHash = move(newScreenNameHash);

    cout << "扩容成功" << endl;
}

// 只能删除一张表的信息
//// 删除学生
//void SM::DeleteStudent(bool byRealName)
//{
//    string key;
//    cout << "输入要删除学生的" << (byRealName ? "真实姓名: " : "网名: ");
//    cin >> key;
//
//    //要操作的哈希表
//    auto& table = byRealName ? _RealNameHash : _ScreenNameHash;
//    size_t index = DJBHash(key, table.size());
//    auto& chain = table[index];
//
//    //删除目标学生
//    chain.remove_if([&](const Student& s)
//        {
//            return byRealName ? s._realName == key : s._screenName == key;
//        });
//
//    //保存数据
//    cout << "学生信息已删除" << endl;
//    SaveToFile();
//}

// 在两张表删除信息
void SM::DeleteStudent(bool byRealName)
{
    string key;
    cout << "输入要删除学生的" << (byRealName ? "真实姓名: " : "网名: ");
    cin >> key;

    // 要查找的主键和副键
    string fkey = key;
    string skey;

    // 确定主表和副表
    auto& ftable = byRealName ? _RealNameHash : _ScreenNameHash;
    auto& stable = byRealName ? _ScreenNameHash : _RealNameHash;

    // 查找目标学生
    size_t findex = DJBHash(fkey, ftable.size());
    auto& fchain = ftable[findex];
    bool found = false;

    // 遍历主表链表，查找目标学生信息并记录副键
    for (const auto& student : fchain)
    {
        // 找到
        if ((byRealName && student._realName == fkey) || (!byRealName && student._screenName == fkey))
        {
            skey = byRealName ? student._screenName : student._realName;
            found = true;
            break;
        }
    }
    // 未找到
    if (!found)
    {
        cout << "未找到该学生信息，删除失败。" << endl;
        return;
    }

    // 在两张表中删除目标学生
    //主表
    fchain.remove_if([&](const Student& student) 
        {
        return byRealName ? student._realName == fkey : student._screenName == fkey;
        });

    //副表
    size_t sindex = DJBHash(skey, stable.size());
    auto& schain = stable[sindex];
    schain.remove_if([&](const Student& student) 
        {
        return byRealName ? student._screenName == skey : student._realName == skey;
        });

    cout << "成功删除" << endl;

    // 保存数据
    SaveToFile();
}


// 显示学生
void SM::ShowStudents()
{
    for (const auto& chain : _RealNameHash)
    {
        for (const auto& student : chain)
        {
            cout << "学号: " << student._id << ", 姓名: " << student._realName
                << ", 网名: " << student._screenName << endl;
        }
    }
}

//查找学生
void SM::SearchStudent(bool byRealName)
{
    string key;
    cout << "输入要查询的" << (byRealName ? "真实姓名: " : "网名: ");
    cin >> key;

    // 选择对应的哈希表
    auto& table = byRealName ? _RealNameHash : _ScreenNameHash;
    size_t index = DJBHash(key, table.size());  // 计算索引值
    auto& chain = table[index];

    bool found = false;
    // 遍历链表寻找匹配的学生
    for (const auto& student : chain)
    {
        if ((byRealName && student._realName == key) || (!byRealName && student._screenName == key))
        {
            // 找到
            found = true;

            // 显示找到的学生详细信息
            cout << "\n找到学生！" << endl;
            cout << "学号: " << student._id << endl;
            cout << "真实姓名: " << student._realName << endl;
            cout << "网名: " << student._screenName << endl;
            cout << "班级号: " << student._classNo << endl;
            cout << "数学成绩: " << student._math << endl;
            cout << "英语成绩: " << student._english << endl;
            cout << "数据结构成绩: " << student._dataStructure << endl;
            cout << "————————————————————————————" << endl;
            break;
        }
    }

    if (!found)
    {
        cout << "未找到该学生！" << endl;
    }
}

//// 编辑学生
//void SM::EditStudent(bool byRealName)
//{
//    string key;
//    cout << "输入要编辑的" << (byRealName ? "真实姓名: " : "网名: ");
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
//            cout << "输入新的学生信息：" << endl;
//            cout << "学号: "; cin >> s._id;
//            cout << "真实姓名: "; cin >> s._realName;
//            cout << "网名: "; cin >> s._screenName;
//            cout << "班级号: "; cin >> s._classNo;
//            cout << "数学成绩: "; cin >> s._math;
//            cout << "英语成绩: "; cin >> s._english;
//            cout << "数据结构成绩: "; cin >> s._dataStructure;
//
//            SaveToFile();
//            cout << "学生信息已更新并保存到文件。" << endl;
//            return;
//        }
//    }
//
//    cout << "未找到学生！" << endl;
//}

void SM::EditStudent(bool byRealName)
{
    string key;
    cout << "输入要编辑的" << (byRealName ? "真实姓名: " : "网名: ");
    cin >> key;

    // 确定主表和副表
    auto& ftable = byRealName ? _RealNameHash : _ScreenNameHash;
    auto& stable = byRealName ? _ScreenNameHash : _RealNameHash;

    size_t findex = DJBHash(key, ftable.size());
    auto& fchain = ftable[findex];

    // 查找目标学生记录
    Student* ps = nullptr;
    for (auto& s : fchain)
    {
        // 找到
        if ((byRealName && s._realName == key) || (!byRealName && s._screenName == key))
        {
            ps = &s;
            break;
        }
    }
    //未找到
    if (!ps)
    {
        cout << "未找到学生信息" << endl;
        return;
    }

    // 获取副键
    string skey = byRealName ? ps->_screenName : ps->_realName;
    size_t sindex = DJBHash(skey, stable.size());
    auto& schain = stable[sindex];

    // 两表删除原有信息
    schain.remove_if([&](const Student& student) 
        {
        return student._realName == ps->_realName && student._screenName == ps->_screenName;
        });

    fchain.remove_if([&](const Student& student) 
        {
        return student._realName == ps->_realName && student._screenName == ps->_screenName;
        });

    // 输入新学生信息
    cout << "输入新的学生信息：" << endl;
    Student news;
    cout << "学号: "; cin >> news._id;
    cout << "真实姓名: "; cin >> news._realName;
    cout << "网名: "; cin >> news._screenName;
    cout << "班级号: "; cin >> news._classNo;
    cout << "数学成绩: "; cin >> news._math;
    cout << "英语成绩: "; cin >> news._english;
    cout << "数据结构成绩: "; cin >> news._dataStructure;

    // 重新插入到两张表中
    size_t new_findex = DJBHash(news._realName, _RealNameHash.size());
    size_t new_sindex = DJBHash(news._screenName, _ScreenNameHash.size());

    _RealNameHash[new_findex].push_back(news);
    _ScreenNameHash[new_sindex].push_back(news);

    cout << "学生信息已更改" << endl;

    // 保存数据
    SaveToFile();
}


//// 测试当前哈希函数的平均查找长度
//void SM::TestHash()
//{
//
//    double success = 0.0; // 成功查找
//    double failue = 0.0;     // 失败查找
//    size_t count = 0;    // 元素总数
//
//    // 遍历并统计成功查找长度
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
//    // 统计失败查找长度
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
//    cout << "成功查找的平均查找长度 (ASL): " << success << endl;
//    cout << "失败查找的平均查找长度 (ASL): " << failue << endl;
//}

void SM::TestHash()
{
    // 整合四个哈希函数
    vector<pair<string, size_t(*)(const string&, size_t)>> HashFunc = {
        {"SimpleHash", SimpleHash},
        {"SquareHash", SquareHash},
        {"DJBHash", DJBHash},
        {"BKDRHash", BKDRHash}
    };

    // 遍历每个哈希函数
    for (const auto& [name, hashFunc] : HashFunc)
    {
        // 以网名为索引都每个哈希函数都构建新的哈希表
        vector<list<Student>> testHashTable(_ScreenNameHash.size());

        //新表插入数据
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

        // 遍历并统计成功查找长度
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

        // 统计失败查找长度
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

        // 输出当前哈希函数的测试结果
        cout << name << " 的测试结果：" << endl;
        cout << "成功查找的平均查找长度 (ASL): " << success << endl;
        cout << "失败查找的平均查找长度 (ASL): " << failue << endl;
        cout << "————————————————————————————" << endl;
    }
}
