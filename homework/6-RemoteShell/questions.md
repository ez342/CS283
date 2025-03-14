1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_answer here_ The remote client determines when a command's output is fully recieved from the server by checking with the EOF marker. In our case, our eof marker was the null terminator and it is defined as RDSH\_EOF\_CHAR. Once the client side reads the data chunks and finds the eof marker, then it knows that it has read the entire message. In order to handle partial reads or to ensure a complete message transmission, we can keep track of the bytes received from recv() in a while loop. This would continue until the max size of the buffer has been hit or until the eof marker has been found. 

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_answer here_ A networked shell should detect the star/end of the command using a marker over the TCP connection. Usually you can use newline or the null terminator. If we do not handle this correctly, then the command could possibly be broken up, leading to a scenario where the command would not be parsed or read correctly.  

3. Describe the general differences between stateful and stateless protocols.

_answer here_ Stateless protocols are network protocols where the client sends requests to the server and the server responds accordingly based on the current state. This means that each request is treated independently and the server does not store any state information between requests. On the other hand, for stateful protocols, when a client sends a request to the server, it expects a response, and resends the request if it does not get a response. This means that it keeps track of the state information vetween requests. Based on these behaviors, this also means that stateful protocol would require more space and resources to store and manage the state, unlike stateless. Thus, stateful protocol is more complex than stateless as it needs to manage the information between each request.  

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_answer here_ Even though UDP is considered "unreliable", it has its advantages such as lower overhead, meaning it does not require a connection setup, allowing it to have a lower complexity, leading to faster runtime. Due to these advantages, it can be used for streaming media, real-time applications, and etc. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_answer here_ The interface/abstraction that is provided by the operating system to enable applications to use network communications is sockets. Sockets are the endpoints where a two way communication between two programs and information are passed in from. Once the data has pass through the socket, it will be broken down to access the header of the message and which then allows it to understand which port the message should go through. After that, the router will route the message to go to the designated ip address for the device to receive its message. 
