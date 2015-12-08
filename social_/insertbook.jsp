<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" import="java.sql.* " %>
    <%@ page import="java.io.*" %> 
     <%@ page import="signup.*" %> 
  <%if(!(session.getAttribute("user_type").toString().equals("employeeinfo")))
	  {
	  String site = new String("Profile.jsp");
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
.bookinfo
{
padding:5px;
font-family:serif;
font-size : 16px;
width : 400px;
height :auto;
border : black solid 2px;
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
	<%if(session.getAttribute("name") != null)
			{
				out.println("<div style = 'float:right;padding:5px;'>");
				out.println("logged in as : <a href = 'Profile.jsp'>" +session.getAttribute("name")+ "</a>");
				out.println("<form action ='RegisterNewUser' method ='post'>");
				out.println("<input type='hidden'  name='Isform' value='logout'/>");
				out.println("<input name='submit' type='image' src='images/logout.png'  value='Logout' />");
				out.println("</form>");
				out.println("</div>");
			}
				%>
	</div>
	</div>
	<div class="profile-content" style = "min-height:700px ;background-color:#DDDDDD;float:left;width:1000px;">
	 <a style ="text-align:center;font-size:16px;text-decoration:none;" href ="index.jsp"> | Home |</a>
	 <a style ="text-align:center;font-size:16px;text-decoration:none;" href ="search.jsp"> | Search |</a>
	    <div style = "width :700px;margin: 0 auto;">
	    	<h2 style="font-family:serif;text-align:center;">Add a Book</h2><br>
			<form action='RegisterNewUser' method = 'post' style = "margin: 0 auto;" >
			<input id ="bookinfo" type="hidden" name="Isform" value="insertbook"/>
			<input id ="bookinfo" type='text'name='bookid' placeholder = "Enter Book ID"/>
			<input id ="bookinfo" type='text'name='subject' placeholder = "Enter subject"/>
			<input id ="bookinfo" type='text'name='area' placeholder = "Enter area"/><br>
			<input id ="bookinfo" type='text'name='title' placeholder = "Enter book title"/>
			<input id ="bookinfo" type='text'name='authors' placeholder = "Enter authors"/>
			<input id ="bookinfo" type='text'name='publishers' placeholder = "Enter Book publishers"/><br>
			<input id ="bookinfo" type='text'name='edition' placeholder = "Enter Book edition"/>
			<input id ="bookinfo" type='text'name='isbn' placeholder = "Enter Book isbn"/>
			<input id ="bookinfo" type='text'name='price' placeholder = "Enter Book price"/><br>
			<input id ="bookinfo" type='text'name='shelfno' placeholder = "Enter Book shelfno"/><br><br><br>
			
			<input type='submit' value=' Add Book'/>
			</form>
		<div><%if(request.getParameter("msg")!=null) out.print(request.getParameter("msg").toString());%></div>
	
		
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
</div>
</body>
</html>

<% } %>



