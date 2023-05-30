//Stephen Mullen
//sjm210002

NOTE: SERVER IS CURRENTLY SET TO RUN ON CS3 USING 10.176.92.17 : TO CHANGE IP, CHANGE CODE IN ECHO_S 

To run:
./log_s &
./echo_s port# optionalport1 optionalport2
./echo_c (t or u for tcp or udp) ip port
message

Examples:
{cslinux1:~/learn/assg/assg6} g++ -o log_s log_s.cpp
{cslinux1:~/learn/assg/assg6} g++ -o echo_s echo_s.cpp
{cslinux1:~/learn/assg/assg6} g++ -o echo_c echo_c.cpp
{cslinux1:~/learn/assg/assg6} ./log_s &
[1] 32700
{cslinux1:~/learn/assg/assg6} ./echo_s 8189 &
[2] 32792
{cslinux1:~/learn/assg/assg6} ./echo_c u 10.176.92.15 8189
Socket successfully created..
Enter your message:
yo bro

Message from UDP client: yo bro

Server:yo bro

Message sent to server: yo bro
{cslinux1:~/learn/assg/assg6} cat echo.log
2023-04-19 13:59:38: yo bro was received from 10.176.92.15
[2]+  Done                    ./echo_s 8189
{cslinux1:~/learn/assg/assg6}

-------------------------------------------------------------------

{cslinux3:~/learn/assg/assg6} ./echo_s 8189 8190 8191 &
[2] 32813
{cslinux3:~/learn/assg/assg6} ./echo_c u 10.176.92.17 8190
Socket successfully created..
Enter your message: Hey
Message from UDP client: Hey
Server:Hey

Message sent to server: Hey

--------------------------------------------------------------------

{cslinux3:~/learn/assg/assg6} ps
  PID TTY          TIME CMD
  13580 pts/2    00:00:00 bash
  15161 pts/2    00:00:00 log_s
  32813 pts/2    00:00:00 echo_s
  32814 pts/2    00:00:00 echo_s
  32815 pts/2    00:00:00 echo_s <defunct>
  34087 pts/2    00:00:00 ps
  {cslinux3:~/learn/assg/assg6} ./echo_c t 10.176.92.17 8191
  Socket successfully created..
  connected to the server..
  Enter your message: Hello
  From Server : Hello
  Message sent to log server: Hello
  Enter your message: Goodbye
  Message sent to log server: Goodbye
  From Server : Goodbye
  Enter your message: exit
  Client Exit...
  Message sent to log server: exit
  {cslinux3:~/learn/assg/assg6}
-------------------------------------------------------------------------

{cslinux3:~/learn/assg/assg6} ./log_s &
[1] 40015
{cslinux3:~/learn/assg/assg6} ./echo_s 8198 &
[2] 40148
{cslinux3:~/learn/assg/assg6} ./echo_c t 10.176.92.17 8198
Socket successfully created..
connected to the server..
Enter your message: First line
From Server : First line
Message sent to log server: First line
Enter your message: Second line
Message sent to log server:  Second line
From Server :  Second line
Enter your message: Third line
From Server : Third line
Message sent to log server: Third line
Enter your message: exit
Client Exit...
Message sent to log server: exit
{cslinux3:~/learn/assg/assg6} cat echo.log
2023-04-21 12:51:02: First line was received from 10.176.92.17
2023-04-21 12:51:08: Second line was received from 10.176.92.17
2023-04-21 12:51:10: Third line was received from 10.176.92.17
2023-04-21 12:51:13: exit was received from 10.176.92.17
[2]+  Exit 1                  ./echo_s 8198
{cslinux3:~/learn/assg/assg6}
