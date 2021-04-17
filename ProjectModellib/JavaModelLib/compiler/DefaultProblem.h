#pragma once
#include <JCDT_Lib/internal/problem/ProblemSeverities.h>
#include <vector>
#include <set>


namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

	class ProblemReasons
		{
public:
	static const int NoError = 0;
	static const int NotFound = 1;
	static const int NotVisible = 2;
	static const int Ambiguous = 3;
	static const int InternalNameProvided = 4; // used if an internal name is used in source
	static const int InheritedNameHidesEnclosingName = 5;
	static const int NonStaticReferenceInConstructorInvocation = 6;
	static const int NonStaticReferenceInStaticContext = 7;
	static const int ReceiverTypeNotVisible = 8;
	static const int IllegalSuperTypeVariable = 9;
	static const int ParameterBoundMismatch = 10; // for generic method
	static const int TypeParameterArityMismatch = 11; // for generic method
	static const int ParameterizedMethodTypeMismatch = 12; // for generic method
	static const int TypeArgumentsForRawGenericMethod = 13; // for generic method
	static const int InvalidTypeForStaticImport = 14;
	static const int InvalidTypeForAutoManagedResource = 15;
	static const int VarargsElementTypeNotVisible = 16;
		};

	class IProblem
	{
public:
	static const int TypeRelated = 0x01000000;
	static const int FieldRelated = 0x02000000;
	static const int MethodRelated = 0x04000000;
	static const int ConstructorRelated = 0x08000000;
	static const int ImportRelated = 0x10000000;
	static const int Internal = 0x20000000;
	static const int Syntax = 0x40000000;
	/** @since 3.0 */
	static const int Javadoc = 0x80000000;

	/**
	 * Mask to use in order to filter out the category portion of the problem ID.
	 */
	static const int IgnoreCategoriesMask = 0xFFFFFF;

	/**
	 * Below are listed all available problem IDs. Note that this list could be augmented in the future,
	 * as new features are added to the Java core implementation.
	 */

	 /**
	  * ID reserved for referencing an internal error inside the JavaCore implementation which
	  * may be surfaced as a problem associated with the compilation unit which caused it to occur.
	  */
	static const int Unclassified = 0;

	/**
	 * General type related problems
	 */
	static const int ObjectHasNoSuperclass = TypeRelated + 1;
	static const int UndefinedType = TypeRelated + 2;
	static const int NotVisibleType = TypeRelated + 3;
	static const int AmbiguousType = TypeRelated + 4;
	static const int UsingDeprecatedType = TypeRelated + 5;
	static const int InternalTypeNameProvided = TypeRelated + 6;
	/** @since 2.1 */
	static const int UnusedPrivateType = Internal + TypeRelated + 7;

	static const int IncompatibleTypesInEqualityOperator = TypeRelated + 15;
	static const int IncompatibleTypesInConditionalOperator = TypeRelated + 16;
	static const int TypeMismatch = TypeRelated + 17;
	/** @since 3.0 */
	static const int IndirectAccessToStaticType = Internal + TypeRelated + 18;

	/**
	 * Inner types related problems
	 */
	static const int MissingEnclosingInstanceForConstructorCall = TypeRelated + 20;
	static const int MissingEnclosingInstance = TypeRelated + 21;
	static const int IncorrectEnclosingInstanceReference = TypeRelated + 22;
	static const int IllegalEnclosingInstanceSpecification = TypeRelated + 23;
	static const int CannotDefineStaticInitializerInLocalType = Internal + 24;
	static const int OuterLocalMustBeFinal = Internal + 25;
	static const int CannotDefineInterfaceInLocalType = Internal + 26;
	static const int IllegalPrimitiveOrArrayTypeForEnclosingInstance = TypeRelated + 27;
	/** @since 2.1 */
	static const int EnclosingInstanceInConstructorCall = Internal + 28;
	static const int AnonymousClassCannotExtendFinalClass = TypeRelated + 29;
	/** @since 3.1 */
	static const int CannotDefineAnnotationInLocalType = Internal + 30;
	/** @since 3.1 */
	static const int CannotDefineEnumInLocalType = Internal + 31;
	/** @since 3.1 */
	static const int NonStaticContextForEnumMemberType = Internal + 32;
	/** @since 3.3 */
	static const int TypeHidingType = TypeRelated + 33;

	// variables
	static const int UndefinedName = Internal + FieldRelated + 50;
	static const int UninitializedLocalVariable = Internal + 51;
	static const int VariableTypeCannotBeVoid = Internal + 52;
	/** @deprecated - problem is no longer generated, use {@link #CannotAllocateVoidArray} instead */
	static const int VariableTypeCannotBeVoidArray = Internal + 53;
	static const int CannotAllocateVoidArray = Internal + 54;
	// local variables
	static const int RedefinedLocal = Internal + 55;
	static const int RedefinedArgument = Internal + 56;
	// final local variables
	static const int DuplicateFinalLocalInitialization = Internal + 57;
	/** @since 2.1 */
	static const int NonBlankFinalLocalAssignment = Internal + 58;
	/** @since 3.2 */
	static const int ParameterAssignment = Internal + 59;
	static const int FinalOuterLocalAssignment = Internal + 60;
	static const int LocalVariableIsNeverUsed = Internal + 61;
	static const int ArgumentIsNeverUsed = Internal + 62;
	static const int BytecodeExceeds64KLimit = Internal + 63;
	static const int BytecodeExceeds64KLimitForClinit = Internal + 64;
	static const int TooManyArgumentSlots = Internal + 65;
	static const int TooManyLocalVariableSlots = Internal + 66;
	/** @since 2.1 */
	static const int TooManySyntheticArgumentSlots = Internal + 67;
	/** @since 2.1 */
	static const int TooManyArrayDimensions = Internal + 68;
	/** @since 2.1 */
	static const int BytecodeExceeds64KLimitForConstructor = Internal + 69;

	// fields
	static const int UndefinedField = FieldRelated + 70;
	static const int NotVisibleField = FieldRelated + 71;
	static const int AmbiguousField = FieldRelated + 72;
	static const int UsingDeprecatedField = FieldRelated + 73;
	static const int NonStaticFieldFromStaticInvocation = FieldRelated + 74;
	static const int ReferenceToForwardField = FieldRelated + Internal + 75;
	/** @since 2.1 */
	static const int NonStaticAccessToStaticField = Internal + FieldRelated + 76;
	/** @since 2.1 */
	static const int UnusedPrivateField = Internal + FieldRelated + 77;
	/** @since 3.0 */
	static const int IndirectAccessToStaticField = Internal + FieldRelated + 78;
	/** @since 3.0 */
	static const int UnqualifiedFieldAccess = Internal + FieldRelated + 79;
	static const int FinalFieldAssignment = FieldRelated + 80;
	static const int UninitializedBlankFinalField = FieldRelated + 81;
	static const int DuplicateBlankFinalFieldInitialization = FieldRelated + 82;
	/** @since 3.6 */
	static const int UnresolvedVariable = FieldRelated + 83;

	// variable hiding
	/** @since 3.0 */
	static const int LocalVariableHidingLocalVariable = Internal + 90;
	/** @since 3.0 */
	static const int LocalVariableHidingField = Internal + FieldRelated + 91;
	/** @since 3.0 */
	static const int FieldHidingLocalVariable = Internal + FieldRelated + 92;
	/** @since 3.0 */
	static const int FieldHidingField = Internal + FieldRelated + 93;
	/** @since 3.0 */
	static const int ArgumentHidingLocalVariable = Internal + 94;
	/** @since 3.0 */
	static const int ArgumentHidingField = Internal + 95;
	/** @since 3.1 */
	static const int MissingSerialVersion = Internal + 96;

	// methods
	static const int UndefinedMethod = MethodRelated + 100;
	static const int NotVisibleMethod = MethodRelated + 101;
	static const int AmbiguousMethod = MethodRelated + 102;
	static const int UsingDeprecatedMethod = MethodRelated + 103;
	static const int DirectInvocationOfAbstractMethod = MethodRelated + 104;
	static const int VoidMethodReturnsValue = MethodRelated + 105;
	static const int MethodReturnsVoid = MethodRelated + 106;
	static const int MethodRequiresBody = Internal + MethodRelated + 107;
	static const int ShouldReturnValue = Internal + MethodRelated + 108;
	static const int MethodButWithConstructorName = MethodRelated + 110;
	static const int MissingReturnType = TypeRelated + 111;
	static const int BodyForNativeMethod = Internal + MethodRelated + 112;
	static const int BodyForAbstractMethod = Internal + MethodRelated + 113;
	static const int NoMessageSendOnBaseType = MethodRelated + 114;
	static const int ParameterMismatch = MethodRelated + 115;
	static const int NoMessageSendOnArrayType = MethodRelated + 116;
	/** @since 2.1 */
	static const int NonStaticAccessToStaticMethod = Internal + MethodRelated + 117;
	/** @since 2.1 */
	static const int UnusedPrivateMethod = Internal + MethodRelated + 118;
	/** @since 3.0 */
	static const int IndirectAccessToStaticMethod = Internal + MethodRelated + 119;
	/** @since 3.4 */
	static const int MissingTypeInMethod = MethodRelated + 120;
	/** @since 3.7 */
	static const int MethodCanBeStatic = Internal + MethodRelated + 121;
	/** @since 3.7 */
	static const int MethodCanBePotentiallyStatic = Internal + MethodRelated + 122;

	// constructors
	/** @since 3.4 */
	static const int MissingTypeInConstructor = ConstructorRelated + 129;
	static const int UndefinedConstructor = ConstructorRelated + 130;
	static const int NotVisibleConstructor = ConstructorRelated + 131;
	static const int AmbiguousConstructor = ConstructorRelated + 132;
	static const int UsingDeprecatedConstructor = ConstructorRelated + 133;
	/** @since 2.1 */
	static const int UnusedPrivateConstructor = Internal + MethodRelated + 134;
	// explicit constructor calls
	static const int InstanceFieldDuringConstructorInvocation = ConstructorRelated + 135;
	static const int InstanceMethodDuringConstructorInvocation = ConstructorRelated + 136;
	static const int RecursiveConstructorInvocation = ConstructorRelated + 137;
	static const int ThisSuperDuringConstructorInvocation = ConstructorRelated + 138;
	/** @since 3.0 */
	static const int InvalidExplicitConstructorCall = ConstructorRelated + Syntax + 139;
	// implicit constructor calls
	static const int UndefinedConstructorInDefaultConstructor = ConstructorRelated + 140;
	static const int NotVisibleConstructorInDefaultConstructor = ConstructorRelated + 141;
	static const int AmbiguousConstructorInDefaultConstructor = ConstructorRelated + 142;
	static const int UndefinedConstructorInImplicitConstructorCall = ConstructorRelated + 143;
	static const int NotVisibleConstructorInImplicitConstructorCall = ConstructorRelated + 144;
	static const int AmbiguousConstructorInImplicitConstructorCall = ConstructorRelated + 145;
	static const int UnhandledExceptionInDefaultConstructor = TypeRelated + 146;
	static const int UnhandledExceptionInImplicitConstructorCall = TypeRelated + 147;

	// expressions
	/** @since 3.6 */
	static const int UnusedObjectAllocation = Internal + 148;
	/** @since 3.5 */
	static const int DeadCode = Internal + 149;
	static const int ArrayReferenceRequired = Internal + 150;
	static const int NoImplicitStringConversionForCharArrayExpression = Internal + 151;
	// constant expressions
	static const int StringConstantIsExceedingUtf8Limit = Internal + 152;
	static const int NonConstantExpression = Internal + 153;
	static const int NumericValueOutOfRange = Internal + 154;
	// cast expressions
	static const int IllegalCast = TypeRelated + 156;
	// allocations
	static const int InvalidClassInstantiation = TypeRelated + 157;
	static const int CannotDefineDimensionExpressionsWithInit = Internal + 158;
	static const int MustDefineEitherDimensionExpressionsOrInitializer = Internal + 159;
	// operators
	static const int InvalidOperator = Internal + 160;
	// statements
	static const int CodeCannotBeReached = Internal + 161;
	static const int CannotReturnInInitializer = Internal + 162;
	static const int InitializerMustCompleteNormally = Internal + 163;
	// assert
	static const int InvalidVoidExpression = Internal + 164;
	// try
	static const int MaskedCatch = TypeRelated + 165;
	static const int DuplicateDefaultCase = Internal + 166;
	static const int UnreachableCatch = TypeRelated + MethodRelated + 167;
	static const int UnhandledException = TypeRelated + 168;
	// switch
	static const int IncorrectSwitchType = TypeRelated + 169;
	static const int DuplicateCase = FieldRelated + 170;

	// labelled
	static const int DuplicateLabel = Internal + 171;
	static const int InvalidBreak = Internal + 172;
	static const int InvalidContinue = Internal + 173;
	static const int UndefinedLabel = Internal + 174;
	//synchronized
	static const int InvalidTypeToSynchronized = Internal + 175;
	static const int InvalidNullToSynchronized = Internal + 176;
	// throw
	static const int CannotThrowNull = Internal + 177;
	// assignment
	/** @since 2.1 */
	static const int AssignmentHasNoEffect = Internal + 178;
	/** @since 3.0 */
	static const int PossibleAccidentalBooleanAssignment = Internal + 179;
	/** @since 3.0 */
	static const int SuperfluousSemicolon = Internal + 180;
	/** @since 3.0 */
	static const int UnnecessaryCast = Internal + TypeRelated + 181;
	/** @deprecated - no longer generated, use {@link #UnnecessaryCast} instead
	 *   @since 3.0 */
	static const int UnnecessaryArgumentCast = Internal + TypeRelated + 182;
	/** @since 3.0 */
	static const int UnnecessaryInstanceof = Internal + TypeRelated + 183;
	/** @since 3.0 */
	static const int FinallyMustCompleteNormally = Internal + 184;
	/** @since 3.0 */
	static const int UnusedMethodDeclaredThrownException = Internal + 185;
	/** @since 3.0 */
	static const int UnusedConstructorDeclaredThrownException = Internal + 186;
	/** @since 3.0 */
	static const int InvalidCatchBlockSequence = Internal + TypeRelated + 187;
	/** @since 3.0 */
	static const int EmptyControlFlowStatement = Internal + TypeRelated + 188;
	/** @since 3.0 */
	static const int UnnecessaryElse = Internal + 189;

	// inner emulation
	static const int NeedToEmulateFieldReadAccess = FieldRelated + 190;
	static const int NeedToEmulateFieldWriteAccess = FieldRelated + 191;
	static const int NeedToEmulateMethodAccess = MethodRelated + 192;
	static const int NeedToEmulateConstructorAccess = MethodRelated + 193;

	/** @since 3.2 */
	static const int FallthroughCase = Internal + 194;

	//inherited name hides enclosing name (sort of ambiguous)
	static const int InheritedMethodHidesEnclosingName = MethodRelated + 195;
	static const int InheritedFieldHidesEnclosingName = FieldRelated + 196;
	static const int InheritedTypeHidesEnclosingName = TypeRelated + 197;

	/** @since 3.1 */
	static const int IllegalUsageOfQualifiedTypeReference = Internal + Syntax + 198;

	// miscellaneous
	/** @since 3.2 */
	static const int UnusedLabel = Internal + 199;
	static const int ThisInStaticContext = Internal + 200;
	static const int StaticMethodRequested = Internal + MethodRelated + 201;
	static const int IllegalDimension = Internal + 202;
	/** @deprecated - problem is no longer generated */
	static const int InvalidTypeExpression = Internal + 203;
	static const int ParsingError = Syntax + Internal + 204;
	static const int ParsingErrorNoSuggestion = Syntax + Internal + 205;
	static const int InvalidUnaryExpression = Syntax + Internal + 206;

	// syntax errors
	static const int InterfaceCannotHaveConstructors = Syntax + Internal + 207;
	static const int ArrayConstantsOnlyInArrayInitializers = Syntax + Internal + 208;
	static const int ParsingErrorOnKeyword = Syntax + Internal + 209;
	static const int ParsingErrorOnKeywordNoSuggestion = Syntax + Internal + 210;

	/** @since 3.5 */
	static const int ComparingIdentical = Internal + 211;

	static const int UnmatchedBracket = Syntax + Internal + 220;
	static const int NoFieldOnBaseType = FieldRelated + 221;
	static const int InvalidExpressionAsStatement = Syntax + Internal + 222;
	/** @since 2.1 */
	static const int ExpressionShouldBeAVariable = Syntax + Internal + 223;
	/** @since 2.1 */
	static const int MissingSemiColon = Syntax + Internal + 224;
	/** @since 2.1 */
	static const int InvalidParenthesizedExpression = Syntax + Internal + 225;

	/** @since 3.0 */
	static const int ParsingErrorInsertTokenBefore = Syntax + Internal + 230;
	/** @since 3.0 */
	static const int ParsingErrorInsertTokenAfter = Syntax + Internal + 231;
	/** @since 3.0 */
	static const int ParsingErrorDeleteToken = Syntax + Internal + 232;
	/** @since 3.0 */
	static const int ParsingErrorDeleteTokens = Syntax + Internal + 233;
	/** @since 3.0 */
	static const int ParsingErrorMergeTokens = Syntax + Internal + 234;
	/** @since 3.0 */
	static const int ParsingErrorInvalidToken = Syntax + Internal + 235;
	/** @since 3.0 */
	static const int ParsingErrorMisplacedConstruct = Syntax + Internal + 236;
	/** @since 3.0 */
	static const int ParsingErrorReplaceTokens = Syntax + Internal + 237;
	/** @since 3.0 */
	static const int ParsingErrorNoSuggestionForTokens = Syntax + Internal + 238;
	/** @since 3.0 */
	static const int ParsingErrorUnexpectedEOF = Syntax + Internal + 239;
	/** @since 3.0 */
	static const int ParsingErrorInsertToComplete = Syntax + Internal + 240;
	/** @since 3.0 */
	static const int ParsingErrorInsertToCompleteScope = Syntax + Internal + 241;
	/** @since 3.0 */
	static const int ParsingErrorInsertToCompletePhrase = Syntax + Internal + 242;

	// scanner errors
	static const int EndOfSource = Syntax + Internal + 250;
	static const int InvalidHexa = Syntax + Internal + 251;
	static const int InvalidOctal = Syntax + Internal + 252;
	static const int InvalidCharacterConstant = Syntax + Internal + 253;
	static const int InvalidEscape = Syntax + Internal + 254;
	static const int InvalidInput = Syntax + Internal + 255;
	static const int InvalidUnicodeEscape = Syntax + Internal + 256;
	static const int InvalidFloat = Syntax + Internal + 257;
	static const int NullSourceString = Syntax + Internal + 258;
	static const int UnterminatedString = Syntax + Internal + 259;
	static const int UnterminatedComment = Syntax + Internal + 260;
	static const int NonExternalizedStringLiteral = Internal + 261;
	/** @since 3.1 */
	static const int InvalidDigit = Syntax + Internal + 262;
	/** @since 3.1 */
	static const int InvalidLowSurrogate = Syntax + Internal + 263;
	/** @since 3.1 */
	static const int InvalidHighSurrogate = Syntax + Internal + 264;
	/** @since 3.2 */
	static const int UnnecessaryNLSTag = Internal + 265;
	/** @since 3.7.1 */
	static const int InvalidBinary = Syntax + Internal + 266;
	/** @since 3.7.1 */
	static const int BinaryLiteralNotBelow17 = Syntax + Internal + 267;
	/** @since 3.7.1 */
	static const int IllegalUnderscorePosition = Syntax + Internal + 268;
	/** @since 3.7.1 */
	static const int UnderscoresInLiteralsNotBelow17 = Syntax + Internal + 269;
	/** @since 3.7.1 */
	static const int IllegalHexaLiteral = Syntax + Internal + 270;

	// type related problems
	/** @since 3.1 */
	static const int DiscouragedReference = TypeRelated + 280;

	static const int InterfaceCannotHaveInitializers = TypeRelated + 300;
	static const int DuplicateModifierForType = TypeRelated + 301;
	static const int IllegalModifierForClass = TypeRelated + 302;
	static const int IllegalModifierForInterface = TypeRelated + 303;
	static const int IllegalModifierForMemberClass = TypeRelated + 304;
	static const int IllegalModifierForMemberInterface = TypeRelated + 305;
	static const int IllegalModifierForLocalClass = TypeRelated + 306;
	/** @since 3.1 */
	static const int ForbiddenReference = TypeRelated + 307;
	static const int IllegalModifierCombinationFinalAbstractForClass = TypeRelated + 308;
	static const int IllegalVisibilityModifierForInterfaceMemberType = TypeRelated + 309;
	static const int IllegalVisibilityModifierCombinationForMemberType = TypeRelated + 310;
	static const int IllegalStaticModifierForMemberType = TypeRelated + 311;
	static const int SuperclassMustBeAClass = TypeRelated + 312;
	static const int ClassExtendFinalClass = TypeRelated + 313;
	static const int DuplicateSuperInterface = TypeRelated + 314;
	static const int SuperInterfaceMustBeAnInterface = TypeRelated + 315;
	static const int HierarchyCircularitySelfReference = TypeRelated + 316;
	static const int HierarchyCircularity = TypeRelated + 317;
	static const int HidingEnclosingType = TypeRelated + 318;
	static const int DuplicateNestedType = TypeRelated + 319;
	static const int CannotThrowType = TypeRelated + 320;
	static const int PackageCollidesWithType = TypeRelated + 321;
	static const int TypeCollidesWithPackage = TypeRelated + 322;
	static const int DuplicateTypes = TypeRelated + 323;
	static const int IsClassPathCorrect = TypeRelated + 324;
	static const int PublicClassMustMatchFileName = TypeRelated + 325;
	/** @deprecated - problem is no longer generated */
	static const int MustSpecifyPackage = Internal + 326;
	static const int HierarchyHasProblems = TypeRelated + 327;
	static const int PackageIsNotExpectedPackage = Internal + 328;
	/** @since 2.1 */
	static const int ObjectCannotHaveSuperTypes = Internal + 329;
	/** @since 3.1 */
	static const int ObjectMustBeClass = Internal + 330;
	/** @since 3.4 */
	static const int RedundantSuperinterface = TypeRelated + 331;
	/** @since 3.5 */
	static const int ShouldImplementHashcode = TypeRelated + 332;
	/** @since 3.5 */
	static const int AbstractMethodsInConcreteClass = TypeRelated + 333;

	/** @deprecated - problem is no longer generated, use {@link #UndefinedType} instead */
	static const int SuperclassNotFound = TypeRelated + 329 + ProblemReasons::NotFound; // TypeRelated + 330
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int SuperclassNotVisible = TypeRelated + 329 + ProblemReasons::NotVisible; // TypeRelated + 331
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int SuperclassAmbiguous = TypeRelated + 329 + ProblemReasons::Ambiguous; // TypeRelated + 332
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int SuperclassInternalNameProvided = TypeRelated + 329 + ProblemReasons::InternalNameProvided; // TypeRelated + 333
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int SuperclassInheritedNameHidesEnclosingName = TypeRelated + 329 + ProblemReasons::InheritedNameHidesEnclosingName; // TypeRelated + 334

	/** @deprecated - problem is no longer generated, use {@link #UndefinedType} instead */
	static const int InterfaceNotFound = TypeRelated + 334 + ProblemReasons::NotFound; // TypeRelated + 335
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int InterfaceNotVisible = TypeRelated + 334 + ProblemReasons::NotVisible; // TypeRelated + 336
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int InterfaceAmbiguous = TypeRelated + 334 + ProblemReasons::Ambiguous; // TypeRelated + 337
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int InterfaceInternalNameProvided = TypeRelated + 334 + ProblemReasons::InternalNameProvided; // TypeRelated + 338
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int InterfaceInheritedNameHidesEnclosingName = TypeRelated + 334 + ProblemReasons::InheritedNameHidesEnclosingName; // TypeRelated + 339

	// field related problems
	static const int DuplicateField = FieldRelated + 340;
	static const int DuplicateModifierForField = FieldRelated + 341;
	static const int IllegalModifierForField = FieldRelated + 342;
	static const int IllegalModifierForInterfaceField = FieldRelated + 343;
	static const int IllegalVisibilityModifierCombinationForField = FieldRelated + 344;
	static const int IllegalModifierCombinationFinalVolatileForField = FieldRelated + 345;
	static const int UnexpectedStaticModifierForField = FieldRelated + 346;

	/** @deprecated - problem is no longer generated, use {@link #UndefinedType} instead */
	static const int FieldTypeNotFound = FieldRelated + 349 + ProblemReasons::NotFound; // FieldRelated + 350
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int FieldTypeNotVisible = FieldRelated + 349 + ProblemReasons::NotVisible; // FieldRelated + 351
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int FieldTypeAmbiguous = FieldRelated + 349 + ProblemReasons::Ambiguous; // FieldRelated + 352
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int FieldTypeInternalNameProvided = FieldRelated + 349 + ProblemReasons::InternalNameProvided; // FieldRelated + 353
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int FieldTypeInheritedNameHidesEnclosingName = FieldRelated + 349 + ProblemReasons::InheritedNameHidesEnclosingName; // FieldRelated + 354

	// method related problems
	static const int DuplicateMethod = MethodRelated + 355;
	static const int IllegalModifierForArgument = MethodRelated + 356;
	static const int DuplicateModifierForMethod = MethodRelated + 357;
	static const int IllegalModifierForMethod = MethodRelated + 358;
	static const int IllegalModifierForInterfaceMethod = MethodRelated + 359;
	static const int IllegalVisibilityModifierCombinationForMethod = MethodRelated + 360;
	static const int UnexpectedStaticModifierForMethod = MethodRelated + 361;
	static const int IllegalAbstractModifierCombinationForMethod = MethodRelated + 362;
	static const int AbstractMethodInAbstractClass = MethodRelated + 363;
	static const int ArgumentTypeCannotBeVoid = MethodRelated + 364;
	/** @deprecated - problem is no longer generated, use {@link #CannotAllocateVoidArray} instead */
	static const int ArgumentTypeCannotBeVoidArray = MethodRelated + 365;
	/** @deprecated - problem is no longer generated, use {@link #CannotAllocateVoidArray} instead */
	static const int ReturnTypeCannotBeVoidArray = MethodRelated + 366;
	static const int NativeMethodsCannotBeStrictfp = MethodRelated + 367;
	static const int DuplicateModifierForArgument = MethodRelated + 368;
	/** @since 3.5 */
	static const int IllegalModifierForConstructor = MethodRelated + 369;

	/** @deprecated - problem is no longer generated, use {@link #UndefinedType} instead */
	static const int ArgumentTypeNotFound = MethodRelated + 369 + ProblemReasons::NotFound; // MethodRelated + 370
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int ArgumentTypeNotVisible = MethodRelated + 369 + ProblemReasons::NotVisible; // MethodRelated + 371
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int ArgumentTypeAmbiguous = MethodRelated + 369 + ProblemReasons::Ambiguous; // MethodRelated + 372
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int ArgumentTypeInternalNameProvided = MethodRelated + 369 + ProblemReasons::InternalNameProvided; // MethodRelated + 373
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int ArgumentTypeInheritedNameHidesEnclosingName = MethodRelated + 369 + ProblemReasons::InheritedNameHidesEnclosingName; // MethodRelated + 374

	/** @deprecated - problem is no longer generated, use {@link #UndefinedType} instead */
	static const int ExceptionTypeNotFound = MethodRelated + 374 + ProblemReasons::NotFound; // MethodRelated + 375
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int ExceptionTypeNotVisible = MethodRelated + 374 + ProblemReasons::NotVisible; // MethodRelated + 376
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int ExceptionTypeAmbiguous = MethodRelated + 374 + ProblemReasons::Ambiguous; // MethodRelated + 377
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int ExceptionTypeInternalNameProvided = MethodRelated + 374 + ProblemReasons::InternalNameProvided; // MethodRelated + 378
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int ExceptionTypeInheritedNameHidesEnclosingName = MethodRelated + 374 + ProblemReasons::InheritedNameHidesEnclosingName; // MethodRelated + 379

	/** @deprecated - problem is no longer generated, use {@link #UndefinedType} instead */
	static const int ReturnTypeNotFound = MethodRelated + 379 + ProblemReasons::NotFound; // MethodRelated + 380
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int ReturnTypeNotVisible = MethodRelated + 379 + ProblemReasons::NotVisible; // MethodRelated + 381
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int ReturnTypeAmbiguous = MethodRelated + 379 + ProblemReasons::Ambiguous; // MethodRelated + 382
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int ReturnTypeInternalNameProvided = MethodRelated + 379 + ProblemReasons::InternalNameProvided; // MethodRelated + 383
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int ReturnTypeInheritedNameHidesEnclosingName = MethodRelated + 379 + ProblemReasons::InheritedNameHidesEnclosingName; // MethodRelated + 384

	// import related problems
	static const int ConflictingImport = ImportRelated + 385;
	static const int DuplicateImport = ImportRelated + 386;
	static const int CannotImportPackage = ImportRelated + 387;
	static const int UnusedImport = ImportRelated + 388;

	static const int ImportNotFound = ImportRelated + 389 + ProblemReasons::NotFound; // ImportRelated + 390
	/** @deprecated - problem is no longer generated, use {@link #NotVisibleType} instead */
	static const int ImportNotVisible = ImportRelated + 389 + ProblemReasons::NotVisible; // ImportRelated + 391
	/** @deprecated - problem is no longer generated, use {@link #AmbiguousType} instead */
	static const int ImportAmbiguous = ImportRelated + 389 + ProblemReasons::Ambiguous; // ImportRelated + 392
	/** @deprecated - problem is no longer generated, use {@link #InternalTypeNameProvided} instead */
	static const int ImportInternalNameProvided = ImportRelated + 389 + ProblemReasons::InternalNameProvided; // ImportRelated + 393
	/** @deprecated - problem is no longer generated, use {@link #InheritedTypeHidesEnclosingName} instead */
	static const int ImportInheritedNameHidesEnclosingName = ImportRelated + 389 + ProblemReasons::InheritedNameHidesEnclosingName; // ImportRelated + 394

	/** @since 3.1 */
	static const int InvalidTypeForStaticImport = ImportRelated + 391;

	// local variable related problems
	static const int DuplicateModifierForVariable = MethodRelated + 395;
	static const int IllegalModifierForVariable = MethodRelated + 396;
	/** @deprecated - problem is no longer generated, use {@link #RedundantNullCheckOnNonNullLocalVariable} instead */
	static const int LocalVariableCannotBeNull = Internal + 397; // since 3.3: semantics are LocalVariableRedundantCheckOnNonNull
	/** @deprecated - problem is no longer generated, use {@link #NullLocalVariableReference}, {@link #RedundantNullCheckOnNullLocalVariable} or {@link #RedundantLocalVariableNullAssignment} instead */
	static const int LocalVariableCanOnlyBeNull = Internal + 398; // since 3.3: split with LocalVariableRedundantCheckOnNull depending on context
	/** @deprecated - problem is no longer generated, use {@link #PotentialNullLocalVariableReference} instead */
	static const int LocalVariableMayBeNull = Internal + 399;

	// method verifier problems
	static const int AbstractMethodMustBeImplemented = MethodRelated + 400;
	static const int FinalMethodCannotBeOverridden = MethodRelated + 401;
	static const int IncompatibleExceptionInThrowsClause = MethodRelated + 402;
	static const int IncompatibleExceptionInInheritedMethodThrowsClause = MethodRelated + 403;
	static const int IncompatibleReturnType = MethodRelated + 404;
	static const int InheritedMethodReducesVisibility = MethodRelated + 405;
	static const int CannotOverrideAStaticMethodWithAnInstanceMethod = MethodRelated + 406;
	static const int CannotHideAnInstanceMethodWithAStaticMethod = MethodRelated + 407;
	static const int StaticInheritedMethodConflicts = MethodRelated + 408;
	static const int MethodReducesVisibility = MethodRelated + 409;
	static const int OverridingNonVisibleMethod = MethodRelated + 410;
	static const int AbstractMethodCannotBeOverridden = MethodRelated + 411;
	static const int OverridingDeprecatedMethod = MethodRelated + 412;
	/** @since 2.1 */
	static const int IncompatibleReturnTypeForNonInheritedInterfaceMethod = MethodRelated + 413;
	/** @since 2.1 */
	static const int IncompatibleExceptionInThrowsClauseForNonInheritedInterfaceMethod = MethodRelated + 414;
	/** @since 3.1 */
	static const int IllegalVararg = MethodRelated + 415;
	/** @since 3.3 */
	static const int OverridingMethodWithoutSuperInvocation = MethodRelated + 416;
	/** @since 3.5 */
	static const int MissingSynchronizedModifierInInheritedMethod = MethodRelated + 417;
	/** @since 3.5 */
	static const int AbstractMethodMustBeImplementedOverConcreteMethod = MethodRelated + 418;
	/** @since 3.5 */
	static const int InheritedIncompatibleReturnType = MethodRelated + 419;

	// code snippet support
	static const int CodeSnippetMissingClass = Internal + 420;
	static const int CodeSnippetMissingMethod = Internal + 421;
	static const int CannotUseSuperInCodeSnippet = Internal + 422;

	//constant pool
	static const int TooManyConstantsInConstantPool = Internal + 430;
	/** @since 2.1 */
	static const int TooManyBytesForStringConstant = Internal + 431;

	// static constraints
	/** @since 2.1 */
	static const int TooManyFields = Internal + 432;
	/** @since 2.1 */
	static const int TooManyMethods = Internal + 433;
	/** @since 3.7 */
	static const int TooManyParametersForSyntheticMethod = Internal + 434;

	// 1.4 features
	// assertion warning
	static const int UseAssertAsAnIdentifier = Internal + 440;

	// 1.5 features
	static const int UseEnumAsAnIdentifier = Internal + 441;
	/** @since 3.2 */
	static const int EnumConstantsCannotBeSurroundedByParenthesis = Syntax + Internal + 442;

	// detected task
	/** @since 2.1 */
	static const int Task = Internal + 450;

	// local variables related problems, cont'd
	/** @since 3.3 */
	static const int NullLocalVariableReference = Internal + 451;
	/** @since 3.3 */
	static const int PotentialNullLocalVariableReference = Internal + 452;
	/** @since 3.3 */
	static const int RedundantNullCheckOnNullLocalVariable = Internal + 453;
	/** @since 3.3 */
	static const int NullLocalVariableComparisonYieldsFalse = Internal + 454;
	/** @since 3.3 */
	static const int RedundantLocalVariableNullAssignment = Internal + 455;
	/** @since 3.3 */
	static const int NullLocalVariableInstanceofYieldsFalse = Internal + 456;
	/** @since 3.3 */
	static const int RedundantNullCheckOnNonNullLocalVariable = Internal + 457;
	/** @since 3.3 */
	static const int NonNullLocalVariableComparisonYieldsFalse = Internal + 458;

	// block
	/** @since 3.0 */
	static const int UndocumentedEmptyBlock = Internal + 460;

	/*
	 * Javadoc comments
	 */
	 /**
	  * Problem signaled on an invalid URL reference.
	  * Valid syntax example: @see "http://www.eclipse.org/"
	  * @since 3.4
	  */
	static const int JavadocInvalidSeeUrlReference = Javadoc + Internal + 462;
	/**
	 * Problem warned on missing tag description.
	 * @since 3.4
	 */
	static const int JavadocMissingTagDescription = Javadoc + Internal + 463;
	/**
	 * Problem warned on duplicated tag.
	 * @since 3.3
	 */
	static const int JavadocDuplicateTag = Javadoc + Internal + 464;
	/**
	 * Problem signaled on an hidden reference due to a too low visibility level.
	 * @since 3.3
	 */
	static const int JavadocHiddenReference = Javadoc + Internal + 465;
	/**
	 * Problem signaled on an invalid qualification for member type reference.
	 * @since 3.3
	 */
	static const int JavadocInvalidMemberTypeQualification = Javadoc + Internal + 466;
	/** @since 3.2 */
	static const int JavadocMissingIdentifier = Javadoc + Internal + 467;
	/** @since 3.2 */
	static const int JavadocNonStaticTypeFromStaticInvocation = Javadoc + Internal + 468;
	/** @since 3.1 */
	static const int JavadocInvalidParamTagTypeParameter = Javadoc + Internal + 469;
	/** @since 3.0 */
	static const int JavadocUnexpectedTag = Javadoc + Internal + 470;
	/** @since 3.0 */
	static const int JavadocMissingParamTag = Javadoc + Internal + 471;
	/** @since 3.0 */
	static const int JavadocMissingParamName = Javadoc + Internal + 472;
	/** @since 3.0 */
	static const int JavadocDuplicateParamName = Javadoc + Internal + 473;
	/** @since 3.0 */
	static const int JavadocInvalidParamName = Javadoc + Internal + 474;
	/** @since 3.0 */
	static const int JavadocMissingReturnTag = Javadoc + Internal + 475;
	/** @since 3.0 */
	static const int JavadocDuplicateReturnTag = Javadoc + Internal + 476;
	/** @since 3.0 */
	static const int JavadocMissingThrowsTag = Javadoc + Internal + 477;
	/** @since 3.0 */
	static const int JavadocMissingThrowsClassName = Javadoc + Internal + 478;
	/** @since 3.0 */
	static const int JavadocInvalidThrowsClass = Javadoc + Internal + 479;
	/** @since 3.0 */
	static const int JavadocDuplicateThrowsClassName = Javadoc + Internal + 480;
	/** @since 3.0 */
	static const int JavadocInvalidThrowsClassName = Javadoc + Internal + 481;
	/** @since 3.0 */
	static const int JavadocMissingSeeReference = Javadoc + Internal + 482;
	/** @since 3.0 */
	static const int JavadocInvalidSeeReference = Javadoc + Internal + 483;
	/**
	 * Problem signaled on an invalid URL reference that does not conform to the href syntax.
	 * Valid syntax example: @see <a href="http://www.eclipse.org/">Eclipse Home Page</a>
	 * @since 3.0
	 */
	static const int JavadocInvalidSeeHref = Javadoc + Internal + 484;
	/** @since 3.0 */
	static const int JavadocInvalidSeeArgs = Javadoc + Internal + 485;
	/** @since 3.0 */
	static const int JavadocMissing = Javadoc + Internal + 486;
	/** @since 3.0 */
	static const int JavadocInvalidTag = Javadoc + Internal + 487;
	/*
	 * ID for field errors in Javadoc
	 */
	 /** @since 3.0 */
	static const int JavadocUndefinedField = Javadoc + Internal + 488;
	/** @since 3.0 */
	static const int JavadocNotVisibleField = Javadoc + Internal + 489;
	/** @since 3.0 */
	static const int JavadocAmbiguousField = Javadoc + Internal + 490;
	/** @since 3.0 */
	static const int JavadocUsingDeprecatedField = Javadoc + Internal + 491;
	/*
	 * IDs for constructor errors in Javadoc
	 */
	 /** @since 3.0 */
	static const int JavadocUndefinedConstructor = Javadoc + Internal + 492;
	/** @since 3.0 */
	static const int JavadocNotVisibleConstructor = Javadoc + Internal + 493;
	/** @since 3.0 */
	static const int JavadocAmbiguousConstructor = Javadoc + Internal + 494;
	/** @since 3.0 */
	static const int JavadocUsingDeprecatedConstructor = Javadoc + Internal + 495;
	/*
	 * IDs for method errors in Javadoc
	 */
	 /** @since 3.0 */
	static const int JavadocUndefinedMethod = Javadoc + Internal + 496;
	/** @since 3.0 */
	static const int JavadocNotVisibleMethod = Javadoc + Internal + 497;
	/** @since 3.0 */
	static const int JavadocAmbiguousMethod = Javadoc + Internal + 498;
	/** @since 3.0 */
	static const int JavadocUsingDeprecatedMethod = Javadoc + Internal + 499;
	/** @since 3.0 */
	static const int JavadocNoMessageSendOnBaseType = Javadoc + Internal + 500;
	/** @since 3.0 */
	static const int JavadocParameterMismatch = Javadoc + Internal + 501;
	/** @since 3.0 */
	static const int JavadocNoMessageSendOnArrayType = Javadoc + Internal + 502;
	/*
	 * IDs for type errors in Javadoc
	 */
	 /** @since 3.0 */
	static const int JavadocUndefinedType = Javadoc + Internal + 503;
	/** @since 3.0 */
	static const int JavadocNotVisibleType = Javadoc + Internal + 504;
	/** @since 3.0 */
	static const int JavadocAmbiguousType = Javadoc + Internal + 505;
	/** @since 3.0 */
	static const int JavadocUsingDeprecatedType = Javadoc + Internal + 506;
	/** @since 3.0 */
	static const int JavadocInternalTypeNameProvided = Javadoc + Internal + 507;
	/** @since 3.0 */
	static const int JavadocInheritedMethodHidesEnclosingName = Javadoc + Internal + 508;
	/** @since 3.0 */
	static const int JavadocInheritedFieldHidesEnclosingName = Javadoc + Internal + 509;
	/** @since 3.0 */
	static const int JavadocInheritedNameHidesEnclosingTypeName = Javadoc + Internal + 510;
	/** @since 3.0 */
	static const int JavadocAmbiguousMethodReference = Javadoc + Internal + 511;
	/** @since 3.0 */
	static const int JavadocUnterminatedInlineTag = Javadoc + Internal + 512;
	/** @since 3.0 */
	static const int JavadocMalformedSeeReference = Javadoc + Internal + 513;
	/** @since 3.0 */
	static const int JavadocMessagePrefix = Internal + 514;

	/** @since 3.1 */
	static const int JavadocMissingHashCharacter = Javadoc + Internal + 515;
	/** @since 3.1 */
	static const int JavadocEmptyReturnTag = Javadoc + Internal + 516;
	/** @since 3.1 */
	static const int JavadocInvalidValueReference = Javadoc + Internal + 517;
	/** @since 3.1 */
	static const int JavadocUnexpectedText = Javadoc + Internal + 518;
	/** @since 3.1 */
	static const int JavadocInvalidParamTagName = Javadoc + Internal + 519;

	/**
	 * Generics
	 */
	 /** @since 3.1 */
	static const int DuplicateTypeVariable = Internal + 520;
	/** @since 3.1 */
	static const int IllegalTypeVariableSuperReference = Internal + 521;
	/** @since 3.1 */
	static const int NonStaticTypeFromStaticInvocation = Internal + 522;
	/** @since 3.1 */
	static const int ObjectCannotBeGeneric = Internal + 523;
	/** @since 3.1 */
	static const int NonGenericType = TypeRelated + 524;
	/** @since 3.1 */
	static const int IncorrectArityForParameterizedType = TypeRelated + 525;
	/** @since 3.1 */
	static const int TypeArgumentMismatch = TypeRelated + 526;
	/** @since 3.1 */
	static const int DuplicateMethodErasure = TypeRelated + 527;
	/** @since 3.1 */
	static const int ReferenceToForwardTypeVariable = TypeRelated + 528;
	/** @since 3.1 */
	static const int BoundMustBeAnInterface = TypeRelated + 529;
	/** @since 3.1 */
	static const int UnsafeRawConstructorInvocation = TypeRelated + 530;
	/** @since 3.1 */
	static const int UnsafeRawMethodInvocation = TypeRelated + 531;
	/** @since 3.1 */
	static const int UnsafeTypeConversion = TypeRelated + 532;
	/** @since 3.1 */
	static const int InvalidTypeVariableExceptionType = TypeRelated + 533;
	/** @since 3.1 */
	static const int InvalidParameterizedExceptionType = TypeRelated + 534;
	/** @since 3.1 */
	static const int IllegalGenericArray = TypeRelated + 535;
	/** @since 3.1 */
	static const int UnsafeRawFieldAssignment = TypeRelated + 536;
	/** @since 3.1 */
	static const int FinalBoundForTypeVariable = TypeRelated + 537;
	/** @since 3.1 */
	static const int UndefinedTypeVariable = Internal + 538;
	/** @since 3.1 */
	static const int SuperInterfacesCollide = TypeRelated + 539;
	/** @since 3.1 */
	static const int WildcardConstructorInvocation = TypeRelated + 540;
	/** @since 3.1 */
	static const int WildcardMethodInvocation = TypeRelated + 541;
	/** @since 3.1 */
	static const int WildcardFieldAssignment = TypeRelated + 542;
	/** @since 3.1 */
	static const int GenericMethodTypeArgumentMismatch = TypeRelated + 543;
	/** @since 3.1 */
	static const int GenericConstructorTypeArgumentMismatch = TypeRelated + 544;
	/** @since 3.1 */
	static const int UnsafeGenericCast = TypeRelated + 545;
	/** @since 3.1 */
	static const int IllegalInstanceofParameterizedType = Internal + 546;
	/** @since 3.1 */
	static const int IllegalInstanceofTypeParameter = Internal + 547;
	/** @since 3.1 */
	static const int NonGenericMethod = TypeRelated + 548;
	/** @since 3.1 */
	static const int IncorrectArityForParameterizedMethod = TypeRelated + 549;
	/** @since 3.1 */
	static const int ParameterizedMethodArgumentTypeMismatch = TypeRelated + 550;
	/** @since 3.1 */
	static const int NonGenericConstructor = TypeRelated + 551;
	/** @since 3.1 */
	static const int IncorrectArityForParameterizedConstructor = TypeRelated + 552;
	/** @since 3.1 */
	static const int ParameterizedConstructorArgumentTypeMismatch = TypeRelated + 553;
	/** @since 3.1 */
	static const int TypeArgumentsForRawGenericMethod = TypeRelated + 554;
	/** @since 3.1 */
	static const int TypeArgumentsForRawGenericConstructor = TypeRelated + 555;
	/** @since 3.1 */
	static const int SuperTypeUsingWildcard = TypeRelated + 556;
	/** @since 3.1 */
	static const int GenericTypeCannotExtendThrowable = TypeRelated + 557;
	/** @since 3.1 */
	static const int IllegalClassLiteralForTypeVariable = TypeRelated + 558;
	/** @since 3.1 */
	static const int UnsafeReturnTypeOverride = MethodRelated + 559;
	/** @since 3.1 */
	static const int MethodNameClash = MethodRelated + 560;
	/** @since 3.1 */
	static const int RawMemberTypeCannotBeParameterized = TypeRelated + 561;
	/** @since 3.1 */
	static const int MissingArgumentsForParameterizedMemberType = TypeRelated + 562;
	/** @since 3.1 */
	static const int StaticMemberOfParameterizedType = TypeRelated + 563;
	/** @since 3.1 */
	static const int BoundHasConflictingArguments = TypeRelated + 564;
	/** @since 3.1 */
	static const int DuplicateParameterizedMethods = MethodRelated + 565;
	/** @since 3.1 */
	static const int IllegalQualifiedParameterizedTypeAllocation = TypeRelated + 566;
	/** @since 3.1 */
	static const int DuplicateBounds = TypeRelated + 567;
	/** @since 3.1 */
	static const int BoundCannotBeArray = TypeRelated + 568;
	/** @since 3.1 */
	static const int UnsafeRawGenericConstructorInvocation = TypeRelated + 569;
	/** @since 3.1 */
	static const int UnsafeRawGenericMethodInvocation = TypeRelated + 570;
	/** @since 3.1 */
	static const int TypeParameterHidingType = TypeRelated + 571;
	/** @since 3.2 */
	static const int RawTypeReference = TypeRelated + 572;
	/** @since 3.2 */
	static const int NoAdditionalBoundAfterTypeVariable = TypeRelated + 573;
	/** @since 3.2 */
	static const int UnsafeGenericArrayForVarargs = MethodRelated + 574;
	/** @since 3.2 */
	static const int IllegalAccessFromTypeVariable = TypeRelated + 575;
	/** @since 3.3 */
	static const int TypeHidingTypeParameterFromType = TypeRelated + 576;
	/** @since 3.3 */
	static const int TypeHidingTypeParameterFromMethod = TypeRelated + 577;
	/** @since 3.3 */
	static const int InvalidUsageOfWildcard = Syntax + Internal + 578;
	/** @since 3.4 */
	static const int UnusedTypeArgumentsForMethodInvocation = MethodRelated + 579;

	/**
	 * Foreach
	 */
	 /** @since 3.1 */
	static const int IncompatibleTypesInForeach = TypeRelated + 580;
	/** @since 3.1 */
	static const int InvalidTypeForCollection = Internal + 581;
	/** @since 3.6*/
	static const int InvalidTypeForCollectionTarget14 = Internal + 582;

	/** @since 3.7.1 */
	static const int DuplicateInheritedMethods = MethodRelated + 583;
	/** @since 3.8 */
	static const int MethodNameClashHidden = MethodRelated + 584;

	/**
	 * 1.5 Syntax errors (when source level < 1.5)
	 */
	 /** @since 3.1 */
	static const int InvalidUsageOfTypeParameters = Syntax + Internal + 590;
	/** @since 3.1 */
	static const int InvalidUsageOfStaticImports = Syntax + Internal + 591;
	/** @since 3.1 */
	static const int InvalidUsageOfForeachStatements = Syntax + Internal + 592;
	/** @since 3.1 */
	static const int InvalidUsageOfTypeArguments = Syntax + Internal + 593;
	/** @since 3.1 */
	static const int InvalidUsageOfEnumDeclarations = Syntax + Internal + 594;
	/** @since 3.1 */
	static const int InvalidUsageOfVarargs = Syntax + Internal + 595;
	/** @since 3.1 */
	static const int InvalidUsageOfAnnotations = Syntax + Internal + 596;
	/** @since 3.1 */
	static const int InvalidUsageOfAnnotationDeclarations = Syntax + Internal + 597;
	/** @since 3.4 */
	static const int InvalidUsageOfTypeParametersForAnnotationDeclaration = Syntax + Internal + 598;
	/** @since 3.4 */
	static const int InvalidUsageOfTypeParametersForEnumDeclaration = Syntax + Internal + 599;
	/**
	 * Annotation
	 */
	 /** @since 3.1 */
	static const int IllegalModifierForAnnotationMethod = MethodRelated + 600;
	/** @since 3.1 */
	static const int IllegalExtendedDimensions = MethodRelated + 601;
	/** @since 3.1 */
	static const int InvalidFileNameForPackageAnnotations = Syntax + Internal + 602;
	/** @since 3.1 */
	static const int IllegalModifierForAnnotationType = TypeRelated + 603;
	/** @since 3.1 */
	static const int IllegalModifierForAnnotationMemberType = TypeRelated + 604;
	/** @since 3.1 */
	static const int InvalidAnnotationMemberType = TypeRelated + 605;
	/** @since 3.1 */
	static const int AnnotationCircularitySelfReference = TypeRelated + 606;
	/** @since 3.1 */
	static const int AnnotationCircularity = TypeRelated + 607;
	/** @since 3.1 */
	static const int DuplicateAnnotation = TypeRelated + 608;
	/** @since 3.1 */
	static const int MissingValueForAnnotationMember = TypeRelated + 609;
	/** @since 3.1 */
	static const int DuplicateAnnotationMember = Internal + 610;
	/** @since 3.1 */
	static const int UndefinedAnnotationMember = MethodRelated + 611;
	/** @since 3.1 */
	static const int AnnotationValueMustBeClassLiteral = Internal + 612;
	/** @since 3.1 */
	static const int AnnotationValueMustBeConstant = Internal + 613;
	/** @deprecated - problem is no longer generated (code is legite)
	 *   @since 3.1 */
	static const int AnnotationFieldNeedConstantInitialization = Internal + 614;
	/** @since 3.1 */
	static const int IllegalModifierForAnnotationField = Internal + 615;
	/** @since 3.1 */
	static const int AnnotationCannotOverrideMethod = MethodRelated + 616;
	/** @since 3.1 */
	static const int AnnotationMembersCannotHaveParameters = Syntax + Internal + 617;
	/** @since 3.1 */
	static const int AnnotationMembersCannotHaveTypeParameters = Syntax + Internal + 618;
	/** @since 3.1 */
	static const int AnnotationTypeDeclarationCannotHaveSuperclass = Syntax + Internal + 619;
	/** @since 3.1 */
	static const int AnnotationTypeDeclarationCannotHaveSuperinterfaces = Syntax + Internal + 620;
	/** @since 3.1 */
	static const int DuplicateTargetInTargetAnnotation = Internal + 621;
	/** @since 3.1 */
	static const int DisallowedTargetForAnnotation = TypeRelated + 622;
	/** @since 3.1 */
	static const int MethodMustOverride = MethodRelated + 623;
	/** @since 3.1 */
	static const int AnnotationTypeDeclarationCannotHaveConstructor = Syntax + Internal + 624;
	/** @since 3.1 */
	static const int AnnotationValueMustBeAnnotation = Internal + 625;
	/** @since 3.1 */
	static const int AnnotationTypeUsedAsSuperInterface = TypeRelated + 626;
	/** @since 3.1 */
	static const int MissingOverrideAnnotation = MethodRelated + 627;
	/** @since 3.1 */
	static const int FieldMissingDeprecatedAnnotation = Internal + 628;
	/** @since 3.1 */
	static const int MethodMissingDeprecatedAnnotation = Internal + 629;
	/** @since 3.1 */
	static const int TypeMissingDeprecatedAnnotation = Internal + 630;
	/** @since 3.1 */
	static const int UnhandledWarningToken = Internal + 631;
	/** @since 3.2 */
	static const int AnnotationValueMustBeArrayInitializer = Internal + 632;
	/** @since 3.3 */
	static const int AnnotationValueMustBeAnEnumConstant = Internal + 633;
	/** @since 3.3 */
	static const int MethodMustOverrideOrImplement = MethodRelated + 634;
	/** @since 3.4 */
	static const int UnusedWarningToken = Internal + 635;
	/** @since 3.6 */
	static const int MissingOverrideAnnotationForInterfaceMethodImplementation = MethodRelated + 636;

	/**
	 * More problems in generics
	 */
	 /** @since 3.4 */
	static const int UnusedTypeArgumentsForConstructorInvocation = MethodRelated + 660;

	/**
	 * Corrupted binaries
	 */
	 /** @since 3.1 */
	static const int CorruptedSignature = Internal + 700;
	/**
	 * Corrupted source
	 */
	 /** @since 3.2 */
	static const int InvalidEncoding = Internal + 701;
	/** @since 3.2 */
	static const int CannotReadSource = Internal + 702;

	/**
	 * Autoboxing
	 */
	 /** @since 3.1 */
	static const int BoxingConversion = Internal + 720;
	/** @since 3.1 */
	static const int UnboxingConversion = Internal + 721;

	/**
	 * Enum
	 */
	 /** @since 3.1 */
	static const int IllegalModifierForEnum = TypeRelated + 750;
	/** @since 3.1 */
	static const int IllegalModifierForEnumConstant = FieldRelated + 751;
	/** @deprecated - problem could not be reported, enums cannot be local takes precedence
	 *   @since 3.1 */
	static const int IllegalModifierForLocalEnum = TypeRelated + 752;
	/** @since 3.1 */
	static const int IllegalModifierForMemberEnum = TypeRelated + 753;
	/** @since 3.1 */
	static const int CannotDeclareEnumSpecialMethod = MethodRelated + 754;
	/** @since 3.1 */
	static const int IllegalQualifiedEnumConstantLabel = FieldRelated + 755;
	/** @since 3.1 */
	static const int CannotExtendEnum = TypeRelated + 756;
	/** @since 3.1 */
	static const int CannotInvokeSuperConstructorInEnum = MethodRelated + 757;
	/** @since 3.1 */
	static const int EnumAbstractMethodMustBeImplemented = MethodRelated + 758;
	/** @since 3.1 */
	static const int EnumSwitchCannotTargetField = FieldRelated + 759;
	/** @since 3.1 */
	static const int IllegalModifierForEnumConstructor = MethodRelated + 760;
	/** @since 3.1 */
	static const int MissingEnumConstantCase = FieldRelated + 761;
	/** @since 3.2 */ // TODO need to fix 3.1.1 contribution (inline this constant on client side)
	static const int EnumStaticFieldInInInitializerContext = FieldRelated + 762;
	/** @since 3.4 */
	static const int EnumConstantMustImplementAbstractMethod = MethodRelated + 763;
	/** @since 3.5 */
	static const int EnumConstantCannotDefineAbstractMethod = MethodRelated + 764;
	/** @since 3.5 */
	static const int AbstractMethodInEnum = MethodRelated + 765;
	/** @since 3.8 */
	static const int MissingEnumDefaultCase = Internal + 766;
	/** @since 3.8 */
	static const int MissingDefaultCase = Internal + 767;
	/** @since 3.8 */
	static const int MissingEnumConstantCaseDespiteDefault = FieldRelated + 768;
	/** @since 3.8 */
	static const int UninitializedLocalVariableHintMissingDefault = Internal + 769;
	/** @since 3.8 */
	static const int UninitializedBlankFinalFieldHintMissingDefault = FieldRelated + 770;
	/** @since 3.8 */
	static const int ShouldReturnValueHintMissingDefault = MethodRelated + 771;

	/**
	 * Var args
	 */
	 /** @since 3.1 */
	static const int IllegalExtendedDimensionsForVarArgs = Syntax + Internal + 800;
	/** @since 3.1 */
	static const int MethodVarargsArgumentNeedCast = MethodRelated + 801;
	/** @since 3.1 */
	static const int ConstructorVarargsArgumentNeedCast = ConstructorRelated + 802;
	/** @since 3.1 */
	static const int VarargsConflict = MethodRelated + 803;
	/** @since 3.7.1 */
	static const int SafeVarargsOnFixedArityMethod = MethodRelated + 804;
	/** @since 3.7.1 */
	static const int SafeVarargsOnNonFinalInstanceMethod = MethodRelated + 805;
	/** @since 3.7.1 */
	static const int PotentialHeapPollutionFromVararg = MethodRelated + 806;
	/** @since 3.8 */
	static const int VarargsElementTypeNotVisible = MethodRelated + 807;
	/** @since 3.8 */
	static const int VarargsElementTypeNotVisibleForConstructor = ConstructorRelated + 808;
	/**
	 * Javadoc Generic
	 */
	 /** @since 3.1 */
	static const int JavadocGenericMethodTypeArgumentMismatch = Javadoc + Internal + 850;
	/** @since 3.1 */
	static const int JavadocNonGenericMethod = Javadoc + Internal + 851;
	/** @since 3.1 */
	static const int JavadocIncorrectArityForParameterizedMethod = Javadoc + Internal + 852;
	/** @since 3.1 */
	static const int JavadocParameterizedMethodArgumentTypeMismatch = Javadoc + Internal + 853;
	/** @since 3.1 */
	static const int JavadocTypeArgumentsForRawGenericMethod = Javadoc + Internal + 854;
	/** @since 3.1 */
	static const int JavadocGenericConstructorTypeArgumentMismatch = Javadoc + Internal + 855;
	/** @since 3.1 */
	static const int JavadocNonGenericConstructor = Javadoc + Internal + 856;
	/** @since 3.1 */
	static const int JavadocIncorrectArityForParameterizedConstructor = Javadoc + Internal + 857;
	/** @since 3.1 */
	static const int JavadocParameterizedConstructorArgumentTypeMismatch = Javadoc + Internal + 858;
	/** @since 3.1 */
	static const int JavadocTypeArgumentsForRawGenericConstructor = Javadoc + Internal + 859;

	/**
	 * Java 7 errors
	 */
	 /** @since 3.7.1 */
	static const int AssignmentToMultiCatchParameter = Internal + 870;
	/** @since 3.7.1 */
	static const int ResourceHasToImplementAutoCloseable = TypeRelated + 871;
	/** @since 3.7.1 */
	static const int AssignmentToResource = Internal + 872;
	/** @since 3.7.1 */
	static const int InvalidUnionTypeReferenceSequence = Internal + TypeRelated + 873;
	/** @since 3.7.1 */
	static const int AutoManagedResourceNotBelow17 = Syntax + Internal + 874;
	/** @since 3.7.1 */
	static const int MultiCatchNotBelow17 = Syntax + Internal + 875;
	/** @since 3.7.1 */
	static const int PolymorphicMethodNotBelow17 = MethodRelated + 876;
	/** @since 3.7.1 */
	static const int IncorrectSwitchType17 = TypeRelated + 877;
	/** @since 3.7.1 */
	static const int CannotInferElidedTypes = TypeRelated + 878;
	/** @since 3.7.1 */
	static const int CannotUseDiamondWithExplicitTypeArguments = TypeRelated + 879;
	/** @since 3.7.1 */
	static const int CannotUseDiamondWithAnonymousClasses = TypeRelated + 880;
	/** @since 3.7.1 */
	static const int SwitchOnStringsNotBelow17 = TypeRelated + 881;	// https://bugs.eclipse.org/bugs/show_bug.cgi?id=348492
	/** @since 3.7.1 */
	static const int UnhandledExceptionOnAutoClose = TypeRelated + 882;
	/** @since 3.7.1 */
	static const int DiamondNotBelow17 = TypeRelated + 883;
	/** @since 3.7.1 */
	static const int RedundantSpecificationOfTypeArguments = TypeRelated + 884;
	/** @since 3.8 */
	static const int PotentiallyUnclosedCloseable = Internal + 885;
	/** @since 3.8 */
	static const int PotentiallyUnclosedCloseableAtExit = Internal + 886;
	/** @since 3.8 */
	static const int UnclosedCloseable = Internal + 887;
	/** @since 3.8 */
	static const int UnclosedCloseableAtExit = Internal + 888;
	/** @since 3.8 */
	static const int ExplicitlyClosedAutoCloseable = Internal + 889;
	/** @since 3.8 */
	static const int SwitchOnEnumNotBelow15 = TypeRelated + 890;	// https://bugs.eclipse.org/bugs/show_bug.cgi?id=360317
	/**
	 * Errors/warnings from annotation based null analysis
	 */
	 /** @since 3.8 */
	static const int RequiredNonNullButProvidedNull = TypeRelated + 910;
	/** @since 3.8 */
	static const int RequiredNonNullButProvidedPotentialNull = TypeRelated + 911;
	/** @since 3.8 */
	static const int RequiredNonNullButProvidedUnknown = TypeRelated + 912;
	/** @since 3.8 */
	static const int MissingNonNullByDefaultAnnotationOnPackage = Internal + 913; // https://bugs.eclipse.org/bugs/show_bug.cgi?id=372012
	/** @since 3.8 */
	static const int IllegalReturnNullityRedefinition = MethodRelated + 914;
	/** @since 3.8 */
	static const int IllegalRedefinitionToNonNullParameter = MethodRelated + 915;
	/** @since 3.8 */
	static const int IllegalDefinitionToNonNullParameter = MethodRelated + 916;
	/** @since 3.8 */
	static const int ParameterLackingNonNullAnnotation = MethodRelated + 917;
	/** @since 3.8 */
	static const int ParameterLackingNullableAnnotation = MethodRelated + 918;
	/** @since 3.8 */
	static const int PotentialNullMessageSendReference = Internal + 919;
	/** @since 3.8 */
	static const int RedundantNullCheckOnNonNullMessageSend = Internal + 920;
	/** @since 3.8 */
	static const int CannotImplementIncompatibleNullness = Internal + 921;
	/** @since 3.8 */
	static const int RedundantNullAnnotation = MethodRelated + 922;
	/** @since 3.8 */
	static const int IllegalAnnotationForBaseType = TypeRelated + 923;
	/** @since 3.8 */
	static const int RedundantNullDefaultAnnotation = Internal + 925; // shouldn't actually occur any more after bug 366063
	/** @since 3.8 */
	static const int RedundantNullDefaultAnnotationPackage = Internal + 926;
	/** @since 3.8 */
	static const int RedundantNullDefaultAnnotationType = Internal + 927;
	/** @since 3.8 */
	static const int RedundantNullDefaultAnnotationMethod = Internal + 928;
	/** @since 3.8 */
	static const int ContradictoryNullAnnotations = Internal + 929;
	/** @since 3.8 */
	static const int MissingNonNullByDefaultAnnotationOnType = Internal + 930; // https://bugs.eclipse.org/bugs/show_bug.cgi?id=372012
	/** @since 3.8 */
	static const int RedundantNullCheckOnSpecdNonNullLocalVariable = Internal + 931;
	/** @since 3.8 */
	static const int SpecdNonNullLocalVariableComparisonYieldsFalse = Internal + 932;
	/** @since 3.8 */
	static const int RequiredNonNullButProvidedSpecdNullable = Internal + 933;

	/**
	 * External problems -- These are problems defined by other plugins
	 */

	 /** @since 3.2 */
	static const int ExternalProblemNotFixable = 900;

	// indicates an externally defined problem that has a quick-assist processor
	// associated with it
	/** @since 3.2 */
	static const int ExternalProblemFixable = 901;
		};


	class DefaultProblem /* :public  JikesError*/
	{
	public:
		
		struct  source_context
		{
			int sourceEnd = 0;
			int sourceStart = 0;
			std::wstring value;
		};
		DefaultProblem();
		Problem::Severity getSeverity()
		{
			return  severity;
		}

		const char* getFileName()
		{
			return fileName.c_str();
		}

		int getLeftLineNo()
		{
			return line;
		}

		int getLeftColumnNo()
		{
			return column;
		}

		int getRightLineNo()
		{
			return rline;
		}

		int getRightColumnNo()
		{
			return rcolumn;
		}

		std::wstring getErrorMessage()
		{
			return message;
		}

		bool DefaultProblem::IsInRange(int pos)
		{
			if (startPosition <= pos && pos <= endPosition)
				return true;
			return false;
		}

		std::wstring getErrorReport() 
		{
			return  message;
		};
		std::string fileName;
		 int id;
		 int startPosition;
		 int endPosition;
		 int line;
		 int column;

		 int rline;
		 int rcolumn;
		 Problem::Severity severity;
		 std::vector<std::wstring> arguments;
		std::wstring message;
		 source_context context;
		 
	};
	struct ECJSource
	{
		void TransferError(std::vector<DefaultProblem*>& future_owner);
	    unsigned	NumErrors();
		std::wstring  filePath;
		std::set<std::wstring> class_file_paths;
		std::vector<DefaultProblem*> problems;
		~ECJSource();
	};

	extern  void ParserXmlData(const std::wstring& data, std::vector< ECJSource*>&);
	} // Close namespace JavaModel block

} // Close namespace Jikes block

