/* verified.i for SWIG */
%module verified
%include <windows.i>
%include "src/peer.h"
%{
extern int signup(string&, string&);
extern int signin(string&, string&);
extern int signout(void);
%}
 
extern int signup(string&, string&);
extern int signin(string&, string&);
extern int signout();
