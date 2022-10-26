all: mkdir date cat rm ls shell2

date : date.c
	gcc date.c -o date
mkdir : mkdir.c
	gcc mkdir.c -o mkdir
rm : rm.c
	gcc rm.c -o rm
shell2 : shell2.c
	gcc shell2.c -o shell2
ls : ls.c
	gcc ls.c -o ls
cat : cat.c
	gcc cat.c -o cat