# CMM词法分析器自动生成

于 2020 年 11 月

## 说明

- 整体逻辑：输入正则表达式->NFA->DFA->mDFA->词法分析器

- 正则表达式规范：
  
   - 字符集包含 26 个英文字母（大小写），英文符号；
   - 支持 `[]` 、`-` 表达，`[a-c] `等价于 `(a|b|c)`，另外 `(a-c)` 表示这部分必须由`a`、`-`、`c`组成；
   - []<sup>\*</sup>、()<sup>\*</sup>由 `[]*` 、 `()*` 表达，[a]<sup>+</sup>、(a)<sup>+</sup>由 `a[a]*` 、 `a(a)*` 表达；
   - 支持 `?` 的非贪婪表达；
   - 不支持 `.` 的缺省表达。
   
如：`[_a-zA-Z][_a-zA-Z]*`
  
- 项目构建与运行

   - for Windows + MinGW32
      > .\\> mkdir build
      > .\\> cd build
      > .\\> cmake -G "MinGW Makefiles" ..
      > .\\> mingw32-make
      > .\\> LexicalAnalyzer.exe