# Socket Programming in C

## Project Statement

I found myself working between different devices under my network. I currently have my desktop and two Raspbery Pis that I tend to transfer different files from one play to another via using `scp`. With this project, I wanted to challenge myself in learning networking programming on a lower level. While there are many protocols and services to transfer files from one device to another `scp` `ftp` `ftps`... etc. I thought this would be a great learning experience to create my own file transfer program in C.

After gaining knowledge on the Linux OS and intercommunication that is happening between embedded devices on my current job, I thought this would a great way to test my skills but also strengthen my ability to understand networking and systems on a lower level.

## Design Philosophy

Since I have been working with Unix based systems, I wanted to design this program to be as POSIX (Portable Operating System Interface) compliant as possible. Emphasising on portablility.

As far as the network stack, it will consist of using **TCP** and only support IPv4... but with a couple tweeks it could also support IPv6. The purpose of using **TCP** is to ensure the proper data is transfered over the network with consistent reliability. 

Although encryption is essential when sending data over the internet for security and privacy, this program will not include it as the transfers will partake within my LAN (local area network).

## Sockets Background

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

**Client** - Request services or resources from a server over a network.

## Requirements

To run the following program a linux environment is needed. I have tested this on WSL (Windows Subsystem for Linux) and Debian-based Linux distributions such as Ubuntu and Raspbian (Raspberry Pi OS).

### Environment Setup

Methods:

WSL 

To install WSL on Windows open up the command prompt and type:
```
wsl --install
```
set up password and other installation requirements as prompted.

Once installed, just type wsl on the command prompt and it will open the WSL terminal.

> [!NOTE]
> Also just use `window + r` and type wsl to open it.

Ubuntu

Raspbian

## Initial Testing

The [version_0](src/Version_0/my_networking.c) was to create the bare skeleteon of the program itself to check for any error handling and to validate if the user input arguments are within the lengths of IPv4 and not utilizing any of the known ports below 1024. 

Although it does not catch proper formatting when entering the first command line argument for IPv4 in the form for of:

- 127.0.0.1
- 192.xxx.xxx.xxx
- 172.xxx.xxx.xxx
- 10.xxx.xxx.xxx

It does have a min and max value of the string length that the user can input for example:

- IP_MIN_LEN 10.0.0.0 = 8 + `'\0'` = 9
- IP_MAX_LEN 192.xxx.xxx.xxx = 15 + `'\0'` = 16

> [!NOTE]
> See [my_network.h](src/Version_0/my_networking.h) for defined macros.

My initial testing consist of:

- Proper command line 
See source code [version](src/Version_0/my_networking.c)
