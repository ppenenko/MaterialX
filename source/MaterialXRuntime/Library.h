//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_LIBRARY_H
#define MATERIALX_RT_LIBRARY_H

/// @file
/// Library-wide includes and types.  This file should be the first include for
/// any public header in the MaterialXRuntime library.
#include <MaterialXCore/Library.h>

namespace MaterialX
{

/// @class ExceptionRuntimeError
/// An exception that is thrown when a runtime operation fails.
class ExceptionRuntimeError : public Exception
{
public:
    using Exception::Exception;
};

} // namespace MaterialX

#endif
