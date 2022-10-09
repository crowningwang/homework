执行方式：
time find ./test -type f -name '*.h' -o -name '*.c' | xargs grep "hello"
find可以用来对目录下的文件进行筛选
grep可以对文件内容进行筛选
shell内部统计时间一般精确到秒，如果要获得更精确的时间需要进行格式转化，因此直接采用time函数会方便许多
但time命令无法得到某些具体命令的执行时间，因此获取毫秒级时间的shell脚本参考如下：
https://blog.csdn.net/weixin_34050427/article/details/85934782


其他参考网站：
https://blog.csdn.net/weixin_42319496/article/details/119371232?ops_request_misc=&request_id=&biz_id=102&utm_term=shell%E4%BD%BF%E7%94%A8grep%E8%AF%BB%E5%8F%96%E6%96%87%E4%BB%B6%E5%A4%B9%E4%B8%8B%E6%89%80%E6%9C%89%E4%BB%A5.h%E4%B8%BA%E7%BB%93%E5%B0%BE%E7%9A%84%E6%96%87%E4%BB%B6&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-119371232.142^v52^control,201^v3^control_1&spm=1018.2226.3001.4187

https://blog.csdn.net/hunter168_wang/article/details/121282964?ops_request_misc=&request_id=&biz_id=102&utm_term=find%E4%BE%BF%E5%88%A9%E5%BD%93%E5%89%8D%E6%96%87%E4%BB%B6%E5%A4%B9%E4%B8%8B%E4%B8%A4%E7%A7%8D%E7%B1%BB%E5%9E%8B%E7%9A%84%E6%96%87%E4%BB%B6&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-121282964.142^v52^control,201^v3^control_1&spm=1018.2226.3001.4187