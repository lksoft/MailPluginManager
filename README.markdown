## Mail Plugin Manager

![Mail Plugin Manager Icon][manager-icon]

Mail plugin developers have specific challenges when creating a product and we all share some similar tasks that we ought to handle similarly, in order to ensure that our customers all understand the issues and can deal with those as easily as possible. These include ensuring that their plugins are up-to-date, working around Mail's sandbox, getting crash reports and proper installation of the plugin.

The Mail Plugin Manager toolset helps to standardize these tasks and give the user some control over the plugin environment, while allowing the developer to focus on writing the product that they need.

### Main Features

* Mail Plugin Manager (for users)
* Installation
* Uninstallation
* Automatic Updates (without having to add a framework to your plugin)
* Ability to send crash reports back to developer
* Checks for compatibility of plugins at boot time (i.e. after an install)
* Determine relevant information about the user's system (Mail, Message, etc.)
* Allow the loading/unloading of LaunchAgents
* Keep updated list of OS versions, including future versions, that are accessible to plugins
* Preference migration into OS X sandbox for Mail
* Ensures that Mail has been launched at least once to ensure that all the directories and such are there
* Display list of plugins available on the internet that can be installed

The features provided are separated into 3 separate applications, a user facing app and a faceless app that always work together and the separate installer. In truth the faceless app (MailPluginTool), is not truly faceless as it does interact with the user, but it is a `NSUIElement = 1` application.

The MailPluginTool application is be embedded inside of the Mail Plugin Manager application. When plugins call it this will be done using a file-based mechanism in order to be compatible with the sandbox [defined below](#commands). A .h file will be available with **macros** defined for the calls to the MailPluginTool. It's done this way in order to avoid namespace conflicts between plugins using this code. [A List](#macros) is provided in these docs so you can see what you can do.

The Installer comes as a pre-built application that your build process will need to modify and resign, but there are scripts available to help you construct your complete installer.

This manager supports Lion, Mountain Lion & Mavericks.

### Why use this toolset?

1. It allows you to easily integrate Sparkle, which isn't difficult in and of itself, but for plugins it can be a problem, when other plugins also include Sparkle. This way you don't have to maintain an additional application to avoid those problems.
2. It gives you some tools to preemptively deal with Mail upgrades and UUID issues.
3. It provides debugging tools for your user, i.e. system info collection and crash reports.
4. It gives the user an easy way to manage the plugin.
5. It provides a simplified installer and uninstaller, again without you having to maintain another separate application.
6. It allows for the installation of plugins that have been made known to it as a way for promotion of a product.
7. Integration for you is simple and straightforward, with no other dependencies.
8. The more developers support Mail Plugin Manager, the more users will have a better Mail plugin experience.

#### Mail Plugin Manager Window

This interface is for the user to interact with and is the default mode when the Mail Plugin Manager is opened without a file. Here is an example of what it looks like:

![Mail Bundle Manager Window][manager-window]

#### Documentation

Visit the [wiki](wiki-link) for more information on the toolset.

---

Thanks to Fabian Jäger from [Chungwasoft](chungwasoft) for his contributions to Version 2.0.

© 2011-2014 Little Known Software, Inc.

---

### License

The Mail Plugin Manager has a very particular license. The code is not being made available as open source (though if you would like to contribute to the code, please contact me at scott@littleknownsoftware.com), but the executables *are* being made freely available for developers to use, with the following single condition:

* You may not resign the Mail Plugin Manager or MailPluginTool applications for any reason

The reason for this is complex, but one thing is sure, the app will terminate if the signature is not the original. The reason for this restriction is to avoid confusion for the end user. Since the application is a Developer ID signed application, if it is resigned with a different ID, then the updates will not work properly and the effectiveness of the app will be diminished tremendously. Thus to simply avoid these kinds of issues, the code has not been made available and the application validates that it is signed with a Developer ID from Little Known Software, Inc.

To be clear, this restriction does not apply to the Installer, in fact for it to work best for your installation, you should resign that application with your Developer ID.

Beyond this restriction, you are free to use the executables and their package contents and distribute it with your plugins as needed.

<!-- links -->
[chungwasoft]: http://chungwasoft.com
[wiki-link]: http://github.com/lksoft/MailPluginManager/wiki

<!-- images -->
[manager-window]: http://media.lksw.eu/mpm/Mail_Plugin_Manager.png
[manager-icon]: http://github.com/lksoft/MailPluginManager/raw/master/Images/ManagerIconMedium.png
