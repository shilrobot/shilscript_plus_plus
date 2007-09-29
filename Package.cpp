#include "Package.h"
#include "Name.h"
#include "CompileErrors.h"
#include "Class.h"

namespace SS {

Package::Package()
{
}

Package::~Package()
{
	// Delete file objects this package used
	for(size_t i = 0; i < m_files.size(); ++i)
		delete m_files[i];

	for(size_t i=0; i<m_classes.size(); ++i)
		delete m_classes[i];
}

}
