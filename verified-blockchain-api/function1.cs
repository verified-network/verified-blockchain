//(c) Kallol Borah, 2020

using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection;

static class NativeLibrary
{
    [DllImport("kernel32.dll")]
    public static extern IntPtr LoadLibrary(string dllToLoad);

    [DllImport("kernel32.dll")]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

    [DllImport("kernel32.dll")]
    public static extern bool FreeLibrary(IntPtr hModule);
       
}

namespace verified_blockchain_api
{
    public static class function1
    {
        
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int signup(string username, string pwd);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int signin(string username, string pwd);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int signout();
        
        [FunctionName("verified-blockchain")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", "post",
            Route = "verified-blockchain/{funcname:alpha}/{username:alpha}/{pwd:alpha}")]
            HttpRequest req, string funcname, string username, string pwd,
            ILogger log)
        {
            log.LogInformation("C# HTTP trigger function processed a request."+funcname+" "+username+" "+pwd);
            
            bool loadNativeLibSuccess = false;
            string managedLibInstallPath = Path.GetDirectoryName(
                Assembly.GetExecutingAssembly().Location).TrimEnd('\\');
            string nativeLibFileName = "VerifiedBlockchain.dll";
            
            log.LogInformation("Managed Lib install path " + managedLibInstallPath);
            
            IntPtr hModule = IntPtr.Zero;
            if (loadNativeLibSuccess == false)
            {
                
                try
                {
                    hModule = NativeLibrary.LoadLibrary(managedLibInstallPath + "\\" + nativeLibFileName); 

                    if (hModule != IntPtr.Zero)
                        loadNativeLibSuccess = true;
                }
                catch (Exception e)
                {
                    log.LogInformation("Exception loading DLL: " + e.ToString());
                }
            }

            if (loadNativeLibSuccess == false)
            {
                log.LogInformation("Failed loading DLL.");

                return (ActionResult)new JsonResult("{" + "Failed loading DLL" + "}");
            }
            else
            {
                log.LogInformation("Loading DLL successful");              
                
                int result = 0;
                if (funcname.Equals("signup", StringComparison.InvariantCultureIgnoreCase))
                {
                    log.LogInformation("Signing up");
                    IntPtr pAddressOfFunctionToCall = NativeLibrary.GetProcAddress(hModule, "signup");

                    if (pAddressOfFunctionToCall != IntPtr.Zero)
                    {
                        log.LogInformation("Sign up function pointer found");
                        signup register = (signup)Marshal.GetDelegateForFunctionPointer(
                            pAddressOfFunctionToCall,
                            typeof(signup));

                        result = register(username, pwd);

                        if (hModule != IntPtr.Zero)
                            NativeLibrary.FreeLibrary(hModule);
                    }
                }
                else if (funcname.Equals("signin", StringComparison.InvariantCultureIgnoreCase))
                {
                    log.LogInformation("Signing in");
                    IntPtr pAddressOfFunctionToCall = NativeLibrary.GetProcAddress(hModule, "signin");

                    if (pAddressOfFunctionToCall != IntPtr.Zero)
                    {
                        log.LogInformation("Sign in function pointer found");
                        signin login = (signin)Marshal.GetDelegateForFunctionPointer(
                            pAddressOfFunctionToCall,
                            typeof(signin));

                        result = login(username, pwd);

                        if (hModule != IntPtr.Zero)
                            NativeLibrary.FreeLibrary(hModule);
                    }
                }
                else if (funcname.Equals("signout", StringComparison.InvariantCultureIgnoreCase))
                {
                    log.LogInformation("Signing out");
                    IntPtr pAddressOfFunctionToCall = NativeLibrary.GetProcAddress(hModule, "signout");

                    if (pAddressOfFunctionToCall != IntPtr.Zero)
                    {
                        log.LogInformation("Sign out function pointer found");
                        signout logout = (signout)Marshal.GetDelegateForFunctionPointer(
                            pAddressOfFunctionToCall,
                            typeof(signout));

                        result = logout();

                        if (hModule != IntPtr.Zero)
                            NativeLibrary.FreeLibrary(hModule);
                    }
                }
            

            log.LogInformation("Result = "+result);
                return funcname.Equals("")
                    ? (ActionResult)new OkObjectResult($"{result}")
                    : new BadRequestObjectResult("Please pass a query string in the request body");
            }
        
        }
    }
}
