//
// $RCSfile: BinIOStream.hh,v $
//
// $Revision: 1.4 $
// $Date: 2003-06-19 14:43:17 $
// $Author: gunter $
// $Locker:  $
// $State: Exp $
// DOSfile: biostream.h
// UNIXfile: BinIOStream.H
//
// $Log: not supported by cvs2svn $
// Revision 1.3  2000/08/02 09:24:33  sscherer
// now using G4cout/G4cerr
//
// Revision 1.2  1999/12/15 14:53:30  gunter
// Modifications to have a ISO C++ compliant source code
//
// Revision 1.1  1999/12/09 11:50:34  sscherer
// adding quark_molecular_dynamics
//
// Revision 1.1.1.1  1998/09/22 16:31:05  mhofmann
// U++ V0.9
//
// Revision 1.1.1.1  1997/07/17 12:54:53  mhofmann
// Initial import
//
// Revision 1.1.1.1  1996/10/04 14:37:49  mhofmann
// Phase Transition Project
//
//
 
#ifndef _BinIOStream_H
#define _BinIOStream_H

#include "globals.hh"
#include <iostream>

#ifdef __MSDOS__
  #include "definiti.h"
#else
  #include "Definitions.hh"
#endif

class BinOStream : public std::ostream { 
  friend BinOStream& operator<<(BinOStream&,char c);
  friend BinOStream& operator<<(BinOStream&,signed char c);
  friend BinOStream& operator<<(BinOStream&,unsigned char c);
  friend BinOStream& operator<<(BinOStream&,int i);
  friend BinOStream& operator<<(BinOStream&,unsigned int i);
  friend BinOStream& operator<<(BinOStream&,long int i);
  friend BinOStream& operator<<(BinOStream&,unsigned long int i);
  friend BinOStream& operator<<(BinOStream&,short int i);
  friend BinOStream& operator<<(BinOStream&,unsigned short int i);
  friend BinOStream& operator<<(BinOStream&,double f);
  friend BinOStream& operator<<(BinOStream&,long double f);
  friend BinOStream& operator<<(BinOStream&,float f);
  public:
    BinOStream(std::ostream& o, int);
    inline BinOStream& flush(); 
  private:
    std::ostream* os;
};

class BinIStream : public std::istream { 
friend BinIStream& operator>>(BinIStream&,char& c);
friend BinIStream& operator>>(BinIStream&,signed char& c);
friend BinIStream& operator>>(BinIStream&,unsigned char& c);
friend BinIStream& operator>>(BinIStream&,int& i);
friend BinIStream& operator>>(BinIStream&,unsigned int& i);
friend BinIStream& operator>>(BinIStream&,long& i);
friend BinIStream& operator>>(BinIStream&,unsigned long& i);
friend BinIStream& operator>>(BinIStream&,short& i);
friend BinIStream& operator>>(BinIStream&,unsigned short& i);
friend BinIStream& operator>>(BinIStream&,double& f);
friend BinIStream& operator>>(BinIStream&,long double& f);
friend BinIStream& operator>>(BinIStream&,float& f);
  public:
    BinIStream(std::istream& i, int);
    inline void clear(int i = 0);
    inline int eof() const;
    inline int fail() const;
    inline int bad() const;
    inline int good() const;
    inline operator void*();
    inline operator const void*() const;
    inline int operator!() const;
  private:
    std::istream* is;  
};                          

#include "BinIOStream.icc"
                   
#endif // _BinIOStream_H
        
