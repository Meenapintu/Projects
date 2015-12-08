create type Reference as
(
	Title varchar(100),
	Author varchar(100)[],
	Edition varchar(20)
);

create type AName as
(
	FName varchar(50),
	MName varchar(50),
	LName varchar(50)
);
