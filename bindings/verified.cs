//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class verified {
  public static SWIGTYPE_p_class peer {
    set {
      verifiedPINVOKE.peer_set(SWIGTYPE_p_class.getCPtr(value));
      if (verifiedPINVOKE.SWIGPendingException.Pending) throw verifiedPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      SWIGTYPE_p_class ret = new SWIGTYPE_p_class(verifiedPINVOKE.peer_get(), true);
      if (verifiedPINVOKE.SWIGPendingException.Pending) throw verifiedPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public static int signup(SWIGTYPE_p_string arg0, SWIGTYPE_p_string arg1) {
    int ret = verifiedPINVOKE.signup(SWIGTYPE_p_string.getCPtr(arg0), SWIGTYPE_p_string.getCPtr(arg1));
    if (verifiedPINVOKE.SWIGPendingException.Pending) throw verifiedPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int signin(SWIGTYPE_p_string arg0, SWIGTYPE_p_string arg1) {
    int ret = verifiedPINVOKE.signin(SWIGTYPE_p_string.getCPtr(arg0), SWIGTYPE_p_string.getCPtr(arg1));
    if (verifiedPINVOKE.SWIGPendingException.Pending) throw verifiedPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int signout() {
    int ret = verifiedPINVOKE.signout();
    return ret;
  }

}