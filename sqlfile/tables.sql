create table if not exists BooksInfo          -- data insertion over and table attributes finalised
(
	BookId varchar(30),
	Subject varchar(100),
	Area varchar(50),
	Title varchar(150) not null,
	Authors varchar(100)[] not null,
	Publishers varchar(150),
	Available bool check(Available=true or Available=false),
	Edition varchar(15),
	ArrivalDate date,
	ISBN varchar(30),
	SimilarBooks Reference[],
	Price numeric(4,0) check(price>0),
	Rate numeric(4,2) check(Rate<10),
	ShelfNo varchar(4),
	No_ratings integer default 0,
	primary key(BookId)
);

create table if not exists MembersInfo				--data insertion over and table attributes are finale			
(
	UserID	varchar(30),
	Password varchar(30) not null,
	Name AName not null,
	Email varchar(50) not null,
	Major varchar(50),
	Dob date,
	Address varchar(200),
	Image bytea,
	TotalFine numeric(3,0),
	ContactNo varchar(15),
	Gender varchar(8) check(Gender='Male' or Gender='Female'),
	primary key(UserID)
);

create table if not exists BorrowInfoForMember			-- insertion of data is over and table attributes finalised
(
	BookId varchar(30),
	UserID varchar(30),
	BorrowDate date,
	ReturnDate date,
	primary key(BookId),
	foreign key(BookId) references BooksInfo on delete cascade,
	foreign key(UserID) references MembersInfo on delete cascade
);

create table if not exists DiscussionForumInfo						--table entries finalised and insert data is over
(
	UserID varchar(30),
	Tag varchar(50),
	Post varchar(250),
	DateOfPost timestamp,
	Title varchar(100),
	foreign key(UserID) references MembersInfo on delete cascade
);

create table if not exists RecommendBookInfo
(
	UserID varchar(30),
	Title varchar(150) not null,
	Edition varchar(15) not null,
	Authors varchar(50)[] not null,
	Publishers varchar(100),
	Subject varchar(15),
	foreign key(UserID) references MembersInfo on delete cascade
);

create table if not exists EmployeeInfo				-- data insertion is over and table attributes are finale	   			
(
	UserID varchar(30),
	Password varchar(30) not null,
	Position varchar(100),
	Responsibility varchar(50)[] not null,
	Name AName not null,
	Email varchar(50) not null,
	Address varchar(200),
	Dob date,
	Image bytea,
	TotalFine numeric(3,0),
	ContactNo varchar(15),
	Gender varchar(8) check(Gender='Male' or Gender='Female'),
	primary key(UserID)		
);

create table if not exists BorrowInfoForEmployee		-- insertion of data is over and table attributes finalised
(
	BookId varchar(30),
	UserID varchar(30),
	BorrowDate date,
	ReturnDate date,
	primary key(BookId),
	foreign key(BookId) references BooksInfo on delete cascade,
	foreign key(UserID) references  EmployeeInfo on delete cascade
);

create table if not exists JournalsInfo	
(
	ID	varchar(30),
	Title varchar(150) not null,
	Subject varchar(100) not null,
	Authors varchar(100)[] not null,
	primary key(ID)
);

create table if not exists ReturnHistoryInfo
(
	BookId varchar(30),
	UserID varchar(30),
	BorrowDate date,
	ReturnDate date,
	primary key(BookId),
	foreign key(BookId) references BooksInfo on delete cascade
);

create table if not exists rate
(
	BookId varchar(30),
	UserID varchar(30),
	rate numeric(4,2) check(rate<10),
	primary key(BookID,UserID),
	foreign key(BookID) references BooksInfo on delete cascade,
	foreign key(UserID) references MembersInfo on delete cascade	
);

create table if not exists claim
(
	BookId varchar(30),
	UserID varchar(30),
	ClaimDate date,
	primary key(BookID,UserID),
	foreign key(BookID) references BooksInfo on delete cascade,
	foreign key(UserID) references MembersInfo on delete cascade	
);
