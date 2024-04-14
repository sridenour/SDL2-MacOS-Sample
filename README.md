# How to Create a macOS SDL2 App Xcode Project For Fun and Profit #

### Initial Project Creation ###

1. In Xcode, create a New Project. Choose "macOS->App".
2. In the window that pops up, enter your project name and other details such as the bundle identifier.
3. For Interface, choose XIB
4. For Language, choose Objective-C (your app doesn't have to be written in Objective-C)
5. For Storage, choose None.
6. Click Next, and choose where you want your project to be created.

You now have an Xcode project that will create a full-blown macOS Cocoa app, with an app bundle that can have SDL2 embedded in it for easy distribution.

### Setup ###

1. Xcode will have created some stuff we don't need (SDL provides it for us). Delete the following files:
* AppDelegate.h
* AppDelegate.m
* MainMenu.xib
* main.m (even this one)

Make sure to choose "Move to Trash" when Xcode asks if you want to really delete them or just remove them from the project file.

2. Using *Finder*, copy `SDL2.framework` (that you either downloaded from libsdl.org or built yourself) to the same folder as `YourProject.xcodeproj`.

3. In your project's build settings, choose the YourProject target.

4. In the `General` tab, find the `Frameworks, Libraries, and Embedded Content` section. Click the "+", choose `Add Other`->`Add Files` and choose `SDL2.framework` from step 2. Make sure it's the one from Step 2, that you copied next to `YourProject.xcodeproj`. You should now see `SDL2.framework` in the `Frameworks, Libraries, and Embedded Content` secion, with `Embed & Sign` next to it.

5. In the `Build Settings` tab, search for `Framework Search Paths` and double check that Xcode has correctly added `$(PROJECT_DIR)`

6. In the `Signing & Capabilities` tab, in the `App Sandbox` section, check USB (otherwise your app won't be able to use game controllers). Change `User Selected File` to `Read/Write`.

7. Set up the rest of your project's settings. Choose the App Category, set up Code Signing, set the Minimum Deployment Target, etc.

8. Either create a new file named `main.c` (or `main.cpp` or whatever) or add your app's source code to the project.

You should now have a working Xcode project that builds a macOS app that uses SDL. `SDL2.framework` will be bundled inside the app.

### Troubleshooting ###

If you run your app and get an error along the lines of not being able to validate `SDL2.framework` or some such, go to the project settings, under the `Signing & Capabilities` tab, scroll down and check `Disable Library Validation`. Then rebuild your app.
