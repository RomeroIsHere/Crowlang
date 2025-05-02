# Crowlang
## Instalación

- Sistema Operativo: Windows 10 o Mas Reciente  
- Memoria: 1 GByte  
- Almacenamiento: 10 KBytes, Para los Binarios de Ejecución  
- Procesador: Intel(R) Celeron(R) N4000 CPU @ 1.10GHz

* * *

Se Necesita el Binario de Ejecucion GCC para ser utilizado como Preprocesador, si ya lo tienes Instalado, asegurate de que el Programa este en la Variable de entorno PATH.

El Lenguaje Crow es Directamente traducido a Lenguaje C, Sin embargo esta diseñado para programar GBA, por lo tanto Es Necesario Tener el Ambiente de Desarollo de GBA DevkitARM, el cual Contiene La Libreria TONC, la cual se usa para Programar en GBA

* * *

Existe el Instalador MSI de CrowLang  
Corre el Instalador y Sigue las Instrucciones dadas por el instalador

Se Puede Instalar de Manera Manual, Descarga el Binario Ejecutable del Compilador,  y Añadelo en la Variable de Entorno PATH

Para crear el ejecutable de Manera Manual, se Puede crear una Copia desde un shell de git, y compilar manualmente o mediante CodeBlocks

Para esta ultima Opcion Simplemente Genera una Copia del Repositorio, Abre Codeblocks y Genera el Archivo Ejecutable mediante las Herramientas de la aplicacion  
-# Actualmente Solo Windows Funciona Correctamente debido a la necesidad de ejecutar el Preprocesador de C++

## Manual de Usuario

Para Generar Archivos de Lenguaje C desde Crow no es necesario crear un ambiente de Desarollo, sin Embargo Para Facilitar esta Parte, tenemos una Extension de VSCode, la cual tiene resaltado de sintaxis para los Archivos .crow y .crow.end

Para el Desarollo de una ROM para GBA son necesarios mas elementos, y un entono de desarollo dedicado, Puedes ver como crear este entorno en el Siguiente Repositorio https://github.com/gbadev-org/libtonc-template.git y puedes ver ejemplo de Codigo C a GBA dentro de Este otro Repositorio https://github.com/gbadev-org/libtonc-examples

Puedes Utilizar las Reglas MAKE existentes en estos Repositorios, o puedes utilizar los comandos de compilacion proporcionados por Devkitpro en su toolkit devkitarm (Es Necesario Agregar los Binarios a PATH si es que aun no estan Agregados)

siguiendo esto

#Compila first.c a Archivo first.o usando instrucciones ARM y THUMB

arm-none-eabi-gcc -mthumb -c first.c

#Linkea el Archivo first.o y las Librerias estandard de Devkitarm y genera el Archivo Linkeado Ejecutable (ELF)

arm-none-eabi-gcc -specs=gba.specs -mthumb first.o -o first.elf

#Elimina toda la Informacion de Debugger que se crea al Compilar y Linkear

arm-none-eabi-objcopy -O binary first.elf first.gba

#Arregla la ROM Para que el GBA Pueda Leer el Archivo sin Problemas de Boot

gbafix first.gba

Puedes Ver y Entender Mejor como funciona el Ambiente de desarollo para Devkitarm dentro de el Siguiente Sitio https://gbadev.net/tonc/setup.html

* * *

Crowlang contiene un Subset de Instrucciones de C, Siendo estas las Operaciones Aritmeticas, Operaciones de BIT, Acceso de Array, Declaraciones y Definiciones, y Llamadas de Funciones, Para un Vistazo Mas A Fondo, Por Favor Consulta la Sección de Traduccion en la Documentacion de Compilador.

* * *

el compilador de Crowlang tiene unicamente 2 Parametros que Necesita, el Nombre del archivo MAIN a Compilar y el Nombre del Archivo de Salida. Si no se proporcionan ambos Argumentos, entonces intentara Obtener los datos de manera manual mediante un Menu Simple, donde se pueden ingresar ambos datos (si solo proporcionas 1 Dato este se utiliza como el archivo a compilar, y el archivo de salida sera el primer dato+ “c”)

