<!-- TOP OF README ANCHOR -->
<a name="top"></a>

> [!NOTE]
the state of the refactor is not current with the in-dev branch... *so then why is this (refactor) the default?*

> Simply because I believe it's more indicative of this projects direction. If you want a version of Bongocat-Plus
that's fully complete. Please check out [in-dev](https://github.com/ZackeryRSmith/bongocat-plus/tree/in-dev)

<!-- PROJECT LOGO -->
<br/>
<div align="center">
  <a href="https://github.com/ZackeryRSmith/bongocat-plus/tree/refactor/cats/plus">
    <img src="./res/catbanner.gif" alt="logo" width="590" height="332">
  </a>

<!-- BUILD STATUS' -->
<p align="center">
  <img alt="Windows build status" src="https://img.shields.io/badge/Windows%20build-Passing-brightgreen?style=for-the-badge&logo=windows">
  <!--<img alt="Windows build status" src="https://img.shields.io/badge/Windows%20build-Testing-yellow?style=for-the-badge&logo=windows">-->
  <!--<img alt="Windows build status" src="https://img.shields.io/badge/Windows%20build-Failing-red?style=for-the-badge&logo=windows">-->
  <img alt="Mac build status" src="https://img.shields.io/badge/Mac%20build-Passing-brightgreen?style=for-the-badge&logo=apple">
  <!--<img alt="Mac build status" src="https://img.shields.io/badge/Mac%20build-Testing-yellow?style=for-the-badge&logo=apple">-->
  <!--<img alt="Mac build status" src="https://img.shields.io/badge/Mac%20build-Failing-red?style=for-the-badge&logo=apple">-->
  <!--<img alt="Linux build status" src="https://img.shields.io/badge/Linux%20build-Passing-green?style=for-the-badge&logo=linux&logoColor=white">-->
  <!--<img alt="Linux build status" src="https://img.shields.io/badge/Linux%20build-Testing-yellow?style=for-the-badge&logo=linux&logoColor=white">-->
  <img alt="Linux build status" src="https://img.shields.io/badge/Linux%20build-Failing-red?style=for-the-badge&logo=linux&logoColor=white">
</p>

<h3 align="center">Bongocat Plus</h3>
  <p align="center">
    The extensible and highly configurable Bongo Cat overlay!
    <br/>
    <a href="https://github.com/kuroni/bongocat-osu"><strong>Original Project</strong></a>
    <br/><br/>
    <a href="https://github.com/ZackeryRSmith/bongocat-plus/issues">Report Bug</a>
    ·
    <a href="https://github.com/ZackeryRSmith/bongocat-plus/issues">Request Feature</a>
  </p>
  
  
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <ul>
      <li><a href="#building">Building</a></li>
      <ul>
          <li><a href="#windows">Windows</a></li>
          <li><a href="#macosx">Windows</a></li>
          <li><a href="#linux">Linux</a></li>
      </ul>
    </ul>
    <li><a href="#contributors">Contributors</a></li>
  </ol>
</details>

# About the Project
A Bongo Cat overlay with ~~smooth paw movement~~ **(**[#27](https://github.com/ZackeryRSmith/bongocat-plus/issues/27)**)** and a highly customizable script, written in C++. *This project is an improved fork* as the original project has lost support from the developers (80+ unsolved issues). This project once aimed just to implement nearly all the feature requests. It has since broken off, becoming unrecognisable from the original. All the code from the original has been rewritten, but it's all thanks to the wonderful people who created Bongocat-Osu, that Bongocat-Plus exists!

# Building
## Windows
To compile on Windows you **need** *(relevant links are provided, but if you have the tools installed already you'll be fine)*
- [git](https://www.git-scm.com/downloads)
- [cygwin](https://cygwin.com/install.html), [MinGW](https://sourceforge.net/projects/mingw/)
  - Make
  - g++
- [CMake](https://cmake.org/download/)

```cmd
git clone https://github.com/ZackeryRSmith/bongocat-plus
cd bongocat-plus
cmake .
make
```

Boom, you have just compiled bongocat-plus.

## MacOSX
To make life much easier for you I'd recommend you install [brew](https://brew.sh/); a package manager for MacOSX. Once installed run `brew install sfml`. Now run the following commands to build bongo-cat:

```sh
git clone https://github.com/ZackeryRSmith/bongocat-plus
cd bongocat-plus
cmake .
make
```

Boom, you have just compiled bongocat-plus.

## Linux
As the linux build is currently failing this section won't be updated until [#48](https://github.com/ZackeryRSmith/bongocat-plus/issues/48) is fixed

<br />

# Contributors
Huge thank you to all the people who have contributed to this project:
<br /><br />
<a href="https://github.com/ZackeryRSmith/bongocat-plus/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ZackeryRSmith/bongocat-plus"/>
</a>
