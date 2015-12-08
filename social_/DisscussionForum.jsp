<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@page import="java.sql.* "%>
    <%@page import="java.io.*" %>
    <%@page import="signup.*" %>

<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Hebrik|Discussion Forum</title>
  <link type="image/png" rel="icon" href="images/hebrik-logo.png">
  <meta name="description" content="This Page Contains discussions about a book, 
   users,new arrival boks, events and latest news">
  <meta name="keywords" content="reply,follow, add a new post">
  <link href="css/style.css" rel="stylesheet" type="text/css">
  <link href="css/discuss.css" rel="stylesheet" type="text/css">
  <link href="css/header.css" rel="stylesheet" type="text/css">
  <link href="css/footer.css" rel="stylesheet" type="text/css">
  <style type="text/css">
  .signup-header
	{
		margin-top : 30px;
		height : 80px;
		width : 1000px;
		border-radius : 5px;
		background-image : url("images/signup-header.jpg")
	}
	.line1
	{
		height : 5px;
		width : 1000px;
		background:transparent;
	}
  </style>
</head>


<body>
<div class="page-in">
<div class="page">
	<div class="main">	
		<div class="header">
			<div class = "signup-header">
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
			<div class = "line1"></div>
						<div class ="line2"></div>
		</div>
	<div class= "discussion-content" style="background-color:aqua;">
	<a style ="text-align:center;font-size:16px;text-decoration:none;" href ="index.jsp"> | Home |</a>
	<a style ="text-align:center;font-size:16px;text-decoration:none;" href ="search.jsp"> | Search |</a>
		<div class="discussion-header">
		</div>
		
		<div class = "discussion-body" style ="background-color:#CCCCCC" >
			<%if (session.getAttribute("name") != null) {%>
			<div id="new-discussion" style="background-color:#6666FF;">
			<table style="width:100%">
			<tr>
			<td>
			<img src = "images/male_profile.gif" style="width:100px">
			<%String userid =session.getAttribute("name").toString();
			out.println("<a href ='Profile.jsp' style = 'text-decoration :none;' ><p style = 'font-size:16px;font-family:serif;color:white;'>"+userid + "</p></a>");
			%>
			</td>
			<td>
			<div id="post_div" >
			<form  method="POST" action="RegisterNewUser">
			<input type="hidden"  name="Isform" placeholder="Isform" value="discuss"/> 
			<input type="hidden"  name="userid" placeholder="Isform" value="<% out.print(userid);%>"/>
			Title
			<input type="text"  name="title" placeholder="Subject" style = "width :400px"/>
			<textarea name="postcontent" rows= "4" cols = "70" style="resize:none;padding:15px;font-family:serif;font-size:16px">
			</textarea>
			<input class="postimg" name ="submit" type="image" src="images/post.png" value="POST" />
			</form>
			<div id = "err_msg"><%if(request.getParameter("err_msg") != null){out.println(request.getParameter("err_msg"));} %></div>
			 </div>
			</td>
			</tr>
			</table>
			</div>
			<% } %>
			<div id = "old-posts" style = "position:relative;background-color:#CCCCCC">
			<% RegisterNewUser posts = new RegisterNewUser();
				ResultSet rs = posts.getposts();
				while(rs.next())
				{	out.println("<div id='post_view' >");
					out.println("<div class='post_user'>");
					out.println("<img id ='profile_pic_small' src = 'images/male_profile.gif'/>");
					out.println("<p id='username'>"+rs.getString(1)+"</p>");
					out.println("</div>");
					out.println("<div class='post_time'>");
					out.println(rs.getString(4));
					out.println("</div>");
					out.println("<div class='post_msg'>");
					out.println(rs.getString(3));
					out.println("</div>");
					out.println("</div>");
				}
				%>
			</div>
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
<!--DO NOT Remove The Footer Links-->
</div>
</div>
</div>

</body></html>