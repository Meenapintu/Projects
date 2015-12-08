<!DOCTYPE html>
<html>
<head>
<script>
function loadXMLDoc(which)
{ //alert(which);
var xmlhttp;
if (window.XMLHttpRequest)
  {// code for IE7+, Firefox, Chrome, Opera, Safari
  xmlhttp=new XMLHttpRequest();
  }
else
  {// code for IE6, IE5
  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
xmlhttp.onreadystatechange=function()
  {
  if (xmlhttp.readyState==4 && xmlhttp.status==200)
    {
    document.getElementById("myDiv").innerHTML=xmlhttp.responseText;
    }
  }
xmlhttp.open("GET",which,true);
xmlhttp.send();
}
</script>
  <meta charset="UTF-8">
  <title>Hebrik Library|Home</title>
  <link type="image/png" rel="icon" href="images/hebrik-logo.png">
  <meta name="description" content="This Page Contains searching a book, 
  login to a registered library account,new arrival boks, events
   and latest news">
  <meta name="keywords" content="search,issue,renew,claim,new arrival,latest events">
  <link href="css/style.css" rel="stylesheet" type="text/css">
  <link href="css/header.css" rel="stylesheet" type="text/css">
   <link href="css/footer.css" rel="stylesheet" type="text/css">
   <style type="text/css">
   #userid1 { font-size: 18px; padding:4px 5px; width: 300px;display:block;margin:1px; background-color:#effcf1;
				font-family:'lucida grande',tahoma,verdana,arial,sans-serif;}
   </style>
   <script type="text/javascript">
		 function slider(){
		 	var i=1;
		 	function what() {
		    document.getElementById("slide").src = "sliding_images/img"+i+".jpg";
		      i++;
		    if (i == 8)  
		     i=1;
		  }
		  var id = setInterval(what, 2000) ;
		 }
		 </script>
</head>


<body onload="slider()">
<div class="page-in">
<div class="page">
	<div class="main">
		<div class="header">
			<div class="header-top">
			<ul>
			<li> <img src="images/img1.png"></li>
			<li style="margin-left:50px"><h1>So<span>ci</span>al Li<span>bra</span>ry <span>(Hebrik)</span></h1></li>
			<%if (session.getAttribute("name") != null)
				{ 
				out.println("<div style = 'float:right;padding:5px;'>");
				out.println("logged in as : <a href = 'Profile.jsp'>" +session.getAttribute("name")+ "</a>");
				out.println("<form action ='RegisterNewUser' method = 'post'>");
				out.println("<input type='hidden'  name='Isform' value='logout'/>");
				out.println("<input name='submit' type='image' src='images/logout.png'  value='Logout' />");
				out.println("</form>");
				out.println("</div>");
				} else {
				%>
				
			<li style = "img-align:right;margin-left :100px" id="loginlink"><img src ="images/login.png" style=""/>
				<% } %>
	            <div id="login-info">
	            <form id="login" action="RegisterNewUser" method="post">
	            <input type="hidden"  name="Isform" placeholder="Isform" value="Login"/> 
				<input id="userid1" type="text" name="uname" placeholder="Username"/>
				<input id="userid1" type="password" name="pword" placeholder="Password"/>
				<div id="gender">
				<ul class= "horizontal-list" style = "margin : 10px">
				<li><input type="radio" id = "sex" name="user_type" value="membersinfo" checked>member</li>
				<li><input type="radio" id= "sex" name="user_type" value="employeeinfo">Staff</li>
				</ul>
				</div><br>
				<input class="login" name="submit" type="image" src="images/login.png" value="Submit" />
				<a class="login" href="#">or Forgot Password</a>
				</form>
	            <a href="Register.jsp"> signUp </a>
	            </div>
			</li>
			</ul>
			</div>
			<div class="header-bottom">
			<div class="thought-left">
			<h2>Library Is The Temple For Studies</h2>		
			</div>
			 
			<div class="img-right">
			<img id = "slide" alt="sliding img1" src="sliding_images/img1.jpg">	
			</div>
			</div>
			<div class="topmenu">
				<ul>
				  <li style="background: transparent none repeat scroll 0% 50%; -moz-background-clip: initial; -moz-background-origin: initial; -moz-background-inline-policy: initial; padding-left: 0px;"><a href="index.jsp" ><span>Home</span></a></li>
				 
				  <li><a href="#" onclick="loadXMLDoc('about.jsp')"><span>About&nbsp;us</span></a></li>
				  <li><a href="#" onclick="loadXMLDoc('whatsnew.jsp')"><span>What's&nbsp;new</span></a></li>
				  <li><a href="#"  onclick="loadXMLDoc('services.jsp')"><span>Services</span></a></li>
				  <li><a href="#"  onclick="loadXMLDoc('Location.jsp')"><span>Location &amp; Hours</span></a></li>
				  <li><a href="#"><span>E-Books/E-Journal</span></a></li>
				</ul>
			</div>
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

		<div class="content">
			<div class="content-left">

				<div class="left-left">
				   <h2> Whats New ? </h2>
				   <hr />
				   <div class="newthings">
				   	<ul>
				   		<li><h4>New Arrival</h4></li>
				   		<li><h4>New Data Bases</h4></li>
				   		<li><h4>Most Fequent Book</h4></li>
				   	</ul>
				   </div>

				   <h2> Today's Hours</h2><hr />
				   <div id="todayshours">
				   	
				   </div>
				   <h2> My Library</h2><hr />
				   <div id="mylibrary">
				   <ul style="list-style:none;font-size:15px">
				   	<li><a href="Profile.jsp">claim book</a></li>
				   	<li><a href="Profile.jsp">renew book</a></li>
				   	<li><a href="DisscussionForum.jsp">Disscussion Forum</a></li>
				   
				   	</ul>
				   </div>
				</div>
		        <div id="myDiv" class="left-right">

					<div class="row1">
						<h1 class="title">Welcome To <span>HEBRIK'S Library Website .</span></h1>
						<h4> Want to Disscuss About Books ? Go To <a href="DisscussionForum.jsp"> Disscussion Forum</a></h4>
						
					<p>&nbsp;</p>
			    	</div>


		        	<div class="row2">
						<h2 class="subtitle">About <span>Us</span></h2>
						
						<p>&nbsp;</p>
						<p align="right"><a href="#" class="more">Read More</a></p>
		       		</div>
				</div>
			</div>
			<div class="content-right">
				<div class="mainmenu">
					<h2 class="sidebar1">News/Events</h2>
					<hr />
					
				</div>
				<div class="contact">
					<h2 class="sidebar2">Contact</h2>
					<div class="contact-detail">
					<p class="green"><strong>+91-7738847553</strong></p>
				</div>
			</div>
		</div>

	</div>

	<div class="footer">
	<p>&copy; Copyright 2014. Designed by Rajesh Roshan Behera,B Somanaik, Pintulal Meena .
	</p>
	<ul>
	  <li style="border-left: medium none;"><a href="index.jsp"><span>Home</span></a></li>
	  <li><a href="#" ><span>About&nbsp;us</span></a></li>
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