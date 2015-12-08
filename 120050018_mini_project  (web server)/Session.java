import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.util.Date;
/**
 * @author pintulalmeena
 *
 */
public class Session {
	static int port =8084;
	static String portstr ="8084";
	static ServerSocket echoSocket;
	public Session() {
		// TODO Auto-generated constructor stub
	}
	    public static void main(String[] args) throws IOException {
		// TODO Auto-generated method 
	    
		try {
		     echoSocket = new ServerSocket(port);
		   //creating new server  :> ref = http://docs.oracle.com/javase/tutorial/networking/sockets/readingWriting.html
		    }
		catch(IOException e) {
	         e.printStackTrace();
	        }
        System.out.println("server started do next ");
        while(true){
    		 Socket remote = echoSocket.accept();
    		 ////creating new server  :> ref = http://docs.oracle.com/javase/tutorial/networking/sockets/readingWriting.html
    		 Pipeline1 pipe = new Pipeline1(remote);
			 Thread t =new Thread(pipe);
			 if(Thread.activeCount() <5)
    	     t.start();
    	     // creating new thread as request accepted 
    	     //:> ref  = http://www.lnaffah.com/aula3/WebServer.java
    	    System.out.println( "active thread are  =============================="+Thread.currentThread().getName()+"===="+Thread.activeCount());
        }
        }
       
}
final class Pipeline1 implements Runnable{
	//static int port =8084;
	static String portstr ="8084";
	 static File OrgFile;
	static String path;
	static String filepath;
	 private  Socket remote;
    public Pipeline1(Socket remote){
    	this.setRemote(remote);
   }
	public  Socket getRemote() {
		return remote;
	}
	public void setRemote(Socket remote) {
		this.remote = remote;
	}
	@Override
	public void run() {
		try{
		PipeLiner();
		}catch(IOException e) {
	         e.printStackTrace();
   	}
		
	}
	 private static String DataType(String path)
	    {
	        if (path.endsWith(".html") || path.endsWith(".htm")) 
	            return "text/html";
	        else if (path.endsWith(".css") || path.endsWith(".CSS")) 
	            return "text/css";
	        else if (path.endsWith(".txt") || path.endsWith(".java")) 
	            return "text/plain";
	        else if (path.endsWith(".gif") || path.endsWith(".GIF") ) 
	            return "image/gif";
	        else if (path.endsWith(".class"))
	            return "application/octet-stream";
	        else if (path.endsWith(".jpg") || path.endsWith(".jpeg"))
	            return "image/jpeg";
	        else if (path.endsWith(".png") || path.endsWith(".PNG"))
	            return "image/png";
	        else if (path.endsWith(".mp4") || path.endsWith(".MP4"))
	            return "media/mp4";
	        else    
	            return "text/plain";
	    }
	 // filtering file as that .extension and return type 
     //:> ref  = http://www.lnaffah.com/aula3/WebServer.java
	   private static void sendFile(FileInputStream file, OutputStream out)
	    {     
	        try {
	        	long time = System.nanoTime();
	            byte[] buffer = new byte[1000];   //buffer size 
	            while (file.available()>0) 
	                out.write(buffer, 0, file.read(buffer));  //sending data to client 
	           long time1 = System.nanoTime();
	           System.out.println(1000.0/(time1-time)); ///time to send file  oracle.com time class 
	         } 
	        catch (IOException e) { System.err.println(e); }
	    }
	   //for sending file to client finally 
	   private static void errorReport(PrintStream pout, Socket remote, String code, String msg)
	    {
	        pout.print("HTTP/1.0 " + code +  "\r\n" +
	                   "\r\n" +
	                   "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n" +
	                   "<TITLE>" + code +  "</TITLE>\r\n" +
	                   "</HEAD><BODY>\r\n" +
	                   "<H1> </H1>\r\n" + msg + "<P>\r\n" +
	                   "<HR><ADDRESS>FileServer 1.0 at " + 
	                   remote.getLocalAddress().getHostName() + 
	                   " Port " + remote.getLocalPort() + "</ADDRESS>\r\n" +
	                   "</BODY></HTML>\r\n");
	        //log(remote);
	    }
	  private static void log(Socket remote)
	    {    
	        System.err.println(new Date() + " [" + remote.getInetAddress().getHostAddress()  + ":" + remote.getPort() + "] " + "200 OK");
	    }  //for tracking client 
	  static String pathme = null;
  	 // static String referer;
	  
	private  void PipeLiner() throws IOException{
		//for(int i=0;i<10 ;i++){
		 // getRemote().setSoTimeout(1000000);
		  //if(getRemote().isInputShutdown())break;
    	  System.out.println("remote, sending data.");
    	  BufferedReader in = new BufferedReader(new InputStreamReader(getRemote().getInputStream())); 
    	  //client input reader  ref shortcut
 	     //:> ref  = http://www.lnaffah.com/aula3/WebServer.java
    	  BufferedOutputStream out = new BufferedOutputStream(getRemote().getOutputStream());
    	//client input reader  ref shortcut
    	  PrintStream pout = new PrintStream(out);
            String request = in.readLine();
            if (request==null)return;
            remote.setKeepAlive(true);
            System.out.println( remote.getSendBufferSize()+"====buffer size ==="+remote.getKeepAlive()+"===="+remote.getReceiveBufferSize());
           
          //  log(getRemote(), request);
            System.out.println(request);
            //referer =null;
           while (true) {
                String misc = in.readLine();
                System.out.println(misc);
               // if(misc.contains("Referer:")){
                	//referer = misc.substring(misc.lastIndexOf(":"+portstr)+5,misc.length());
                //	System.out.println(referer+"===========");
               // }
               if (misc==null || misc.length()==0)
                    break;
           }  //checking is get ,minimum length GET / HTTP/1.1 ,HTTP/1.1 ?
            if (!request.startsWith("GET") || request.length()<14 || !(request.endsWith("HTTP/1.0") || request.endsWith("HTTP/1.1"))) {
                // bad request
                errorReport(pout, getRemote(), "400   Bad Request",  "Your browser sent a request that " +  "this server could not understand.");
            }
            else {
            	
                String req = request.substring(4, request.length()-9).trim();
              //  System.out.println(req+"++++++++++++++++request print");  filter file name
                if (req.indexOf("..")!=-1 ||  req.indexOf("/.ht")!=-1 || req.endsWith("~")) {
                    errorReport(pout, getRemote(), "403  Forbidden", "You don't have permission to access the requested URL.");
                }
                
                else {
                	//defining root folder for file 
                	String Dir = "/home/pintulalmeena/workspace/Webserver"; 
                	String  root =Dir+"/src";
                //	 System.out.println(req);
                   /* if(req.endsWith("/")){
                    	 System.out.println(1.0);
                    	filepath =root+req+"index.html";
                    	pathme =root+req;
                    	
                    } */
                      if(req.contains("~")) req ="/"+req.substring(req.indexOf("~")+1);
                      if (req.lastIndexOf(".")==-1){
                    //	 System.out.println(2.0);
                    	 filepath =root+req+"/public_html/index.html";
                     	 pathme =root+req+"/public_html";
                    }
                     else {
                    	 if(pathme==null){
                    	//	 System.out.println(3.1);
                    	 filepath =root+req;}
                    	 else {filepath =pathme+req;
                    	// System.out.println(3.2);
                    	 }
                     }
                	//System.out.println(remote.;
                	
                	//if(pathme==null){
                	//	System.out.println("pathme null ");
                   // path = Dir+"/src";
                    
                	//}
                	//else{
                	//	System.out.println(referer+"true and false");
                		//if(!(referer=="/") &&!(referer==null) ){
                	  // path = Dir+"/src"+referer;
                	  //// System.out.println(path+"   contain .");
                	//	}
                		//else {System.out.println("refer null ");path =pathme;}
                		//}
                   // filepath= path+req;
                   
                    URL MYurl = new URL("http://"+filepath);
                    System.out.println(filepath+"             ======this is file path");
                    filepath =MYurl.getPath();
                    OrgFile = new File(filepath); 
                     
                      if (OrgFile.isDirectory()) {   //isDirectory   eclipse hint
                             pathme =root+req;
                             filepath =pathme+"/index.html"; 
                             System.out.println(filepath);
                           OrgFile = new File(filepath);   
                      }
                      //===================================================================================path 
                      try { 
                          // send file to client
                          FileInputStream file = new FileInputStream(OrgFile);
                          pout.print("HTTP/1.0 200 OK\r\n" +  "Content-Type: " + DataType(filepath) +"   "+ "\r\n" + "Date: " + new Date() + "\r\n" + "Server: FileServer 1.0\r\n\r\n");
                          sendFile(file, out); // send raw file 
                          log(getRemote());
                      }
                      catch (FileNotFoundException e) { 
                           // file not found
                          errorReport(pout, getRemote(), "404  Not Found", "The requested URL was not found on this server.");
                      }
                 }
             } 
            out.flush();
		   // }  
	        getRemote().close();
            }
}
