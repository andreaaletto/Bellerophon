//
// Bellerophon - Design Space Exploration for Approximate Computing technique using
// Gentic Algorithm
// Copyright (C) 2019 Andrea Aletto <andrea.aletto8@gmail.com>
//
// This file is part of Bellerophon.
//
// Bellerophon is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bellerophon is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Bellerophon.  If not, see <http://www.gnu.org/licenses/>.
//

/******************************************************************************
* @file   InAx1AprxTechnique.cpp
* @author Andrea Aletto
* @date   30 gen 2019
* @brief  Implementation for the InAx1 aprx technique
******************************************************************************/

// Tools Headers
#include "Plugins/InAx1/InAx1AprxTechnique.h"
#include "Plugins/FLAP/fap.h" //FIXME: rimouvere
#include "lib/csv.h"
#include "Log.h"
// LLVM Headers
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
// C/C++ Headers
#include <map>
#include <memory>
#include <utility>

using namespace bellerophon;

::std::vector<::std::string> InAx1AprxTechnique::getGlobalValueNames() const
{
  ::std::vector<::std::string> names;
  names.push_back(this->OpId);
  return names;
}

void InAx1AprxTechnique::applyApproximation(
    ::core::AprxGrade g, ::std::vector<uint64_t> addrs) const
{
  // Take the maximum
  unsigned short expPrec = 11;
  unsigned short maxMantPrec = 52;
  if (this->OpRetTy == FLOAT) {
    expPrec = 8;
    maxMantPrec = 23;
  }

  assert(g <= maxMantPrec); // Precondition

  ::fap::FloatPrecTy *p = (::fap::FloatPrecTy *)addrs.at(0);

  log::BellerophonLogger::verbose(
    "Address: " + ::std::to_string(addrs.at(0)) +
    " - Prec("  +::std::to_string(p->exp_size) + ":" + ::std::to_string(p->mant_size) +
    ")\n");

  *p = ::fap::FloatPrecTy(expPrec, maxMantPrec - g);

  log::BellerophonLogger::verbose(
    "After approximation: " + ::std::to_string(p->exp_size) +
    ":" + ::std::to_string(p->mant_size) +
    "\n");
}

::std::vector<::std::string> InAx1AprxTechnique::applyApproximation(
      ::bellerophon::core::AprxGrade g) const
{
  ::std::vector<::std::string> args;

  // Take the maximum
  unsigned short expPrec = 11;
  unsigned short maxMantPrec = 52;
  if (this->OpRetTy == FLOAT) {
    expPrec = 8;
    maxMantPrec = 23;
  }

  assert(g <= maxMantPrec); // Precondition

  ::std::string prec = "{" + ::std::to_string(expPrec) + "," +
                       ::std::to_string(maxMantPrec - g) + "}";

  args.push_back("-D" + this->OpId + "=" + prec + "");
  log::BellerophonLogger::verbose(
    "AprxLoxContext[" + ::std::to_string(this->Id) + "] - \n" +
    "Adding argument: -D" + this->OpId + "=" + prec + "\n");

  return args;
}


void InAx1AprxTechnique::dump(::llvm::raw_ostream &out) const {}
