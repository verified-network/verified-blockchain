package com.verifiedblockchain;

import java.util.*;
import com.microsoft.azure.functions.annotation.*;
import com.microsoft.azure.functions.*;

/**
 * Azure function to interface with the Verified Blockchain 
 */
public class Function {

    static {
        try {
            System.load("VERIFIEDBLOCKCHAIN.dll");
        } catch (UnsatisfiedLinkError e) {
          System.err.println("Verified blockchain native library failed to load.\n" + e);
          System.exit(1);
        }
    }

    public static native int signup(String username, String pwd);
    public static native int signin(String username, String pwd);
    public static native int signout();

    /**
     * This function listens at endpoint "/api/VerifiedBlockchain". Two ways to invoke it using "curl" command in bash:
     * 1. curl -d "HTTP Body" {your host}/api/VerifiedBlockchain
     * 2. curl "{your host}/api/VerifiedBlockchain?name=HTTP%20Query"
     */
    @FunctionName("VerifiedBlockchain")
    public HttpResponseMessage run(
            @HttpTrigger(name = "req", methods = {HttpMethod.GET, HttpMethod.POST}, authLevel = AuthorizationLevel.ANONYMOUS) HttpRequestMessage<Optional<String>> request,
            final ExecutionContext context) {
        context.getLogger().info("Java HTTP trigger processed a request.");

        // Parse query parameter
        String name = request.getQueryParameters().get("name");
        String query = request.getBody().orElse(name);

        if (query == null) {
            return request.createResponseBuilder(HttpStatus.BAD_REQUEST).body("Please pass a query for the Verified blockchain API.").build();
        } else {
            //parse query json and call Verified blockchain APIs here
            String funcname="";
            String username="";
            String pwd="";

            StringTokenizer urlparams = new StringTokenizer(query,"&",false);
            while(urlparams.hasMoreTokens()){
                StringTokenizer tokenizedparam = new StringTokenizer(urlparams.nextToken(),"=",false);
                while(tokenizedparam.hasMoreTokens()){
                    if(tokenizedparam.nextToken().equalsIgnoreCase("name")){
                        funcname = tokenizedparam.nextToken();
                    }
                    else if(tokenizedparam.nextToken().equalsIgnoreCase("username")){
                        username = tokenizedparam.nextToken();
                    }
                    else if(tokenizedparam.nextToken().equalsIgnoreCase("pwd")){
                        pwd = tokenizedparam.nextToken();
                    }
                }
            }

            int result=0;
            if(funcname.equalsIgnoreCase("signup")){
                result = signup(username,pwd);
            }
            else if(funcname.equalsIgnoreCase("signin")){
                result = signin(username,pwd);
            }
            else if(funcname.equalsIgnoreCase("signout")){
                result = signout();
            }            

            return request.createResponseBuilder(HttpStatus.OK).body(result).build();
        }
    }
}
