# ENGR 103: Dictionary

## Getting Started

To begin the programming portion of this assignment, follow these steps:

1. **Clone the Repository**:
   - Copy the SSH URL of this repository by clicking the "<> Code" button at the top right of the main page, selecting the "SSH" tab, and clicking the clipboard icon.
   - Open your terminal and connect to the ENGR servers via SSH.
   - Navigate to the directory where you want to store your assignment work using commands like `ls`, `pwd`, and `cd`.
   - Execute `git clone <SSH_connection_string>`.

2. **Navigate to the Repository**:
   - Execute `ls` to confirm the presence of a new directory named `dictionary-XXX`, where `XXX` is your GitHub username.
   - Enter that directory using `cd dictionary-XXX`.
   - Execute `ls` again to see this file (`README.md`) along with the starter code.
   - Use `vim` to edit and complete the starter code according to the assignment requirements.

## Dependencies

This program communicates with the [Free Dictionary API](https://dictionaryapi.dev/) to retrieve definitions, audio URLs, and pronunciations of words.

The [cpp-httplib](https://github.com/yhirose/cpp-httplib) library is used to communicate with the API, and the [json11](https://github.com/dropbox/json11) library is used to parse its JSON responses. See the "Building" section below for details on how to build your program with these dependencies in mind.

## What to Complete

The only file you'll need to modify is `main.cpp`. It contains a lot of starter code, including functions that you'll use to interface with the dictionary API (each starting with the prefix `query_`). Read the documentation (block comments) supplied for these functions very carefully along with the commented-out example calls in the skeleton `main()` function. Make sure you understand how to use them. Then, complete the program as described in the assignment document.

## Building the Program

To build your program, you must compile your code (`main.cpp`) with `json11`'s code (`cpp-httplib` is "header-only", which means that it doesn't need to be compiled manually). You must also link your program against the `ssl` and `crypto` DLLs (you don't have to understand what this means---they're just other system-installed libraries that `cpp-httplib` depends on).

In practice, this means that you should always compile this program with:

```bash
g++ -o dictionary json11/json11.cpp main.cpp -lssl -lcrypto
```

As you know, `-o dictionary` just sets the name of the built executable to be "dictionary". You can change that part to whatever you want.

One option is to run the above command manually from the terminal. However, an alternative, simpler option is to use the GNU make utility. This starter code contains something called a Makefile. This Makefile is configured to execute the above command when you use it. To use the Makefile, simply run the following terminal command from the project directory:

```bash
make
```

Again, this will build the executable and name it "dictionary". Feel free to modify the EXE constant in the Makefile to change the executable name to whatever you'd like.

Note that the `make` command won't do anything if you haven't modified `main.cpp` since you last built your executable.

## Running the Program

When you run your program, know that the `query_XXX` functions provided in the template code can sometimes be a bit slow since they have to communicate with the dictionary API over the internet. In addition, the dictionary API server occasionally goes down momentarily, perhaps for routine maintenance, which will cause these functions to fail in somewhat unpredictable ways. For example, it's been observed that the API will sometimes stop working at exactly noon (PST) and start working again within a minute or two.

## Submitting your work

As you work on your assignment, frequently test your code and create new commits to reflect small changes. This practice allows you to rollback changes if needed.

To submit your work:

1. Ensure all your work is committed (`git status` should show no modified or staged files).
2. Push your commits to this GitHub repository using `git push`.
3. Open this repository in your browser, refresh the page, and verify it contains your latest work.

The instructor and TAs have access to this GitHub repository. As long as your work is pushed here, it is considered submitted. The timestamp of your latest commit will be checked to determine if it is a late submission.
