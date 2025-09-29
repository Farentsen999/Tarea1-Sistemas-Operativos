# Inicializar shell

- Ingresar make myprog, para compilar todo el código.
- usar ./myprog para correr el ejecutable, a partir de aqui, sera nuestra shell la que reciba los comandos.

# Consideraciones
- A la hora de hacer piping, es necesario utilizar espacios antes y despues de "|", por ejemplo: "ps -aux | sort -nrk 4 | head -10".
- A la hora de usar comandos para ingresar datos a un archivo o extraer datos de un archivo, es necesario utiizar espacios antes y despues de ">" y "<" respectivamente, por ejemplo: echo hola mundo > archivo.txt o wc -l < archivo.txt (mencionar que al usar > se sobreescribe lo que esta en el archivo correspondiente)
- A la hora de ingresar comandos que involucren guardado de archivos, por ejemplo, "miprof ejecsave output.txt date", para que output.txt aparezca en el directorio
donde se encuentran los modulos de la shell, podria ser necesario actualizar el IDE (En la mayoria de IDE's basta con hacer click izquierdo en cualquier modulo ".c" para actualizar el directorio) 
  
## Sobre el comando miprof

- "miprof" se puede continuar con:

- "ejec": Para ejecutar el comando y qué te imprima el resultado por pantalla.

- "ejecsave text.txt": para o abrir o guardar dentro de un archivo el comando ejecutado, su resultado, y tiempo de ejecución.

- "maxtiempo 'numero'": para establer un tiempo maxímo cuanto tiempo puede tardar los comandos en ejecutarse.
