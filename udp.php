<?
/*
usage:
php upd.php UDP_IP UDP_PORT MESSAGE
example:
php udp.php 192.168.1.57 8888 "My Message"
*/

$server_ip = $_SERVER['argv'][1];
$server_port = $_SERVER['argv'][2];
$message = $_SERVER['argv'][3];

print "Sending message to IP $server_ip, port $server_port\n";
print $message."\n";
if ($socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP)) {
	socket_sendto($socket, $message, strlen($message), 0, $server_ip, $server_port);
	print('sended');
} else {
  print("can't create socket\n");
}
