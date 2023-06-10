#pragma once

#include <string>
#include "Json.h"

using namespace std;

namespace xdj {
namespace json {

class Parser
{
    //step1、内部存储数据的成员变量
	string m_str;//待处理的string对象
	size_t m_idx;//当前处理的字符的下标
 	//step4、内部使用的依赖函数   
    void skip_white_space();	//跳过空白字符
    char get_next_token();		//得到下一个字符

    Json parse_null();			//解析NULL对象
    Json parse_bool();			//解析Bool对象
    Json parse_number();		//解析number对象
    string parse_string();		//解析string对象
    Json parse_array();			//解析array对象
    Json parse_object();		//解析object对象

    bool in_range(int x, int lower, int upper)//判断x是否在范围[lower,upper]中
    {
        return (x >= lower && x <= upper);
    }
public:
    //step2、公共的构造与析构
    Parser();	//无参构造函数
    ~Parser();	//析构函数
	//step3、提供核心功能的公共接口
    void load(const string & str);//核心函数1，加载字符串到成员变量m_str中
    Json parse();				//核心函数2，对加载的字符串进行解析

};

}}
