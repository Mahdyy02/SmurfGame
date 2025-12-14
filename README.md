# SmurfGame

A 2D game developed in C++ using SDL2 and Visual Studio.

## Demo Video

Watch the gameplay demo:

[![SmurfGame Demo](https://img.youtube.com/vi/nGTickMdguQ/0.jpg)](https://youtu.be/nGTickMdguQ)

## Project Resources

**Full Project Files (with all media and assets):** [Google Drive Link](https://drive.google.com/drive/folders/1jLfLkvvtdvgW3S0uUtTdIIFfoaE8QMFT?usp=drive_link)

## Prerequisites

- **Visual Studio 2019 or later** (with C++ Desktop Development workload)
- **Windows OS**

## Required Libraries

This project requires the following SDL2 libraries:

- SDL2-devel-2.30.2-VC
- SDL2_image-devel-2.8.2-VC
- SDL2_mixer-devel-2.8.0-VC
- SDL2_ttf-devel-2.22.0-VC

## Setup Instructions

### 1. Download SDL Libraries

Download the required SDL2 development libraries from the official SDL website:

- [SDL2-devel-2.30.2-VC](https://www.libsdl.org/download-2.0.php)
- [SDL2_image-devel-2.8.2-VC](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_mixer-devel-2.8.0-VC](https://www.libsdl.org/projects/SDL_mixer/)
- [SDL2_ttf-devel-2.22.0-VC](https://www.libsdl.org/projects/SDL_ttf/)

### 2. Extract SDL Libraries

1. Create a folder for your SDL libraries (e.g., `C:\SDL2`)
2. Extract all downloaded SDL libraries to this folder
3. Your folder structure should look like:
   ```
   C:\SDL2\
   ├── SDL2-2.30.2\
   ├── SDL2_image-2.8.2\
   ├── SDL2_mixer-2.8.0\
   └── SDL2_ttf-2.22.0\
   ```

### 3. Configure Visual Studio Project

#### Step 1: Open Project Properties
1. Open `SmurfGame.sln` in Visual Studio
2. Right-click on the project in Solution Explorer → **Properties**
3. Make sure **Configuration** is set to **All Configurations** and **Platform** is set to **x64**

#### Step 2: Include Directories
1. Navigate to: **C/C++** → **General** → **Additional Include Directories**
2. Add the following paths (adjust paths based on your SDL installation):
   ```
   C:\SDL2\SDL2-2.30.2\include
   C:\SDL2\SDL2_image-2.8.2\include
   C:\SDL2\SDL2_mixer-2.8.0\include
   C:\SDL2\SDL2_ttf-2.22.0\include
   ```

#### Step 3: Library Directories
1. Navigate to: **Linker** → **General** → **Additional Library Directories**
2. Add the following paths:
   ```
   C:\SDL2\SDL2-2.30.2\lib\x64
   C:\SDL2\SDL2_image-2.8.2\lib\x64
   C:\SDL2\SDL2_mixer-2.8.0\lib\x64
   C:\SDL2\SDL2_ttf-2.22.0\lib\x64
   ```

#### Step 4: Link Libraries
1. Navigate to: **Linker** → **Input** → **Additional Dependencies**
2. Add the following library files:
   ```
   SDL2.lib
   SDL2main.lib
   SDL2_image.lib
   SDL2_mixer.lib
   SDL2_ttf.lib
   ```

#### Step 5: Subsystem Configuration
1. Navigate to: **Linker** → **System** → **SubSystem**
2. Set to **Console (/SUBSYSTEM:CONSOLE)** or **Windows (/SUBSYSTEM:WINDOWS)** depending on your preference

### 4. Copy DLL Files

Copy the following DLL files from the SDL `lib\x64` folders to your project's output directory (where the `.exe` is generated):

From `SDL2-2.30.2\lib\x64`:
- SDL2.dll

From `SDL2_image-2.8.2\lib\x64`:
- SDL2_image.dll
- (and any additional image format DLLs like libpng, libjpeg, etc.)

From `SDL2_mixer-2.8.0\lib\x64`:
- SDL2_mixer.dll
- (and any additional audio codec DLLs)

From `SDL2_ttf-2.22.0\lib\x64`:
- SDL2_ttf.dll

**Tip:** You can set up a Post-Build Event to copy DLLs automatically:
1. Go to **Build Events** → **Post-Build Event** → **Command Line**
2. Add:
   ```batch
   xcopy /y /d "C:\SDL2\SDL2-2.30.2\lib\x64\*.dll" "$(OutDir)"
   xcopy /y /d "C:\SDL2\SDL2_image-2.8.2\lib\x64\*.dll" "$(OutDir)"
   xcopy /y /d "C:\SDL2\SDL2_mixer-2.8.0\lib\x64\*.dll" "$(OutDir)"
   xcopy /y /d "C:\SDL2\SDL2_ttf-2.22.0\lib\x64\*.dll" "$(OutDir)"
   ```

### 5. Build and Run

1. Select your build configuration (**Debug** or **Release**)
2. Select platform: **x64**
3. Press **F5** or click **Debug** → **Start Debugging**

## Project Structure

The project includes the following key components:

- **ECS (Entity Component System)**: Core game architecture
- **AssetManager**: Handles loading and managing game assets
- **Collision System**: Handles collision detection
- **Sound System**: Audio playback using SDL_mixer
- **Animation System**: Sprite animation management
- **Map System**: Tile-based map rendering
- **Menu System**: Game menu interface

## Troubleshooting

### Missing DLL Error
If you get an error about missing DLLs when running the game:
- Make sure all SDL DLL files are copied to the same directory as your `.exe` file

### Linker Errors (LNK2019)
If you get unresolved external symbol errors:
- Double-check that all library paths are correct
- Ensure you've added all required `.lib` files to Additional Dependencies
- Verify you're building for the correct platform (x64)

### Include Errors
If you get errors about missing header files:
- Verify your Additional Include Directories are set correctly
- Make sure the paths match where you extracted the SDL libraries

## License

This is a student/educational project.

## Contact & Resources

For the complete project with all media files and assets, visit the [Google Drive folder](https://drive.google.com/drive/folders/1jLfLkvvtdvgW3S0uUtTdIIFfoaE8QMFT?usp=drive_link).
