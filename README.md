# Kanban Board Application

KanbanBoard is a task management application built using **QtQuick**, **QML**, and **C++**. The app allows users to organize tasks into columns, manage task descriptions, and visually track the status of tasks with different priority levels. It features drag-and-drop task management, editable task details, and smooth transitions for an intuitive user experience.

## Features

### Task Board Management
- **Add and remove columns**: Easily add new columns and remove existing ones.
- **Add new task cards**: Create new task cards with descriptions and add them to columns.
- **Edit task cards**: Modify the content of task cards to update task details.
- **Drag & Drop functionality**: Tasks can be dragged and dropped between columns for easy organization.

### Task States and Priorities
- Task cards change their appearance based on their current state.
  - Cards are color-coded to represent priority levels (low, medium, high).

### Animations
- **Smooth transitions**: Tasks are smoothly moved when dragged between columns.
- **Visual effects**: There are animations for adding and removing tasks and columns, enhancing the user experience.

### Mouse and Keyboard Support
- **Mouse support**: Tasks can be moved by dragging them with the mouse.

### Saving Board State
- The task board state is saved in a local **SQL database**.
- The layout and content of the board are restored when the application is restarted.

## Current Version

The following features have been implemented so far:
- Adding and removing columns.
- Adding new tasks with descriptions to columns.
- Dragging and dropping tasks between columns.
- Changing task appearance based on priority (low, medium, high).
- Dragging tasks with the mouse.
- Editting columns.
- Editting task.
- Animation for adding and removing tasks.
- Animation for adding and removing columns.
- Saving Board State.
