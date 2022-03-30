if [ $# -eq 0 ]
then
#./CustomCompiler -Cg++ -oBang -lstdc++ -I/usr/local/include $(find . -name "*.c") $(find . -name "*.cpp") -Iinclude $(pkg-config --static --libs glfw3) -g "->"
./CustomCompiler -Cg++ -oBang -lrt -lm -ldl -lX11 -lpthread -Llib -lglfw3 -I/usr/local/include $(find . -name "*.c") $(find . -name "*.cpp") -Iinclude -g "->"
exit
fi

./CustomCompiler -Cg++ -oBang -lstdc++ -I/usr/local/include $@ -Iinclude -L/usr/local/lib -L. -lglfw -lpthread -g "->"
