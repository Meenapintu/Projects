<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Hebrik|Registration Status</title>
  <link type="image/png" rel="icon" href="images/hebrik-logo.png">
  <meta name="description" content="This Page Contains searching a book, 
  login to a registered library account,new arrival boks, events
   and latest news">
  <meta name="keywords" content="search,issue,renew,claim,new arrival,latest events">
  <link href="css/main.css" rel="stylesheet" type="text/css">
  <link href="css/header.css" rel="stylesheet" type="text/css">
   <link href="css/footer.css" rel="stylesheet" type="text/css">
   <script src="js/signup.js"></script>   
   <script type="text/javascript">
		 function circular_img_slider(){
		 	var i=1;
		 	function what() {
		    document.getElementById("slide").src = "images/image"+i+".png";
		      i++;
		    if (i == 6)  
		     i=1;
		  }
		  var id = setInterval(what, 3000) ;
		 }
		 </script>
</head>


<body onload="circular_img_slider()">
<div class="page-in">
<div class="page">
	<div class="main">
		<div class="header">
			<div class = "signup-header">
			
			</div>
			<div class = "line1"></div>
		</div>
		<div class ="content" style="height : 500px;">
		<a style ="text-align:center;font-size:16px;text-decoration:none;" href ="index.jsp"> | Home |</a>
			<div class="signup-content" style = "background-color : #abbaab">
			<p style ="font-size:20px; margin :50px;color : white;" ><% if(request.getParameter("status") != null){out.print(request.getParameter("status"));}%></p>
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
				<div id= "err_msg"><% if(request.getParameter("err_msg") != null){out.print(request.getParameter("err_msg")) ;}%></div>
				<input class="login" name="submit" type="image" src="images/login.png" value="Submit" />
				<a class="login" href="#">or Forgot Password</a>or
				<a class="login" href="Register.jsp"> Signup </a>
				</form>
			</div>

				<div class="img-content">
				<img id="slide" src="images/image1.png" >
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