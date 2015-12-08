<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" import="java.sql.* " %>
 <%@ page import="java.io.*" %>    
<%
try {
String driver = "org.postgresql.Driver";
String url = "jdbc:postgresql://localhost/sociallib";
String username = "pintulal";
String password = "pintulal";
String myDataField = null;

Connection conn1 = null;
PreparedStatement myPreparedStatement = null;
ResultSet myResultSet = null;

Class.forName("org.postgresql.Driver");
conn1 = DriverManager.getConnection(url,username,password);
System.out.println(conn1);
PreparedStatement prest = conn1.prepareStatement("SELECT * FROM membersinfo WHERE UserID = ?");
prest.setString(1, "rajesh");
//prest.setString(2, pword);
ResultSet result =prest.executeQuery();
int i=1;
while(result.next()){
   myDataField = result.getString(2);
out.print(myDataField);
i++;
}
}
catch(ClassNotFoundException e){e.printStackTrace();}
catch (SQLException ex)
{
out.print("SQLException: "+ex.getMessage());
out.print("SQLState: " + ex.getSQLState());
out.print("VendorError: " + ex.getErrorCode());
} 

%>





