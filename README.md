# Socket Programming in C

## Table of Contents

* [_Introduction_](#introduction)
	* [_Concepts & Terminology_](#concept-&-terminology)
## Introduction

### Concepts & Terminology

Before I get into ***sockets*** it is good to know the many concepts that are mentioned such as acrynoms or other descriptions. Here I am highlighting the items below to refer back to if it has been mentioned. I am a person when reading and understanding concepts I can visualize the concept better in my head if I knew the actually name rather than the acrynom. All the following terms I just searched up and basically copied and pasted just to have these concepts in a centralized location. If interested go right and head read/search more on the following concepts to get a better understanding.

* **IP (Internet Protocol)** -
	* **IPv4 (Internet Protocol version 4)** -
	* **IPv6 (Internet Protocol version 6)** -
* **TCP (Transport Control Protocol)** -
* **UDP (User Datagram Protocol)** -
* **Ports** -
* **TCP/IP** -
* **BSD (Berkeley Software Distribution)** -
* **Internet Domain (INET)** -
* **IPC (Inter-Process Communication)** -
* **Big-Endian** -
* **Little-Endian** -

### Sockets

What is a ***socket***?

A ***socket*** is a way to communicate between two programs via software, either on the same machine or across a network. It is a way of opening up a _door_ to recieve and send data through inter-process communication (IPC) and network interactions.

The sockets API in was released in 1983 under the 4.2BSD (_Berkeley Software Distribution_) Unix operating system.

The two main type of ***Socket*** domains are:

1.) ***Internet Domain (INET)*** - Used to communicate between process on different systems connected over a network via TCP/IP. Utilizes the 2 main IPs, `ipv4` 32 bit and `ipv6` 128 bit.

2.) ***Unix DOMAIN (Unix)*** - 

Types of ***Sockets***:

1.) ***Stream Sockets (SOCK_STREAM)*** - Provide a reliable, connection-oriented communication channel, ensuring that data is delivered in order and without dupliocation. Usually used with `TCP`. 

2.) ***Datagram Sockets (SOCK_DGRAM)*** - Offer a connectionless communication method, faster but does not guarantee order or delivery. Usually used with `UDP`. 

3.) ***Raw Sockets (SOCK_RAW)*** - Provides access to low-level network protocols and interfaces. Ordinary user programs usually have no need to use this style. 

## Roles Client/Server


**Server** - Sends data to your computer over the internet.

Role of **Server**:

* Provides the data
* Waits for incoming connections

**Client** - Sends and recieves data from the server.

Role of **Client**:

* Initiates the connection
* Identifies the server by name or IP address, and a port

Purpose of client/server model is to exchange information.

## TCP Client

Functions used:

`getaddrinfo()`

`socket()`

`connect()`

`send()`

`recv()`

`close()`
