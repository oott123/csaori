# CSAORI 本体 #

## Version 1.0.0.0 2008/03/21 ##
  * 新規作成

## Version 1.0.1.0 2008/04/14 ##
  * Argumentの順序がばらばらでも対応可能なようにしました。
  * (KAWARI-827)

## Version 1.1.0.0 2008/06/13 ##
  * 実際に使ってみて出てきた色々な問題を修正。(ponapalt)
  * VC++6でコンパイルが通らない問題を修正。(ponapalt)

## Version 1.2.0.0 2008/07/21 ##
  * CSAORIライブラリ本体を高速化（参照渡しに）(ponapalt)
  * その他細かい調整 (ponapalt)
  * intToString系関数復活 (ukiya)


---

# FileVersion #

## Version 1.0.0.0 2008/07/01 ##
  * 新規作成 (ponapalt)


---

# SetCursorPos #

## Version 1.0.0.0 2008/07/21 ##
  * 新規作成 (ukiya)


---

# XPATH SAORI "Xaoric" #

## Version 1.0.0.0 2008/12/30 ##
  * 新規作成 (ponapalt)

## Version 1.1.0.0 2009/2/23 ##
  * /hoge/@fuga のようにAttributeノードを指定した場合、自動で内容テキストを取るようにした。今まではきちんと /hoge/@fuga/text() のように指定しないとだめだった。

## Version 1.1.1.0 2009/2/23 ##
  * 通常のテキストノード自動選択処理が動かなくなっていたエンバグ修正

## Version 1.1.2.0 2009/2/24 ##
  * 複数候補がある場合はとにかく全部何も考えずテキスト化して列挙するよう変更


---

# TextCopy2 #

## Version 1.0.0.0 2009/3/21 ##
  * 新規作成 (ponapalt)


---

# gainer #

## Version 1.0.0.0 2009/7/10 ##
  * 新規作成 (ponapalt)

## Version 1.0.1.0 2009/7/11 ##
  * OnGainerDigitalInput のRef1が0/1のはずがそれ以外の値も出していた問題を修正

## Version 1.1.0.0 2009/7/12 ##
  * disconnect と scan7segLED を追加

## Version 1.1.1.0 2009/7/14 ##
  * 通信パラメータの誤りを修正
  * configureが一発しか打てない問題を修正

## Version 1.1.2.0 2009/7/16 ##
  * COMポートをポーリングするコードを修正
  * SecurityLevelヘッダの解釈に問題があった点を修正 (CSAORI Core)


---

# uds (UpDown Speed SAORI) #

## Release 1 20070804-0224 ##
  * Initial Version (roytam)

## Release 2 20070804-1028 ##
  * Resolves "Get Up/down speed needs 1 second to measure."

## Release 3 20080531-1359 ##
  * Resolve "Error loading Iphlpapi.dll" after first call.
  * Resolve failure of loading in yaya shori.

## Release 4 20081121-1416 ##
  * Add UpDownSpeed "-1" option for counting all interface (including loopback interface)


---

# screenshot SAORI #

## Release 1 20090310-2137 ##
  * Initial Version (roytam)

## Release 2 20090312-2014 ##
  * update CSaori to SVN [r65](https://code.google.com/p/csaori/source/detail?r=65)
  * use ANSI version of File I/O API due to limitation of CSaori
  * fix not closing file after write

## Release 3 20090313-1927 ##
  * Windows with transparency will be captured
  * output file will be in same directory of dll


---

# SQLori #

## Version 1.0.0 20090720 ##
  * Initial release (twinsec)

## Version 1.1.10 20090723 ##
  * remove GET\_TABLE command, integrated into EXEC command (twinsec)
  * add OPEN and CLOSE command for opening database from other file name (twinsec)
  * Fix demo code (twinsec)
  * Support UTF-8 (twinsec)

## Version 1.1.10+ 20090723 ##
  * add back SQLite 3.6.16 source code as SQLite is Public Domain (roytam)
  * Fix: when no filepath is specified, default file name is be "sqlori.db" but not "sqlori.＿db" (roytam)
  * Fix missing SAORI return status (roytam)
  * move real CSAORI implementation from csaori.cpp/caori.h to csaori\_interface.cpp for easier update of CSAORI (roytam)
  * update CSAORI to svn rev 88 (roytam)

## Version 1.2.13 20090810 ##
  * change return format to "return code(0x2)〔column name〔(0x3)column name...〕〕(0x2)〔data field〔(0x3)data field...〕〕" to avoid (0x1) translation (roytam)


---

# chttpc #

## Release 1 20090802 ##
  * Initial Version (roytam)

## Release 2 20090804 ##
  * make strip and translate work with start= and end=
  * convert non translated output to one line string

## Release 3 20090806 ##
  * add removeTags option

## Release 4 20090829 ##
  * add multiSearch option

## Release 5 20091205 ##
  * add ability of opening local file with "file:" prefix

## Release 6 20100419 ##
  * fix fopen fail if wide char in module path
  * fix Status 500 when using local file and translate


---

# SQLori-SQLite2 #

## Version 1.2.13 20090816 ##
  * Initial release, ported from sqlori 1.2.13 with SQLite 2.8.17 (roytam)


---

# CCPUID #

## Release 1 20090830 ##
  * Initial Version (roytam)

## Release 2 20090831 ##
  * add "cpu.cache" function
  * fix cpu.clockex always return with ".000" ending problem

## Release 3 20090903 ##
  * add fallback when branding can't be retrieved from CPUID
  * fix: don't try to retrieve CPUID information which is higher than reported highest CPUID

## Release 4 20100420 ##
  * add mem.viraex
  * add "cpu.usage" function
  * remove "Microsoft " from os.name
  * fix mem.`*`(except mem.os) to match description in saori\_cpuid\_README.txt (All in MB, not in bytes)