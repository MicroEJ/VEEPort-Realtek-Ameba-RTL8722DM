Dim WshShell

Set WshShell = WScript.CreateObject("WScript.Shell")

WshShell.Run "cmd /c "+"""""" + WScript.Arguments.Item(0) + "\..\..\..\..\sdk\sdk-amebad_v6.2C-RC\component\soc\realtek\amebad\misc\iar_utility\common\rtl8721d_prebuild.bat" + """" + " " + """" + WScript.Arguments.Item(0) + """" + " " + """" + WScript.Arguments.Item(1) + """""",0