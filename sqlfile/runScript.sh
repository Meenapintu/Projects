#!/usr/bin/expect -f
password="dbproject"
psql -h localhost -U dbproject -f drop.sql postgres
psql -h localhost -U dbproject -f types.sql postgres
psql -h localhost -U dbproject -f tables.sql postgres
psql -h localhost -U dbproject -f triggers.sql postgres
psql -h localhost -U dbproject -f insertMembers.sql postgres
psql -h localhost -U dbproject -f insertEmployee.sql postgres
psql -h localhost -U dbproject -f insertPost.sql postgres
psql -h localhost -U dbproject -f insertBook.sql postgres
psql -h localhost -U dbproject -f insertBorrow4M.sql postgres
psql -h localhost -U dbproject -f insertBorrow4E.sql postgres
psql -h localhost -U dbproject -f insertRHI.sql postgres