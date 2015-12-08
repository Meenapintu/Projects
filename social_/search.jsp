<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@page import="java.sql.* "%>
    <%@page import="java.io.*" %>
    <%@page import="signup.*" %>
    
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="css/main.css" rel="stylesheet" type="text/css">
<link href="css/header.css" rel="stylesheet" type="text/css">
<link href="css/footer.css" rel="stylesheet" type="text/css">
<link type="image/png" rel="icon" href="images/hebrik-logo.png">
<title>Search Result</title>
</head>
<script>

	 function rate(){
		 document.write( "<select name ='rate'value='Year'>");
		 for(var datef= 0;datef<11;datef++)
		 document.write("<option value = "+datef+" >"+datef+".0</option>");
		 document.write("</select>");
		 }

</script>
<body>
<div class="page-in">
<div class="page">
	<div class="main">
		<div class="header">
			<div class = "signup-header">
			<% //System.out.println(session.getAttribute("name"));
			if(session.getAttribute("name") != null)
			{
				out.println("<div style = 'float:right;padding:5px;'>");
				out.println("logged in as : <a href = 'Profile.jsp'>" +session.getAttribute("name")+ "</a>");
				out.println("<form action ='RegisterNewUser' method = 'post'>");
				out.println("<input type='hidden'  name='Isform' value='logout'/>");
				out.println("<input name='submit' type='image' src='images/logout.png'  value='Logout' />");
				out.println("</form>");
				out.println("</div>");
			}
			 //System.out.println(session.getAttribute("name"));
				%>
			
			</div>
			<div class = "line1"></div>
			<div class="search-bar">
		     <form action= "RegisterNewUser" method="post">
				<ul>
				<input type="hidden"  name="Isform" placeholder="Isform" value="search"/> 
				  <li> <h2 style="color:white"> Search </h2>  </li>
				  <li>
				    <select name="m_thing">
				        <option value="book" selected>Book</option>
				        <option value="ebook" >E-book</option>
				        <option value="journal" >Journal</option>
				        <option value="ejournal" >E-journal</option>
				        <option value="websit" >Website</option>
				        <!--<option value="global" >ALL</option>-->
				      </select>
				  </li>
				  <li><h2 style="color:white">By</h2></li>
				  <li>
				    <select name="m_field">
				        <option value="title" selected>Title</option>
				        <option value="bookid" >Acc. No.</option>
				       
				        <option value="publishers" >Publisher</option>
				        <option value="isbn" >ISBN No</option>
				         <!--<option value="global" >ALL</option>-->
				      </select>
				  </li>
				  <li><h2 style="color:white">With</h2></li>
				  <li>
				    <select name="m_match">
				      <option value="Words" >Any of Words</option>
				          <option value="AllWords" >All Words</option>
				          <option value="Embeded" >Strict Search</option>
				      		<option value="Exact" >Exact</option> 
				      </select>
				  </li>
				  <li><h2 style="color:white"> For </h2></li>
				  <li>
				  <input type="text" MAXLENGTH="100" size="30" name="searchstring" placeholder="Enter The String">
				  </li>
				  <li>
				    <input id="s1" type="submit" value="Search" >
				  </li>
				</ul>
				</form>
			</div>
		</div>
		<div class ="content" style ="min-height:700px">
		<a style ="text-align:center;font-size:16px;text-decoration:none;" href ="index.jsp"> | Home |</a>
			<h2 style =" color : blue ;text-align : center;">Search Result</h2><br>
			<% //System.out.println(session.getAttribute("searchString") + "*************************");%>
			<%if(session.getAttribute("searchString") != null && session.getAttribute("searchString") !="" )
				
			{ System.out.println(session.getAttribute("searchString").toString().length() + "*************************");%>
			<div>
			<% RegisterNewUser books = new RegisterNewUser();
			String m =session.getAttribute("material").toString();
			String s =session.getAttribute("searchby").toString();
			String w =session.getAttribute("with").toString();
			String ss=session.getAttribute("searchString").toString();
			 ResultSet rs = books.searchresult(m,s,w,ss);
			 session.setAttribute("material",null);
			 session.setAttribute("searchby",null);
			 session.setAttribute("with",null);
			 session.setAttribute("searchString",null);
			out.println("<table style ='width : 80%;alignment:center;margin: 0 auto;' >");
			 while(rs.next())
			{
				out.print("<tr><td style='width:20%;'>");
				out.print("<img src='images/image3.png' style ='height :50px' />");
				out.print("</td>");
				out.print("<td style='width:80%;background-color:#EEEEEE; padding:15px;'>");
				out.println("<div style='font-size:16px;font-family:serif;'>");
				out.println("<i><b>"+rs.getString(4)+"</b></i><br>");
				out.println(rs.getString(8)+"<br>");
				out.println(rs.getString(5)+"<br>");
				out.println(rs.getString(6)+"<br>");
				out.println(rs.getString(1)+"<br>");
				out.println("ISBN : "+ rs.getString(10)+"<br>");
				out.println("Rating : "+ rs.getString(13)+"<br>");
				out.println(rs.getString(2)+"<br>");
				if(rs.getString(7).equals("f"))
				{
					out.println("<h3> ISSUED </h3>");
					if(session.getAttribute("name") != null)
					{   out.println("<form action='RegisterNewUser' method = 'post'><input type='hidden'name='Isform'  value='rate'/><script>rate()</script><input type='hidden'name='bookid' value='"+rs.getString(1)+"'/><input type='hidden'name='userid' value='"+session.getAttribute("name")+"'/><input type='submit' value='Rate'/></form>");
					if(session.getAttribute("user_type").toString().equals("membersinfo"))
					{
					out.println("<form action='RegisterNewUser' method = 'post'><input type='hidden'name='Isform'  value='claimbook'/><input type='hidden'name='bookid' value='"+rs.getString(1)+"'/><input type='hidden'name='userid' value='"+session.getAttribute("name")+"'/><input type='submit' value='claim'/></form>");
					}
					}
				}
				else
				{
					if(session.getAttribute("name") != null)
					{     out.println("<form action='RegisterNewUser' method = 'post'><input type='hidden'name='Isform'  value='rate'/><script>rate()</script><input type='hidden'name='bookid' value='"+rs.getString(1)+"'/><input type='hidden'name='userid' value='"+session.getAttribute("name")+"'/><input type='submit' value='Rate'/></form>");
						out.println("<form action='RegisterNewUser' method = 'post'><input type='hidden'name='Isform'  value='issuebook'/><input type='hidden'name='bookid' value='"+rs.getString(1)+"'/><input type='hidden'name='userid' value='"+session.getAttribute("name")+"'/><input type='submit' value='issue book'/></form>");
					}
				}
				
				out.println("</div>");
				out.println("</td>");
				out.println("</tr>");
			
			}
			 out.println("</table>");
			%>
			</div>
			<% } %>
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