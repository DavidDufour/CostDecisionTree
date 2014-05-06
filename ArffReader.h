#ifndef ArffReader_h__
#define ArffReader_h__

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "global.h"

int arffReader(string file);

#endif
