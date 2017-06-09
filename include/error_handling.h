/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

/*! \file error_handling.h
 *  \brief Provide macros for error handling
 *  \author Roberto Di Remigio
 *  \date 2017
 *
 *  Exceptions:
 *
 *  \verbatim
 *  NUMGRID_ERROR(<Error Message>)
 *  \endverbatim
 *
 *  Use this to catch errors that might occur because of faulty
 *  data, i.e. other people's errors. Mainly in the API to the
 *  external world.
 *
 *  Assertions:
 *
 *  \verbatim
 *  NUMGRID_ASSERT(<Condition>)
 *  \endverbatim
 *
 *  This is just a wrapper around the standard assert macro.
 *  Use this to catch your own errors, i.e. broken preconditions
 *  to the internal functions/classes. In release mode assertions
 *  are not checked!
 */

/// Macro and helper functions to be used to signal error conditions
#define NUMGRID_ERROR(msg)                          \
  {                                                 \
    std::ostringstream errmsg;                      \
    errmsg << std::string("Numgrid fatal error.\n") \
            << std::string(" In function ")         \
            << std::string(__func__)                \
            << std::string(" at line ") << __LINE__ \
            << std::string(" of file ")             \
            << std::string(__FILE__)                \
            << std::endl;                           \
    message_and_die(errmsg.str(), msg);             \
  }

inline void message_and_die(const std::string & err,
    const std::ostringstream & msg) {
  std::fprintf(stderr, "%s\n", (err + msg.str()).c_str());
  std::exit(EXIT_FAILURE);
}

inline void message_and_die(const std::string & err,
    const std::string & msg) {
  std::fprintf(stderr, "%s\n", (err + msg).c_str());
  std::exit(EXIT_FAILURE);
}

inline void message_and_die(const std::string & err,
    const char * c_msg) {
  std::fprintf(stderr, "%s\n", (err + std::string(c_msg)).c_str());
  std::exit(EXIT_FAILURE);
}

/// Macro to be used for assertions
#define NUMGRID_ASSERT(arg) assert(arg)

#endif /* ERRORHANDLING_H */
