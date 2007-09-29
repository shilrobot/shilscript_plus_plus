#ifndef SS_PACKAGE_H
#define SS_PACKAGE_H

#include "Base.h"
#include "Node.h"
#include "Scope.h"

namespace SS {

class Class;

class SS_EXPORT Package : public Node
{
public:
	SS_CLASS(Package, "Package", Node);

	Package();
	virtual ~Package();

	FileInfo* AddFile(const String& filename)
	{
		FileInfo* f = new FileInfo(filename);
		m_files.push_back(f);
		return f;
	}

	SS_ADD(Class*, AddClass, m_classes);
	SS_INDEX(Class*, Class, m_classes);


	SS_NAMED_NODE
	SS_CONTAINER_NODE
	SS_NODE_TYPE_NAME("package");

private:

	typedef std::vector<Class*> ClassVector;

	ClassVector		m_classes;

	typedef std::vector<FileInfo*> FileInfoVector;

	FileInfoVector	m_files;
};

}

#endif // SS_PACKAGE_H
