## 游戏介绍
这是一款轻量版的Phigros音乐游戏，支持 Window/Linux/Mac 系统。为保证游戏难度，游戏虽仅包含两种音符但分布了8个轨道，从左到右分别由'A''S''D''F''J''K''L'';'8个按键控制——作为玩家，你也可以在设置界面自由绑定轨道按键并调整音频偏移，当音符到达判定线时候按下对应轨道的按键可获得分数，分数判定为S,A,B,C,D五档，请享受旋律与游戏，取得高分吧！

## 项目结构:
```
PhigrosLite/
├── CMakeLists.txt
├── vcpkg.json
│
├── src/
│   ├── main.cpp
│   |
│   ├── CMakeLists.txt
│   |
│   ├── app/
│   │   └── Application.h/cpp
│   │
│   ├── common/
│   │   ├── Constants.h
│   │   ├── GameConfig.h/cpp
│   │   └── Note.h
│   │
│   ├── model/
│   │   └── Chart.h/cpp
│   │
│   ├── viewmodel/
│   │   ├── MainViewModel.h/cpp
│   │   ├── SelectViewModel.h/cpp
│   │   ├── GameViewModel.h/cpp
│   │   └── ResultViewModel.h/cpp
│   │
│   └── view/
│       ├── Font.h
│       ├── GameConfig.h/cpp
│       ├── HitEffect.h
│       ├── MainWindow.h/cpp
│       ├── StartPage.h/cpp
│       ├── SelectPage.h/cpp
│       ├── GamePage.h/cpp
│       ├── Timer.h/cpp
│       └── ResultPage.h/cpp
│
├── resources/
│   ├── charts/
│   ├── music/
│   ├── images/
│   └── config/
└── tests/
```

## 项目编译（推荐使用 VS Code）
【注意】请安装好 Cmake 相关插件
```cmd
cmake -B build
cmake --build build
```

## 开始游玩
编译成功后，可在顶层文件夹下发现 build 目录，打开 build/src/Debug 下的 PhigrosGame.exe 文件，即可启动文件

## 常见问题
不同系统在编译时可能出现一定差异，若在打开 .exe 后缀文件后，系统提示缺少 dll 关键文件，你可以将 build/src/Debug 目录内所有子文件夹中的 .dll 后缀文件一并移植到 Debug 父文件夹中，重新打开 .PhigrosGame.exe 即可解决