#ifdef IS_GCC
#pragma implementation "InputReader.hh"
#include "DList.hh"
#include "InputItem.hh"
typedef DList<InputItem> DList_InputItem;
#else
#include "InputItem.hh"         
#endif

#include "InputReader.hh"
#include "Boolean.h"

InputReader::InputReader()
  : commentLeader("#")
{}

void 
InputReader::setCommentLeader(const String& cl) {
  commentLeader = cl;
}
            
InputReader& 
InputReader::operator>>(InputItem& item) {
  for( DList<InputItem>::Iterator i(itemList); i.more(); i.advance() ) {
    if( i.current()->getKey() == item.getKey() ) {
      Throw(ErrDuplicateItem(item.getKey()));
    }
  }      
  itemList.appendLast(&item);
  return *this;
}

std::istream& operator>>(std::istream& is, InputReader& reader) {
  String key;
  int count = 0;
  while( is >> key ) {
    if( key.subString(0,reader.commentLeader.getLength()) == reader.commentLeader ) {
      char c;
      while( is.get(c) ) {
        if( c == '\n' ) {
	  is.putback('\n');
          break;
        }
      }
    }
    else { 

      Boolean isKnown = Boolean::False;
      count++;
      for( DList<InputItem>::Iterator i(reader.itemList); i.more(); i.advance() )
      {
        if( key == i.current()->getKey() ) {
	  //	  if( i.current()->hasBeenSet() ) {
	  //	    Throw(InputReader::ErrDuplicateInput(key, count));
	  //	  }
          i.current()->read(is);
	  if( !is.good() ) {
	    Throw(InputReader::ErrReadFailed(key, count));
	  }
          isKnown = Boolean::True;
          break;   
        }
      }
      if( !isKnown ) {
	Throw(InputReader::ErrNotKnown(key, count));
      }
    }
  }    
  return is;
}

InputReader::ErrDuplicateItem::ErrDuplicateItem(const String& key)
  : theKey(key)
{}

InputReader::ErrDuplicateItem::ErrDuplicateItem(const InputReader::ErrDuplicateItem& err) 
  : theKey(err.theKey) {}

void
InputReader::ErrDuplicateItem::writeMessage(std::ostream& os) const {
  os << "in function InputReader::operator>>(InputItem&)" << G4endl;
  os << "Duplicate key \"" << theKey << "\"";
}

InputReader::ErrDuplicateInput::ErrDuplicateInput(const String& key, int count)
  : theKey(key),
    theCount(count)
{}

InputReader::ErrDuplicateInput::ErrDuplicateInput(const InputReader::ErrDuplicateInput& err) 
  : theKey(err.theKey),theCount(err.theCount) {}

void
InputReader::ErrDuplicateInput::writeMessage(std::ostream& os) const {
  os << "in function operator>>(std::istream&,InputReader&)" << G4endl;
  os << "InputItem #" << theCount << G4endl;
  os << "\"" << theKey << "\" has already been set";
}

InputReader::ErrReadFailed::ErrReadFailed(const String& key, int count)
  : theKey(key),
    theCount(count)
{}

InputReader::ErrReadFailed::ErrReadFailed(const InputReader::ErrReadFailed& err) 
  : theKey(err.theKey),theCount(err.theCount) {}

void
InputReader::ErrReadFailed::writeMessage(std::ostream& os) const {
  os << "in function operator>>(std::istream&,InputReader&)" << G4endl;
  os << "InputItem #" << theCount << G4endl;
  os << "value for \"" << theKey << "\" could not be read";
}

InputReader::ErrNotKnown::ErrNotKnown(const String& key, int count)
  : theKey(key),
    theCount(count)
{}

InputReader::ErrNotKnown::ErrNotKnown(const InputReader::ErrNotKnown& err) 
  : theKey(err.theKey),theCount(err.theCount) {}

void
InputReader::ErrNotKnown::writeMessage(std::ostream& os) const {
  os << "in function operator>>(std::istream&,InputReader&)" << G4endl;
  os << "InputItem #" << theCount << G4endl;
  os << "key \"" << theKey << "\" not known";
}
