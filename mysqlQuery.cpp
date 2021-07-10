#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <mysql/mysql.h>
#include <jsoncpp/json/json.h>

#define MYSQL_HOST      "127.0.0.1"
#define MYSQL_DB_NAME   "Blog_system"
#define MYSQL_USER      "Root"
#define MYSQL_PASS      "11111111"

namespace blog_system
{
    // ��ʼ��mysql
    static MYSQL* MysqlInit()
    {
        MYSQL* mysql = nullptr;
        // 1.��ʼ��mysql���
        mysql = mysql_init(nullptr);
        if (mysql == nullptr)
        {
            std::cout << "mysql init failed��" << std::endl;
            return nullptr;
        }
        // 2.����mysql������
        if (mysql_real_connect(mysql, "MYSQL_HOST", "MYSQL_USER", "MYSQL_PASS", "MYSQL_DB_NAME", 0, nullptr, 0) == nullptr)
        {
            std::cout << mysql_error(mysql) << "��mysql connect failed��" << std::endl;
            mysql_close(mysql);
            return nullptr;
        }
        // 3.����mysql�ͻ����ַ����뼯
        if (mysql_set_character_set(mysql, "uft8") != 0)
        {
            std::cout << mysql_error(mysql) << "��set character failed��" << std::endl;
            mysql_close(mysql);
            return nullptr;
        }

        return mysql;
    }
    // �ر����ݿ⣬�ͷ�mysql�������
    static void MysqlRelease(MYSQL* mysql)
    {
        mysql_close(mysql);
    }
    // mysqlִ��
    static bool MysqlQuery(MYSQL* mysql, const std::string& sql)
    {
        int ret = mysql_query(mysql, sql.c_str());
        if (ret != 0)
        {
            std::cout << mysql_error(mysql) << "��query sql��[" << sql.c_str() << "] error��" << std::endl;
            return false;
        }
        return true;
    }

    // ��ǩ��Ϣ��
    class TableTag
    {
    public:
        TableTag(MYSQL* mysql) : _mysql(mysql) {}
        
        // ����һ���±�ǩ
        bool Insert(const Json::Value& tag)
        {
            // 1.��֯sql���
#define INSERT_TAG "insert table_tag value(null, '%s');"
            char tmp[4096] = {0};

            // Json::Value.asCString(),����C�����ַ���
            // sprintf() ����ָ����ʽ�����������֯��Ϊһ���ַ����ŵ�tmp��
            sprintf(tmp, INSERT_TAG, tag["name"].asCString());

            // 2.ִ��sql���
            bool ret = MysqlQuery(_mysql, tmp);
            if (!ret)
            {
                std::cout << "Insert failed��" << std::endl;
                return false;
            }
            return true; // ÿ���Ȱѷ���ֵ���ϣ�û�з���ֵ������£�Ĭ�Ϸ��ؼ�
        }
    
        // ɾ��һ����ǩ
        bool Delete(int tag_id)
        {
            // 1.��֯sql���
#define DELETE_TAG "delete from table_tag where id=%d;"
            char tmp[4096] = {0};
            sprintf(tmp, DELETE_TAG, tag_id);

            // 2.ִ��sql���
            bool ret = MysqlQuery(_mysql, tmp);
            if (!ret)
            {
                std::cout << "delete failed��" << std::endl;
                return false;
            }
            return true;
        }
        // ���±�ǩ
        bool Updata(int tag_id)
        {
            // 1.��֯sql���
#define UPDATA_TAG "updata table_tag set name='%s' where id='%d';"
            char tmp[4096] = {0};
            sprintf(tmp, UPDATA_TAG, tag_id);

            // 2.ִ��sql���
            bool ret = MysqlQuery(_mysql, tmp);
            if (!ret)
            {
                std::cout << "updata failed��" << std::endl;
                return false;
            }
            return true;
        }

        // ��ѯ��ȡȫ����ǩ
        bool GetAll(Json::Value* tags)
        {
            // 1.��֯sql���
#define SELECT_ALL "select * from table_tag;"
            // 2.ִ��sql���
            bool ret = MysqlQuery(_mysql, SELECT_ALL);
            if (!ret)
            {
                std::cout << "getAll failed��" << std::endl;
                return false;
            }

            // 3.��ȡ���������Ҫ��
            MYSQL_RES* res = mysql_store_result(_mysql); // ������ռ���׵�ַ

            // 4.���������
            uint64_t num_rows = mysql_num_rows(res); // ��ȡ������е��������������ж���������
            if (num_rows <= 0)
            {
                std::cout << "No tag information��" << std::endl;
                mysql_free_result(res); // �ͷŽ����
            }
            for (int i = 0; i < num_rows; ++i)
            {
                // MYSQL_ROW ��һ������ָ�룬������ÿ��Ԫ�ر������һ��char*���͵Ľ����
                MYSQL_ROW row = mysql_fetch_row(res); //�������������ȡÿһ������

                // ����ȡ����ÿһ�н������֯��Json��
                Json::Value tag;
                tag["id"] = std::stoi(row[0]);
                tag["name"] = std::stoi(row[1]);

                tags->append(tag); // ����ȡ���Ľ������ӵ�tag��Ȼ�����
            }

            mysql_close(_mysql);
            return true;
        }

        ~TableTag()
        {}
    private:
        MYSQL* _mysql;
    };

    // �û���Ϣ��
    class TableUser
    {
    public:
        TableUser()
        {}
        ~TableUser()
        {}
    private:
        MYSQL* _mysql;
    };

    // ������Ϣ��
    class TableBlog
    {
    public:
        TableBlog()
        {}
        ~TableBlog()
        {}
    private:
        MYSQL* _mysql;
    };
};
