#include "stdafx.h"
#include "JavaCorrectionProcessor.h"
#include <JCDT_Lib/internal/problem/SemanticError.h>
#include <CodeCorrectionLib/core/ProblemPosition.h>
#include "ReorgCorrectionsSubProcessor.h"
#include "UnresolvedElementsSubProcessor.h"
#include <CodeCorrectionLib/core/SimilarElementsRequestor.h>
#include <JavaModelLib/compiler/DefaultProblem.h>
using  namespace  Jikes::JavaModel;
namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		//bool JavaCorrectionProcessor::hasCorrections(int problemId)
		//{
		//	switch (problemId)
		//	{
		//	case SemanticError::DUPLICATE_IMPORT_NAME:
		//	case SemanticError::UNNECESSARY_TYPE_IMPORT:
		//	case SemanticError::UNUSED_TYPE_IMPORT:
		//	case SemanticError::UNUSED_PACKAGE_IMPORT:// 对应UnusedImport


		//	case SemanticError::METHOD_NOT_FOUND: //对应UndefinedMethod

		//	case SemanticError::CONSTRUCTOR_NOT_FOUND:// 对应UndefinedConstructor

		//		// ParameterMismatch
		//		// MethodButWithConstructorName
		//	case SemanticError::FIELD_NOT_FOUND:// UndefinedField

		//		// UndefinedName

		//		// PublicClassMustMatchFileName


		//		// PackageIsNotExpectedPackage


		//		// FieldTypeNotFound


		//		// ArgumentTypeNotFound


		//	// ReturnTypeNotFound

		//		// SuperclassNotFound

		//		// ExceptionTypeNotFound


		//		// InterfaceNotFound


		//	case SemanticError::METHOD_NAME_MISSPELLED:
		//	case SemanticError::FIELD_NAME_MISSPELLED:

		//	case  SemanticError::METHOD_OVERLOAD_NOT_FOUND:
		//	case SemanticError::CONSTRUCTOR_OVERLOAD_NOT_FOUND:

		//	case SemanticError::MISMATCHED_TYPE_AND_FILE_NAMES:

		//	case SemanticError::TYPED_METHOD_WITH_NO_RETURN:// MissingReturnType

		//	case SemanticError::TYPE_NOT_FOUND:

		//	case SemanticError::NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD:
		//	case SemanticError::FIELD_NOT_METHOD:
		//		return true;


		//		// NonExternalizedStringLiteral
		//	default:
		//		return false;

		//	}
		//}

		//void JavaCorrectionProcessor::collectCorrections(ProblemPosition* problemPos,
		//	vector<CorrectionProposal*>& proposals)
		//{
		//	if (!problemPos)
		//		return;

		//	switch (problemPos->problemId)
		//	{
		//	case SemanticError::DUPLICATE_IMPORT_NAME:
		//	case SemanticError::UNNECESSARY_TYPE_IMPORT:
		//	case SemanticError::UNUSED_TYPE_IMPORT:
		//	case SemanticError::UNUSED_PACKAGE_IMPORT:// 对应UnusedImport
		//		ReorgCorrectionsSubProcessor::removeImportStatementProposals(problemPos, proposals);
		//		break;

		//
		//	case SemanticError::FIELD_NAME_MISSPELLED:


		//
		//											  // ParameterMismatch
		//											  // MethodButWithConstructorName
		//	case SemanticError::FIELD_NOT_FOUND:// UndefinedField
		//		UnresolvedElementsSubProcessor::getVariableProposals(problemPos, proposals);

		//	// UndefinedName

		//	// PublicClassMustMatchFileName


		//	// PackageIsNotExpectedPackage


		//	// FieldTypeNotFound


		//	// ArgumentTypeNotFound
		//		break;

		//	case SemanticError::TYPE_NOT_FOUND:
		//		UnresolvedElementsSubProcessor::getTypeProposals(problemPos, SimilarElementsRequestor::ALL_TYPES, proposals);
		//		break;
		//		// ReturnTypeNotFound

		//		// SuperclassNotFound

		//		// ExceptionTypeNotFound


		//		// InterfaceNotFound


		//	case SemanticError::TYPED_METHOD_WITH_NO_RETURN:// MissingReturnType


		//		break;

		//	case SemanticError::MISMATCHED_TYPE_AND_FILE_NAMES:
		//		ReorgCorrectionsSubProcessor::getWrongTypeNameProposals(problemPos, proposals);
		//		break;
		//	case SemanticError::NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD:
		//		ReorgCorrectionsSubProcessor::get_NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD_Proposals(problemPos, proposals);
		//		break;

		//	case SemanticError::METHOD_NOT_FOUND: //对应UndefinedMethod
		//	case SemanticError::CONSTRUCTOR_NOT_FOUND:// 对应UndefinedConstructor
		//	case SemanticError::CONSTRUCTOR_OVERLOAD_NOT_FOUND:
		//	case SemanticError::METHOD_OVERLOAD_NOT_FOUND:
		//	case SemanticError::METHOD_NAME_MISSPELLED:
		//	case SemanticError::FIELD_NOT_METHOD:
		//		 UnresolvedElementsSubProcessor::getMethodProposals(problemPos, proposals);
		//		 break;
		//		// NonExternalizedStringLiteral
		//	default:
		//		break;
		//	}
		//}

		//bool JavaCorrectionProcessor::UnusedImportProblem(int problemId)
		//{
		//	switch (problemId)
		//	{

		//	case SemanticError::DUPLICATE_IMPORT_NAME:
		//	case SemanticError::UNNECESSARY_TYPE_IMPORT:
		//	case SemanticError::UNUSED_TYPE_IMPORT:
		//	case SemanticError::UNUSED_PACKAGE_IMPORT:// 对应UnusedImport
		//		return true;
		//		break;

		//	default:
		//		return  false;

		//	}
		//} // Close namespace Correction block
	


bool JavaCorrectionProcessor::hasCorrections(int problemId)
{
	switch (problemId)
	{
	case IProblem::UnusedImport:
	case IProblem::DuplicateImport:
	
	case IProblem::UndefinedField:
	case IProblem::UndefinedName:
	case IProblem::MissingReturnType:// MissingReturnType
	case IProblem::UndefinedType:
		
	case IProblem::TypeMismatch:
	case IProblem::PublicClassMustMatchFileName:
		
	case IProblem::UndefinedMethod:
	case IProblem::UndefinedConstructor:
	case IProblem::ParameterMismatch:
		return true;

	default:
		return  false;
	}
}

void JavaCorrectionProcessor::collectCorrections(ProblemPosition* problemPos,
	vector<CorrectionProposal*>& proposals)
{
	if (!problemPos)
		return;

	switch (problemPos->problemId)
	{
	case IProblem::UnusedImport:
	case IProblem::DuplicateImport:
		ReorgCorrectionsSubProcessor::removeImportStatementProposals(problemPos, proposals);
		break;



		// ParameterMismatch
		// MethodButWithConstructorName
	case IProblem::UndefinedField:
	case IProblem::UndefinedName:
		UnresolvedElementsSubProcessor::getVariableProposals(problemPos, proposals);

	case IProblem::MissingReturnType:// MissingReturnType
	case IProblem::UndefinedType:
		UnresolvedElementsSubProcessor::getTypeProposals(problemPos, SimilarElementsRequestor::ALL_TYPES, proposals);
		break;

	case IProblem::TypeMismatch:
	case IProblem::PublicClassMustMatchFileName:
		ReorgCorrectionsSubProcessor::getWrongTypeNameProposals(problemPos, proposals);
		break;
	//case SemanticError::NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD:
	//	ReorgCorrectionsSubProcessor::get_NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD_Proposals(problemPos, proposals);
	//	break;
	case IProblem::UndefinedMethod:
	case IProblem::UndefinedConstructor:
	case IProblem::ParameterMismatch:

		UnresolvedElementsSubProcessor::getMethodProposals(problemPos, proposals);
		break;
		// NonExternalizedStringLiteral
	default:
		break;
	}
}

bool JavaCorrectionProcessor::UnusedImportProblem(int problemId)
{
	switch (problemId)
	{

	case IProblem::UnusedImport:
	case IProblem::DuplicateImport:

		return true;
	

	default:
		return  false;

	}
} // Close namespace Correction block



		

}



} // Close namespace Jikes block

