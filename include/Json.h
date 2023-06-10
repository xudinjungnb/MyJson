#ifndef JSON_H
#define JSON_H

#include <string>
using std::string;

#include <list>
#include <map>

namespace xdj {
namespace json {

class Json
{
public:
    //通过枚举定义Json类型
    enum Type
    {
        json_null = 0,  // null type
        json_bool,      // bool type
        json_int,       // integer type
        json_double,    // double type
        json_string,    // string type
        json_array,     // array type
        json_object     // object type
    };
private:
    //联合体定义值,null类型没有对应值
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string * m_string;
        std::list<Json> * m_array;
        std::map<string, Json> * m_object;
    };
    void copy(const Json & other);// 供类内其他成员函数使用，无法在类对象直接调用
    void clear();	// 释放堆内存
    
    Type m_type;	//Json对象的类型
    Value m_value;	//Json对象的值
public:
    // 一、重载四种在类中默认提供的函数
	//1、无参构造与有参构造
    Json();						//默认初始化
    Json(Type type);			//利用类型初始化Json对象 Json json=json_null;
    Json(bool value);			//Json json=true;
    Json(int value);			//Json json=10;
    Json(double value);			//Json json=0.0;
    Json(const char* value);	//Json json="abc"
    Json(const string& value); 	//Json json=string("abc");
    //2、拷贝构造函数
    Json(const Json& other);	//用已有的Json对象初始化新创建的Json对象
    //3、赋值函数
    Json & operator = (const Json & other);
    //4、析构函数
    ~Json();
    
    // 二、有关array对象的API，添加、按下标删除、按下标访问
    void append(const Json & value); //尾添加
    void remove(int index);			//按下标删除
	Json & operator [] (int index);	//通过下标获取Json对象
    
    // 三、有关Object对象的API，按键值访问、按键值删除
    Json & operator [] (const char * key);		//通过键值获取Json对象对应的value
    Json & operator [] (const string & key);
    void remove(const char * key);				//按键值删除
    void remove(const string & key);
    
    // 四、提供两组从Json对象转换到bool、int、double、string的API
    // 1、隐式转换
    operator bool();    //Json->bool
    operator int();    	//Json->int
    operator double();  //Json->double
    operator string();  //Json->string
    // 2、显式转换
    bool asBool() const;    //Json->bool
    int asInt() const;
    double asDouble() const;
    string asString() const;
    
    // 五、比较两个Json对象
    bool operator == (const Json & other);
    bool operator != (const Json & other);
	
    // 六、用于获取Json对象信息的API
    // 1、判断有没有、是不是
    bool has(int index);
    bool has(const char * key);
    bool has(const string & key);
    
    bool isNull() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;
    // 2、array对象或object对象中元素个数
    int size() const;
    // 3、当array对象为空、object对象空、对象为null时返回true，其余情况返回false
    bool empty() const;
    // 4、获得Json对象的类型
    Type type() const;
    // 5、获得Json对象的值
    Value value() const; 
    
    // 七、解析字符串
    void parse(const string & str);
    // 八、打印Json对象
    string str() const;
};

}
}
#endif
