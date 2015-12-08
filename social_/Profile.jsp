<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" import="java.sql.* " %>
    <%@ page import="java.io.*" %> 
     <%@ page import="signup.*" %> 
  <%if(session.getAttribute("name") == null )
	  {
	  String site = new String("signup_result.jsp");
	  response.setStatus(response.SC_MOVED_TEMPORARILY);
	 //out.print("bvbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	   response.setHeader("Location", site);
	  }
	  else
	  {
		  %>
	  
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="css/header.css" rel="stylesheet" type="text/css">
<link href="css/footer.css" rel="stylesheet" type="text/css">
<title>Hebrik| User-Details</title>
<style type="text/css">
.profile-header
{
	margin-top : 30px;
	height : 80px;
	width : 1000px;
	background-image : url("images/signup-header.jpg");
	
}
table tr td{text-align:centre;}
</style>
</head>
<body> 
<div class="page-in">
<div class="page">
	<div class="main">
	<div class = "header">
	<div class = "profile-header">
	<div style = "float:right;padding:5px;">
	logged in as : <%out.print(session.getAttribute("name")); %>
	<form action ="RegisterNewUser" method = "post">
	<input type="hidden"  name="Isform" value="logout"/>
	<input name="submit" type="image" src="images/logout.png"  value="Logout" />
	</form>
	</div>
	</div>
	<div class="profile-content" style = "min-height:700px ;background-color:#DDDDDD;float:left;width:1000px;">
	   <a style ="text-align:center;font-size:16px;text-decoration:none;" href ="index.jsp"> | Home |</a>
	   <a style ="text-align:center;font-size:16px;text-decoration:none;" href ="search.jsp"> | Search |</a>
	   <%System.out.println(session.getAttribute("user_type").toString());
	   if(session.getAttribute("user_type").toString().equals("employeeinfo") )
	   {out.println("<a style ='text-align:center;font-size:16px;text-decoration:none;' href ='Staff.jsp'> | Staff Privilage |</a>");
	   }
	   %>
	   
	    <h2 style ="text-align:center">User - Details</h2>
	    <%RegisterNewUser rt = new RegisterNewUser(); 
		  ResultSet rs =null;
		  String name = session.getAttribute( "name").toString(); 
	    if(session.getAttribute("user_type").toString().equals("membersinfo")) { %>
	    <table border='2' style='width:80% ;margin :0 auto'>
	    <tr style="font-size:18px;font-family:serif;text-align:center">
	    <td>Name</td>
	    <td>Email</td>
	    <td>DOB</td>
	    <td>Address</td>
	    <td>Fine</td>
	    <td>Phone No.</td>
	    <td>Gender</td>
	    </tr>
		<% 
		   rs =rt.userinfo(name,session.getAttribute("user_type").toString());
		 while(rs.next()){
		 out.print("<tr>");
		 out.print(" <td>"); out.print( rs.getString(3)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(4)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(6)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(7)); out.print(" </td>");
		 //out.print(" <td>"); out.print( rs.getString(8)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(9)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(10)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(11)); out.print(" </td>");
		 out.print("</tr>");
		 }
		 
		 %></table><br><br><br><br><%} 
	    else { %>
	    <table border='2' style='width:80% ;margin :0 auto'>
	    <tr style="font-size:18px;font-family:serif;text-align:center">
	    <td>Name</td>
	    <td>Position</td>
	    <td>Email</td>
	    <td>Responsibility</td>
	    <td>DOB</td>
	    <td>Address</td>
	    <td>Fine</td>
	    <td>Phone No.</td>
	    <td>Gender</td>
	    </tr>
		<% 
		   rs =rt.userinfo(name,session.getAttribute("user_type").toString());
		  
		 while(rs.next()){
		 out.print("<tr>");
		 out.print(" <td>"); out.print( rs.getString(5)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(3)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(6)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(4)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(8)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(7)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(10)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(11)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(12)); out.print(" </td>");
		 out.print("</tr>");
		 }
		 
		 %></table><br><br><br><br><%} %>
		 
		 <div style = "width :500px;margin: 0 auto;">
		<form action='RegisterNewUser' method = 'post'>
		<select name='Isform'>
		<option value = "issuebook">issue</option>
		<option value = "claimbook"> claim</option>
		</select>
		<input type='text'name='bookid' placeholder = "Enter Book ID"/>
		<input type='hidden'name='userid' value='<%out.print(session.getAttribute("name")); %>'/>
		<input type='submit' value='submit'/>
		</form>
		 </div><br><br><br>
		 <div id="borrowedbooks">
		 <h2 style ="text-align:center">Books - Issued</h2>
		<table border='2' style='width:80% ;margin :0 auto'>
	    <tr style="font-size:18px;font-family:serif;text-align:center">
	    <td>Book_ID</td>
	    <td>Title</td>
	    <td>Issued Date</td>
	    <td>Return Date</td>
	    <td>Renew ?</td>
	    </tr>
		<% rs=rt.Borrow(name,session.getAttribute("user_type1").toString());
		 while(rs.next()){
		 out.print("<tr>");
		 out.print(" <td>"); out.print( rs.getString(1)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(2)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(3)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(4)); out.print(" </td>");
		 //out.print(" <td>"); out.print( rs.getString(8)); out.print(" </td>");
		 out.println("<td> <form action='RegisterNewUser' method = 'post'><input type='hidden'name='Isform'  value='renew'/><input type='hidden'name='bookid' value='"+rs.getString(1)+"'/><input type='hidden'name='userid' value='"+session.getAttribute("name")+"'/><input type='submit' value='Renew'/></form></td>");
				
		 out.print("</tr>");
		 }
		 
		 %></table>
		 <br><br><br><br>
		 </div>
		 <div id="claimedbooks">
		 <h2 style ="text-align:center">Books - Claimed</h2>
		  <% 
		  rs = rt.claim(name);
		  out.print("<table border='2' style='width:100%'>");
		 while(rs.next()){
		 out.print("<tr>");
		 out.print(" <td>"); out.print( rs.getString(1)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(2)); out.print(" </td>");
		 out.print(" <td>"); out.print( rs.getString(3)); out.print(" </td>");
		 out.print("</tr>");
		 }
		 out.print("</table>");
		 %>
		 </div>
		
		
		 </div>
		<div class = 'user_img' >
		
		</div>
		</div>
<div class="footer">
	<p>&copy; Copyright 2014. Designed by Rajesh Roshan Behera,B Somanaik, Pintulal Meena .
	</p>
	<ul>
	  <li style="border-left: medium none;"><a href="index.jsp"><span>Home</span></a></li>
	  <li><a href="#"><span>About&nbsp;us</span></a></li>
	  <li><a href="#"><span>What's&nbsp;new</span></a></li>
	  <li><a href="#"><span>Services</span></a></li>
	  <li><a href="#"><span>Contact</span></a></li>
	  <li><a href="#"><span>Hours &amp; Location</span></a></li>
	</ul>
	</div>
</div>
</div>
</body>
</html>

<% } %>



