#include "stdafx.h"
#include "SearchPatternFactory.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/lex/scanner.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/impl/JikesOption.h"
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <queue>

#include <SearchLib/internal/core/IJavaSearchConstants.h>
#include "MethodDeclarationPattern.h"
#include "MethodReferencePattern.h"
#include "OrPattern.h"
#include "PackageDeclarationPattern.h"
#include "PackageReferencePattern.h"
#include "FieldDeclarationPattern.h"
#include "FieldReferencePattern.h"
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include "ConstructorDeclarationPattern.h"
#include "ConstructorReferencePattern.h"
#include "TypeDeclarationPattern.h"
#include "TypeReferencePattern.h"
#include "SuperInterfaceReferencePattern.h"
#include <SearchLib/internal/core/IIndexConstants.h>
#include <boost/algorithm/string.hpp>
#include "QualifiedTypeDeclarationPattern.h"
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/IImportDeclaration.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <PathModelLib/MosesPath/MosesPath.h>
#include "LocalFieldReferencePattern.h"
#include "JCDT_Lib/internal/parser/LPG/javasym.h"
namespace Jikes { // Open namespace Jikes block


	
namespace Search{


	SearchPattern* SearchPatternFactory::createConstructorPattern(std::wstring patternString, int limitTo, int matchMode, bool isCaseSensitive)
	{
		
		auto option = new Option();
		JikesClassPaths _no_paths;
		auto name_env = new INameEnvironment(option, _no_paths);

		auto lex = new MemorySourceLexStream(option);

		auto scanner = new Scanner(option, name_env);
		scanner->Scan(lex);


		
		const int InsideName = 1;
		
		const int InsideParameter = 2;

		std::wstring declaringQualification  , typeName , parameterType ;
		vector< std::wstring >parameterTypes ;
		int parameterCount = -1;
		bool foundClosingParenthesis = false;
		int mode = InsideName;
		
		auto curtok = lex->Gettoken();
		auto token = lex->Kind(curtok);

		while (token && token != TK_EOF) {
			switch (mode) {

				// read declaring type and selector
			case InsideName:

				switch (token) {
				case TK_DOT:
					if (declaringQualification.empty()) {
						if (typeName.empty()) return{};
						declaringQualification = typeName;
					}
					else {
						std::wstring tokenSource = lex->getNameString(curtok);
						declaringQualification += tokenSource + typeName;
					}
					typeName.clear();
					break;
				case TK_LPAREN:
					parameterCount = 0;
					mode = InsideParameter;
					break;
				case TK_Identifier:
				case TK_MULTIPLY:
					typeName += lex->getNameString(curtok);
					break;
				default:
					return {};
				
				}
				break;
				// read parameter types
			case InsideParameter:
				switch (token) {

				case TK_COMMA:

					parameterTypes.push_back(parameterType);
					parameterCount++;
					parameterType.clear();
					break;
				case TK_RPAREN:
					foundClosingParenthesis = true;
					if (!parameterType.empty()) {
						parameterTypes.push_back(parameterType);
						parameterCount++;
						parameterType.clear();
					}
					break;
				case TK_DOT:
				case TK_Identifier:
				case TK_MULTIPLY:
				case TK_LBRACKET:
				case TK_RBRACKET:
				case TK_boolean:
				case TK_byte:
				case TK_char:
				case TK_double:
				case TK_float:
				case TK_int:
				case TK_long:
				case TK_short:
				case TK_void:
					parameterType += lex->getNameString(curtok);
					break;
				default:
					return{};
				}
				break;
				// read return type
	
			}
			curtok = lex->Gettoken();
			token = lex->Kind(curtok);
		}
		
		// parenthesis mismatch
		if (parameterCount > 0 && !foundClosingParenthesis) return nullptr;
		if (typeName.empty() == true) return nullptr;

		std::wstring typeNameChars = typeName;
		if (typeNameChars.length() == 1 && typeNameChars[0] == '*') typeNameChars = nullptr;

		std::wstring declaringQualificationChars = nullptr;
		if (declaringQualification.empty() != true)
			declaringQualificationChars = declaringQualification;

		vector< std::wstring >
			parameterTypeQualifications, parameterTypeSimpleNames;

		// extract parameter types infos
		if (parameterCount >= 0)
		{
		
			for (int i = 0; i < parameterCount; i++)
			{
				std::wstring parameterTypePart = parameterTypes[i];
				auto lastDotPosition = parameterTypePart.rfind('.');
				if (lastDotPosition != wstring::npos)
				{
					parameterTypeQualifications[i] = parameterTypePart.substr(0, lastDotPosition);
					if (parameterTypeQualifications[i].length() == 1 && parameterTypeQualifications[i][0] == '*')
					{
						parameterTypeQualifications[i].clear();
					}
					else
					{
						// prefix with a '*' as the full qualification could be bigger 
						// (i.e. because of an import)
						parameterTypeQualifications[i] = IIndexConstants::ONE_STAR + parameterTypeQualifications[i] ;
					}
					parameterTypeSimpleNames[i] = parameterTypePart.substr(
						lastDotPosition + 1, parameterTypePart.length());
				}
				else
				{
					parameterTypeQualifications[i].clear();
					parameterTypeSimpleNames[i] = parameterTypePart;
				}
				if (parameterTypeSimpleNames[i].length() == 1 && parameterTypeSimpleNames[i][0] == '*')
					parameterTypeSimpleNames[i] = nullptr;
			}
		}
		vector< wstring>* _parameterTypeQualifications = nullptr;
		vector< wstring>* _parameterTypeSimpleNames = nullptr;
		SearchPattern* searchPattern = nullptr;
		switch (limitTo)
		{
		case IJavaSearchConstants::DECLARATIONS:
			searchPattern =
				new ConstructorDeclarationPattern(
					typeNameChars,
					matchMode,
					isCaseSensitive,
					declaringQualificationChars,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames);
			break;
		case IJavaSearchConstants::REFERENCES:
			searchPattern =
				new ConstructorReferencePattern(
					typeNameChars,
					matchMode,
					isCaseSensitive,
					declaringQualificationChars,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames,
					nullptr);
			break;
		case IJavaSearchConstants::ALL_OCCURRENCES:
			searchPattern = new OrPattern(
				new ConstructorDeclarationPattern(
					typeNameChars,
					matchMode,
					isCaseSensitive,
					declaringQualificationChars,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames),
				new ConstructorReferencePattern(
					typeNameChars,
					matchMode,
					isCaseSensitive,
					declaringQualificationChars,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames,
					nullptr));
			break;
		}
		return searchPattern;
	}

	SearchPattern* SearchPatternFactory::createFieldPattern(std::wstring patternString, int limitTo, int matchMode, bool isCaseSensitive)
	{
		auto option = new Option();
		JikesClassPaths _no_paths;
		auto name_env = new INameEnvironment(option, _no_paths);

		auto lex = new MemorySourceLexStream(option);

		auto scanner = new Scanner(option, name_env);
		scanner->Scan(lex);

		const int InsideDeclaringPart = 1;
		const int InsideType = 2;
		int lastToken = -1;

		std::wstring declaringType , fieldName ;
		std::wstring type ;
		int mode = InsideDeclaringPart;

		auto curtok = lex->Gettoken();
		auto token = lex->Kind(curtok);

		
		while (token && token != TK_EOF)
		{
			switch (mode)
			{
				// read declaring type and fieldName
			case InsideDeclaringPart:
				switch (token)
				{
				case TK_DOT:
					if (declaringType.empty() == true)
					{
						if (fieldName.empty() == true) 
							return nullptr;
						declaringType = fieldName;
					}
					else
					{
						std::wstring tokenSource = lex->getNameString(curtok);
						declaringType += tokenSource + fieldName;
					}
					fieldName.clear();
					break;
			
			
				case TK_Identifier:
				case TK_MULTIPLY:
					fieldName += lex->getNameString(curtok);
					break;
				default: {
				
					if (!(TK_DOT == lastToken)){
						mode = InsideType;
						break;
					}
					return nullptr;
				}
					
				}
				break;
				// read type 
			case InsideType:
				switch (token)
				{
				case TK_DOT:
				case TK_Identifier:
				case TK_MULTIPLY:
				case TK_LBRACKET:
				case TK_RBRACKET:
				case TK_boolean:
				case TK_byte:
				case TK_char:
				case TK_double:
				case TK_float:
				case TK_int:
				case TK_long:
				case TK_short:
				case TK_void:
					type += lex->getNameString(curtok);
					
					break;
				default:
					return nullptr;
				}
				break;
			}
			lastToken = token;
			curtok = lex->Gettoken();
			token = lex->Kind(curtok);
		}
		if (fieldName.empty() == true) return nullptr;

		std::wstring fieldNameChars = fieldName;
		if (fieldNameChars.length() == 1 && fieldNameChars[0] == '*')
			fieldNameChars.clear();

		std::wstring declaringTypeQualification = nullptr, declaringTypeSimpleName = nullptr;
		std::wstring typeQualification = nullptr, typeSimpleName = nullptr;

		// extract declaring type infos
		if (declaringType.empty() != true)
		{
			std::wstring declaringTypePart = declaringType;
			int lastDotPosition = declaringTypePart.rfind('.');
			if (lastDotPosition >= 0)
			{
				declaringTypeQualification = declaringTypePart.substr(0, lastDotPosition);
				if (declaringTypeQualification.length() == 1 && declaringTypeQualification[0] == '*')
					declaringTypeQualification.clear();
				declaringTypeSimpleName = declaringTypePart.substr(lastDotPosition + 1, declaringTypePart.length());
			}
			else
			{
				declaringTypeQualification = nullptr;
				declaringTypeSimpleName = declaringTypePart;
			}
			if (declaringTypeSimpleName.length() == 1 && declaringTypeSimpleName[0] == '*')
				declaringTypeSimpleName.clear();
		}
		// extract type infos
		if (type.empty() != true)
		{
			std::wstring typePart = type;
			int lastDotPosition = typePart.rfind('.');
			if (lastDotPosition >= 0)
			{
				typeQualification = typePart.substr(0, lastDotPosition);
				if (typeQualification.length() == 1 && typeQualification[0] == '*')
				{
					typeQualification.clear();
				}
				else
				{
					// prefix with a '*' as the full qualification could be bigger 
					// (i.e. because of an import)
					typeQualification = L"*"  +typeQualification;
				}
				typeSimpleName = typePart.substr(lastDotPosition + 1, typePart.length() );
			}
			else
			{
				typeQualification = nullptr;
				typeSimpleName = typePart;
			}
			if (typeSimpleName.length() == 1 && typeSimpleName[0] == '*') 
				typeSimpleName.clear();

		}
		SearchPattern* searchPattern = nullptr;
		switch (limitTo)
		{
		case IJavaSearchConstants::DECLARATIONS:
			searchPattern =
				new FieldDeclarationPattern(
					fieldNameChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					typeQualification,
					typeSimpleName);
			break;
		case IJavaSearchConstants::REFERENCES:
			searchPattern =
				new FieldReferencePattern(
					fieldNameChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					typeQualification,
					typeSimpleName,
					true, // read access
					true); // write access
			break;
		case IJavaSearchConstants::READ_ACCESSES:
			searchPattern =
				new FieldReferencePattern(
					fieldNameChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					typeQualification,
					typeSimpleName,
					true, // read access only
					false);
			break;
		case IJavaSearchConstants::WRITE_ACCESSES:
			searchPattern =
				new FieldReferencePattern(
					fieldNameChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					typeQualification,
					typeSimpleName,
					false,
					true); // write access only
			break;
		case IJavaSearchConstants::ALL_OCCURRENCES:
			searchPattern = new OrPattern(
				new FieldDeclarationPattern(
					fieldNameChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					typeQualification,
					typeSimpleName),
				new FieldReferencePattern(
					fieldNameChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					typeQualification,
					typeSimpleName,
					true, // read access
					true)); // write access
			break;
		default:
			break;

		}
		return searchPattern;
	}

	

	SearchPattern* SearchPatternFactory::createMethodPattern(std::wstring patternString, int limitTo, 
		int matchMode, bool isCaseSensitive)
	{

		
		auto option = new Option();
		JikesClassPaths _no_paths;
		auto name_env = new INameEnvironment(option, _no_paths);
	
		auto lex = new MemorySourceLexStream(option);
		
		auto scanner = new Scanner(option, name_env);
		scanner->Scan(lex);

		const int  InsideSelector = 1;
		const int  InsideParameter = 2;
		const int  InsideReturnType = 3;
		int lastToken = -1;

		std::wstring declaringType, selector, parameterType;
		std::vector< std::wstring > parameterTypes ;
		int parameterCount = -1;
		std::wstring returnType ;
		bool foundClosingParenthesis = false;
		int mode = InsideSelector;

		auto curtok = lex->Gettoken();
		auto token = lex->Kind(curtok);

		while (token && token != TK_EOF) {
			switch (mode) 
			{
				// read declaring type and selector
			case InsideSelector:

				switch (token) {
				case TK_DOT:
					if (declaringType.empty()) {
						if (selector.empty()) return {};
						declaringType = selector;
					}
					else {
						std::wstring tokenSource = lex->getNameString(curtok);
						declaringType += tokenSource + selector;
					}
					selector.clear();
					break;
				case TK_LPAREN:
					parameterCount = 0;
					mode = InsideParameter;
					break;
				case TK_Identifier:
				case TK_MULTIPLY:
						selector += lex->getNameString(curtok);
					break;
				default:
				{
					if (!(TK_DOT == lastToken)) {
						mode = InsideReturnType;
						break;
					}	
					return{};
				}

				}
				break;
				// read parameter types
			case InsideParameter:
				switch (token) {
			
				case TK_COMMA:
					
					parameterTypes.push_back(parameterType);
					parameterCount++;
					parameterType.clear();
					break;
				case TK_RPAREN:
					foundClosingParenthesis = true;
					if (!parameterType.empty()) {
						parameterTypes.push_back(parameterType);
						parameterCount++;
						parameterType.clear();
					}
					mode = InsideReturnType;
					break;
				case TK_DOT:
				case TK_Identifier:
				case TK_MULTIPLY:
				case TK_LBRACKET:
				case TK_RBRACKET:
				case TK_boolean:
				case TK_byte:
				case TK_char:
				case TK_double:
				case TK_float:
				case TK_int:
				case TK_long:
				case TK_short:
				case TK_void:	
						parameterType += lex->getNameString(curtok);	
					break;
				default:
					return {};
				}
				break;
				// read return type
			case InsideReturnType:
				switch (token) {
			
				case TK_DOT:
				case TK_Identifier:
				case TK_MULTIPLY:
				case TK_LBRACKET:
				case TK_RBRACKET:
				case TK_boolean:
				case TK_byte:
				case TK_char:
				case TK_double:
				case TK_float:
				case TK_int:
				case TK_long:
				case TK_short:
				case TK_void:		
						returnType += lex->getNameString(curtok);	
					break;
				default:
					return {};
				}
				break;

			default:
				break;

			}
			lastToken = token;
			 curtok = lex->Gettoken();
			 token = lex->Kind(curtok);
		}
		// parenthesis mismatch
		if (parameterCount>0 && !foundClosingParenthesis)
			return {};
		if (selector.empty() ) 
			return {};


		std::wstring& selectorChars = selector;
		if (selectorChars.length() == 1 && selectorChars[0] == '*')
			selectorChars = L"";

		std::wstring declaringTypeQualification , declaringTypeSimpleName ;
		std::wstring returnTypeQualification , returnTypeSimpleName ;
		std::vector<std::wstring>  temp_parameterTypeQualifications , parameterTypeSimpleNames ;

		// extract declaring type infos
		if (!declaringType.empty()) {
			std::wstring &declaringTypePart = declaringType;
			
			int lastDotPosition = declaringTypePart.rfind('.');
			if (lastDotPosition != wstring::npos) {
				declaringTypeQualification = declaringTypePart.substr(0, lastDotPosition);
				if (declaringTypeQualification.length() == 1 && declaringTypeQualification[0] == '*') 
					declaringTypeQualification.clear();

				declaringTypeSimpleName = declaringTypePart.substr(lastDotPosition + 1, declaringTypePart.length());
			}
			else {
				declaringTypeQualification.clear();
				declaringTypeSimpleName = declaringTypePart;
			}
			if (declaringTypeSimpleName.length() == 1 && declaringTypeSimpleName[0] == '*')
				declaringTypeSimpleName.clear();
		}
		// extract parameter types infos
		if (parameterCount >= 0) {
		
			for (int i = 0; i < parameterCount; i++) {
				std::wstring parameterTypePart = parameterTypes[i];
				
				int lastDotPosition = parameterTypePart.rfind('.');
				if (lastDotPosition != wstring::npos) {
					auto temp_parameterType = parameterTypePart.substr(0, lastDotPosition);
					temp_parameterTypeQualifications.push_back(temp_parameterType);
					if (temp_parameterType.length() == 1 && temp_parameterType[0] == '*') {
						temp_parameterTypeQualifications[i].clear();
					}
					else {
						// prefix with a '*' as the full qualification could be bigger 
						// (i.e. because of an import)
						temp_parameterTypeQualifications[i] = L"*" + temp_parameterType;
					}
					auto temp_parameterTypeSimpleNames = parameterTypePart.substr(lastDotPosition + 1, parameterTypePart.length());
					parameterTypeSimpleNames.push_back(temp_parameterTypeSimpleNames);
				}
				else {
					temp_parameterTypeQualifications.push_back(L"");
					parameterTypeSimpleNames.push_back( parameterTypePart);
				}
				if (parameterTypeSimpleNames[i].length() == 1 && parameterTypeSimpleNames[i][0] == '*') 
					parameterTypeSimpleNames[i] = {};
			}
		}
		// extract return type infos
		if (!returnType.empty()) {
			std::wstring& returnTypePart = returnType;

			int lastDotPosition = returnTypePart.rfind('.');
			if (lastDotPosition  != wstring::npos) {

				returnTypeQualification = returnTypePart.substr(0, lastDotPosition);
				if (returnTypeQualification.length() == 1 && returnTypeQualification[0] == '*') {
					returnTypeQualification.clear();
				}
				else {
					// (i.e. because of an import)
					returnTypeQualification =  L"*" + returnTypeQualification;
					
				}
				returnTypeSimpleName = returnTypePart.substr(lastDotPosition + 1, returnTypePart.length());
			}
			else {
				returnTypeQualification.clear();
				returnTypeSimpleName = returnTypePart;
			}
			if (returnTypeSimpleName.length() == 1 && returnTypeSimpleName[0] == '*') 
				returnTypeSimpleName.clear();
		}
		std::vector<std::wstring>*  _parameterTypeQualifications = nullptr;
		std::vector<std::wstring>*  _parameterTypeSimpleNames = nullptr;
		if(temp_parameterTypeQualifications.size())
		{
			_parameterTypeQualifications = new std::vector<std::wstring>();
			_parameterTypeQualifications->swap(temp_parameterTypeQualifications);
		}
		if(parameterTypeSimpleNames.size())
		{
			_parameterTypeSimpleNames  = new std::vector<std::wstring>();
			_parameterTypeSimpleNames->swap(parameterTypeSimpleNames);
		}

		SearchPattern* searchPattern = nullptr;
		switch (limitTo) {
		case IJavaSearchConstants::DECLARATIONS:
			searchPattern =
				new MethodDeclarationPattern(
					selectorChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					returnTypeQualification,
					returnTypeSimpleName,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames);
			break;
		case IJavaSearchConstants::REFERENCES:
			searchPattern =
				new MethodReferencePattern(
					selectorChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					returnTypeQualification,
					returnTypeSimpleName,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames,
					{});
			break;
		case IJavaSearchConstants::ALL_OCCURRENCES:
			searchPattern = new OrPattern(
				new MethodDeclarationPattern(
					selectorChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					returnTypeQualification,
					returnTypeSimpleName,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames),
				new MethodReferencePattern(
					selectorChars,
					matchMode,
					isCaseSensitive,
					declaringTypeQualification,
					declaringTypeSimpleName,
					returnTypeQualification,
					returnTypeSimpleName,
					_parameterTypeQualifications,
					_parameterTypeSimpleNames,
					{}));
			break;
		}
		return searchPattern;

	}

	SearchPattern* SearchPatternFactory::createPackagePattern(std::wstring patternString,
		int limitTo, int matchMode, bool isCaseSensitive)
	{
		SearchPattern* searchPattern = nullptr;
		switch (limitTo) {
		case IJavaSearchConstants::DECLARATIONS:
			searchPattern = new PackageDeclarationPattern(patternString, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::REFERENCES:
			searchPattern = new PackageReferencePattern(patternString, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::ALL_OCCURRENCES:
			searchPattern = new OrPattern(
				new PackageDeclarationPattern(patternString, matchMode, isCaseSensitive),
				new PackageReferencePattern(patternString, matchMode, isCaseSensitive)
			);
			break;
		default:
			break;

		}
		return searchPattern;

	}

	SearchPattern* SearchPatternFactory::createPattern(std::wstring patternString, int searchFor, int limitTo, int matchMode, bool isCaseSensitive)
	{
		
		if (patternString.length() == 0)
			return nullptr;

		SearchPattern* searchPattern = nullptr;
		switch (searchFor) {

		case IJavaSearchConstants::TYPE:
			searchPattern = createTypePattern(patternString, limitTo, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::METHOD:
			searchPattern = createMethodPattern(patternString, limitTo, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::CONSTRUCTOR:
			searchPattern = createConstructorPattern(patternString, limitTo, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::FIELD:
			searchPattern = createFieldPattern(patternString, limitTo, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::PACKAGE:
			searchPattern = createPackagePattern(patternString, limitTo, matchMode, isCaseSensitive);

		default:
			break;
		}
		return searchPattern;
	}

	SearchPattern* SearchPatternFactory::createPattern(JavaModel::IJavaElement* element, int limitTo)
	{
		using namespace Jikes::JavaModel;

		SearchPattern* searchPattern = nullptr;
		if (!element)
			return nullptr;

		int lastDot;
		switch (element->getElementType()) 
		{
		case IJavaElementType::FIELD:
		{
			IField* field = reinterpret_cast<IField*>(element);

			auto field_info = reinterpret_cast<SourceFieldElementInfo*>(field->getElementInfo());

		    if(!field_info)
				return nullptr;

			auto fullDeclaringName = boost::join(field_info->qualifiedTypeNames, L".");
			lastDot = fullDeclaringName.rfind('.');
			std::wstring declaringSimpleName = (lastDot != wstring::npos ? fullDeclaringName.substr(lastDot + 1) : fullDeclaringName);
			std::wstring declaringQualification = lastDot != wstring::npos ? fullDeclaringName.substr(0, lastDot) : L"";
			std::wstring name = field->getElementName();
			std::wstring typeSimpleName;
			std::wstring typeQualification;
			
			std::wstring typeSignature = field_info->_type;
			lastDot = typeSignature.rfind('.');
			typeSimpleName = (lastDot != wstring::npos ? typeSignature.substr(lastDot + 1) : typeSignature);
			typeQualification =
				lastDot != wstring::npos ?
				// prefix with a '*' as the full qualification could be bigger 
				// (i.e. because of an import)
				(L"*" + typeSignature.substr(0, lastDot)) :
				L"";
			
			
			switch (limitTo) {
			case IJavaSearchConstants::DECLARATIONS:
				searchPattern =
					new FieldDeclarationPattern(
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						declaringQualification,
						declaringSimpleName,
						typeQualification,
						typeSimpleName);
				break;
			case IJavaSearchConstants::REFERENCES:
				searchPattern =
					new FieldReferencePattern(
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						declaringQualification,
						declaringSimpleName,
						typeQualification,
						typeSimpleName,
						true,  // read access
						true); // write access
				break;
			case IJavaSearchConstants::READ_ACCESSES:
				searchPattern =
					new FieldReferencePattern(
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						declaringQualification,
						declaringSimpleName,
						typeQualification,
						typeSimpleName,
						true,  // read access only
						false);
				break;
			case IJavaSearchConstants::WRITE_ACCESSES:
				searchPattern =
					new FieldReferencePattern(
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						declaringQualification,
						declaringSimpleName,
						typeQualification,
						typeSimpleName,
						false,
						true); // write access only
				break;
			case IJavaSearchConstants::ALL_OCCURRENCES:
				searchPattern = new OrPattern(
					new FieldDeclarationPattern(
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						declaringQualification,
						declaringSimpleName,
						typeQualification,
						typeSimpleName),
					new FieldReferencePattern(
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						declaringQualification,
						declaringSimpleName,
						typeQualification,
						typeSimpleName,
						true,  // read access
						true)); // write access
				break;
			}
		}
		break;
		case LOCAL_FIELD:
		{
			IField* field = reinterpret_cast<IField*>(element);
			auto point = reinterpret_cast<MemberInFileMoses*>(field->getMosesPath());
			if (!point)
				return nullptr;
			wstring _path = point->root;
			_path += L"/" +  boost::replace_all_copy(point->pkg, L".", L"/");
			_path += L"/" +  point->file + L".java";

			auto field_info = reinterpret_cast<SourceFieldElementInfo*>(field->getElementInfo());
			if (!field_info)
				return nullptr;
			std::wstring name = field->getElementName();
			std::wstring typeSimpleName;
			std::wstring typeQualification;

			std::wstring typeSignature = field_info->_type;
			lastDot = typeSignature.rfind('.');
			typeSimpleName = (lastDot != wstring::npos ? typeSignature.substr(lastDot + 1) : typeSignature);
			typeQualification =
				lastDot != wstring::npos ?
				// prefix with a '*' as the full qualification could be bigger 
				// (i.e. because of an import)
				(L"*" + typeSignature.substr(0, lastDot)) :
				L"";

			ISourceRange* _rng = nullptr;
			if (field_info->local_fild_father_rng) {
				_rng = field_info->local_fild_father_rng->Clone();
			}
			auto _utf8_path = CharOperation::ConvertUnicodeToUtf8(_path);
				searchPattern =
					new LocalFieldReferencePattern(_utf8_path, _rng,
						name,
						IJavaSearchConstants::EXACT_MATCH,
						IJavaSearchConstants::CASE_SENSITIVE,
						typeQualification,
						typeSimpleName,
						true,  // read access
						true); // write access
			
		}
			break;
		case IJavaElementType::IMPORT_DECLARATION:
		{
			std::wstring elementName = element->getElementName();
			lastDot = elementName.rfind('.');
			if (lastDot == wstring::npos)
				return nullptr; // invalid import declaration
			IImportDeclaration* importDecl = (IImportDeclaration*)element;
			if (importDecl->isOnDemand()) {
				searchPattern = createPackagePattern(elementName.substr(0, lastDot),
					limitTo, IJavaSearchConstants::EXACT_MATCH, IJavaSearchConstants::CASE_SENSITIVE);
			}
			else {
				searchPattern =
					createTypePattern(
						elementName.substr(lastDot + 1),
						elementName.substr(0, lastDot),
						{},
						limitTo);
			}
		}
			break;
		case IJavaElementType::METHOD: 
		{
			IMethod* method = reinterpret_cast<IMethod*>(element);
		
			bool isConstructor = method->isConstructor();
			auto info = method->getElementInfo();
			if (!info)
				return nullptr;

			auto fullDeclaringName = boost::join(info->qualifiedTypeNames, L".");
			lastDot = fullDeclaringName.rfind('.');
			std::wstring declaringSimpleName = (lastDot != wstring::npos ? fullDeclaringName.substr(lastDot + 1) : fullDeclaringName);
			std::wstring declaringQualification = lastDot != wstring::npos ? fullDeclaringName.substr(0, lastDot) : L"";
			std::wstring selector = method->getElementName();
			std::wstring returnSimpleName;
			std::wstring returnQualification;
			
			std::wstring returnType = method->getReturnType();
				lastDot = returnType.rfind('.');
				returnSimpleName = (lastDot != -1 ? returnType.substr(lastDot + 1) : returnType);
				returnQualification =
					lastDot != wstring::npos ?
						// prefix with a '*' as the full qualification could be bigger 
						// (i.e. because of an import)
						L"*" + returnType.substr(0, lastDot) :
						L"";
			
			
			auto parameterTypes = method->getParameterTypes();
			int paramCount = parameterTypes.size();
			std::vector<std::wstring> parameterSimpleNames(paramCount);
			std::vector<std::wstring> parameterQualifications(paramCount);

			for (int i = 0; i < paramCount; i++) {
				std::wstring signature = parameterTypes[i];
				lastDot = signature.rfind('.');
				parameterSimpleNames[i] = (lastDot != wstring::npos ? signature.substr(lastDot + 1) : signature);
				parameterQualifications[i] =
					lastDot != wstring::npos ?
						// prefix with a '*' as the full qualification could be bigger 
						// (i.e. because of an import)
					L"*" + signature.substr(0, lastDot) :	L"";
			}
			std::vector<std::wstring>* _parameterSimpleNames = nullptr;/* = new char[paramCount][];*/
			std::vector<std::wstring>* _parameterQualifications = nullptr;// = new char[paramCount][];
			if(parameterSimpleNames.size())
			{
				_parameterSimpleNames = new std::vector<std::wstring>();
				_parameterSimpleNames->swap(parameterSimpleNames);
			}
			if(parameterQualifications.size())
			{
				_parameterQualifications = new std::vector<std::wstring>();
				_parameterQualifications->swap(parameterQualifications);
			}

			switch (limitTo) {
			case IJavaSearchConstants::DECLARATIONS:
				if (isConstructor) {
					searchPattern =
						new ConstructorDeclarationPattern(
							declaringSimpleName,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							_parameterQualifications,
							_parameterSimpleNames);
				}
				else {
					searchPattern =
						new MethodDeclarationPattern(
							selector,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							declaringSimpleName,
							returnQualification,
							returnSimpleName,
							_parameterQualifications,
							_parameterSimpleNames);
				}
				break;
			case IJavaSearchConstants::REFERENCES:
				if (isConstructor) {
					auto pos = method->getMosesPath();
					if(pos){
						pos = pos->Clone();
					}
					auto _clone_pos = reinterpret_cast<PathModel::MemberInFileMoses*>(pos);
					searchPattern =
						new ConstructorReferencePattern(
							declaringSimpleName,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							_parameterQualifications,
							_parameterSimpleNames, _clone_pos);
				}
				else {
					auto pos = method->getMosesPath();
					if (pos) {
						pos = pos->Clone();
					}
					auto _clone_pos = reinterpret_cast<PathModel::MemberInFileMoses*>(pos);
					searchPattern =
						new MethodReferencePattern(
							selector,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							declaringSimpleName,
							returnQualification,
							returnSimpleName,
							_parameterQualifications,
							_parameterSimpleNames,
							_clone_pos);
				}

				break;
			case IJavaSearchConstants::ALL_OCCURRENCES:
				if (isConstructor) {
					auto pos = method->getMosesPath();
					if (pos) {
						pos = pos->Clone();
					}
					auto _clone_pos = reinterpret_cast<PathModel::MemberInFileMoses*>(pos);
					searchPattern = new OrPattern(
						new ConstructorDeclarationPattern(
							declaringSimpleName,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							_parameterQualifications,
							_parameterSimpleNames),
						new ConstructorReferencePattern(
							declaringSimpleName,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							_parameterQualifications,
							_parameterSimpleNames,
							_clone_pos));
				}
				else {
					auto pos = method->getMosesPath();
					if (pos) {
						pos = pos->Clone();
					}
					auto _clone_pos = reinterpret_cast<PathModel::MemberInFileMoses*>(pos);
					searchPattern = new OrPattern(
						new MethodDeclarationPattern(
							selector,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							declaringSimpleName,
							returnQualification,
							returnSimpleName,
							_parameterQualifications,
							_parameterSimpleNames),
						new MethodReferencePattern(
							selector,
							IJavaSearchConstants::EXACT_MATCH,
							IJavaSearchConstants::CASE_SENSITIVE,
							declaringQualification,
							declaringSimpleName,
							returnQualification,
							returnSimpleName,
							_parameterQualifications,
							_parameterSimpleNames,
							_clone_pos));
				}
				break;
				}
			}
			break;
		case IJavaElementType::TYPE:
		{
			IType* type = (IType*)element;
			auto mose = reinterpret_cast<PathModel::MemberInFileMoses*>( element->getMosesPath() );
			if (!mose)
				return nullptr;

			searchPattern =
				createTypePattern(
					type->getElementName(),
					mose->pkg,
					mose->enclosing_types,
					limitTo);
		}
	
			break;
		case IJavaElementType::PACKAGE_DECLARATION:
		case IJavaElementType::PACKAGE_FRAGMENT:
		{
			searchPattern = createPackagePattern(element->getElementName(), limitTo,
				IJavaSearchConstants::EXACT_MATCH, IJavaSearchConstants::CASE_SENSITIVE);
		}
		
			
			break;
		default:
			break;
		}
		return searchPattern;
	}
	/**
	* Returns the enclosing type names of the given type.
	*/
	
	SearchPattern* SearchPatternFactory::createTypePattern(std::wstring simpleName,
		std::wstring packageName, std::vector<std::wstring> enclosingTypeNames, int limitTo)
	{
		SearchPattern* searchPattern = nullptr;
		switch (limitTo) {
		case IJavaSearchConstants::DECLARATIONS:
			searchPattern =
				new TypeDeclarationPattern(
					packageName,
					enclosingTypeNames,
					simpleName,
					IIndexConstants::TYPE_SUFFIX,
					IJavaSearchConstants::IJavaSearchConstants::EXACT_MATCH,
					IJavaSearchConstants::CASE_SENSITIVE);
			break;
		case IJavaSearchConstants::REFERENCES:
		{
			auto _qualification = packageName;
			auto part = boost::join(enclosingTypeNames, L".");
			if (!_qualification.empty()) {
				if(!part.empty())
					_qualification += L"." + part;
			}
			else {
				_qualification.swap(part);
			}
			searchPattern =
				new TypeReferencePattern(
					_qualification,
					simpleName,
					IJavaSearchConstants::EXACT_MATCH,
					IJavaSearchConstants::CASE_SENSITIVE);
		}
		
			break;
		case IJavaSearchConstants::IMPLEMENTORS:
		{

			auto _qualification = packageName;
			auto part = boost::join(enclosingTypeNames, L".");
			if (!_qualification.empty()) {
				_qualification += L"." + part;
			}
			else {
				_qualification.swap(part);
			}

			searchPattern =
				new SuperInterfaceReferencePattern(
					_qualification,
					simpleName,
					IJavaSearchConstants::EXACT_MATCH,
					IJavaSearchConstants::CASE_SENSITIVE);
		}
	
			break;
		case IJavaSearchConstants::ALL_OCCURRENCES:
		{
			auto _qualification = packageName;
			auto part = boost::join(enclosingTypeNames, L".");
			if(_qualification.empty() !=true){
				if(!part.empty())
					_qualification += L"." + part;
			}
			else{
				_qualification.swap(part);
			}

			searchPattern = new OrPattern(
				new TypeDeclarationPattern(
					packageName,
					enclosingTypeNames,
					simpleName,
					IIndexConstants::TYPE_SUFFIX,
					IJavaSearchConstants::EXACT_MATCH,
					IJavaSearchConstants::CASE_SENSITIVE),

				new TypeReferencePattern(
					_qualification,
					simpleName,
					IJavaSearchConstants::EXACT_MATCH,
					IJavaSearchConstants::CASE_SENSITIVE));
		}
			
			break;
		}
		return searchPattern;
	}

	SearchPattern* SearchPatternFactory::createTypePattern(std::wstring patternString, int limitTo, 
		int matchMode, bool isCaseSensitive)
	{

		auto option = new Option();
		JikesClassPaths _no_paths;
		auto name_env = new INameEnvironment(option, _no_paths);

		auto lex = new MemorySourceLexStream(option);

		auto scanner = new Scanner(option, name_env);
		scanner->Scan(lex);

		auto curtok = lex->Gettoken();
		auto token = lex->Kind(curtok);

		std::wstring type ;
		
		while (token && token != TK_EOF) {
			switch (token) {
			
			case TK_DOT:
			case TK_Identifier:
			case TK_MULTIPLY:
			case TK_LBRACKET:
			case TK_RBRACKET:
			case TK_boolean:
			case TK_byte:
			case TK_char:
			case TK_double:
			case TK_float:
			case TK_int:
			case TK_long:
			case TK_short:
			case TK_void:
					type += lex->getNameString(curtok);
				
				break;
			default:
				return {};
			}
			 curtok = lex->Gettoken();
			 token = lex->Kind(curtok);

		}
		if (type.empty()) return {};

		std::wstring qualificationChars, typeChars;

		// extract declaring type infos
		
		std::wstring& typePart = type;
		int lastDotPosition = typePart.rfind('.');
		if (lastDotPosition != wstring::npos) {
			qualificationChars = typePart.substr( 0, lastDotPosition);
			if (qualificationChars.length() == 1 && qualificationChars[0] == '*') 
				qualificationChars.clear();
			typeChars = typePart.substr(lastDotPosition + 1, typePart.length());
		}
		else {
			qualificationChars.clear();
			typeChars = typePart;
		}
		if (typeChars.length() == 1 && typeChars[0] == '*')
			typeChars.clear();

		SearchPattern *searchPattern = nullptr;
		switch (limitTo) {
		case IJavaSearchConstants::DECLARATIONS: // cannot search for explicit member types
			searchPattern = new QualifiedTypeDeclarationPattern(qualificationChars, typeChars,
				IIndexConstants::TYPE_SUFFIX, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::REFERENCES:
			searchPattern = new TypeReferencePattern(qualificationChars, typeChars, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::IMPLEMENTORS:
			searchPattern = new SuperInterfaceReferencePattern(qualificationChars, typeChars, matchMode, isCaseSensitive);
			break;
		case IJavaSearchConstants::ALL_OCCURRENCES:
			searchPattern = new OrPattern(
				new QualifiedTypeDeclarationPattern(qualificationChars, typeChars,
				                  IIndexConstants::TYPE_SUFFIX, matchMode, isCaseSensitive),// cannot search for explicit member types
				new TypeReferencePattern(qualificationChars, typeChars, matchMode, isCaseSensitive));
			break;
		}
		return searchPattern;

	}

	ISearchPattern* SearchPatternFactory::createOrSearchPattern(ISearchPattern* leftPattern, ISearchPattern* rightPattern)
	{
		return nullptr;
	}

	ISearchPattern* SearchPatternFactory::createSearchPattern(const wstring& stringPattern, int searchFor, int limitTo, bool isCaseSensitive)
	{
		return createPattern(stringPattern, searchFor, limitTo, isCaseSensitive, IJavaSearchConstants::EXACT_MATCH);
	}

	SearchPatternFactory::SearchPatternFactory()
	{}
}


} // Close namespace Jikes block


