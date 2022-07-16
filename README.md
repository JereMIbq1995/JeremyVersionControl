<h1>JVC (Jeremy's Version Control)</h1>
<br>
JVC is another crazy version control like git. It's very similar to git. But much simpler to use :) <br>

<h2>Compilation:</h2>

<b>Requirements:</b> <br>
&emsp; - g++ version 11.2.0 (This is the version I used to develop the program. Older or newer versions might work but no guarantee)

<b>Step 1:</b><br>
&emsp; Run command: <b>g++ main.cpp -o bin/jvc.exe</b><br>
&emsp; Once this command is ran, a bin folder in the project folder will be created<br>

<b>Step 2:</b><br>
&emsp; Add the full path of the "./bin" folder to the PATH variable of the environment <br>
&emsp; so that jvc can be used anywhere on the computer <br>

<b>Step 3:</b><br>
&emsp; Finally, open a test folder and test the 5 functionalities that JVC has: <b>init</b>, <b>status</b>, <b>save</b>, <b>revert</b>, and <b>history</b><br>


<h2>Usage:</h2>

<b> 1. init </b><br>
&emsp; From any folder, open the command line and run <b>jvc init</b>. This is to initialize a folder as a jvc repository <br>
and start tracking any changes made to the repository.<br>

<b> 2. status </b><br>
&emsp; Now, feel free to add, modify, or delete files and folders inside the repository. At any point, to see if there is any change <br>
to the repository compared to the last save, run command <b>jvc status</b>. Notice that the status functionality also show the version index<br>
of the current version to which changes are compared to.<br>

<b> 3. save </b><br>
&emsp; At any point, to save changes made to the repository, run <b>jvc save</b>. This functionality also takes in an optional parameter <br>
which is the message associated with the save. For example, you can run jvc save as followed: <br>
&emsp;&emsp; > <b>jvc save "Added a new text file"</b> <br>
&emsp; If the message is not provided, a default message will be used: "Version (new-version-index)"<br>

<b> 4. history </b><br>
&emsp; If you would like to see the save history of the repository, run <b>jvc history</b>, which shows the chronological order<br>
of all saves leading up to the current version. <br>

<b> 5. revert </b><br>
&emsp; If you would like to revert to a certain version in your save history, you can run <b>jvc revert</b>. For example, to revert to a version<br>
that has index '3', you would run the command:<br>
&emsp;&emsp; > <b>jvc revert 3 </b> <br>
&emsp; Notice that the version index is required to run this command. Without it, the command will not do anything and display a usage or <br>
an error message. It also is recommended that you use <b>jvc history</b> to look at the save history to familiarize yourself with the<br>
version index system before running the revert command.


<b> 6. ignore </b><br>
To specify which file should be ignored by jvc, create a ".jvcignore" file and put the paths of all to be ignored files or folders <br>
inside this file, sparated by the return character. <br>
This is very similar to ".gitignore"<br>
CAUTION: This feature is still a little immature. So to avoid bugs, please make sure that the format of the paths put inside .jvcignore <br>
are as simple and intiutive as possible :) <br>


<h1> Have fun! And thank you for checking out JVC! :)</h1>
