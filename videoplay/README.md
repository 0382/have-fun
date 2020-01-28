# 命令行播放视频

需要支持真彩色的terminal，比如说在linux控制台
```bash
python3 videoplay.py --file test.mp4 --lines $LINES --columns $COLUMNS
```

不过即使删掉了一部分帧数，速度依然很慢，基本不可能按照原来的速度播放。

另外，附带一个命令行显示图片的。你把terminal的字体设置的足够小（很多情况下，用快捷键`Ctrl+-`就可以缩小字体），其实还是挺清晰的。
```bash
python3 imshow.py --file test.png --lines $LINES --columns $COLUMNS
```