package signup;

import java.io.IOException;
import java.sql.Array;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 * Servlet implementation class RegisterNewUser
 */
@WebServlet("/RegisterNewUser")
public class RegisterNewUser extends HttpServlet {
		/**
	 * Servlet implementation class RgisterNewUser
	 */
			Connection conn1 =null;
			Statement st =null;
			PreparedStatement prest;
			ResultSet result = null;
			public void init() throws ServletException {
			      //Open the connection here
				String dbURL2 = "jdbc:postgresql://localhost/pintulal";
		        String user = "pintulal";
		        String pass = "pintulal";
	
			      try {
						Class.forName("org.postgresql.Driver");
					
						conn1 = DriverManager.getConnection(dbURL2, user, pass);
						st = conn1.createStatement();
						System.out.println("connection established :::: "+conn1);
			      	} catch (Exception e) {
						// TODO Auto-generated catch block
			      		e.printStackTrace();
			      	}
			   }
			
			 public void destroy() {
			     //Close the connection here
			    	try{
			    		conn1.close();
			    		System.out.println("connection closed");
			    	}catch(Exception e)
			    	{
			    		System.out.println(e);
			    	}
			    }
	       
	    /**
	     * @see HttpServlet#HttpServlet()
	     
	    public RegisterNewUser() {
	        super();
	        // TODO Auto-generated constructor stub
	    }*/

		/**
		 * @throws SQLException 
		 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
		 */
	 private Connection connect(){
		    String dbURL2 = "jdbc:postgresql://localhost/pintulal";
		    String user = "pintulal";
		   String pass = "pintulal";
		
		     try {
			     Class.forName("org.postgresql.Driver");
				 conn1 = DriverManager.getConnection(dbURL2, user, pass);
				 st = conn1.createStatement();
				 System.out.println("connection established :::: "+conn1);
				      	} catch (Exception e) {
							// TODO Auto-generated catch block
				      		e.printStackTrace();
				      	}
				return conn1;
			  }
		public ResultSet userinfo(String Uname,String table) {
			conn1 =connect();
			try {
				prest = conn1.prepareStatement("SELECT * FROM "+table+" WHERE UserID = ?");
				prest.setString(1, Uname);
				result =prest.executeQuery();
				System.out.println(prest);
			} catch (SQLException e) {
				e.printStackTrace();
			}
			return result;
		}
		public ResultSet claim(String Uname) {
			conn1 =connect();
			try {
				prest = conn1.prepareStatement("SELECT * FROM claim WHERE UserID = ?");
				prest.setString(1, Uname);
				result =prest.executeQuery();
				System.out.println(prest);
			} catch (SQLException e) {
				e.printStackTrace();
			}
			return result;
		}
		public ResultSet Borrow(String Uname,String table){
			conn1 =connect();
			try {
				prest = conn1.prepareStatement("SELECT "+table+".bookid,title,borrowdate,returndate FROM "+table+" inner join booksinfo ON ("+table+".bookid = booksinfo.bookid) WHERE UserID = ?");
				prest.setString(1, Uname);
				result =prest.executeQuery();
			} catch (SQLException e) {
				e.printStackTrace();
			}
			return result;
		}
		
		public ResultSet searchresult(String material,String searchby,String with,String searchString)
		 {
			 conn1 = connect();
			 String Query = "SELECT * from BooksInfo where " ;
			 ResultSet rs = null;
			 String[] lists = searchString.split(" ");
			 int length = lists.length;
			 String newSS=lists[0];
			 for(int i=1;i<length;i++)
				 newSS=newSS+" "+lists[i];
			
			 switch(with)
			 {
			 case("Words"): 
			 				for (int i=0;i<length-1;i++)
			 				{
			 					Query = Query + searchby +" like '%"+lists[i]+"%' or ";
			 				}
			 				Query = Query + searchby + " like '%" +lists[length-1]+"%'";
			 				System.out.println(Query);
							try {
								rs = st.executeQuery(Query);
							} catch (SQLException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							} break;
			 case("AllWords"):
								for (int i=0;i<length-1;i++)
								{
									Query = Query + searchby +" like '%"+lists[i]+"%' and ";
								}
								Query = Query + searchby + " like '%" +lists[length-1]+"%'";
								System.out.println(Query);
								try {
									rs = st.executeQuery(Query);
								} catch (SQLException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								} break;
			 case("Embeded"):    
				 				Query = Query + searchby + " like '%" +newSS + "%'";
			 					System.out.println(Query);
								 try {
										rs = st.executeQuery(Query);
									} catch (SQLException e) {
										// TODO Auto-generated catch block
										e.printStackTrace();
									} break;
			 case("Exact"): Query = Query + searchby + " = '"+ newSS +"'" ;
			 							System.out.println(Query);
										 try {
												rs = st.executeQuery(Query);
											} catch (SQLException e) {
												// TODO Auto-generated catch block
												e.printStackTrace();
											} break;
			default : break;
			 }	
			 
			 return rs;
		 }
		
		public ResultSet getposts()
		{
			ResultSet rs = null;
			conn1 = connect();
			try {
				rs = st.executeQuery("SELECT * FROM discussionforuminfo order by dateofpost DESC limit 40");
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return rs;
		}
		@SuppressWarnings("deprecation")
		protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
			
			HttpSession session = null;
			session = request.getSession(true);
			String Isform = request.getParameter("Isform");
			System.out.println(Isform);
			if(Isform.equals("Register")){
			String firstname=request.getParameter("firstname");
			String middlename=request.getParameter("middlename");
			String lastname=request.getParameter("lastname");
			String Username=request.getParameter("username");
			String email=request.getParameter("email");
			String password = request.getParameter("password");
			String phoneno = request.getParameter("phoneno");
			String Address = request.getParameter("Address");
			String gender=request.getParameter("sex");
			String member_type=request.getParameter("member_type");
			String Dob = request.getParameter("Year")+"-"+request.getParameter("Month")+"-"+request.getParameter("Date");

			try{                   	  
          	  st.executeUpdate("INSERT INTO membersinfo VALUES('"+ Username +"','"+ password +"',row('"+ firstname  +"','" + middlename + 
          			  					"','"+ lastname + "'),'"+ email  +"','','"+Dob+"','"+ Address   +"','',0,'"+ phoneno  +"','"+ gender+"')");
          	  response.sendRedirect("signup_result.jsp?status= Registration sucessfull !!!!");
            }catch (SQLException ex) {
                ex.printStackTrace();
                response.sendRedirect("Register.jsp?err_msg= username already exists ..");
                }
			}
			//==else ===============next else if condition 
			//System.out.println("----------------------------------------------------");
			else if(Isform.equals("Login")){
				System.out.println("----------------------------------------------------");
				String Uname=request.getParameter("uname");
				String pword = request.getParameter("pword");
				String user_type = request.getParameter("user_type");
				String tablename;
				if(user_type.equals("membersinfo"))
				{
					tablename = "borrowinfoformember";
				}else{tablename = "borrowinfoforemployee";}
				try {
					System.out.println(user_type+"----------------------------------------------------");
				//int result =	st.executeUpdate("SELECT UserID from membersinfo where UserID ='"+Uname+"' and Password ='"+pword+"'");
				// ResultSet result = st.executeQuery("SELECT UserID from membersinfo where UserID ='"+Uname+"' and Password ='"+pword+"'");
					PreparedStatement prest = conn1.prepareStatement("SELECT UserID FROM "+ user_type +" WHERE UserID = ? and Password = ?");
					prest.setString(1, Uname);
					prest.setString(2, pword);
					ResultSet result =prest.executeQuery();
					//prest.close();
					String name = null ;
					String page =null;
				 if(result.next()){
					  session= request.getSession(true);
					  name =result.getString(1); 
					 session.setAttribute("name",name);
					 session.setAttribute("user_type",user_type);
					 session.setAttribute("user_type1",tablename);
					 page = "Profile.jsp";
				  }
				 else{
					page = "signup_result.jsp?err_msg= Wrong username or Password";
				 }
				 System.out.println(name);
				// request.setAttribute("result",result);
				// getServletConfig().getServletContext().getRequestDispatcher("/Profile.jsp").forward(request, response);
				//// System.out.println(name);
				 response.sendRedirect(page);
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					 response.sendRedirect("signup_result.jsp?err_msg= username or password wrong");
				}
			}
			else if(Isform.equals("search")){
				String material = request.getParameter("m_thing");
				String searchby = request.getParameter("m_field");
				String with = request.getParameter("m_match");
				String searchString = request.getParameter("searchstring");
				String[] lists = searchString.split(" ");
				 int length = lists.length;
				 System.out.println(length + "#####");
				  session = request.getSession(true);
				 if(length == 0)
				 {
					 
					 session.setAttribute("material", null);
					 session.setAttribute("searchby", null);
					 session.setAttribute("with", null);
					 session.setAttribute("searchString", null);
					 
				 }
				 else{
				session.setAttribute("material", material);
				session.setAttribute("searchby", searchby);
				session.setAttribute("with", with);
				session.setAttribute("searchString", searchString);
				 }
				 response.sendRedirect("search.jsp");
			}
			
			else if(Isform.equals("discuss")){
				String userid = request.getParameter("userid");
				String postcontent = request.getParameter("postcontent");
				String title = request.getParameter("title");
				  java.util.Date dNow = new java.util.Date();
			     // SimpleDateFormat ft = 
			     // new SimpleDateFormat ("yyyy-MM-dd hh:mm:ss");
			    //  Current Date: Sun 2004.07.18 at 04:14:09 PM PDT
			     // System.out.println("Current Date: " + ft.format(dNow));
				try {
					prest = conn1.prepareStatement("INSERT into discussionforuminfo values (?,?,?,?,?)");
					prest.setString(1,userid);
					prest.setString(2, "");
					prest.setString(3, postcontent);
					prest.setTimestamp(4,new java.sql.Timestamp(dNow.getTime()) );
					prest.setString(5, title);
					prest.executeUpdate();
					response.sendRedirect("DisscussionForum.jsp");
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					response.sendRedirect("DisscussionForum.jsp/err_msg=couldn't post");
				}
			}
			else if(Isform.equals("issuebook"))
			{   
				String userid = request.getParameter("userid");
				String bookid = request.getParameter("bookid");
				String tablename = null;
				if(session.getAttribute("user_type").toString().equals("employeeinfo"))
						{
							tablename = "borrowinfoforemployee";
						}
				else
				{
					tablename = "borrowinfoformember";
				}
			    Date dNow = new Date();
			      java.sql.Date date=  new java.sql.Date(dNow.getYear(),dNow.getMonth(),dNow.getDate());
			      java.sql.Date dexp=  new java.sql.Date(dNow.getYear(),dNow.getMonth()+1,dNow.getDate());
			      System.out.println(date);
				try {
					prest = conn1.prepareStatement("INSERT into " + tablename + " values (?,?,?,?)");
					prest.setString(1,bookid);
					prest.setString(2, userid);
					prest.setDate(3, date);
					prest.setDate(4, dexp);
					prest.executeUpdate();
					//prest = conn1.prepareStatement("UPDATE booksinfo set available = 'f' where bookid = ?");
					//prest.setString(1,bookid);
					//prest.executeUpdate();
					//System.out.println(new java.sql.Date(dNow.getTime()));
					System.out.println(date);
					response.sendRedirect("Profile.jsp");
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					
					e.printStackTrace();
					response.sendRedirect("somethingwrong.jsp");
				}
				
				
			}
			else if(Isform.equals("renew"))
			{
				String userid = request.getParameter("userid");
				String bookid = request.getParameter("bookid");
			    Date dNow = new Date();
			      java.sql.Date date=  new java.sql.Date(dNow.getYear(),dNow.getMonth(),dNow.getDate());
			      java.sql.Date dexp=  new java.sql.Date(dNow.getYear(),dNow.getMonth()+1,dNow.getDate());
			      System.out.println(date);
			      String tablename = null;
					if(session.getAttribute("user_type").toString().equals("employeeinfo"))
							{
								tablename = "borrowinfoforemployee";
							}
					else
					{
						tablename = "borrowinfoformember";
					}
				try {
					prest = conn1.prepareStatement("update  "+tablename+" set borrowdate = ? , returndate = ? where userid = ? and bookid = ?");
					
					prest.setDate(1, date);
					prest.setDate(2, dexp);
					prest.setString(3, userid);
					prest.setString(4,bookid);
					
					
					prest.executeUpdate();
					//prest = conn1.prepareStatement("UPDATE booksinfo set available = 'f' where bookid = ?");
					//prest.setString(1,bookid);
					//prest.executeUpdate();
					//System.out.println(new java.sql.Date(dNow.getTime()));
					System.out.println(date);
					response.sendRedirect("Profile.jsp");
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					response.sendRedirect("somethingwrong.jsp");
				}
				
				
			}
			else if(Isform.equals("returnbook"))
			{
				String bookid = request.getParameter("bookid");
				String user1_type = request.getParameter("user1_type");
			    Date dNow = new Date();
			    String tablename = null;
				if(user1_type.equals("staff"))
						{
							tablename = "borrowinfoforemployee";
						}
				else
				{
					tablename = "borrowinfoformember";
				}
			      java.sql.Date date=  new java.sql.Date(dNow.getYear(),dNow.getMonth(),dNow.getDate());
				try {
					prest = conn1.prepareStatement("select borrowdate,userid from "+tablename+" where bookid = ?");
					prest.setString(1, bookid);
					ResultSet rs = prest.executeQuery();
					
					if(rs.next())
					{
						java.sql.Date borrowdate = rs.getDate(1);
						String userid = rs.getString(2);
						prest = conn1.prepareStatement("delete from "+tablename+" where bookid = ?");
						prest.setString(1, bookid);
						prest.executeUpdate();
					//	System.out.println(rs);
						prest = conn1.prepareStatement("insert into returnhistoryinfo values (?,?,?,?)");
						prest.setString(1, bookid);
						prest.setString(2, userid);
						prest.setDate(3, date);
						prest.setDate(4, borrowdate);
						prest.executeUpdate();
					//	System.out.println(rs);
						//prest = conn1.prepareStatement("UPDATE booksinfo set available = 't' where bookid = ?");
						//prest.setString(1,bookid);
						//prest.executeUpdate();
					//	System.out.println(rs);
						
					}

					response.sendRedirect("Staff.jsp?msg=Book Returned");
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					response.sendRedirect("somethingwrong.jsp");
				}
				
				
			}
			else if (Isform.equals("claimbook")){
				String userid =request.getParameter("userid");
				String bookid=request.getParameter("bookid");
				try {
					prest = conn1.prepareStatement("insert into claim values (?,?,'20010-1-1')");
					prest.setString(1,bookid);
					prest.setString(2, userid);
					prest.executeUpdate();
					response.sendRedirect("Profile.jsp?msg=Book claimed");
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					response.sendRedirect("somethingwrong.jsp");
				}
			}
				
				else if (Isform.equals("rate")){
					String userid =request.getParameter("userid");
					String bookid=request.getParameter("bookid");
					int rate = Integer.parseInt(request.getParameter("rate"));
					String page =null;
					try {
						prest = conn1.prepareStatement("SELECT userid from rate where userid=? and bookid=?");
						prest.setString(1,userid);
						prest.setString(2, bookid);
						ResultSet rs = prest.executeQuery();
						if(rs.next()){
							System.out.println("===============1================");
							prest = conn1.prepareStatement("update  rate set rate=? where userid=? and bookid=?");
							prest.setInt(1, rate);
							prest.setString(2, userid);
							prest.setString(3,bookid);
							prest.executeUpdate();
							page="search.jsp?msg=book rated";
						}
						else{
							System.out.println("===============2================");
						prest = conn1.prepareStatement("insert into rate values(?,?,?)");
						prest.setString(1,bookid);
						prest.setString(2, userid);
						prest.setInt(3,rate);
						prest.executeUpdate();
						page="search.jsp?msg=couldn't rated";
						}
						response.sendRedirect(page);
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
						response.sendRedirect("somethingwrong.jsp");
					}
				
				
			}
			else if(Isform.equals("userinfo"))
			{
				String username = request.getParameter("userid");
				String user1_type = request.getParameter("user1_type");
				String tablename=null,t2;
				if(user1_type.equals("staff")) {tablename="employeeinfo";t2="borrowinfoforemployee";}
				else {tablename="membersinfo";t2="borrowinfoformember";}
				session.setAttribute("user1_type",tablename);
				session.setAttribute("user", username);
				session.setAttribute("user_type2", t2);
				response.sendRedirect("Staff.jsp");
			}
			else if(Isform.equals("insertbook")){
				String bookid =request.getParameter("bookid");
				String subject =request.getParameter("subject");
				String area =request.getParameter("area");
				String title =request.getParameter("title");
				String author =request.getParameter("authors");
				String publishers =request.getParameter("publishers");
				String edition =request.getParameter("edition");
				String isbn =request.getParameter("isbn");
				int price =Integer.parseInt(request.getParameter("price"));
				String shelfno =request.getParameter("shelfno");
				Date dNow = new Date();
			    java.sql.Date date=  new java.sql.Date(dNow.getYear(),dNow.getMonth(),dNow.getDate());
				boolean available =true;
				//String author ="rplmeena";
				String[] authorls=author.split(",");
				try {
				  //System.out.println("insert into booksinfo values(?,?,?,?, '"+conn1.createArrayOf("varchar",author)+"',?,?,?,?,?,' ',?,' ',?)");
					prest = conn1.prepareStatement("insert into BooksInfo values(?,?,?,?,array[?],?,?,?,?,?,array[]::reference[],?,'0.0',?);");
					prest.setString(1,bookid   );
					prest.setString(2, subject   );
					prest.setString(3,area       );
					prest.setString(4,title   );
					String authorstr="";
					for(int i=0; i<authorls.length-1; i++)
						authorstr=authorstr+"'"+authorls[i]+"',";
					authorstr=authorstr+"'"+authorls[authorls.length-1]+"'";
					System.out.println(authorstr);
					prest.setString(5,authorstr	);
					prest.setString(6, publishers );
					prest.setBoolean(7, available);
					prest.setString(8, edition      );
					prest.setDate(9, date);
					prest.setString(10, isbn         );
					prest.setInt(11, price);
					prest.setString(12, shelfno );
					prest.executeUpdate();
                    response.sendRedirect("insertbook.jsp?msg=Added a New Book");
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					response.sendRedirect("insertbook.jsp?msg=Could Not Add Book");
				}
				
				
			}
			else if(Isform.equals("logout"))
			{
				System.out.println(session.getAttribute("name"));
				session.setAttribute("name",null);
				System.out.println(session.getAttribute("name"));
				response.sendRedirect("signup_result.jsp?status= sucessfully logged out");
			}
			
		}
		

		
		/**
		 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
		 
		protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
			// TODO Auto-generated method stub
		}*/
	}
	

	
