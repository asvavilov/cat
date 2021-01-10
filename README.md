UDP-server waiting incoming string more 1 symbol for PIN to HIGH, or 1 symbol for PIN to LOW.
PIN to LOW after LAST_TIMEOUT (ms).

Need copy "include/secure.example.h" to "include/secure.h" (and set real credentials) for compilation.

usage udp-client:
=================

test (shell/netcat):
--------------------
```
nc -u 192.168.esp.address 8888
```

test (bash):
------------
```
echo -n "This is my data " > /dev/udp/192.168.1.57/8888
```

test (php):
-----------
```
php udp.php 192.168.1.57 8888 "My Message"
```
