// version.cc: implementation of function show_version()
//////////////////////////////////////////////////////////////////////////
//
// Copyright 1990-2012 John Cremona
// 
// This file is part of the eclib package.
// 
// eclib is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
// 
// eclib is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License
// along with eclib; if not, write to the Free Software Foundation,
// Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
// 
//////////////////////////////////////////////////////////////////////////
 
#include <iostream>

using namespace std;

void show_version()
{
  cerr << "Version compiled on " << __DATE__ << " at " << __TIME__ << " by GCC " << __VERSION__ << "\n";
  cerr << "using base arithmetic option ";
#ifdef NTL_ALL
  cerr << "NTL_ALL (NTL bigints and multiprecision floating point)";
#else
  cerr << "NTL_INTS (NTL bigints and no multiprecision floating point)";
#endif
  cerr<<endl;
}

