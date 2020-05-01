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

    [DllImport("kernel32.dll")]
    public static extern void InitErrorHandling(ILogger log);
}

namespace verified_blockchain_api
{
    public static class Function1
    {
        private static bool IsInFolder(string searchFolder, string requestedLibrary)
        {
            foreach (string s in Directory.GetFiles(searchFolder))
            {
                if (s.EndsWith(requestedLibrary))
                    return true;
            }

            return false;
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int signup(string username, string pwd);
        public delegate int signin(string username, string pwd);
        public delegate int signout();

        [FunctionName("verified-blockchain")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Function, "get", "post", Route = null)] HttpRequest req,
            ILogger log)
        {
            log.LogInformation("C# HTTP trigger function processed a request.");

            bool loadNativeLibSuccess = false;
            var loadNativeLibProtocol = new List<Tuple<string, string>>();
            string managedLibInstallPath = Path.GetDirectoryName(
                Assembly.GetExecutingAssembly().Location).TrimEnd('\\');
            string nativeLibFileName = "VerifiedBlockchain.dll";

            try
            {
                NativeLibrary.InitErrorHandling(log);
                loadNativeLibProtocol.Add(new Tuple<string, string>("ATTEMPT_01",
                    "SUCCEEDED: Loading library without any preparation successfull."));
                loadNativeLibSuccess = true;
            }
            catch (Exception e)
            {

                loadNativeLibProtocol.Add(new Tuple<string, string>("ATTEMPT_01",
                    "FAILED: Loading library without any preparation not successfull."));
                loadNativeLibProtocol.Add(new Tuple<string, string>("exception", e.Message));
                loadNativeLibProtocol.Add(new Tuple<string, string>("installPath",
                    managedLibInstallPath));
                loadNativeLibProtocol.Add(new Tuple<string, string>("environmentPath",
                    Environment.GetEnvironmentVariable("PATH")));
                loadNativeLibProtocol.Add(new Tuple<string, string>("libraryLocated",
                    (IsInFolder(managedLibInstallPath, nativeLibFileName) ?
                        "YES-" + nativeLibFileName : "NO")));
            }

            if (loadNativeLibSuccess == false)
            {
                //NativeLibrary.SetDllDirectory("D:\\home\\site\\wwwroot\\bin");
                string oldCWD = Directory.GetCurrentDirectory();
                Directory.SetCurrentDirectory(managedLibInstallPath);

                try
                {
                    NativeLibrary.InitErrorHandling(log);
                    loadNativeLibProtocol.Add(new Tuple<string, string>("ATTEMPT_02",
                        "SUCCEEDED: Loading library with adopted CWD successfull."));
                    loadNativeLibSuccess = true;
                }
                catch (Exception e)
                {
                    loadNativeLibProtocol.Add(new Tuple<string, string>("ATTEMPT_02",
                        "FAILED: Loading library with adopted CWD not successfull."));
                    loadNativeLibProtocol.Add(new Tuple<string, string>("exception", e.Message));
                    loadNativeLibProtocol.Add(new Tuple<string, string>("oldCWD", oldCWD));
                    loadNativeLibProtocol.Add(new Tuple<string, string>("newCWD",
                        Directory.GetCurrentDirectory()));
                }

                Directory.SetCurrentDirectory(oldCWD);
            }
            IntPtr hModule = IntPtr.Zero;
            if (loadNativeLibSuccess == false)
            {
                
                try
                {
                    hModule = NativeLibrary.LoadLibrary(managedLibInstallPath + "\\" +
                        nativeLibFileName);

                    NativeLibrary.InitErrorHandling(log);
                    loadNativeLibProtocol.Add(new Tuple<string, string>("ATTEMPT_03",
                        "SUCCEEDED: Loading library with previous call to 'kernel32.dll' " +
                        "successfull."));
                    loadNativeLibSuccess = true;

                    if (hModule != IntPtr.Zero)
                        NativeLibrary.FreeLibrary(hModule);
                }
                catch (Exception e)
                {
                    loadNativeLibProtocol.Add(new Tuple<string, string>("ATTEMPT_03",
                        "FAILED: Loading library with previous call to 'kernel32.dll' " +
                        "not successfull."));
                    loadNativeLibProtocol.Add(new Tuple<string, string>("exception", e.Message));
                    loadNativeLibProtocol.Add(new Tuple<string, string>("hModule",
                        (hModule).ToString()));
                }
            }

            if (loadNativeLibSuccess == false)
            {
                StringBuilder sb = new StringBuilder();
                bool firstEntry = true;
                foreach (var tuple in loadNativeLibProtocol)
                {
                    sb.Append((firstEntry ? "" : ",") + "\"" +
                       tuple.Item1 + "\":\"" + tuple.Item2 + "\"");
                    if (firstEntry)
                        firstEntry = false;
                }
                return (ActionResult)new JsonResult("{" + sb.ToString() + "}");
            }
            else
            {
                              
                string name = req.Query["name"];

                string query = await new StreamReader(req.Body).ReadToEndAsync();
                //dynamic data = JsonConvert.DeserializeObject(requestBody);
                //name = name ?? data?.name;

                string funcname = "";
                string username = "";
                string pwd = "";

                string[] urlparams = query.Split("&");
                foreach(string urlparam in urlparams)
                {
                    string[] tokenizedparams = urlparam.Split("=");
                    for(int i=0; i<tokenizedparams.Length-1; i++)
                    {
                        if (tokenizedparams[i].Equals("name",StringComparison.InvariantCultureIgnoreCase))
                        {
                            funcname = tokenizedparams[i++];
                        }
                        else if (tokenizedparams[i].Equals("username",StringComparison.InvariantCultureIgnoreCase))
                        {
                            username = tokenizedparams[i++];
                        }
                        else if (tokenizedparams[i].Equals("pwd",StringComparison.InvariantCultureIgnoreCase))
                        {
                            pwd = tokenizedparams[i++];
                        }
                    }
                }

                int result = 0;
                if (funcname.Equals("signup", StringComparison.InvariantCultureIgnoreCase))
                {
                    IntPtr pAddressOfFunctionToCall = NativeLibrary.GetProcAddress(hModule, "signup");

                    if (pAddressOfFunctionToCall != IntPtr.Zero)
                    {

                        signup register = (signup)Marshal.GetDelegateForFunctionPointer(
                            pAddressOfFunctionToCall,
                            typeof(signup));

                        result = register(username, pwd);

                        bool res = NativeLibrary.FreeLibrary(hModule);
                    }
                }
                else if (funcname.Equals("signin", StringComparison.InvariantCultureIgnoreCase))
                {
                    IntPtr pAddressOfFunctionToCall = NativeLibrary.GetProcAddress(hModule, "signin");

                    if (pAddressOfFunctionToCall != IntPtr.Zero)
                    {

                        signin login = (signin)Marshal.GetDelegateForFunctionPointer(
                            pAddressOfFunctionToCall,
                            typeof(signin));

                        result = login(username, pwd);

                        bool res = NativeLibrary.FreeLibrary(hModule);
                    }
                }
                else if (funcname.Equals("signout", StringComparison.InvariantCultureIgnoreCase))
                {
                    IntPtr pAddressOfFunctionToCall = NativeLibrary.GetProcAddress(hModule, "signout");

                    if (pAddressOfFunctionToCall != IntPtr.Zero)
                    {

                        signout logout = (signout)Marshal.GetDelegateForFunctionPointer(
                            pAddressOfFunctionToCall,
                            typeof(signout));

                        result = logout();

                        bool res = NativeLibrary.FreeLibrary(hModule);
                    }
                }

                return name != null
                    ? (ActionResult)new OkObjectResult($"{result}")
                    : new BadRequestObjectResult("Please pass a query string in the request body");
            }
        
        }
    }
}
