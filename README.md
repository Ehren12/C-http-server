# HTTP Server in C 🖥️
Very small flask-like HTTP web framework/server for serving static files

### Inspiration 💤
I had just finished the last lecture in the [Harvard's CS50x Course](https://pll.harvard.edu/course/cs50-introduction-computer-science)
and the obvious next step was the final project 😱 This was the moment I've been waiting for! I wanted to build something that was technical enough so I could showcase all the skillsets I acquired throughout the course 🛣️ I also wanted a project that would help me learn more concepts and technologies. At first, I wanted to build a web app with Sveltekit (Javascript framework) and Firebase (Backend as a Service) 🔥However, I was already familiar with these technologies and it wouldn't have taken me out of my comfort zone, so I decided to build something with C.

### About my C web-server
If you actually took the time to read my `inspiration`, thanks 💖
However, It's time to talk about the actual project!
 This project was aimed at building a basic implementation of an HTTP server and possibly evolving to be C's version of the Flask framework in Python (at least that's the end goal) 🛣️
 
 The idea is simple...
 Build an HTTP server that can serve static files 🗃️
 
This involved building sockets, working with child processes, some string manipulation, and Binary Search Trees (for routing) 👷‍♂️
 
If you want to play around with the project first clone the repository and make sure you have GCC installed on your machine.

Clone repo

    https://github.com/Ehren12/C-http-server.git
 After cloning the repo you can very simply run 🐥
 

    make

After running the above you can now use the server by running

    ./server PORT
Replace PORT with your desired port 🔁
> 🔴It is required you are on Linux or at least running WSL2 on Windows. If you're on Mac, it may work, not entirely sure...


‼️‼️ Be sure to put all the static files you want to serve in the view folder

The project is open for any contributions and It will mean a lot to me if you decide to contribute or even open an issue ✍️

HAPPY HACKING ✨

 

