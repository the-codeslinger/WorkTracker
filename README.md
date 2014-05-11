WorkTracker
===========

WorkTracker helps record the amount of time you've spent on different tasks throughout the day. 

![Main UI](https://raw.githubusercontent.com/drunk-on-pain/WorkTracker/master/img/Main%20UI.png)

By starting a new "workday" you can add several tasks you've performed and at the end of the day, let WorkTracker show you a summary of how long you have worked on your tasks. If you're in a working environment with a lot of interruptions then WorkTracker can help you as well. All you need to do is to diligently start and stop the tasks you're performing and WorkTracker adds them all together.

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

## License

The source code and binary files are distributed under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0).

## Binaries

The latest version for Windows can be found on [Sourceforge](https://sourceforge.net/projects/tnworktracker/files/) as a 7-Zip archive. Copy the contents of the archive to a location on your computer (you can use a USB flash drive if you like) and you are good to go.

## Supported Platforms

The application is able to run on all platforms that are supported by Qt 5.2. Development takes place on Windows. An early version of the application has been successfuly compiled and run on Linux, Mac OS X should be fine as well.

## Database File

All data is written to and read from an XML file that is located in the current user's `AppData` folder on Windows, e.g. `C:\Users\Robert\AppData\Local\Typical Nerd\WorkTracker\Database.xml`. 