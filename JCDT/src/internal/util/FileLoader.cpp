#include "stdafx.h"
#include "JCDT_Lib/internal/util/FileLoader.h"
#include <assert.h>


#include <unicode/ucsdet.h>
#include <unicode/ucnv.h>

#include <cstdio>

pair<string, int> detectFileEncoding(const char *fileName)
{
	
  

    const int bufsize = 4096;
    char *buf = new char[bufsize];

    FILE *pFile = fopen(fileName, "r");
	pair<string, int> encoding;
	assert(pFile);
	if (!pFile)
		return  encoding;

    size_t count = fread(buf, 1, bufsize, pFile);
    fclose(pFile);

   encoding = detectTextEncoding(buf, count);

    delete[] buf;
    return encoding;
}

pair<string, int> detectTextEncoding(const char *data, int len)
{

    string matchMost;

    UErrorCode status = U_ZERO_ERROR;
    UCharsetDetector *csd = ucsdet_open(&status);
    if (status != U_ZERO_ERROR)
        return pair<string, int>("",0);

    ucsdet_setText(csd, data, len, &status);
    if ( U_FAILURE(status) )
        goto DET_ERROR;

    int matchCount = 0;
    const UCharsetMatch **csm = ucsdet_detectAll(csd, &matchCount, &status);
    if ( U_FAILURE(status) )
        goto DET_ERROR;

    if (matchCount <= 0)
        goto DET_ERROR;

    int confidence = ucsdet_getConfidence(csm[0], &status);
    if ( U_FAILURE(status) )
        goto DET_ERROR;

    matchMost = ucsdet_getName(csm[0], &status);
    if ( U_FAILURE(status) )
        goto DET_ERROR;

    return pair<string, int>(matchMost, confidence);

DET_ERROR:
    ucsdet_close(csd);
    return pair<string, int>(string(), 0);
}
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
			int32_t targetCapacity, 
			const char *source, 
			int32_t sourceLength,__out int& outlen)  
{  
    UErrorCode error = U_ZERO_ERROR;  
   outlen = ucnv_convert(toConverterName, fromConverterName, target, targetCapacity, source, sourceLength, &error);  
  
    return error;  
}  
const string &getDefaultEncoding()
{
	static string defaultEncoding;
	if (defaultEncoding.empty()) {
		UErrorCode errCode = U_ZERO_ERROR;
		UConverter *dftCnv = ucnv_open(NULL, &errCode);
		const char *defEncodingName = ucnv_getName(dftCnv, &errCode);//ucnv_getDefaultName();
		if (defEncodingName) {
			const char * standarName = ucnv_getStandardName(defEncodingName, "IANA", &errCode);//name of the standard governing the names; MIME and IANA are such standard 
			if (standarName == NULL) {
				standarName = ucnv_getStandardName(defEncodingName, "MIME", &errCode);
			}
			if (standarName == NULL)
				defaultEncoding = defEncodingName;
			else
				defaultEncoding = standarName;
		}

		ucnv_close(dftCnv);
	}

	return defaultEncoding;
}

const string getCheckedEncoding(const pair<string, int> &encoding)
{
    string encode;
    if (!encoding.first.empty() && encoding.second > 50)
        encode = encoding.first;
    else
        encode = getDefaultEncoding();

    return encode;
}

