#ifndef FILE_LOADER_H__
#define FILE_LOADER_H__

#include <string>
#include <utility>

using std::string;
using std::pair;

class FileLoader{
public:
private:
    string m_encoding;

};

// 探测文件的编码
// @return : <编码名称, 可信度>
pair<string, int> detectFileEncoding(const char *fileName);

// 探测一段文本的编码
pair<string, int> detectTextEncoding(const char *data, int len);

// 获取系统默认的编码名称
const string &getDefaultEncoding();

/* 
 * toConverterName,      转换后的字符编码 
 * fromConverterName,    转换前的字符编码 
 * target,               存储转换后的字符串， 传出参数 
 * targetCapacity,       存储容量，target的大小 
 * source，              需要转换的字符串 
 * sourceLength,         source的大小 
**/  
int convert(const char *toConverterName,
	        const char *fromConverterName,  
            char *target,
			signed int targetCapacity, 
			const char *source, 
			signed int sourceLength,
			__out int& outlen
			) ;
// 
const string getCheckedEncoding(const pair<string, int> &encoding);

#endif