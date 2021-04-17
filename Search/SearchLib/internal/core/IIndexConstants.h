#ifndef  IIndexConstants_JIKES_SERACH_INCLUDE
#define  IIndexConstants_JIKES_SERACH_INCLUDE


namespace Jikes { // Open namespace Jikes block

	namespace Search {

		class  IIndexConstants
		{
		public:
			/* index encoding */
			//static const wchar_t* REF = L"ref/"; //$NON-NLS-1$
			//static const wchar_t* FIELD_REF = L"fieldRef/"; //$NON-NLS-1$
			//static const wchar_t* METHOD_REF = L"methodRef/"; //$NON-NLS-1$
			//static const wchar_t* CONSTRUCTOR_REF = L"constructorRef/"; //$NON-NLS-1$
			//static const wchar_t* TYPE_REF = L"typeRef/"; //$NON-NLS-1$
			//static const wchar_t* SUPER_REF = L"superRef/"; //$NON-NLS-1$
			//static const wchar_t* TYPE_DECL = L"typeDecl/"; //$NON-NLS-1$
			//static const int 	TYPE_DECL_LENGTH = 9;
			//static const wchar_t* CLASS_DECL = L"typeDecl/C/"; //$NON-NLS-1$
			//static const wchar_t* INTERFACE_DECL = L"typeDecl/I/"; //$NON-NLS-1$
			//static const wchar_t* METHOD_DECL = L"methodDecl/"; //$NON-NLS-1$
			//static const wchar_t* CONSTRUCTOR_DECL = L"constructorDecl/"; //$NON-NLS-1$
			//static const wchar_t* FIELD_DECL = L"fieldDecl/"; //$NON-NLS-1$
			//static const wchar_t* OBJECT = L"Object"; //$NON-NLS-1$
			/*static const wchar_t*[] COUNTS =
				new static const wchar_t*[]{ new static const wchar_t* {'0'},
				new static const wchar_t* {'1'}, 
				new static const wchar_t* {'2'}, new static const wchar_t* {'3'},
				new static const wchar_t* {'4'}, new static const wchar_t* {'5'}, new static const wchar_t* {'6'}, 
				new static const wchar_t* {'7'}, new static const wchar_t* {'8'}, new static const wchar_t* {'9'}
			};*/
			static const wchar_t CLASS_SUFFIX = 'C';
			static const wchar_t INTERFACE_SUFFIX = 'I';
			static const wchar_t TYPE_SUFFIX = 0;
			static const wchar_t SEPARATOR = '/';

			static const wchar_t* ONE_STAR;
		//	static const wchar_t*[] ONE_STAR_CHAR = new static const wchar_t*[]{ ONE_STAR };
		//	static const wchar_t* NO_CHAR = new wchar_t[0];
			//static const wchar_t*[] NO_CHAR_CHAR = new wchar_t[0][];

			// used as special marker for enclosing type name of local and anonymous classes
		//	static const wchar_t* ONE_ZERO = new static const wchar_t* {'0'};
		//	static const wchar_t*[] ONE_ZERO_CHAR = new static const wchar_t*[]{ ONE_ZERO };
		};
	}// Close namespace Search block


} // Close namespace Jikes block
#endif