<a name="readme-top"></a>

# 📗 Table of Contents

- [📖 About the Project](#about-project)
  - [🛠 Built With](#built-with)
    - [Tech Stack](#tech-stack)
    - [Key Features](#key-features)
- [💻 Getting Started](#getting-started)
  - [Setup](#setup)
  - [Prerequisites](#prerequisites)
  - [Install](#install)
  - [Usage](#usage)
  - [Run tests](#run-tests)
  - [Deployment](#triangular_flag_on_post-deployment)
- [👥 Authors](#authors)
- [🔭 Future Features](#future-features)
- [🤝 Contributing](#contributing)
- [⭐️ Show your support](#support)
- [🙏 Acknowledgements](#acknowledgements)
- [📝 License](#license)

# 📖 [3D_PaintTool] <a name="about-project"></a>

**[Recipe app]**
With the 3D_PaintTool app you are able to draw 3D figures in space. Use your mouse and the arrow keys in order to move arround all the space of the app. Click around to create new shapes. Edit their colors and forms to create even better 3D figures. You can import and export other object into the space too!

3-dimensional drawing engine that includes camera rotation and translation at will
using the A,S,D,Q,W,E keys. With right click and drag you can rotate the camera
on the same axis. If you hold down R and click the left button, the program begins to take
coordinates for a new figure.

Pre-programmed figures can be imported, as well as figures generated under the program format.

All models created with the program can be programmed.

Using the ESC button releases the program from any instructions it has entered.

## 🛠 Built With <a name="built-with"></a>

### Tech Stack <a name="tech-stack"></a>

<details>
  <summary>Client</summary>
  <ul>
    <li><a href="https://learn.microsoft.com/es-es/cpp/cpp/welcome-back-to-cpp-modern-cpp?view=msvc-170">C++</a></li>
     <li><a href="https://www.opengl.org/">OpenGL</a></li>
  </ul>
</details>

### Key Features <a name="key-features"></a>

- **[Models]**
-- camera
-- impExp
-- inputAdd
-- mesh
-- model
-- renderizador
-- tools
- **[Controllers]**
-- glad
-- ImGUIFileDialog
-- main_window

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## 💻 Getting Started <a name="getting-started"></a>

To get a local copy up and running, follow these steps.

### Prerequisites

In order to run this project you need:

- Visual Studio 2017
- Install OpenGL on your local enviroment

### Setup

Clone this repository to your desired folder:

```sh
  cd my-folder
  git https://github.com/alexansaa/3D_PaintTool.git
```

### Install

Install this project with:

In order to use the program, the repository must be copied to a local folder (unzip or clone repository).
Once inside the folder, open the .sln file to open the project in Visual Studio. Here we are going to change one
global configuration that could not be shared by repository:
- Right click on the node with the project name shown in the project explorer (just below the root node)
- In this menu we select the "properties" option
- On the screen that appears, we go to "Debugging"
- Once here, make sure that the "Plataform" option located in the upper central part of the window is in the "All Platforms" option
Usually it happens that no values ​​are shown, in that case select the "All Platforms" option again so that the values ​​appear
- Then, we edit the "Enviroment" option with the value "PATH=%PATH%;$(ProjectDir)\Linking\Dll"
- We accept everything and close pop-ups

### Usage

To run the project, press F5 or click the green "play" button in Visual Studio

### Deployment

You can deploy this project using:
Coming Soon

<!--
Example:

```sh

```
 -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- AUTHORS -->

## 👥 Authors <a name="authors"></a>

👤 **Kleber Saavedra**

- GitHub: [Kleber Saavedra](https://github.com/alexansaa)
- LinkedIn: [@alexansaa](https://www.linkedin.com/in/alexander-saavedra-garcia/)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- FUTURE FEATURES -->

## 🔭 Future Features <a name="future-features"></a>

- [ ] **[Optimize Deep Calculation and Renderization algorithm]**
- [ ] **[Select and Edit Shapes new algorithm]**

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->

## 🤝 Contributing <a name="contributing"></a>

Contributions, issues, and feature requests are welcome!

Feel free to check the [issues page](https://github.com/alexansaa/3D_PaintTool/issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## ⭐️ Show your support <a name="support"></a>

If you like this project, please give it a star on GitHub

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## 🙏 Acknowledgments <a name="acknowledgements"></a>

I would like to thank EPN - Escuela Politecnica Nacional for giving me the chance to study my second degree on Computer Science

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->

## 📝 License <a name="license"></a>

This project is [MIT](./LICENSE.md) licensed.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
