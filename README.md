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
  <img alt="Linux build status" src="https://img.shields.io/badge/Linux%20build-Passing-brightgreen?style=for-the-badge&logo=linux&logoColor=white">
  <!--<img alt="Linux build status" src="https://img.shields.io/badge/Linux%20build-Testing-yellow?style=for-the-badge&logo=linux&logoColor=white">-->
  <!--<img alt="Linux build status" src="https://img.shields.io/badge/Linux%20build-Failing-red?style=for-the-badge&logo=linux&logoColor=white">-->
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

<!-- This section could be written a LOT better -->
# About the Project
An improved Bongo Cat overlay with ~~smooth paw movement~~ **(**[#27](https://github.com/ZackeryRSmith/bongocat-plus/issues/27)**)** and customizable lua scripting. Originally created due to the lack of support from developers in the original project (90+ unsolved issues), it now stands as a distinct iteration, thanks to a complete rewrite of the code. Bongocat-Plus owes its existence to the creators of Bongocat-Osu, so thank you!

# Compiling
To compile bongocat-plus you need to install some dependencies:

- **Git** *(this is an optional dependancy)*
- **GCC**, **G++**, *any c/c++ compiler really*
- **Make**
- **CMake** >=3.10 *(may be lowered soon, see [#55](https://github.com/ZackeryRSmith/bongocat-plus/issues/55))*
- **SFML** >=2.5 *(not required on Windows, see [#28](https://github.com/ZackeryRSmith/bongocat-plus/issues/28))*
- **Lua** >=5.4 *(this is an optional dependancy, see [#51](https://github.com/ZackeryRSmith/bongocat-plus/issues/51))*

*Now, that's out of the way*. We can now actually compile Bongocat-Plus

1. First clone the repository, (assuming you have Git) thats as simple as `git clone https://github.com/ZackeryRSmith/bongocat-plus`
2. Jump into the repo with a `cd bongocat-plus`
3. Then to generate the make files run `cmake .`
4. Top it off by running `make`

Boom, you have just compiled Bongocat-Plus! (if you experience any issues compiling bongocat-plus please make an [issue](https://github.com/ZackeryRSmith/bongocat-plus/issues))

<br />

# Contributors
Huge thank you to all the people who have contributed to this project:
<br /><br />
<a href="https://github.com/ZackeryRSmith/bongocat-plus/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ZackeryRSmith/bongocat-plus"/>
</a>
