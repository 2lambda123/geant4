//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4UIparameter.cc,v 1.7 2001-10-11 13:29:18 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "G4UIparameter.hh"
#include "G4UIcommandStatus.hh"

#include "G4Tokenizer.hh"

#include "G4ios.hh"
G4UIparameter::G4UIparameter():paramERR(0)
{
  G4String nullString;
  parameterName = nullString;
  parameterType = '\0';
  omittable = false;
  parameterGuidance = nullString;
  defaultValue = nullString;
  parameterRange = nullString;
  currentAsDefaultFlag = false;
  parameterCandidate = nullString;
  widget = 0;
}

G4UIparameter::G4UIparameter(char theType):paramERR(0)
{
  G4String nullString;
  parameterName = nullString;
  parameterType = theType;
  omittable = false;
  parameterGuidance = nullString;
  defaultValue = nullString;
  parameterRange = nullString;
  currentAsDefaultFlag = false;
  parameterCandidate = nullString;
  widget = 0;
}

G4UIparameter::G4UIparameter(const char * theName, char theType, G4bool theOmittable):paramERR(0)
{
  parameterName = theName;
  parameterType = theType;
  omittable = theOmittable;
  G4String nullString;
  parameterGuidance = nullString;
  defaultValue = nullString;
  parameterRange = nullString;
  currentAsDefaultFlag = false;
  parameterCandidate = nullString;
  widget = 0;
}

G4UIparameter::~G4UIparameter()
{ }

int G4UIparameter::operator==(const G4UIparameter &right) const
{
  return ( this == &right );
}

int G4UIparameter::operator!=(const G4UIparameter &right) const
{
  return ( this != &right );
}

void G4UIparameter::List()
{
  G4cout << G4endl << "Parameter : " << parameterName << G4endl;
  if( ! parameterGuidance.isNull() )
  G4cout << parameterGuidance << G4endl ;
  G4cout << " Parameter type  : " << parameterType << G4endl;
  if(omittable)
  { G4cout << " Omittable       : True" << G4endl; }
  else
  { G4cout << " Omittable       : False" << G4endl; }
  if( currentAsDefaultFlag )
  { G4cout << " Default value   : taken from the current value" << G4endl; }
  else if( ! defaultValue.isNull() )
  { G4cout << " Default value   : " << defaultValue << G4endl; }
  if( ! parameterRange.isNull() )
  G4cout << " Parameter range : " << parameterRange << G4endl;
  if( ! parameterCandidate.isNull() )
  G4cout << " Candidates      : " << parameterCandidate << G4endl;
}

void G4UIparameter::SetDefaultValue(G4int theDefaultValue)
{
  char defVal[20];
  G4std::ostrstream os(defVal,20);
  os << theDefaultValue << '\0';
  defaultValue = defVal;
}

void G4UIparameter::SetDefaultValue(G4double theDefaultValue)
{
  char defVal[20];
  G4std::ostrstream os(defVal,20);
  os << theDefaultValue << '\0';
  defaultValue = defVal;
}


// ---------- CheckNewValue() related routines -----------
#include <ctype.h>
#include "G4UItokenNum.hh"

//#include "checkNewValue_debug.icc"
//#define DEBUG 1

int G4UIparameter::
CheckNewValue( G4String newValue ) {
    if( TypeCheck(newValue) == 0) return fParameterUnreadable;
    if( ! parameterRange.isNull() )
    { if( RangeCheck(newValue) == 0 ) return fParameterOutOfRange; }
    if( ! parameterCandidate.isNull() )
    { if( CandidateCheck(newValue) == 0 ) return fParameterOutOfCandidates; }
    return 0;   // succeeded
}

int G4UIparameter::
CandidateCheck(G4String newValue) {
    G4Tokenizer candidateTokenizer(parameterCandidate);
    G4String aToken;
    int iToken = 0;
    while( ! (aToken=candidateTokenizer()).isNull() )
    {
      iToken++;
      if(aToken==newValue) return iToken;
    } 
    G4cerr << "parameter value is not listed in the candidate List." << G4endl;
    return 0;
}

int G4UIparameter::
RangeCheck(G4String newValue) {
    yystype result;
    bp = 0;                   // reset buffer pointer for G4UIpGetc()
    const char* t = newValue;
    G4std::istrstream is((char*)t); 
    char type = toupper( parameterType );
    switch (type) {
        case 'D': { is >> newVal.D; } break;
        case 'I': { is >> newVal.I; } break;
        default:   ;
    }
   // PrintToken();          // Print tokens (consumes all tokens)
   token= Yylex();
   result = Expression();
   if( paramERR == 1 ) return 0;
   if( result.type != CONSTINT) {
      G4cerr << "Illegal Expression in parameter range." << G4endl;
      return 0;
   }
   if ( result.I ) return 1;
   G4cerr << "parameter out of range: "<< parameterRange << G4endl;
   return 0;
}


int G4UIparameter::
TypeCheck(G4String newValue)
{
    char type = toupper( parameterType );
    switch(type) {
        case 'D':
            if( IsDouble(newValue.data())== 0) {
                G4cerr<<newValue<<": double value expected."
                    << G4endl;
                return 0;
             } break;
        case 'I':
            if( IsInt(newValue.data(),20)== 0) {
                G4cerr<<newValue<<": integer expected."
                    << G4endl;
                return 0;
             } break;
        case 'S': break;
        case 'B':
             newValue.toUpper();
             if (  newValue == "Y" || newValue == "N"
                  ||newValue == "YES" || newValue == "NO"
                  ||newValue == "1"   || newValue == "0"
                  ||newValue == "T" || newValue == "F"
                  ||newValue == "TRUE" || newValue == "FALSE") 
                  return 1;
             else {
                    G4cerr<<newValue<<": bool expected." << G4endl;
                    return 0; 
             } 
        default: ;
    }
    return 1;
}


int G4UIparameter::
IsInt(const char* buf, short maxDigits)  // do not allow any G4std::ws
{
    const char* p= buf;
    int length=0;
    if( *p == '+' || *p == '-') { ++p; }
    if( isdigit( (int)(*p) )) {
        while( isdigit( (int)(*p) )) { ++p;  ++length; }
        if( *p == '\0' ) {
            if( length > maxDigits) {
                G4cerr <<"digit length exceeds"<<G4endl;
                return 0;
            }
            return 1;
        } else {
            // G4cerr <<"illegal character after int:"<<buf<<G4endl;
        }
    } else {
        // G4cerr <<"illegal int:"<<buf<<G4endl;
    }
    return 0;
}


int G4UIparameter::
ExpectExponent(const char* str)   // used only by IsDouble()
{
    int maxExplength;
    if( IsInt( str, maxExplength=7 )) return 1;
    else return 0;
}

int G4UIparameter::
IsDouble(const char* buf)  // see state diagram for this spec.
{
    const char* p= buf;
    switch( *p) {
        case '+':  case '-': ++p;
            if( isdigit(*p) ) {
                 while( isdigit( (int)(*p) )) { ++p; }
                 switch ( *p ) {
                     case '\0':    return 1;  //break;
                     case 'E':  case 'e':
                         return ExpectExponent(++p );  //break;
                     case '.':  ++p;
                         if( *p == '\0' )  return 1;
                         if( *p == 'e' || *p =='E' ) return ExpectExponent(++p );
                         if( isdigit(*p) ) {
                             while( isdigit( (int)(*p) )) { ++p; }
                             if( *p == '\0' )  return 1;
                             if( *p == 'e' || *p =='E') return ExpectExponent(++p);
                         } else return 0;   break;
                     default: return 0;
                 }
            }
            if( *p == '.' ) { ++p;
                 if( isdigit(*p) ) {
                     while( isdigit( (int)(*p) )) { ++p; }
                     if( *p == '\0' )  return 1;
                     if( *p == 'e' || *p =='E')  return ExpectExponent(++p);
                 }
            }
            break;
        case '.':  ++p;
            if( isdigit(*p) ) {
                 while( isdigit( (int)(*p) )) { ++p; }
                 if( *p == '\0' )  return 1;
                 if( *p == 'e' || *p =='E' )  return ExpectExponent(++p);
            }    break;
        default: // digit is expected
            if( isdigit(*p) ) {
                 while( isdigit( (int)(*p) )) { ++p; }
                 if( *p == '\0' )  return 1;
                 if( *p == 'e' || *p =='E')  return ExpectExponent(++p);
                 if( *p == '.' ) { ++p;
                      if( *p == '\0' )  return 1;
                      if( *p == 'e' || *p =='E')  return ExpectExponent(++p);
                      if( isdigit(*p) ) {
                          while( isdigit( (int)(*p) )) { ++p; }
                          if( *p == '\0' )  return 1;
                          if( *p == 'e' || *p =='E') return ExpectExponent(++p);
                      }
                 }
            }
     }
     return 0;
}


// ------------------ syntax node functions  ------------------

yystype G4UIparameter:: 
Expression(void)
{
    yystype result;
    #ifdef DEBUG
        G4cerr << " Expression()" << G4endl;
    #endif
    result = LogicalORExpression();
    return result;
}

yystype G4UIparameter:: 
LogicalORExpression(void)
{
    yystype result;
    yystype p;
    p = LogicalANDExpression();
    if( token != LOGICALOR)  return p;
    if( p.type == CONSTSTRING || p.type == IDENTIFIER ) {
        G4cerr << "Parameter range: illegal type at '||'" << G4endl;
        paramERR = 1;
    }
    result.I = p.I;
    while (token == LOGICALOR) 
    {  
        token = Yylex();
        p = LogicalANDExpression();
        if( p.type == CONSTSTRING || p.type == IDENTIFIER ) {
            G4cerr << "Parameter range: illegal type at '||'" <<G4endl;
            paramERR = 1;
        }
        switch (p.type) {
            case CONSTINT: 
                result.I  += p.I; 
                result.type = CONSTINT;      break;
            case CONSTDOUBLE:
                result.I += (p.D != 0.0); 
                result.type = CONSTINT;      break;
            default: 
                G4cerr << "Parameter range: unknown type"<<G4endl; 
                paramERR = 1;
        } 
    }
    return result;
}

yystype G4UIparameter:: 
LogicalANDExpression(void)
{
    yystype result;
    yystype p;
    p = EqualityExpression();
    if( token != LOGICALAND)  return p;
    if( p.type == CONSTSTRING || p.type == IDENTIFIER ) {
        G4cerr << "Parameter range: illegal type at '&&'" << G4endl;
        paramERR = 1;
    }
    result.I = p.I;
    while (token == LOGICALAND)
    {
        token = Yylex();
        p = EqualityExpression();
        if( p.type == CONSTSTRING || p.type == IDENTIFIER ) {
            G4cerr << "Parameter range: illegal type at '&&'" << G4endl;
            paramERR = 1;
        }
        switch (p.type) {
            case CONSTINT:
                result.I  *= p.I;
                result.type = CONSTINT;      break;
            case CONSTDOUBLE:
                result.I *= (p.D != 0.0);
                result.type = CONSTINT;      break;
            default:
                G4cerr << "Parameter range: unknown type."<< G4endl;
                paramERR = 1;
        } 
    }
    return result;
}


yystype G4UIparameter:: 
EqualityExpression(void)
{ 
    yystype  arg1, arg2;
    int operat;
    yystype result;
    #ifdef DEBUG
        G4cerr << " EqualityExpression()" <<G4endl;
    #endif
    result = RelationalExpression();
    if( token==EQ || token==NE ) {
        operat = token;
        token =  Yylex();
        arg1 = result;
        arg2 = RelationalExpression();
        result.I = Eval2( arg1, operat, arg2 );   // semantic action
        result.type = CONSTINT;
        #ifdef DEBUG
            G4cerr << " return code of Eval2(): " << result.I <<G4endl;
        #endif
    } else {
        if (result.type != CONSTINT && result.type != CONSTDOUBLE) {  
            G4cerr << "Parameter range: error at EqualityExpression"
                 << G4endl;
            paramERR = 1;
        }
    }
    return  result;
}


yystype G4UIparameter:: 
RelationalExpression(void)
{ 
    yystype  arg1, arg2;
    int operat;
    yystype result;
    #ifdef DEBUG
        G4cerr << " RelationalExpression()" <<G4endl;
    #endif

    arg1 = AdditiveExpression();
    if( token==GT || token==GE || token==LT || token==LE  ) {
        operat = token;
        token =  Yylex();
        arg2 = AdditiveExpression();
        result.I = Eval2( arg1, operat, arg2 );    // semantic action
        result.type = CONSTINT;
        #ifdef DEBUG
            G4cerr << " return  Eval2(): " << G4endl;
        #endif
    } else {
        result = arg1;
    }
    #ifdef DEBUG
       G4cerr <<" return RelationalExpression()" <<G4endl;
    #endif
    return  result;
}

yystype G4UIparameter::
AdditiveExpression(void)
{   yystype result;
    result = MultiplicativeExpression();
    if( token != '+' && token != '-' )  return result;
    G4cerr << "Parameter range: operator " 
         << (char)token 
         << " is not supported." << G4endl;
    paramERR = 1;
    return  result;
}

yystype G4UIparameter::
MultiplicativeExpression(void)
{   yystype result;
    result = UnaryExpression();
    if( token != '*' && token != '/' && token != '%' ) return result;
    G4cerr << "Parameter range: operator "
         << (char)token
         << " is not supported." << G4endl;
    paramERR = 1;
    return  result;
}

yystype G4UIparameter::
UnaryExpression(void)
{
    yystype result;
    yystype p;
    #ifdef DEBUG
        G4cerr <<" UnaryExpression"<< G4endl;
    #endif
    switch(token) {
        case '-':
            token = Yylex();
            p = UnaryExpression();
            if (p.type == CONSTINT) {
                result.I = - p.I;
                result.type = CONSTINT;
            }
            if (p.type == CONSTDOUBLE) {
                result.D = - p.D;
                result.type = CONSTDOUBLE;
            }                              break;
        case '+':
            token = Yylex();
            result = UnaryExpression();   break;
        case '!':
            token = Yylex();
            G4cerr << "Parameter range error: "
                 << "operator '!' is not supported (sorry)."
                 << G4endl;
            paramERR = 1;
            result = UnaryExpression();   break;
        default:
            result = PrimaryExpression();
    }
    return result;
}


yystype G4UIparameter:: 
PrimaryExpression(void)
{
     yystype result;
     #ifdef DEBUG
         G4cerr <<" PrimaryExpression" << G4endl;
     #endif
     switch (token) {
         case IDENTIFIER:
              result.S = yylval.S;
              result.type =  token;
              token = Yylex();           break;
         case CONSTINT:
              result.I = yylval.I;
              result.type =  token;
              token= Yylex();            break;
         case CONSTDOUBLE:
              result.D = yylval.D;
              result.type =  token;
              token = Yylex();           break;
         case '(' :
              token= Yylex();
              result = Expression();
              if( token !=  ')'  ) {
                  G4cerr << " ')' expected" << G4endl;
                  paramERR = 1;
              }
              token = Yylex();
                                         break;
         default:
         return result;
    }
    return result; // never executed
}

//---------------- semantic routines ---------------------------------

int G4UIparameter::
Eval2(yystype arg1, int op, yystype arg2)
{
    if( (arg1.type != IDENTIFIER) && (arg2.type != IDENTIFIER)) {
        G4cerr << parameterName
             << ": meaningless comparison "
             << int(arg1.type) << " " << int(arg2.type) << G4endl;
        paramERR = 1;
    }
    char type = toupper( parameterType );
    if( arg1.type == IDENTIFIER) {
        switch (type) {
            case 'I':
                if ( arg2.type == CONSTINT ) {
                    return CompareInt( newVal.I, op, arg2.I );
                } else {
                    G4cerr << "integer operand expected for "
                         << parameterRange << '.' 
                         << G4endl; 
                }
                 break;
            case 'D': 
                if ( arg2.type == CONSTDOUBLE ) {
                    return CompareDouble( newVal.D, op, arg2.D );
                } else
                if ( arg2.type == CONSTINT ) { // integral promotion
                    return CompareDouble( newVal.D, op, arg2.I );
                } break;
            default: ;
        }
    }
    if( arg2.type == IDENTIFIER) {
        switch (type) {
            case 'I':
                if ( arg1.type == CONSTINT ) {
                    return CompareInt( arg1.I, op, newVal.I );
                } else {
                    G4cerr << "integer operand expected for "
                         << parameterRange << '.' 
                         << G4endl; 
                }
                 break;
            case 'D': 
                if ( arg1.type == CONSTDOUBLE ) {
                    return CompareDouble( arg1.D, op, newVal.D );
                } else
                if ( arg1.type == CONSTINT ) { // integral promotion
                    return CompareDouble( arg1.I, op, newVal.D );
                } break;
            default: ;
        }
    }
    G4cerr << "no param name is specified at the param range."<<G4endl;
    return 0;
}

int G4UIparameter::
CompareInt(int arg1, int op, int arg2)
{   
    int result=-1;
    G4String opr;
    switch (op) {
       case GT:  result = ( arg1 >  arg2); opr= ">" ;  break;
       case GE:  result = ( arg1 >= arg2); opr= ">=";  break;
       case LT:  result = ( arg1 <  arg2); opr= "<" ;  break;
       case LE:  result = ( arg1 <= arg2); opr= "<=";  break;
       case EQ:  result = ( arg1 == arg2); opr= "==";  break;
       case NE:  result = ( arg1 != arg2); opr= "!=";  break;
       default: 
           G4cerr << "Parameter range: error at CompareInt" << G4endl;
           paramERR = 1;
    }
    #ifdef DEBUG
        G4cerr << "CompareInt "
             << arg1 << " " << opr << arg2 
             << " result: " << result
             << G4endl;
    #endif
    return result;
}

int G4UIparameter::
CompareDouble(double arg1, int op, double arg2)
{   
    int result=-1;
    G4String opr;
    switch (op) {
        case GT:  result = ( arg1 >  arg2); opr= ">";   break;
        case GE:  result = ( arg1 >= arg2); opr= ">=";  break;
        case LT:  result = ( arg1 <  arg2); opr= "<";   break;
        case LE:  result = ( arg1 <= arg2); opr= "<=";  break;
        case EQ:  result = ( arg1 == arg2); opr= "==";  break;
        case NE:  result = ( arg1 != arg2); opr= "!=";  break;
        default:
           G4cerr << "Parameter range: error at CompareDouble" << G4endl;
           paramERR = 1;
    }
    #ifdef DEBUG
        G4cerr << "CompareDouble " 
             << arg1 <<" " << opr << " "<< arg2
             << " result: " << result
             << G4endl;
    #endif
    return result;
}

// --------------------- utility functions --------------------------

tokenNum G4UIparameter::
Yylex()         // reads input and returns token number KR486
{               // (returns EOF)
    int c;             
    G4String buf;

    while(( c= G4UIpGetc())==' '|| c=='\t' || c== '\n' )
        ;
    if (c== EOF)
        return (tokenNum)EOF;            // KR488 
    buf= "";
    if (isdigit(c) || c== '.') {         // I or D
        do {
             buf += G4String((unsigned char)c);
             c=G4UIpGetc();
         }  while (c=='.' || isdigit(c) || 
                   c=='e' || c=='E' || c=='+' || c=='-');
         G4UIpUngetc(c);
	 const char* t = buf;
	 G4std::istrstream is((char*)t);
         if ( IsInt(buf.data(),20) ) {
             is >> yylval.I;
             return  CONSTINT;
         } else 
         if ( IsDouble(buf.data()) ) {
             is >> yylval.D;
             return  CONSTDOUBLE;
         } else {
             G4cerr << buf<<": numeric format error."<<G4endl;
         }
    }
    buf="";
    if (isalpha(c)|| c=='_') {           // IDENTIFIER
        do {
            buf += G4String((unsigned char)c); 
        } while ((c=G4UIpGetc()) != EOF && (isalnum(c) || c=='_'));
        G4UIpUngetc(c);
        if( buf == parameterName ) {
            yylval.S =buf;
            return IDENTIFIER;
        } else {
            G4cerr << buf << " is not a parameter name."<< G4endl;
            paramERR = 1;
       }
    }
    switch (c) {
      case '>':   return (tokenNum) Follow('=', GE,        GT);
      case '<':   return (tokenNum) Follow('=', LE,        LT);
      case '=':   return (tokenNum) Follow('=', EQ,        '=');
      case '!':   return (tokenNum) Follow('=', NE,        '!');
      case '|':   return (tokenNum) Follow('|', LOGICALOR, '|');
      case '&':   return (tokenNum) Follow('&', LOGICALAND, '&');
      default:
          return (tokenNum) c;
    }
}


int G4UIparameter::
Follow(int expect, int ifyes, int ifno)
{
    int c = G4UIpGetc();
    if ( c== expect)
          return ifyes;
    G4UIpUngetc(c);
    return ifno;
}

//------------------ low level routines -----------------------------
int G4UIparameter::
G4UIpGetc() {                        // emulation of getc() 
    int length = parameterRange.length();
    if( bp < length)
        return  parameterRange(bp++);
    else 
        return EOF;
}
int G4UIparameter::
G4UIpUngetc(int c) {                 // emulation of ungetc() 
    if (c<0) return -1;
    if (bp >0 && c == parameterRange(bp-1)) {
         --bp;
    } else {
         G4cerr << "G4UIpUngetc() failed." << G4endl;
         G4cerr << "bp="<<bp <<" c="<<c
              << " pR(bp-1)=" << parameterRange(bp-1)
              << G4endl;
         paramERR = 1;
         return -1;
    }
    return 0;
}
// *****  end of CheckNewValue() related code  ******

