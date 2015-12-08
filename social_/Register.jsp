<!DOCTYPE html>
<html>
<%   %>
<head>
  <meta charset="UTF-8">
  <title>Hebrik| Membership Forum</title>
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
		 
		 function date_of_birth()
		 {
		 document.write("<select name ='Date'value='Date'>");
		 for(var i =1;i <= 31;i++){
		 if(i <10)document.write("<option value = 0"+i+">0"+i+"</option>");
		 else{
		 document.write("<option value = "+i+">"+i+"</option>");
		 }
		 }
		 document.write("</select>");
		 }
		 
		 function year_of_birth(){
		 var curTime = new Date()
		 var datef = curTime.getFullYear();
		 var datef_ = datef -100;
		 document.write( "<select name ='Year'value='Year'>");
		 for( datef;datef >datef_;datef--)
		 document.write("<option value = "+datef+" >"+datef+"</option>");
		 document.write("</select>");
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
		<div class ="content">
		<a style ="text-align:center;font-size:16px;text-decoration:none;" href ="index.jsp"> | Home |</a>
			<div class="signup-content">
				<form id="signup" name="signup" action="RegisterNewUser"  onsubmit ="return validateForm()" method="post" >
				
				<input type="hidden"  name="Isform" placeholder="Isform" value="Register"/> 
				<div id="userdata" ><div id="sign_up">Sign Up </div>
				<ul class="horizonal-list">
				<li><input id = "firstname" type="text" name="firstname" placeholder="First Name" onblur = "checkFName('nameerror1');"/></li>
				<li><input id = "middlename" type="text" name="middlename" placeholder="Middle Name" onblur = "checkMName('nameerror2')"/></li>
				<li><input id= "lastname" type="text" name="lastname" placeholder="Last Name" onblur = "checkLName('nameerror3')"/></li>
				</ul>
				<div id= "nameerror1" class = "err_msg"></div>
				<div id= "nameerror2" class = "err_msg"></div>
				<div id= "nameerror3" class = "err_msg"></div>
				
				<input type="text" id = "username" name="username" placeholder="Username" onblur = "validateUsername('unerror')"/> 
				<div id= "unerror" class="err_msg">
				<% if (request.getParameter("err_msg") != null) { %>
				Username Already Exists ..
				<% } %>
				</div>
				
				<input id="email"type="text" name="email" placeholder="Email" onblur= "echeck('eerror')"/>
				<div id= "eerror" class="err_msg"></div>
				
				<input id="reemail" type="text" name="remail" placeholder="Re-Enter Email" onkeyup= "recheck('recheckEmail')" onblur= "recheck('recheckEmail')"/>
				<div class="err_msg" id = "recheckEmail"></div>
				
				<input id="pass" type="password" name="password" placeholder="Choose Your Password" onkeyup="pstatus('pass_result');" onblur= "pcheck('pass','wpass');"/>
				<div id = "pass_result" class ="status"></div>
				<div class = "err_msg" id="wpass"></div>
				
				<input id="repass" type="password" name="repassword" placeholder="Confirm Password" onblur= "rpcheck('re_pass_result');"/>
				<div class = "err_msg" id="re_pass_result"></div>
				
				<input type="text" id = "phoneno" name="phoneno" placeholder="Mobile No(10 digit)" onblur = "mbcheck('mobile_err')"/>
				<div id = "mobile_err" class="err_msg"></div>
				
				</div>
				<div id="userdataAddress">
				<p style ="font-size:20px; margin-left : 7px;" > Address</p>
				<textarea id="address" name="Address" rows = "3" cols="45"  placeholder="Address:building,street,city,State,Country,PIN Code" onblur= "adcheck('address_err');"></textarea>
				</div>
				<div id="address_err" class="err_msg"></div>
				
				<div id="gender">
				<p style ="font-size:20px; margin :10px;" > Gender</p>
				<ul class= "horizontal-list" style = "margin : 10px">
				<li><input type="radio" id = "sex" name="sex" value="Male" checked>Male</li>
				<li><input type="radio" id= "sex" name="sex" value="Female">Female</li>
				</ul>
				</div>
				<br>
				<div id="dobn"> Date Of Birth</div>
				<div id="dob">
				<script>
				date_of_birth();
				</script>
				<select name ="Month">
					<option value = "01">jan</option>
					<option value = "02">Feb</option>
					<option value = "03">Mar</option>
					<option value = "04">Apr</option>
					<option value = "05">May</option>
					<option value = "06">Jun</option>
					<option value = "07">Jul</option>
					<option value = "08">Aug</option>
					<option value = "09">Sep</option>
					<option value = "10">Oct</option>
					<option value = "11">Nov</option>
					<option value = "12">Dec</option>
					</select>
				<script>
				year_of_birth();
				</script>
				</div>
				<input name="submit" type="image" src="images/signup.png"  value="Sign Up" />
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
	  <li style="border-left: medium none;"><a href="Home.html"><span>Home</span></a></li>
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