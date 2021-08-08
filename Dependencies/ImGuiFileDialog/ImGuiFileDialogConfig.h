#pragma once

// uncomment and modify defines under for customize ImGuiFileDialog

//#define MAX_FILE_DIALOG_NAME_BUFFER 1024
//#define MAX_PATH_BUFFER_SIZE 1024

//#define USE_EXPLORATION_BY_KEYS
// this mapping by default is for GLFW but you can use another
//#include <GLFW/glfw3.h> 
// Up key for explore to the top
//#define IGFD_KEY_UP GLFW_KEY_UP
// Down key for explore to the bottom
//#define IGFD_KEY_DOWN GLFW_KEY_DOWN
// Enter key for open directory
//#define IGFD_KEY_ENTER GLFW_KEY_ENTER
// BackSpace for comming back to the last directory
//#define IGFD_KEY_BACKSPACE GLFW_KEY_BACKSPACE

// widget
// filter combobox width
//#define FILTER_COMBO_WIDTH 120.0f
// button widget use for compose path
//#define IMGUI_PATH_BUTTON ImGui::Button
// standard button
//#define IMGUI_BUTTON ImGui::Button

// locales string
#define createDirButtonString "\ue2cc"

#define cancelButtonString "\ue5cd Cancel"
#define resetButtonString "\ue042"
#define drivesButtonString "\ue875"
#define searchString "\uf02f"
#define dirEntryString "\ue2c7 "
#define linkEntryString "\ue157 "
#define fileEntryString "\uf1fc "
#define fileNameString "File Name: "
#define dirNameString "Directory: "
#define buttonResetSearchString "Reset search"
#define buttonDriveString "Drives"
#define buttonResetPathString "Go to current directory"
#define buttonCreateDirString "New folder"
#define OverWriteDialogTitleString "The file already exist"
#define OverWriteDialogMessageString "Would you like to overWrite it?"
#define OverWriteDialogConfirmButtonString "\ue5ca Confirm"
#define OverWriteDialogCancelButtonString "\ue5cd Cancel"

// DateTimeFormat
// see strftime functionin <ctime> for customize
// "%Y/%m/%d %H:%M" give 2021:01:22 11:47
// "%Y/%m/%d %i:%M%p" give 2021:01:22 11:45PM
//#define DateTimeFormat "%Y/%m/%d %i:%M%p"

// theses icons will appear in table headers
#define USE_CUSTOM_SORTING_ICON
#define tableHeaderAscendingIcon "\ue316"
#define tableHeaderDescendingIcon "\ue313"
#define tableHeaderFileNameString " File name"
#define tableHeaderFileTypeString " Type"
#define tableHeaderFileSizeString " Size"
#define tableHeaderFileDateTimeString " Date"

//#define USE_BOOKMARK
//#define bookmarkPaneWith 150.0f
//#define IMGUI_TOGGLE_BUTTON ToggleButton
//#define bookmarksButtonString "Bookmark"
//#define bookmarksButtonHelpString "Bookmark"
//#define addBookmarkButtonString "+"
//#define removeBookmarkButtonString "-"
