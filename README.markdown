## Mail Plugin Manager

<div style="font-size:1.5em; color:red;">I have deleted and recreated this repository in order to ensure that the source code disappears, but not the executables. Please read below for more information.</div>

**This is to ensure some safety measures of maintaining a single executable that is used in public.**

I still intend for the executable and integration with it to be openly available for others to use, but want to ensure that we don't end up with duplicates signed accidentally (or intentionally) by other _Developer IDs_. The goal here is to ensure that Mail Plugin Manager has a single point of reference without getting updated in an incorrect way on the user's machine.

I am hoping that this will not discourage others from using the different executables for either a manager or even as the installer for you plugin, however I think that these steps are necessary to ensure accurate use. Although the code is no longer "**open source**", I definitely intend for the apps to be "**open executables**".

I will include more documentation on how to use the Mail Plugin Manager, MailPluginTool and Plugin Installer with your plugin in the next few weeks.

Mail Plugin Manager is a tool to help Mac Mail bundle authors manage, install & keep up to date for their users.

### Why use this?

1. This tool allows you to easily integrate Sparkle, which isn't difficult in and of itself, but for plugins it can be a problem, when other plugins also include Sparkle. This way you don't have to maintain an additional application to avoid those problems.
2. It gives you some tools to preemptively deal with Mail upgrades and UUID issues.
3. It provides debugging tools for your user, i.e. system info collection and crash reports.
4. It gives the user an easy way to manage the plugin.
5. It provides a simplified installer and uninstaller, again without you having to maintain another separate application. The installer also support Mountain Lion and migration of preferences into the sandbox.

### Main Features

* Installation
* Uninstallation
* Plugin Manager (for users)
* Automatic Updates (without having to add a framework to your plugin)
* Sends crash reports back to developer
* Checks for compatibility of plugins at boot time (i.e. after an install)
* Determine relevant information about the user's system (Mail, Message, etc.)
* Allow the loading/unloading of a LaunchAgent
* Keep updated list of OS versions, including future versions, that are accessible to plugins
* Preference migration into OS X sandbox for Mail.
* Ensures that Mail has been launched at least once to ensure that all the directories and such are there.

The features provided are separated into 2 separate applications, a user facing app and a faceless app. In truth the faceless app (MailPluginTool), is not truly faceless as it does interact with the user, but it is a `NSUIElement = 1` application. The feature split between the two apps should be obvious, but keep reading for more details.

The tool application will be embedded inside of the manager application. When plugins call it this will be done using a file-based mechanism in order to be compatible with Mountain Lion and the sandbox [defined below](#commands). A .h file will be available with **macros** defined for the calls to the tool. It's done this way in order to avoid namespace conflicts between plugins using this code. [A List](#macros) is provided in these docs so you can see what you can do.

This manager supports Snow Leopard, Lion & Mountain Lion.

<a name="manager"></a>
### Mail Plugin Manager(MPM)

It has the following main features:

1. Installation
2. Uninstallation (by user double-click)
3. Plugin Manager

#### Installation

A file package format has been defined (with an extension of `mpinstall`) that can be used to create a Mail Plugin specific installer that Mail Plugin Manager handles for you, very similar to Installer, but it is aware of the specific constraints of Mail Bundles and will install the Mail Plugin Manager as well, if desired.

A single plist manifest file ([see below](#manifest)) defines what will be installed, you can have other components besides just the plugin, and where they should go. You can define Release Notes sections (using either an rtf file or html) and a license agreement as well. The user is presented with a view of what will be installed, so that we are not hiding anything from them. When necessary, it will prompt for required authorization.

Here are images of a dummy installation:

![Installation Release Notes][install-1]

![Installation License Info][install-2]

![Installation License Agreement Dialog][install-3]

![Installation Review][install-4]

#### Uninstallation

Similarly a file package format has been defined (with an extension of `mpremove`) for an uninstall, so the user can double-click something to start an uninstall. It has an almost identical feature set to the uninstall and uses the same format for the manifest file ([see below](#manifest)). There is the ability to trigger an uninstall directly from your plugin as well if you want to have a button in your prefs to uninstall ([see tool](#tool)).


#### Mail Plugin Manager Window

This interface is for the user to interact with and is the default mode when the Mail Plugin Manager is opened without a file. Here is an example of what it looks like:

![Mail Bundle Manager Window][manager-window]

I have tried to add as much information about the plugin as I can get from the plugin itself. There are ways to add more detailed information to the `Info.plist` file so that MPM can provide a better experience, but it's good to try to show something.

The user can enable/disable, remove, update and change the domain of the plugin from this window. They can also click on the name to go to a product site and the company name to go to the company site.


<a name="tool"></a>
### MailPluginTool (MPT)

All interactions with MPT are done using a file-based mechanism and the Tool is run as a LaunchAgent.

It has the following main features:

1. Uninstallation (by plugin demand)
2. Automatic update checking (by plugin demand)
3. Send crash reports to developer (by plugin demand)
4. Check all plugins for compatibility/updates at boot time
5. Get relevant System information (by plugin request)
6. Get list of *past & future* compatibility information (by plugin request)

#### Uninstallation

A plugin will be able to request the uninstallation of itself. When this happens MPT will present a standard type of uninstall "Are you sure?" dialog and then proceed to uninstall and restart Mail.

#### Automatic update checking

A plugin can request an update check and ask that it happen regularly, using the path to the plugin to update. It will check (using Sparkle) to see if there is an update available and will let Sparkle progress through a normal update process if it is found.

<a name="crash-reports"></a>
#### Send crash reports to developer

Look for any crash reports for the plugin (whose path will have been passed in) and if found send these back to the developer, targeting some type of web service. It will send reports for both Mail and your particular plugin - up to the last 10 reports.

You will obviously need to setup a web service to accept the input from this and add that URL to the `Info.plist` file of your plugin. See the [Info.plist Keys](#infoplistkeys) section below about this.

The data is sent in a JSON package that contains the crash reports plus information about the version of Mail.app, Message.Framework, Mac OS X and the machine itself, plus a list of Mail plugins (both active and disabled).

An example php script to accept this data and load into a mysql database can be found in the [Github repository][crash-script-link]. It is based on the similar script written by Tom Harrington for the Sparkle feed values and in fact uses the `profileDB.php` and `profileConfig.php` from Tom's code, which you can [find here][sparkle-script]. In addition, it uses the php JSON extension to parse the data, which you'll need to ensure is included in your php installation (should be in version 5.2.0 or greater). For more information about that extension [see here][json-php]. Also included with my script is an sql script to create the database tables.

#### Check all plugins for compatibility/updates at boot

At boot time (though a Launch Agent) check for any incompatibilities or updates for existing plugins, either enabled or disabled.

This feature will be initiated by a Launch Agent setup for the user (which shouldn't need any special authorization) and will be launched each time the user logs in to a full session (excludes terminal sessions).

This mode will look at all plugins in both "Bundles" and "Bundles (Disabled[ X])" to find the latest ones and look to see if they have incompatibilities or updates and notify the user.

A dialog/window will be presented to the user to potentially do something. For a single item a window similar to the following will appear:

![Single Incompatible/Updatable Plugin Found][single-notice]

When there are more than one plugins that need the user's attention a different presentation is shown, similar to the Manager's look, like this:

![Multiple Incompatible/Updatable Plugins Found][multi-notice]

Through both of these the user can perform and Update, Disable a plugin, Remove it or ignore the message altogether.

I am thinking of also running it whenever the Bundles folder list changes as well, but not sure that would be a proper time, so I need to think about that one some more.

<a name="sys-info"></a>
#### Get relevant system information

This allows the calling bundle to get back a dictionary of information about the system, such as OS Version, Mail Version & UUID, Message.framework version & UUID.

<a name="uuid-list"></a>
#### Get list of *past & future* compatibility information

The tool will keep an updated version of a list of UUIDs that Mail and Message.framework have defined and supported for plugins to access, so they can actually test in *advance* if they will be compatible with an upcoming OS release. Or even just an OS release after the one the user has.

<a name="launch-agent"></a>
#### Load or Unload a LaunchAgent for a plugin

The tool will install the `.plist` into the User's LaunchAgent folder `~/Library/LaunchAgents` and load it (or unload and delete the `.plist` file) for a plugin. This allows the plugin to install a helper that can run outside the sandbox in order to perform some capabilities that it might currently be limited from doing.


<a name="manifest"></a>
### Manifest File Format

The manifest file, which should be named **`mpm-manifest.plist`** is a plist file that contains a description of the items to install or uninstall. It also is used to describe to the user what is being installed.

There are several keys at the top level of the plist that are used to configure the installer window and define information about what we are doing.

		manifest-type			[install/uninstall]						(String)
		display-name			Plugin Name								(String)
		background-image-path	path/to/image_to_show_in_installer		(String)
		completion-message		Text to show at completion.				(String)
		action-items			(see below)								(Array)
		confirmation-steps		(see below)								(Array)
		
Install specific (ignored during uninstall). These define the min and max versions of OS and min version of Mail supported. Pretty self-explanatory. The `configure-mail-min-bundle-version` key indicates the minimum bundle version value for configuring Mail. This key is required to have the installer configure Mail properly.
		
		min-os-major-version				10.X						(Number)
		max-os-major-version				10.X						(Number)
		min-mail-version					X.X							(Number)
		configure-mail-min-bundle-version	Min Mail Bundle Version		(Number)

Uninstall specific (ignored during install). These are used to determine how the Mail Plugin Manager application can be deleted in different situations. Assuming that the Manager is included in an uninstall manifest.
		
		can-delete-bundle-manager-if-no-other-plugins-use				(Boolean)
		can-delete-bundle-manager-if-no-plugins-left					(Boolean)
		
<a name="action-items"></a>
#### Action Items

This is an array of dictionary objects that describe what is to be installed or uninstalled. The objects will not be installed in any particular order. Here are the keys and example values with a description of them afterward.

		path					Delivery/MyPlugin.mailbundle			(String)
		destination-path		<LibraryDomain>/Mail/Bundles/			(String)
		name					My Bundle								(String)
		description				A Bundle for Testing					(String)
		is-bundle-manager												(Boolean)
		user-can-choose-domain											(Boolean)

The key `path` is the path where to find the original object to act upon.

* In the case of an **install**, this value should be a path *relative* to the package file or an absolute URL. For instance, in the example above there would be a folder called `Delivery` inside the `.mpinstall` package that would contain the mail bundle. If it is a full URL it will try to install directly from the web, but please remember that no access will mean a failure. **_Currently not yet implemented_**.
* For an **uninstall**, this value should be a path to where the item to be deleted should be. This should be a full path or a path using a tilda (~).

The key `destination-path` is the path to where the item should be copied during an install. **It is ignored during an uninstall**. This is a full path, a path using a tilda (~), or a path beginning with the special marker `<LibraryDomain>`. The value `<LibraryDomain>` will be expanded to the Library folder in the domain the users selects to install to. Tildas will also be expanded.

The key `name` is the value used in display during the install/uninstall.
*This key is optional, but strongly recommended*

The key `description` is the value used in display during the install/uninstall during the Review step.
*This key is optional*

The key `is-bundle-manager` indicates that the item described is the Mail Plugin Manager application. This is a boolean flag that is used to facilitate the proper installation/removal of this item. Special handling is done to ensure the most recent version is available/installed and that it is not removed if another plugin is using it. 
*This key is optional*

The key `user-can-choose-domain` let's the user select the domain they want to install to. The default values is `NO` and the User Domain will be used. This is ignored during uninstall.
*This key is optional*.
**_Currently not implemented_**.


<a name="confirm-steps"></a>
#### Confirmation Steps

This is an array of dictionary objects with values describing each of the steps to display to the user, in the desired order, during the confirmation of the install/uninstall process. 

Here are the keys and example values with a description of them afterward.

		type						[license|information|confirm]		(String)
		title						(Name shown at top of page)			(String)
		bullet-title				(Name of step in list at left)		(String)
		path						/path/to/resource					(String)
		license-agreement-required										(Boolean)

The key `type` defines one of three types of display steps, `license` and `information` are essentially the same and just display some content to the user, however the `license` type step will test to see if the user must [agree to a license](#license-agree) and, if so, present a proper alert to ensure that they agree. The `confirm` step shows the list of what will happen and show the Install/Remove button before doing the actions.

The key `title` is used at the top of the information display and is used as the default bullet title, if one of those is not present.
*This key is required*.

The key `bullet-title` is used in the step list on the left side of the window. If no value is provided the `title` value is used.
*This key is optional, but recommended*.

The key `path` is the path where to find the resource to display for either an `information` or `license` type. The path should be relative to the manifest file and should be either an RTF/RTFD file or an HTML file (with resources relative to that file. *It is required for the `license` and `information` types*.

<a name="license-agree"></a>
The key `license-agreement-required` works in conjunction with the `license` type and is ignored in other cases. If set to **YES**, then the license will require the user to agree before continuing. The default is **NO**.
*This key is optional*.

<a name="localization"></a>
#### Localizing Install/Uninstall packages

These packages can be localized in the same way that applications or bundles can. If you put an .lproj folder for a language in the package, any text values that are pulled out of your manifest file will attempt to be localized from those localization folders.

Unfortunately this doesn't include the base of the project itself and thus all the parts around this that are not localized will come through in English. Hopefully we can get some localization done for the rest of the app.

---

<a name="macros"></a>
### Macros Defined

#### Fire and Forget Macros

These macros are the ones that you should call when the app launches to do updates or send crash reports in the background. They do not require your plugin to take any other action. The names are pretty self-explanatory, I think. The ones that end with "Now" should be used if you want to force an update, such as when the user clicks a button saying "Check Now".

		MPTUninstallForBundle(mptMailBundle)
		MPTCheckForUpdatesForBundle(mptMailBundle)
		MPTCheckForUpdatesForBundleNow(mptMailBundle)
		MPTSendCrashReportsForBundle(mptMailBundle)
		MPTUpdateAndSendReportsForBundle(mptMailBundle)
		MPTUpdateAndSendReportsForBundleNow(mptMailBundle)

You need to pass in the bundle for your plugin, so it can properly determine the bundle identifier and it's path.

#### Managing a LaunchAgent

These macros are to be used to create a LaunchAgent that the plugin might need to configure in order to perfomr some actions outside of the sandbox.

		MPTInstallLaunchAgent(mptMailBundle, mptAgentConfig, mptReplaceExisting, mptResultBlock)
		MPTRemoveLaunchAgent(mptMailBundle, mptAgentLabel, mptResultBlock)
		
The first argument is the bundle of your plugin, as with the macros above. The last argument is the block that you want to run to notify you of success or failure. It is optional – pass nil, if you don't care. It is defined as a `MPTLaunchResultBlock` and has been `typedef`'d like this:

		typedef void(^MPTLaunchResultBlock)(NSError *);

It takes the single argument which is an error. It is `nil` if successful and will contain a reasonable description of any issues incurred.

For `MPTInstallLaunchAgent` the second argument is a `NSDictionary` with the contents of the LaunchAgent file you need to create. See [the launchd man page][launchd] for more information about the contents of this dictionary. The `mptReplaceExisting` argument is a `BOOL` to indicate if the Tool should remove an existing agent with the same Label and replace it.

For `MPTRemoveLaunchAgent` the second argument is simply the Label for the service to remove. It should be the same as the one passed into the above macro in the `mptAgentConfig`.


#### Handling Up To Date Sparkle Results

These macros are to be used in conjunction with the Update "Now" macros from the above list and allow you to properly display a dialog telling the user that the current plugin is up to date. You have an option of a modal version or a sheet version (pass in the window to put the sheet on). An optional block can be passed as well, that will be run upon completion of checking for an update.

		MPTPresentModalDialogWhenUpToDate(mptBundle)
		MPTPresentModalDialogWhenUpToDateWithBlock(mptBundle, mptFinishBlock)
		MPTPresentDialogWhenUpToDateUsingWindow(mptBundle, mptSheetWindow, mptFinishBlock)

The block argument is the block that you want to run when the results are returned. It is defined as a `MPTUpdateTestingCompleteBlock` and has been `typedef`'d like this:

		typedef void(^MPTUpdateTestingCompleteBlock)(void);

#### Notification Block Macros

These macros allow you to get information back from MPT by having a block run which is passed a dictionary of the results.

		MPTMailInformationForBundleWithBlock(mptMailBundle, mptNotificationBlock)
		MPTUUIDListForBundleWithBlock(mptMailBundle, mptNotificationBlock)

The first argument is the bundle of your plugin, as with the macros above. The second argument is the block that you want to run when the results are returned. It is defined as a `MPTResultNotificationBlock` and has been `typedef`'d like this:

		typedef void(^MPTResultNotificationBlock)(NSDictionary *);

It takes the single argument of the dictionary of results and has a void result.

Macros for the keys for the results in the dictionary are listed below.

##### UUID List Query

These are the keys for the results of the uuid list query:

		MPT_UUID_COMPLETE_LIST_KEY				NSArray of all uuid NSDictionaries
		MPT_UUID_LATEST_SUPPORTED_UUID_KEY		NSDictionary of latest uuid data supported by Plugin
		MPT_UUID_FIRST_UNSUPPORTED_UUID_KEY		NSDictionary of 1st uuid data known that the Plugin does **not** support

Each of the above `NSDictionary` objects should contain the following keys. The OS display keys are for the standard user visible descriptions of the OS, i.e. 10.6.8, 10.7 or 10.7.3. The latest version key is a relative value integer that tells us it if is greater or less than another in the list - makes comparing easier that deciphering the 10.7.3 numbers.

		MPT_UUID_TYPE_KEY
		MPT_UUID_EARLIEST_OS_VERSION_DISPLAY_KEY
		MPT_UUID_LATEST_OS_VERSION_DISPLAY_KEY
		MPT_UUID_LATEST_VERSION_KEY
		
And the value of the `MPT_UUID_TYPE_KEY` key should be one of:

		MPT_UUID_TYPE_VALUE_MAIL				Mail.app Type
		MPT_UUID_TYPE_VALUE_MESSAGE				Message.Framework Type

##### System info Query

These are the keys for the results of the system info query:

		MPT_SYSINFO_ANONYMOUS_ID_KEY			String of Anonymized Unique Identifier
		MPT_SYSINFO_HARDWARE_KEY				String of hardware identifier
		MPT_SYSINFO_SYSTEM_KEY					NSDictionary of BUILD & VERSION
		MPT_SYSINFO_MAIL_KEY					NSDictionary of BUILD, VERSION & UUID
		MPT_SYSINFO_MESSAGE_KEY					NSDictionary of BUILD, VERSION & UUID
		MPT_SYSINFO_INSTALLED_PLUGINS_KEY		NSArray of NSDictionaries of NAME, PATH, BUILD & VERSION
		MPT_SYSINFO_DISABLED_PLUGINS_KEY		NSArray of NSDictionaries of NAME, PATH, BUILD & VERSION

The key macros in capitals above are the following:

		MPT_SYSINFO_NAME_KEY
		MPT_SYSINFO_PATH_KEY
		MPT_SYSINFO_VERSION_KEY
		MPT_SYSINFO_BUILD_KEY
		MPT_SYSINFO_UUID_KEY

---

<a name="infoplistkeys"></a>
### Info.plist Keys

To make your plugin work optimally with MPM and MPT, you should add some specific values to your info.plist. These will determine where to send crash reports, how to handle uninstalls and other features. Below is a list of the keys and a description of what they do.

Required to use the [Send Crash Reports](#crash-reports) feature, see that link for the format of the report:

		MPCCrashReportURL		URL to your web service to accept crash reports
		
The rest are all optional, but recommended:
		
		MPCMaxCrashReportsToSend
		MPCPluginUsesMailPluginManager			
		MPCSupplementalSparkleFeedParameters
		
The `MPCMaxCrashReportsToSend` key allows you to indicate a maximum of crash reports that you want to send in any one session. This allows you to manage your server storage better. **_The default value is 20_**.

The `MPCPluginUsesMailPluginManager` key identifies your plugin as one that is aware of MPM and MPT and is used during an install to ensure that another uninstaller doesn't remove the apps when you depend on them.

The `MPCSupplementalSparkleFeedParameters` key allows you to add an array of other keys within your `info.plist` file that are to be sent as Sparkle feed parameters for the anonymous data.

The following values are used by the apps whenever a plugin is displayed to show the company name, URL and product URL. All of them are optional, but make the presentation nicer. There is a companies.plist file which I try to update with the company name and url based on the reverse domain name of the bundle id, but these values will **always** override that information.
		
		MPCCompanyName			Company name for display
		MPCCompanyURL			Company URL to link to in Apps
		MPCProductURL			Product URL to link to in Apps


---

### Still to do (mostly notes for me)

#### Common Pieces

* Add better error handling to the secureMoveTo methods.
* *Would be nice*
* Change the MailBundle class to cache the list of mail plugins so that there is no need to rehit for update info.
* Setup actions to watch changes of files to note when Plugins become active, disabled, domain change.

#### Manager Interface

* *Would be nice*
* Add support for different domains in installation and uninstall.
* Add an Update All Plugins button to Manager window when relevant.
* Allow user to select destination for Manager application.

#### Tool

* *Would be nice*
* Build out Launch Agent scheduling for boot-time validation and plugin scheduling.
* Add an Update All Plugins button to Multi Plugin window when relevant.

#### launchd values of interest (for boot time agent setup)

* RunAtLoad = YES
* WatchPaths = Array (a single path to watch)
* QueueDirectories = Array (directories to watch)
* StartCalendarInterval = dict (run once a week perhaps in order to update plists)
* StartInterval (in seconds)
* StandardOutPath = path
* [man page][launchd]
* LaunchOnlyOnce = YES (nope)

#### Other

* Add support for .mpinstall install through sparkle updater?
* What about devs that want to use this but have there own installer?

---

2011-2012 Little Known Software

You can use this software any way that you like, as long as you don't blame me for anything ;-)

<!-- links -->
[launchd]: http://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man5/launchd.plist.5.html#//apple_ref/doc/man/5/launchd.plist
[sparkle-script]: http://sparkle.andymatuschak.org/files/php_sparkle_stats_server.zip
[crash-script-link]: https://github.com/lksoft/MailPluginManager/raw/master/Remote/MPTCrashScripts.zip
[json-php]: http://bg.php.net/manual/en/book.json.php

<!-- images -->
[install-1]: http://media.lksw.eu/mpm/Example_Install_1.png
[install-2]: http://media.lksw.eu/mpm/Example_Install_2.png
[install-3]: http://media.lksw.eu/mpm/Example_Install_3.png
[install-4]: http://media.lksw.eu/mpm/Example_Install_4.png
[manager-window]: http://media.lksw.eu/mpm/Mail_Bundle_Manager.png
[single-notice]: http://media.lksw.eu/mpm/Single_Plugin_Notice.png
[multi-notice]: http://media.lksw.eu/mpm/Multi_Plugin_Notice.png
