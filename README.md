WorkTracker
===========

WorkTracker helps record the amount of time you've spent on different tasks throughout the day. Since version 1.2.3 the application is supported on Windows and Linux.

![Main UI](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Main%20UI.png)

By starting a new "workday" you can add several tasks you've performed and at the end of the day, let WorkTracker show you a summary of how long you have worked on your tasks. If you're in a working environment with a lot of interruptions then WorkTracker can help you as well. All you need to do is to diligently start and stop the tasks you're performing and WorkTracker adds them all together. And if you ever run into the situation where you forgot to do this or couldn't do it, no worry, WorkTracker comes with an editor to help you out.

## Start Working

Getting started is simple, just click "Start New Workday" and you're good to go. It doesn't matter when a workday is started or stopped, only the individual tasks are summed up to create the total result. A "workday" is simply a container to accommodate for all the tasks.

Next, you click "Start Task" and enter a description for what you are about to do. Don't worry, this can be changed later, but it will be shown in the UI as a reminder. Previously used descriptions are presented and filtered as you type.

![Start Task](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Start%20Task.png)

Once you finished your work - or have been interrupted - you click "Stop Task", confirm the description or enter a new one, e.g. an identifier from an issue tracking system if you're a developer, and this part of the day is in the books.

You continue this procedure over and over until you decide (or company working hours dictate) to call it a day.

## Stop Working

To stop working you simply click "Stop Workday" and thus close the "container". Now you can let the application display the results by clicking on "Show Summary".

![Summary](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Summary.png)

In fact, the summary can be displayed at any time which then shows the status up until that point in the day.

## Edit After the Fact

WorkTracker features a simple editor with which you can edit previous entries or even create a whole day from scratch in case you haven't been able to click "Start" and "Stop" throughout the day.

First you have to select the day for which you want to edit task items. This page can be revisited as much as you want. Select a day, edit its items, go back to the first page and pick another day.

![Edit-Day](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Editor%20Page%201.png)

The second page presents you with the actual editor. On the left side there are the tasks and on the right side all the start and stop timestamps that have been recorded. Tasks can be renamed, removed or added however you like. Removing a task removes all the time spent as well. Modifying the timestamps is governed by the rule that there can only be one active task, i.e. a task with a set of timestamps where the "stop" part is missing.

![Edit-Tasks](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Editor%20Page%202.png)

Changes take immediate effect, no matter how the editor is closed. This allows the continuous back and forth between the selection of the day and editing the items of a day.

## Language

The application supports two languages, German and English (U.S. based, if any distinction is necessary). Changing the language immediately updates user-visible texts.

![Language](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Language.png)

## License

The source code and binary files are distributed under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0).

## Binaries

Binaries are provided for 64-bit versions of Windows 7 and 8 and since v1.2.3 for Debian flavored Linux distributions as well. You can download the 7z archive or deb files on the [releases][] page.

The application should be able to run on earlier version of Windows as well, given you are willing to compile it yourself (which, in theory, is quite easy). The mingw package should contain everything you need. Other operating systems like Mac OS X or non-Debian based Linux distributions should also work. Again, you'll need to compile the application yourself.

In the best case, all you need to do is the following.

    qmake WorkTracker.pro
    make 
        or 
    mingw32-make 
        or 
    nmake
    

## Supported Platforms

The application is able to run on all platforms that are supported by Qt 5.2. Development takes mainly place on Windows. The Linux version is compiled on Linux Mint 17.1.

Your compiler of choice should support C++11. The reference compiler used is Microsofts Visual Studio 2013 and GCC 4.8.2.

## Database File

All data is written to and read from an XML file that is located in the current user's `AppData` folder on Windows, e.g. `C:\Users\Robert\AppData\Local\Typical Nerd\WorkTracker\Database.xml`.

## Future Plans

*   Support global hotkeys
*   Hide UI in sys-tray and only show an input field for the task description on demand 

[releases]: https://github.com/drunk-on-pain/WorkTracker/releases
