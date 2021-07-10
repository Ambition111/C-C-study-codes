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
    // 初始化mysql
    static MYSQL* MysqlInit()
    {
        MYSQL* mysql = nullptr;
        // 1.初始化mysql句柄
        mysql = mysql_init(nullptr);
        if (mysql == nullptr)
        {
            std::cout << "mysql init failed！" << std::endl;
            return nullptr;
        }
        // 2.连接mysql服务器
        if (mysql_real_connect(mysql, "MYSQL_HOST", "MYSQL_USER", "MYSQL_PASS", "MYSQL_DB_NAME", 0, nullptr, 0) == nullptr)
        {
            std::cout << mysql_error(mysql) << "：mysql connect failed！" << std::endl;
            mysql_close(mysql);
            return nullptr;
        }
        // 3.设置mysql客户端字符编码集
        if (mysql_set_character_set(mysql, "uft8") != 0)
        {
            std::cout << mysql_error(mysql) << "：set character failed！" << std::endl;
            mysql_close(mysql);
            return nullptr;
        }

        return mysql;
    }
    // 关闭数据库，释放mysql操作句柄
    static void MysqlRelease(MYSQL* mysql)
    {
        mysql_close(mysql);
    }
    // mysql执行
    static bool MysqlQuery(MYSQL* mysql, const std::string& sql)
    {
        int ret = mysql_query(mysql, sql.c_str());
        if (ret != 0)
        {
            std::cout << mysql_error(mysql) << "：query sql：[" << sql.c_str() << "] error！" << std::endl;
            return false;
        }
        return true;
    }

    // 标签信息表
    class TableTag
    {
    public:
        TableTag(MYSQL* mysql) : _mysql(mysql) {}
        
        // 插入一个新标签
        bool Insert(const Json::Value& tag)
        {
            // 1.组织sql语句
#define INSERT_TAG "insert table_tag value(null, '%s');"
            char tmp[4096] = {0};

            // Json::Value.asCString(),返回C风格的字符串
            // sprintf() 按照指定格式将多个数据组织成为一个字符串放到tmp中
            sprintf(tmp, INSERT_TAG, tag["name"].asCString());

            // 2.执行sql语句
            bool ret = MysqlQuery(_mysql, tmp);
            if (!ret)
            {
                std::cout << "Insert failed！" << std::endl;
                return false;
            }
            return true; // 每次先把返回值给上，没有返回值的情况下，默认返回假
        }
    
        // 删除一个标签
        bool Delete(int tag_id)
        {
            // 1.组织sql语句
#define DELETE_TAG "delete from table_tag where id=%d;"
            char tmp[4096] = {0};
            sprintf(tmp, DELETE_TAG, tag_id);

            // 2.执行sql语句
            bool ret = MysqlQuery(_mysql, tmp);
            if (!ret)
            {
                std::cout << "delete failed！" << std::endl;
                return false;
            }
            return true;
        }
        // 更新标签
        bool Updata(int tag_id)
        {
            // 1.组织sql语句
#define UPDATA_TAG "updata table_tag set name='%s' where id='%d';"
            char tmp[4096] = {0};
            sprintf(tmp, UPDATA_TAG, tag_id);

            // 2.执行sql语句
            bool ret = MysqlQuery(_mysql, tmp);
            if (!ret)
            {
                std::cout << "updata failed！" << std::endl;
                return false;
            }
            return true;
        }

        // 查询获取全部标签
        bool GetAll(Json::Value* tags)
        {
            // 1.组织sql语句
#define SELECT_ALL "select * from table_tag;"
            // 2.执行sql语句
            bool ret = MysqlQuery(_mysql, SELECT_ALL);
            if (!ret)
            {
                std::cout << "getAll failed！" << std::endl;
                return false;
            }

            // 3.获取结果集（重要）
            MYSQL_RES* res = mysql_store_result(_mysql); // 结果集空间的首地址

            // 4.遍历结果集
            uint64_t num_rows = mysql_num_rows(res); // 获取结果集中的数据条数，即有多少条数据
            if (num_rows <= 0)
            {
                std::cout << "No tag information！" << std::endl;
                mysql_free_result(res); // 释放结果集
            }
            for (int i = 0; i < num_rows; ++i)
            {
                // MYSQL_ROW 是一个二级指针，数组里每个元素保存的是一个char*类型的结果集
                MYSQL_ROW row = mysql_fetch_row(res); //遍历结果集，获取每一行数据

                // 将获取到的每一行结果集组织成Json串
                Json::Value tag;
                tag["id"] = std::stoi(row[0]);
                tag["name"] = std::stoi(row[1]);

                tags->append(tag); // 将获取到的结果集添加到tag里然后出参
            }

            mysql_close(_mysql);
            return true;
        }

        ~TableTag()
        {}
    private:
        MYSQL* _mysql;
    };

    // 用户信息表
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

    // 博客信息表
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
