
function checkFName(id)
{
	var letters = /^[a-zA-Z]+$/;
	var firstname= signup.firstname.value;
	var err_msg = document.getElementById(id);
	err_msg.innerHTML = '';
		if (firstname == null || firstname == "")
		{
		err_msg.innerHTML = "First name Field can't be remain empty";
		return false;
		}
		else 
			{
			if( !(firstname.match(letters)))
			{
				err_msg.innerHTML = "Name must be consists of only letters :)";
				return false;
			}else return true;
			
			}		
}
function checkMName(id)
{
	var letters = /^[a-zA-Z]+$/;
	var middlename= signup.middlename.value;
			if( !(middlename.match(letters) || middlename == null || middlename == ""))
			{
				document.getElementById(id).innerHTML = "Invalid MiddleName : must be consists of only letters :)";
				return false;
			}
			else {document.getElementById(id).innerHTML = null;return true;}
					
}
function checkLName(id)
{
	var letters = /^[a-zA-Z]+$/;
	var lastname= signup.lastname.value;
			if( !(lastname.match(letters) || lastname == null || lastname == ""))
			{
				document.getElementById(id).innerHTML = "Invalid LastName : must be consists of only letters :)";
				return false;
			}
			else {document.getElementById(id).innerHTML = null;return true;}
					
}
function validateUsername(id)
{
	var letterNumber = /^[0-9a-zA-Z]+$/;
	username = signup.username.value;
	
	if (username == null || username == "")
	{
	document.getElementById(id).innerHTML = "user name  can't be remain empty";
	return false;
	}
	else 
		{
		if( !(username.match(letterNumber)))
		{
			document.getElementById(id).innerHTML = "username must be consists of only alphanumeric charecters ^..^";
			return false;
		}
		else {document.getElementById(id).innerHTML = null;return true;}
		}
}

function echeck(id)
{
	var email = document.forms["signup"]["email"].value;
	var validemail = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/;
	
	if (email == null || email == "")
	{
	document.getElementById(id).innerHTML = "email Addr. can't be remain empty";
	return false;
	}
	else 
		{
		if(!(validemail.test(email)))
		{
			document.getElementById(id).innerHTML = "invalid imail ...";
			return false;
		}
		else
			{
			document.getElementById(id).innerHTML = null;
			return true;
			}
		}
}
function recheck(id)
{
	var remail = document.forms["signup"]["remail"].value;
	var email = document.forms["signup"]["email"].value;
	if (remail != email)
		{
		document.getElementById(id).innerHTML = "Email Doesn't Match";
		return false;
		}
	else {document.getElementById(id).innerHTML = null;return true;}
		
}

function pstatus(id)
{
	var desc = new Array();
    desc[0] = "Very Weak";
    desc[1] = "Weak";
    desc[2] = "Better";
    desc[3] = "Medium";
    desc[4] = "Strong";
    desc[5] = "Strongest";
    txtpass = document.forms["signup"]["password"].value;
    var strengthMsg = document.getElementById(id);
    strengthMsg.innerHTML = "";
    var score   = 0;

    //if txtpass bigger than 6 give 1 point
    if (txtpass.length > 6) score++;

    //if txtpass has both lower and uppercase characters give 1 point
    if ( ( txtpass.match(/[a-z]/) ) && ( txtpass.match(/[A-Z]/) ) ) score++;

    //if txtpass has at least one number give 1 point
    if (txtpass.match(/\d+/)) score++;

    //if txtpass has at least one special caracther give 1 point
    if ( txtpass.match(/.[!,@,#,$,%,^,&,*,?,_,~,-,(,)]/) ) score++;

    //if txtpass bigger than 12 give another 1 point
    if (txtpass.length > 12) score++;

    strengthMsg.innerHTML = desc[score];
    
  }

function pcheck(id1,id2)
{
	errorMsg = document.getElementById(id2);
	errorMsg.innerHTML = "";
	document.getElementById(id1).className = "correct";
	var psswd = document.forms["signup"]["password"].value;
	if (psswd.length < 6)
    {
    errorMsg.innerHTML = "Password Should be Minimum 6 Characters" ;
    return false;
    document.getElementById(id1).className = "wrong";
    }
	else return true;
}

function rpcheck(id)
{
	var rpsswd = document.forms["signup"]["repassword"].value;
	var psswd = document.forms["signup"]["password"].value;
	var err_msg = document.getElementById(id);
	err_msg.innerHTML = '';
	if (rpsswd != psswd)
		{
		err_msg.innerHTML = "Password Doesn't Match";
		return false;
		}
	else return true;
}

function mbcheck(id)
{
	var phoneno = /^\d{10}$/;
	var phoneNo = document.forms["signup"]["phoneno"].value;
	var err_msg = document.getElementById(id);
	err_msg.innerHTML = '';
	if(phoneNo == null || phoneNo == "")
		{
		err_msg.innerHTML = "Enter Phone no.";
		return false;
		}
	else 
		{
		if(!(phoneNo.match(phoneno)))
			{
				err_msg.innerHTML = 'Invalid Phone no. plz enter your 10 digit ph. no.';
				return false;
			}
			else return true;
      }
}

function adcheck(id)
{
	var address = document.forms["signup"]["Address"].value;
	var err_msg = document.getElementById(id);
	err_msg.innerHTML = '';
	if(address == null || address == "")
		{
		err_msg.innerHTML = "Address Filed can't be Empty";
		return false;
		}
	else if(address.length <25 && address.legth >120)
		{
		err_msg.innerHTML = "Address must be between 25 - 120 charecters";
		return false;
		} else return true;
}
function validateForm()
{
	var t1,t2,t3,t4,t5,t6,t7,t8,t9,t10;
	t1 = checkFName('nameerror1');	
	t2=checkMName('nameerror2');
	t3=checkLName('nameerror3');
	t4=validateUsername('unerror');
	t5=echeck('eerror');
	t6=recheck('recheckEmail');
	t7=pcheck('pass','wpass');
	t8=rpcheck('re_pass_result');
	t9=mbcheck('mobile_err');
	t10=adcheck('address_err');
	return (t1 && t2 && t3 && t4 && t5 && t6 && t7 && t8 && t9 && t10);
}