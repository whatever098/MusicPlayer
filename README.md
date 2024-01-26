## **开发环境：基于MSVC2017，QT5.9.3**
 ## **一.项目设计**

### **环境说明：（一般无需配置，如果无法运行可以尝试，记得替换为自己的路径）**

#### 1.项目->属性->VC++目录->

包含目录：D:\qt\5.9.3\msvc2017\_64\include;\$(IncludePath)

库目录：D:\qt\5.9.3\msvc2017\_64\lib;`$(VC_LibraryPath_x64);$`(WindowsSDK\_LibraryPath\_x64)

#### 2.项目->属性->C++->常规

附加包含目录：D:\qt\5.9.3\msvc2017\_64\include

#### 3.项目->属性->链接器->输入

Qt5Multimediad.lib
Qt5Cored.lib
Qt5Networkd.lib
Qt5Widgetsd.lib
Qt5Sqld.lib

#### 4.vs\_2019  x64 Debug Qt5.9.3

### **使用MVC（Model-View-Controller）设计模式**

### **1.模型（Model）**：

**音乐播放引擎（Music Engine）：**

&#x9;负责处理音乐的播放、暂停、切换、进度调节等功能。使用Qt的QMediaPlayer或其他适合的多媒体库作为底层实现。

**音乐库管理（Music Library）：(单例)**

&#x9;维护在线音乐资源的列表信息，负责在线音乐的获取、存储和管理。可以涵盖歌曲信息、歌词、专辑图片等元数据。

**本地缓存管理（Local Cache）：**

&#x9;处理本地音乐缓存和播放，负责缓存的存储、读取和删除。

### **2.视图（View）：**

**播放器界面（MusicPlayerUI）：**

&#x9;实现用户界面，展示音乐播放器界面的各种控制按钮、进度条、专辑图片、歌词显示等。

**歌词控制显示界面（LyricWidget）：MusicPlayerUI的成员**

&#x9;控制歌词更新并递交到ui层，与MusicPlayerUI使用同一个ui，MusicPlayerUI将ui指针传递给LyricWidget避免多次构造

**音乐列表视图（MusicListWidget）**

&#x9;处理歌曲列表的显示和递交点击信号，同时显示和处理搜索信号

**主窗口（MainWindow）**

&#x9;显示主窗口，负责托盘最小化

### **3.控制器（Controller）：**

**音乐播放控制器（MusicPlayerController）：**

&#x9;监听用户界面的交互事件，将这些事件转发给模型层来处理，例如播放、暂停、切换歌曲等操作。



## **二.功能实现**

*   支持 MP3 格式音乐播放&#x20;
*   支持基本的播放控制功能，如播放、暂停、上一曲、下一曲
*   支持在线音乐播放，通过 HTTP 请求获取音乐资源
*   支持歌词自动匹配并显示
*   支持专辑图片自动匹配并显示
*   支持音量调节、静音功能。
*   支持播放进度显示,时间显示
*   支持歌曲切换
*   支持拖动进度条调整进度
*   支持专辑图片动画,在播放时可同步旋转
*   支持本地音乐缓存与播放
*   支持 MP3 以外的格式播放,WAV、FLAC、AAC
*   支持播放模式选择，顺序播放、单曲循环、随机播放
*   支持最小化托盘，并可以在系统托盘处退出程序
*   对于没有严格文件目录格式的音乐可以识别并播放，即对于任意localcache文件夹中代码已写的音乐格式的音乐，都能识别并播放而且是迭代识别，代表在localcache目录下新建任意深度目录存放音乐，都能识别并播放，例如新建文件夹my_like，在其中存放几首音乐，能够获取音乐斌播放，不需要严格的目录结构和music.json等。
*   静音后取消静音恢复原有音量
*   数据库存储音乐信息，支持多线程
*   可查看当前播放列表以便知晓播放顺序
