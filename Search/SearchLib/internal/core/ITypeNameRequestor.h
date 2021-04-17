

/**
 * A <code>ITypeNameRequestor</code> collects search results from a <code>searchAllTypeNames</code>
 * query to a <code>SearchEngine</code>. Clients must implement this interface and pass
 * an instance to the <code>searchAllTypeNames(...)</code> method. Only top-level and
 * member types are reported. Local types are not reported.
 * <p>
 * This interface may be implemented by clients.
 * </p>
 */

#ifndef  ITypeNameRequestor_JIKES_SERACH_INCLUDE
#define ITypeNameRequestor_JIKES_SERACH_INCLUDE
#include <string>
#include <vector>
namespace Jikes { // Open namespace Jikes block

	namespace Search {
class  ITypeNameRequestor {
    public :
	virtual ~ITypeNameRequestor()
	{
	}

	/**
	 * Accepts a top-level or a member class.
	 *
	 * @param packageName the dot-separated name of the package of the class
	 * @param simpleTypeName the simple name of the class
	 * @param enclosingTypeNames if the class is a member type, 
	 *          the simple names of the enclosing types from the outer-most to the
	 *          direct parent of the class (e.g. if the class is x.y.A$B$C then
	 *          the enclosing types are [A, B]. This is an empty array if the class
	 *          is a top-level type.
	 * @param path the full path to the resource containing the class. If the resource is a .class file
	 *          or a .java file, this is the full path in the workspace to this resource. If the
	 *          resource is a .zip or .jar file, this is the full OS path to this file.
	 */
virtual void acceptClass(const std::wstring& packageName, const std::wstring& simpleTypeName, std::vector<std::wstring>& enclosingTypeNames, std::wstring path) = 0;
/**
 * Accepts a top-level or a member interface.
 *
 * @param packageName the dot-separated name of the package of the interface
 * @param simpleTypeName the simple name of the interface
 * @param enclosingTypeNames if the interface is a member type, 
 *          the simple names of the enclosing types from the outer-most to the
 *          direct parent of the interface (e.g. if the interface is x.y.A$B$I then
 *          the enclosing types are [A, B]. This is an empty array if the interface
 *          is a top-level type.
 * @param path the full path to the resource containing the interface. If the resource is a .class file
 *          or a .java file, this is the full path in the workspace to this resource. If the
 *          resource is a .zip or .jar file, this is the full OS path to this file.
 */
virtual void acceptInterface(const std::wstring& packageName, const std::wstring& simpleTypeName, 
	 std::vector< std::wstring> & enclosingTypeNames, std::wstring& path) = 0;
};
	}// Close namespace Search block


} // Close namespace Jikes block

#endif