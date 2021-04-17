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

// ̽���ļ��ı���
// @return : <��������, ���Ŷ�>
pair<string, int> detectFileEncoding(const char *fileName);

// ̽��һ���ı��ı���
pair<string, int> detectTextEncoding(const char *data, int len);

// ��ȡϵͳĬ�ϵı�������
const string &getDefaultEncoding();

/* 
 * toConverterName,      ת������ַ����� 
 * fromConverterName,    ת��ǰ���ַ����� 
 * target,               �洢ת������ַ����� �������� 
 * targetCapacity,       �洢������target�Ĵ�С 
 * source��              ��Ҫת�����ַ��� 
 * sourceLength,         source�Ĵ�С 
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