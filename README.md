# Socket Programming in C

### Sockets

What is a ***socket***?

A ***socket*** is a way to communicate between two programs via software, either on the same machine or across a network.

The sockets API in was released in 1983 under the 4.2BSD (_Berkeley Software Distribution_) Unix operating system.

The two main type of ***Socket*** domains are:

1.) ***Internet Domain (INET)*** - Used to communicate between process on different systems connected over a network via TCP/IP. Utilizes the 2 main IPs, `ipv4` 32 bit and `ipv6` 128 bit.

2.) ***Unix DOMAIN (Unix)*** - 

Types of ***Sockets***:

1.) ***Stream Sockets (SOCK_STREAM)*** - Provide a reliable, connection-oriented communication channel, ensuring that data is delivered in order and without dupliocation. Usually used with `TCP`. 

2.) ***Datagram Sockets (SOCK_DGRAM)*** - Offer a connectionless communication method, faster but does not guarantee order or delivery. Usually used with `UDP`. 

3.) ***Raw Sockets (SOCK_RAW)*** - Provides access to low-level network protocols and interfaces. Ordinary user programs usually have no need to use this style. 

**Server** - Provides a specific service depending to the client request, usually over the internet

**Client** - Request services or reasource from a server over a network.

### Requirements

The purpose of this service is to send a request (`client`) and the (`server`) will respond accordingly to what is needs.

The server will be sitting on the Raspberry Pi IPv4 address, using TCP and and its own port 8080.

> [!NOTE]
> To ensure the IP address does not change, be sure to set the RPi address statically. I achieved this by changing my DHCP settings to a specific range and leaving a section of host unused.
