Primero ir a la carpeta src, la cual contiene los archivos fuente de la Wish Shell.<br /><br />

Ejecutar Wish Shell: <br />
**reqs:** <br />
Ubuntu: sudo apt-get install libreadline-dev <br />
macOS: brew install readline <br />
**compile:** gcc -Wall -o wish wish.c -lreadline <br />
**run:** ./wish

Ejecutar Tests: <br />
**run:** ./test-wish.sh
