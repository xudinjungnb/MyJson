#include <string.h>
#include <stdexcept>//定义了一些标准的异常类，分为两大类：逻辑错误和运行时错误。其中运行时错误是程序员不能控制的。
#include <cstdlib>
#include "Parser.h"

using namespace xdj::json;

Parser::Parser() : m_idx(0)//从字符串下标为0的字符开始，字符串在运行时加载
{}

Parser::~Parser()
{}

void Parser::load(const string & str)//加载字符串到m_str对象
{
    //m_str=str;
    m_str = *(&str);//改为深拷贝
    m_idx = 0;
}

Json Parser::parse()
{
    char ch = get_next_token();//获取下一个未处理的字符
    switch (ch)
    {
        case 'n'://是null
			--m_idx;
            return parse_null();
        case 't': case 'f'://是bool
			--m_idx;
            return parse_bool();
        //是int
        case '-':case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			--m_idx;
            return parse_number();
        case '"'://是string
            return Json(parse_string());
        case '['://是数组
            return parse_array();
        case '{'://是对象
            return parse_object();
        default:
            break;
    }
    throw std::logic_error("unexpected character in parse json");
}

void Parser::skip_white_space()//跳过空白字符，直到遇到下一个非空白字符
{
    //官方文档中定义的四种空白字符：空格、回车、换行、制表
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
        ++m_idx;//直接跳过
}

char Parser::get_next_token()
{
    skip_white_space();//跳过空白字符，直到遇到下一个非空白字符
    if (m_idx == m_str.size())//若是直接跳到尾，说明输入的字符串有问题，直接抛出一个逻辑异常
        throw std::logic_error("unexpected end of input");
    return m_str[m_idx++];//返回下一个未处理的字符
}

Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;//+4刚好到下一个未处理的字符
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }
    if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}
//会停在第一个不是数字字符的位置
Json Parser::parse_number()
{
    size_t pos = m_idx;

    if (m_str[m_idx] == '-')//遇到负号跳过
    {
     	m_idx++;   
    }

    // 1、整数部分
    if (m_str[m_idx] == '0')//（1）0开头，可能是小数，在这部分先不处理
    {
        m_idx++;
    }
    else if (in_range(m_str[m_idx], '1', '9'))//（2）1~9开头，正常数字
    {
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }
    else//（3）其他字符开头
    {
        throw std::logic_error("invalid character in number");
    }
	//2、遇到的第一个不是数字的字符不是. 说明这只是一个整数
    if (m_str[m_idx] != '.')
    {
        return Json(std::atoi(m_str.c_str() + pos));//atoi遇到第一个不是数字的字符就会返回
    }
	//3、遇到的第一个不是数字的字符是. 说明这是一个小数
    // 小数部分
    m_idx++;//跳过.
    if (!in_range(m_str[m_idx], '0', '9'))//（1）不是数字字符，小数部分没东西，出错
    {
        throw std::logic_error("at least one digit required in fractional part");
    }
    while (in_range(m_str[m_idx], '0', '9'))//（2）0~9数字字符
    {
        m_idx++;
    }
    return Json(std::atof(m_str.c_str() + pos));
}
//读完m_idx在“后面
string Parser::parse_string()
{
    size_t pos = m_idx;
    while (true)//死循环
    {
        if (m_idx == m_str.size())//读到字符串尾都没读到”，抛出一个异常
        {
            throw std::logic_error("unexpected end of input in string");
        }

        char ch = m_str[m_idx++];
        if (ch == '"')//读到”正常退出
        {
            break;
        }

        // The usual case: non-escaped characters
        if (ch == '\\')//斜杆，处理转义字符
        {
            ch = m_str[m_idx++];
            switch (ch)
            {
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':
                case '"':
                case '/':
                case '\\':
                    break;
                case 'u':
                    m_idx += 4;
                    break;
                default:
                    break;
            }
        }
    }
    return m_str.substr(pos, m_idx - pos - 1);
}

Json Parser::parse_array()
{
    Json arr(Json::json_array);//创建一个空的array对象
    char ch = get_next_token();//获得下一个非空字符
    if (ch == ']')//[]空的，直接返回
    {
        return arr;
    }
    --m_idx;//返回
    while (true)
    {
        arr.append(parse());//解析出一个Json对象，并加入array
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in array");
        }
    }
    return arr;
}

Json Parser::parse_object()
{
    Json obj(Json::json_object);
    //1、为空{}直接返回
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }
	--m_idx;
    while (true)
    {
        //2、读出string键值
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("expected '\"' in object");
        }
        string key = parse_string();
        //3、
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }
        //4、读出值
        obj[key] = parse();
        
        ch = get_next_token();
        if (ch == '}')//读到}直接退出循环
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}
