#include <iostream>
#include <strstream>
#include "StreamBuffer.hh"
#include "array.hh"

std::ostream& operator<<(std::ostream& o,const StreamBuffer& x) { return o << x.buffer; }

StreamBuffer::StreamBuffer() : in(0) {}

StreamBuffer::StreamBuffer(std::istream& i) : in(0) 
{
  String s;
  while ( i >> s ) 
    *this << s;
}

StreamBuffer& StreamBuffer::operator<<(const String& s) 
{ 
  buffer += " "+s; 
  return *this; 
}
  
StreamBuffer::operator std::istream&() 
{ 
  return ( in ) ? *in : *(in = new std::istrstream((char*)buffer,length(buffer))); 
}

void StreamBuffer::reset() 
{ 
  if (in) 
    delete in; 
  in = new std::istrstream((char*)buffer,length(buffer)); 
}

void StreamBuffer::createStream(int n,char** args) 
{
  for (int i=0; i<n; i++) {
    String s(args[i]);
    if ( s.findFirst("--")>=0 ) {
      s.erase("--");
      if ( s.empty() )
	s = args[++i];
      Array<String> X = s.divide("=");
      for (int j=0; j<X.size(); j++)
	(*this) << X[j];
      (*this) << "\n";
    }
  }
}

